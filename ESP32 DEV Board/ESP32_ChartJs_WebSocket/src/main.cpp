#include <Arduino.h>
#include <HardwareSerial.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <map>
#include <math.h>
#include "web.h"

// ================= USER CONFIG =================
#define WIFI_SSID "LAPTOP-CCB26F1U 5621"
#define WIFI_PASSWORD "12345678"

// Toggle for testing
#define USE_DUMMY true

// UART pins (only used when USE_DUMMY==false)
#define STM32_RX_PIN 16
#define STM32_TX_PIN 17
#define STM32_BAUD 115200

// Aggregation / publish interval (ms)
#define AGG_MS 100 // 100 ms -> 10 Hz UI update

WebServer server (80);
WebSocketsServer webSocket (81);
HardwareSerial stm32Serial (1);

// ----------------- ring buffer (byte-level) -----------------
class RingBuf {
    public:
    RingBuf (size_t capacity = 4096) : cap (capacity) {
        buf = new uint8_t[cap];
        head = tail = 0;
    }
    ~RingBuf () { delete[] buf; }
    size_t available () const {
        if (head >= tail)
            return head - tail;
        return cap - tail + head;
    }
    bool writeByte (uint8_t b) {
        size_t next = (head + 1) % cap;
        if (next == tail)
            return false; // full
        buf[head] = b;
        head = next;
        return true;
    }
    // read up to n bytes into dest, return bytes read
    size_t readBytes (uint8_t* dest, size_t n) {
        size_t r = 0;
        while (r < n && available ()) {
            dest[r++] = buf[tail];
            tail = (tail + 1) % cap;
        }
        return r;
    }
    // read single byte if available
    bool readByte (uint8_t& b) {
        if (!available ())
            return false;
        b = buf[tail];
        tail = (tail + 1) % cap;
        return true;
    }
    // clear buffer (useful for debug)
    void clear () { head = tail = 0; }

    private:
    uint8_t* buf;
    size_t cap;
    size_t head, tail;
};

static RingBuf ring (8192);

// ----------------- data model -----------------
struct Motor {
    int V = 0;
    int status = 0;
    int current_mA = 0;
    float current_A = 0.0f;
    int rpm = 0;
};

// keep latest state per motor (thread-safe-ish in single-threaded ESP loop)
std::map<String, Motor> motors;

// ----------------- helper: build JSON from latest motor state
// -----------------
String buildJson () {
    String out = "{\"motors\":{";
    bool f = true;
    for (auto& p : motors) {
        if (!f)
            out += ",";
        f = false;
        String name = p.first;
        Motor m = p.second;
        out += "\"" + name + "\":{";
        out += "\"V\":" + String (m.V) + ",";
        out += "\"status\":" + String (m.status) + ",";
        out += "\"current_A\":" + String (m.current_A, 3) + ",";
        out += "\"rpm\":" + String (m.rpm);
        out += "}";
    }
    out += "}}";
    return out;
}

// safe broadcast (WebSocketsServer expects String&)
void broadcastMotors () {
    String payload = buildJson ();
    webSocket.broadcastTXT (payload);
}

// ----------------- Parser: extract lines from ring buffer, parse NV lines
// -----------------
void parseAndUpdate () {
    // read bytes until newline and parse each complete line
    static char linebuf[256];
    static size_t idx = 0;
    uint8_t b;
    // limit number of lines per call to avoid starvation (tune if needed)
    int linesProcessed = 0;
    const int MAX_LINES_PER_ITER = 10;

    while (linesProcessed < MAX_LINES_PER_ITER && ring.readByte (b)) {
        if (b == '\r')
            continue;
        if (b == '\n') {
            if (idx == 0)
                continue;
            linebuf[idx] = 0;
            String s (linebuf);
            // parse "NV:M1: 220 03 0660 60"
            // robust minimal parser
            int p1 = s.indexOf (':'); // after NV
            int p2 = s.indexOf (':', p1 + 1);
            if (p1 > 0 && p2 > p1) {
                String prefix = s.substring (0, p1);
                String dev = s.substring (p1 + 1, p2);
                dev.trim ();
                String rest = s.substring (p2 + 1);
                rest.trim ();
                // split by spaces
                int vals[8];
                int vc = 0;
                int start = 0;
                while (start < (int)rest.length () && vc < 8) {
                    int sp = rest.indexOf (' ', start);
                    if (sp < 0)
                        sp = rest.length ();
                    String token = rest.substring (start, sp);
                    token.trim ();
                    if (token.length () > 0)
                        vals[vc++] = token.toInt ();
                    start = sp + 1;
                }
                if (prefix == "NV" && vc >= 4) {
                    Motor m;
                    m.V = vals[0];
                    m.status = vals[1];
                    m.current_mA = vals[2];
                    m.current_A = m.current_mA / 1000.0f;
                    m.rpm = vals[3];
                    motors[dev] = m;
                }
            }
            idx = 0;
            linesProcessed++;
        } else {
            if (idx < sizeof (linebuf) - 1)
                linebuf[idx++] = (char)b;
            else
                idx = 0; // overflow, drop
        }
    }
}

// ----------------- Dummy high-rate producer (writes into ring buffer)
// -----------------
unsigned long lastDummy = 0;
void dummyProducer () {
    // push many lines per second to simulate heavy UART (e.g., 200 Hz)
    unsigned long now = millis ();
    // produce bursts; adjust frequency here to simulate heavy data
    if (now - lastDummy < 5)
        return; // ~200 Hz
    lastDummy = now;

    static int phase = 0;
    phase++;
    // create three lines per cycle (vary values slightly)
    char line[64];
    int v1 = 220 + (int)(5.0 * sin (phase * 0.07));
    int c1 = 500 + (int)(20.0 * sin (phase * 0.11));
    int r1 = 600 + (int)(10.0 * sin (phase * 0.05));
    snprintf (line, sizeof (line), "NV:M1: %d 3 %d %d\n", v1, c1, r1);
    for (char* p = line; *p; ++p)
        ring.writeByte ((uint8_t)*p);

    int v2 = 219 + (int)(4.0 * sin (phase * 0.08));
    int c2 = 480 + (int)(18.0 * sin (phase * 0.12));
    int r2 = 590 + (int)(12.0 * sin (phase * 0.06));
    snprintf (line, sizeof (line), "NV:M2: %d 3 %d %d\n", v2, c2, r2);
    for (char* p = line; *p; ++p)
        ring.writeByte ((uint8_t)*p);

    int v3 = 221 + (int)(6.0 * sin (phase * 0.06));
    int c3 = 520 + (int)(22.0 * sin (phase * 0.09));
    int r3 = 610 + (int)(11.0 * sin (phase * 0.04));
    snprintf (line, sizeof (line), "NV:M3: %d 3 %d %d\n", v3, c3, r3);
    for (char* p = line; *p; ++p)
        ring.writeByte ((uint8_t)*p);
}

// ----------------- WebSocket event -----------------
void webSocketEvent (uint8_t num, WStype_t type, uint8_t* payload,
    size_t length) {
    if (type == WStype_CONNECTED) {
        Serial.printf ("WS client %u connected\n", num);
    } else if (type == WStype_TEXT) {
        // handle snapshot request etc.
        String msg = String ((char*)payload);
        if (msg.indexOf ("snapshot") >= 0) {
            broadcastMotors ();
        }
    }
}

// ----------------- Setup / Loop -----------------
unsigned long lastAgg = 0;

void setup () {

    Serial.begin (STM32_BAUD);
    delay (50);
    Serial.println ("Booting...");

    // --- TEMP: WiFi scan for debugging ---
    Serial.println ("Scanning WiFi networks...");
    int n = WiFi.scanNetworks ();
    Serial.printf ("Found %d networks\n", n);
    for (int i = 0; i < n; ++i) {
        Serial.printf ("%d: '%s' (RSSI %d) encryption=%d\n", i, WiFi.SSID (i).c_str (),
            WiFi.RSSI (i), WiFi.encryptionType (i));
    }
    // --- end scan ---

    if (!USE_DUMMY) {
        stm32Serial.begin (STM32_BAUD, SERIAL_8N1, STM32_RX_PIN, STM32_TX_PIN);
    }

    // WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
    // Serial.print ("Connecting to WiFi");
    // while (WiFi.status () != WL_CONNECTED) {

    //     delay (200);
    //     Serial.print (".");
    // }

    // ---- ADD THIS BLOCK ----
    IPAddress local_IP (192, 168, 137, 50);
    IPAddress gateway (192, 168, 137, 1);
    IPAddress subnet (255, 255, 255, 0);

    if (!WiFi.config (local_IP, gateway, subnet)) {
        Serial.println ("STA Failed to configure static IP");
    }
    // ---- END ADD ----

    // --- TEMP: WiFi scan for debugging ---
    Serial.printf ("Attempting connect to '%s'\n", WIFI_SSID);
    WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
    unsigned long start = millis ();
    while (WiFi.status () != WL_CONNECTED && millis () - start < 15000) {
        delay (500);
        Serial.print (".");
    }
    Serial.println ();
    Serial.printf ("WiFi.status()=%d, freeHeap=%u\n", WiFi.status (),
        ESP.getFreeHeap ());
    if (WiFi.status () == WL_CONNECTED) {
        Serial.print ("Connected, IP: ");
        Serial.println (WiFi.localIP ());
    } else {
        Serial.println ("Failed to connect within timeout");
    }
    // --- end scan ---

    Serial.println ();
    Serial.print ("IP: ");
    Serial.println (WiFi.localIP ());

    server.on ("/", []() { server.send_P (200, "text/html", INDEX_HTML); });
    server.onNotFound ([]() { server.send (404, "text/plain", "Not found"); });
    server.begin ();

    webSocket.begin ();
    webSocket.onEvent (webSocketEvent);

    Serial.println ("Server ready");
}

void loop () {
    server.handleClient ();
    webSocket.loop ();

    // Stage A: fast I/O: push bytes from UART to ring buffer (non-blocking)
    if (!USE_DUMMY) {
        while (stm32Serial.available ()) {
            int c = stm32Serial.read ();
            // try to write; if ring is full we drop bytes (keeps realtime safe)
            ring.writeByte ((uint8_t)c);
        }
    } else {
        // in dummy mode produce high-rate data into ring
        dummyProducer ();
    }

    // Stage B: parse available complete lines (bounded work)
    parseAndUpdate ();

    // Stage C: aggregation/publish (on timer, decoupled from UART)
    unsigned long now = millis ();
    if (now - lastAgg >= AGG_MS) {
        lastAgg = now;
        // For simplicity we push the last values (no averaging yet). This is where
        // you'd implement average/min/max.
        broadcastMotors ();
    }
}
