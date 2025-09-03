#include <Arduino.h>

// // UART connection with STM32
// #define STM32_RX_PIN 16  // ESP32 RX -> STM32 TX
// #define STM32_TX_PIN 17  // ESP32 TX -> STM32 RX (optional)
// HardwareSerial stm32Serial (1);

// // Variables for segregated data
// int vrms_int = 0, irms_int = 0;          // From NV
// int power_int = 0, temperature_int = 0;  // From NPT
// int sine_voltage = 0;                    // From SV
// int sine_current = 0;                    // From SI

// void setup () {
//     Serial.begin (115200); // Debug monitor
//     stm32Serial.begin (115200, SERIAL_8N1, STM32_RX_PIN, STM32_TX_PIN);

//     Serial.println ("ESP32 ready to parse STM32 UART data...");
// }

// void loop () {
//     if (stm32Serial.available ()) {
//         String line = stm32Serial.readStringUntil ('\n');
//         line.trim ();  // remove \r, spaces

//         if (line.startsWith ("NV:")) {
//             sscanf (line.c_str (), "NV:%d %d", &vrms_int, &irms_int);
//             Serial.printf ("[NV] Vrms=%d, Irms=%d\n", vrms_int, irms_int);

//         } else if (line.startsWith ("NPT:")) {
//             sscanf (line.c_str (), "NPT:%d %d", &power_int, &temperature_int);
//             Serial.printf ("[NPT] Power=%d, Temp=%d\n", power_int, temperature_int);

//         } else if (line.startsWith ("SV:")) {
//             sscanf (line.c_str (), "SV:%d", &sine_voltage);
//             Serial.printf ("[SV] Voltage=%d\n", sine_voltage);

//         } else if (line.startsWith ("SI:")) {
//             sscanf (line.c_str (), "SI:%d", &sine_current);
//             Serial.printf ("[SI] Current=%d\n", sine_current);

//         } else {
//             Serial.printf ("Unknown message: %s\n", line.c_str ());
//         }
//     }
// }

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// ==== WiFi Config ====
const char* ssid = "LAPTOP-CCB26F1U 5621";
const char* password = "12345678";

// ==== Web Server + WebSocket ====
WebServer server (80);
WebSocketsServer webSocket = WebSocketsServer (81);

// ==== UART Config ====
#define STM32_RX_PIN 16  // ESP32 RX -> STM32 TX
#define STM32_TX_PIN 17  // ESP32 TX -> STM32 RX (optional)
HardwareSerial stm32Serial (1);

// ==== Latest values ====
int NV_v = 0, NV_i = 0;
int NPT_p = 0, NPT_t = 0;
float SV = 0.0f;   // sine voltage (scaled)
float SI = 0.0f;   // sine current (scaled)

// ==== HTML Page ====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>STM32 Live Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { font-family: Arial, sans-serif; margin: 0; padding: 12px; background:#f6f7fb; color:#222; }
    .container { display: flex; gap: 12px; height: calc(100vh - 24px); }
    /* left column - reduced width */
    .left { flex: 0.8; display:flex; flex-direction: column; gap:12px; }
    /* make chart cards compact: fixed height so charts are shorter */
    .chart-card { background: #fff; border-radius: 8px; box-shadow: 0 2px 6px rgba(0,0,0,0.08); padding:12px; display:flex; flex-direction:column; }
    .chart-card { height: 200px; max-height: 220px; }
    .chart-title { font-weight:600; margin-bottom:8px; }
    /* right column - increased width */
    .right { width: 500px; display:flex; flex-direction: column; gap:12px; }
    .stat { background:#fff; border-radius:8px; padding:12px; box-shadow:0 2px 6px rgba(0,0,0,0.06); }
    .stat h3 { margin:0 0 6px 0; font-size:14px; color:#666; font-weight:600; }
    .stat .value { font-size:20px; font-weight:700; color:#111; }
    .small { font-size:12px; color:#888; margin-top:6px; }
    canvas { width:100% !important; height:100% !important; }
    footer { margin-top:8px; font-size:12px; color:#666; }
    @media (max-width: 900px) {
      .container { flex-direction: column; height: auto; }
      .right { width: 100%; }
      .chart-card { height: 200px; }
    }
  </style>
</head>
<body>
  <h2>STM32 Realtime Dashboard</h2>
  <div class="container">
    <div class="left">
      <div class="chart-card">
        <div class="chart-title">Sine Voltage (V)</div>
        <canvas id="chartVoltage"></canvas>
      </div>
      <div class="chart-card">
        <div class="chart-title">Sine Current (A)</div>
        <canvas id="chartCurrent"></canvas>
      </div>
    </div>
    <div class="right">
      <div class="stat">
        <h3>Normal Voltage</h3>
        <div id="nv" class="value">-- V, -- A</div>
        <div class="small">NV: RMS Voltage and RMS Current</div>
      </div>
      <div class="stat">
        <h3>Normal Power & Temp</h3>
        <div id="npt" class="value">-- W, -- °C</div>
        <div class="small">NPT: Power and Temperature</div>
      </div>
      <div class="stat">
        <h3>Sine Voltage (instant)</h3>
        <div id="sv" class="value">-- V</div>
        <div class="small">SV from STM32 (scaled)</div>
      </div>
      <div class="stat">
        <h3>Sine Current (instant)</h3>
        <div id="si" class="value">-- A</div>
        <div class="small">SI from STM32 (scaled)</div>
      </div>
      <footer>Updated via WebSocket</footer>
    </div>
  </div>
<script>
(() => {
  const maxPoints = 120; // keep last 120 points (~buffer)
  let dataPointCounter = 0; // counter for x-axis labels
  
  // Create voltage chart
  const ctxV = document.getElementById('chartVoltage').getContext('2d');
  const voltageChart = new Chart(ctxV, {
    type: 'line',
    data: {
      labels: [],
      datasets: [{
        label: 'Sine Voltage (V)',
        data: [],
        fill: false,
        borderWidth: 2,
        pointRadius: 0,
        cubicInterpolationMode: 'monotone',
        tension: 0.4,
        borderColor: 'rgb(37, 99, 235)'
      }]
    },
    options: {
      animation: false,
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        x: { display: true },
        y: { display: true, suggestedMin: -5, suggestedMax: 5 }
      },
      plugins: { legend: { display: false } }
    }
  });
  
  // Create current chart
  const ctxI = document.getElementById('chartCurrent').getContext('2d');
  const currentChart = new Chart(ctxI, {
    type: 'line',
    data: {
      labels: [],
      datasets: [{
        label: 'Sine Current (A)',
        data: [],
        fill: false,
        borderWidth: 2,
        pointRadius: 0,
        cubicInterpolationMode: 'monotone',
        tension: 0.4,
        borderColor: 'rgb(220, 38, 38)'
      }]
    },
    options: {
      animation: false,
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        x: { display: true },
        y: { display: true, suggestedMin: -5, suggestedMax: 5 }
      },
      plugins: { legend: { display: false } }
    }
  });
  
  // WebSocket connection
  const wsUrl = "ws://" + location.hostname + ":81/";
  console.log("Connecting to WS", wsUrl);
  const ws = new WebSocket(wsUrl);
  ws.onopen = () => console.log("WS open");
  ws.onclose = () => console.log("WS closed");
  ws.onerror = (e) => console.error("WS error", e);
  
  ws.onmessage = (evt) => {
    // Expect JSON: {"NV_v":..,"NV_i":..,"NPT_p":..,"NPT_t":..,"SV":..,"SI":..}
    try {
      const d = JSON.parse(evt.data);
      // update right-side numeric UI
      if (d.NV_v !== undefined && d.NV_i !== undefined) {
        document.getElementById('nv').innerText = d.NV_v + " V, " + d.NV_i + " A";
      }
      if (d.NPT_p !== undefined && d.NPT_t !== undefined) {
        document.getElementById('npt').innerText = d.NPT_p + " W, " + d.NPT_t + " °C";
      }
      if (d.SV !== undefined) {
        document.getElementById('sv').innerText = Number(d.SV).toFixed(3) + " V";
      }
      if (d.SI !== undefined) {
        document.getElementById('si').innerText = Number(d.SI).toFixed(3) + " A";
      }
      
      // push to charts (SV -> voltageChart, SI -> currentChart)
      dataPointCounter++;
      const label = dataPointCounter.toString(); // use simple number instead of time
      
      // Voltage chart
      if (d.SV !== undefined) {
        voltageChart.data.labels.push(label);
        voltageChart.data.datasets[0].data.push(Number(d.SV));
        if (voltageChart.data.labels.length > maxPoints) {
          voltageChart.data.labels.shift();
          voltageChart.data.datasets[0].data.shift();
        }
        voltageChart.update('none');
      }
      // Current chart
      if (d.SI !== undefined) {
        currentChart.data.labels.push(label);
        currentChart.data.datasets[0].data.push(Number(d.SI));
        if (currentChart.data.labels.length > maxPoints) {
          currentChart.data.labels.shift();
          currentChart.data.datasets[0].data.shift();
        }
        currentChart.update('none');
      }
    } catch (err) {
      console.error("WS JSON parse error", err, evt.data);
    }
  };
})();
</script>
</body>
</html>
)rawliteral";


// ==== Serve Web Page ====
void handleRoot () {
    server.send_P (200, "text/html", index_html);
}

// ==== WebSocket Event ====
void onWebSocketEvent (uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_CONNECTED) {
        Serial.printf ("Client %u connected\n", num);
    }
}

// ==== Send JSON over WebSocket ====
// Note: SV and SI are floats and formatted with 3 decimal places
void sendData () {
    char buffer[128];
    snprintf (buffer, sizeof (buffer),
        "{\"NV_v\":%d,\"NV_i\":%d,\"NPT_p\":%d,\"NPT_t\":%d,\"SV\":%.3f,\"SI\":%.3f}",
        NV_v, NV_i, NPT_p, NPT_t, SV, SI);
    webSocket.broadcastTXT (buffer);
}

// ==== Setup ====
void setup () {
    Serial.begin (115200);

    stm32Serial.begin (115200, SERIAL_8N1, STM32_RX_PIN, STM32_TX_PIN);

    WiFi.begin (ssid, password);
    Serial.print ("Connecting to WiFi");
    while (WiFi.status () != WL_CONNECTED) {
        delay (500);
        Serial.print (".");
    }
    Serial.println ("\nConnected!");
    Serial.println (WiFi.localIP ());

    server.on ("/", handleRoot);
    server.begin ();

    webSocket.begin ();
    webSocket.onEvent (onWebSocketEvent);
}

// ==== Loop ====
void loop () {
    server.handleClient ();
    webSocket.loop ();

    // Read STM32 UART line
        // Read STM32 UART line
    if (stm32Serial.available ()) {
        String line = stm32Serial.readStringUntil ('\n');
        line.trim ();               // remove \r and whitespace
        if (line.length () == 0) return;

        Serial.println ("RX: " + line);

        // Convert to a C string for robust parsing
        const char* c = line.c_str ();
        // find ':' position
        const char* colon = strchr (c, ':');
        if (colon == NULL) {
            Serial.println ("Parse error: no ':' found");
            sendData ();
            return;
        }

        // prefix length and prefix extraction
        size_t prefix_len = colon - c;
        String prefix = String (c, prefix_len);   // e.g. "SV", "NV", "NPT", "SI"
        const char* numstr = colon + 1;          // numeric part starts after ':'

        // For tags with two numbers (NV, NPT) parse separately
        if (prefix == "NV") {
            // Expect "NV:<v> <i>" possibly with signs (but usually positive)
            long v = 0, i = 0;
            // use strtol twice
            char* endptr = nullptr;
            v = strtol (numstr, &endptr, 10);
            while (*endptr == ' ') endptr++; // skip spaces
            i = strtol (endptr, NULL, 10);
            NV_v = (int)v;
            NV_i = (int)i;
            Serial.printf ("[NV] parsed v=%ld i=%ld\n", v, i);

        } else if (prefix == "NPT") {
            // Expect "NPT:<p> <t>"
            long p = 0, t = 0;
            char* endptr = nullptr;
            p = strtol (numstr, &endptr, 10);
            while (*endptr == ' ') endptr++;
            t = strtol (endptr, NULL, 10);
            NPT_p = (int)p;
            NPT_t = (int)t;
            Serial.printf ("[NPT] parsed p=%ld t=%ld\n", p, t);

        } else if (prefix == "SV") {
            // Expect "SV:<raw>" where raw is integer = value * 1000 (may be negative)
            char* endptr = nullptr;
            long raw = strtol (numstr, &endptr, 10);
            // If no digits parsed, strtol sets endptr==numstr
            if (endptr == numstr) {
                Serial.println ("[SV] parse failed");
            } else {
                SV = (float)raw * 150.0f / 1000.0f;
                Serial.printf ("[SV] raw=%ld -> %.3f V\n", raw, SV);
            }

        } else if (prefix == "SI") {
            // Expect "SI:<raw>" where raw is integer = value * 1000 (may be negative)
            char* endptr = nullptr;
            long raw = strtol (numstr, &endptr, 10);
            if (endptr == numstr) {
                Serial.println ("[SI] parse failed");
            } else {
                SI = (float)raw * 15.0f / 1000.0f;
                Serial.printf ("[SI] raw=%ld -> %.3f A\n", raw, SI);
            }

        } else {
            Serial.printf ("Unknown prefix: %s\n", prefix.c_str ());
        }

        // Send updated values (even if only some changed)
        sendData ();
    }
}