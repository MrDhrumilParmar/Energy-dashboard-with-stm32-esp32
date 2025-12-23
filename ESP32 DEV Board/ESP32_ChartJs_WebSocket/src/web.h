#pragma once
//#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Realtime Energy Monitoring</title>

        <style>
/* =========================================================
   BASE / RESET
   ========================================================= */
* {
    box-sizing: border-box;
}

html {
    font-size: 10px; /* GLOBAL SCALE — makes UI less zoomed */
}

body {
    margin: 0;
    min-height: 100vh;
    font-family: system-ui, -apple-system, "Segoe UI", Roboto, Arial;
    background: #f1f4f6;
    color: #1f2933;
    -webkit-font-smoothing: antialiased;
}

/* =========================================================
   TOP BAR
   ========================================================= */
.navbar {
    background: #ffffff;
    padding: 1rem 1.6rem;
    font-size: 1.8rem;
    font-weight: 800;
    border-bottom: 1px solid #e6e9ec;
    color: #0f1720;
}

/* =========================================================
   LAYOUT
   ========================================================= */
.container {
    display: grid;
    grid-template-columns: minmax(0, 1fr) 22rem;
    gap: 1.5rem;
    padding: 1.5rem;
    padding-left: 6em;
    padding-right: 6em;
    align-items: start;
}

.main {
    overflow-y: auto;
    padding-right: 0.5rem;
}

.side {
    display: flex;
    flex-direction: column;
    gap: 1.2rem;
}

/* =========================================================
   CARD BASE
   ========================================================= */
.card {
    background: #ffffff;
    border-radius: 0.8rem;
    padding-left: 1.25rem;
    padding-right: 1.25rem;
    padding-top: 0.85rem;
    padding-bottom: 0.85rem;
    margin-bottom: 1.4rem;
    box-shadow: 0 6px 18px rgba(18, 24, 34, 0.08);
}

/* =========================================================
   CARD HEADER
   ========================================================= */
.card-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 0.75rem;
}

.title {
    font-size: 1.4rem;
    font-weight: 600;
    color: #0b1220a1;
}

/* =========================================================
   STATUS BADGE
   ========================================================= */
.badge {
    background: #22c55e;
    color: #ffffff;
    padding: 0.35rem 0.75rem;
    border-radius: 999px;
    font-size: 1.3rem;
    font-weight: 600;
    box-shadow: 0 2px 6px rgba(34, 197, 94, 0.25);
}

/* =========================================================
   METRICS (V / I / P / T)
   ========================================================= */
.metrics.large-metrics {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 0.6rem 1.4rem;
    margin-bottom: 1rem;
    padding-inline: 1rem;
}

.metric {
    text-align: left;
}

.metric-value {
    font-size: 1.8rem; /* industrial density */
    font-weight: 800;
    line-height: 1.1;
    color: #191919;
}

.metric-label {
    font-size: 1.1rem;
    font-weight: 600;
    margin-top: 0.2rem;
}

/* =========================================================
   METRIC COLORS
   ========================================================= */
.v {
    color: #1f77b4;
} /* Voltage */
.i {
    color: #ff7f0e;
} /* Current */
.p {
    color: #6f42c1;
} /* Power */
.t {
    color: #d62828;
} /* Temperature */

/* =========================================================
   TOTAL POWER HIGHLIGHT
   ========================================================= */
.totalKW {
    width: 100%;
    padding: 0.9rem;
    border-radius: 0.7rem;
    background: #edf1f2;
    margin-bottom: 1rem;
}

.total-label {
    font-size: 1rem;
    font-weight: 600;
    color: #6b7280;
    margin-bottom: 0.4rem;
}

.total-value {
    font-size: 2.3rem;
    font-weight: 800;
    color: #111827;
}

/* =========================================================
   CHART AREA
   ========================================================= */
.chart-wrap {
    margin-top: 0.4rem;
}

canvas {
    width: 100% !important;
    height: 10rem !important; /* compact but readable */
    display: block;
}

/* =========================================================
   BUTTONS
   ========================================================= */
.control-buttons {
    display: flex;
    flex-direction: column;
    gap: 0.8rem;
}

.btn {
    width: 100%;
    padding: 0.8rem;
    border-radius: 0.6rem;
    border: none;
    font-size: 1.3rem;
    font-weight: 700;
    cursor: pointer;
}

.btn.start {
    background: #16a34a;
    color: #ffffff;
}

.btn.stop {
    background: #ef4444;
    color: #ffffff;
}

.export {
    background: #aab6cf6e;
    color: #111827;
}

/* =========================================================
   EVENT LOG
   ========================================================= */
.log-card .log-title {
    font-size: 1.3rem;
    font-weight: 700;
    margin-bottom: 0.6rem;
}

.event-log {
    background: #efefefb8;
    border: 1px solid #e6e6e6;
    border-radius: 0.6rem;
    padding: 0.75rem;
    margin-bottom: 0.75rem;
    height: 12rem;
    overflow-y: auto;
    font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, "Roboto Mono",
        monospace;
    font-size: 0.85rem;
    color: #333;
}

.log-item {
    padding: 0.4rem 0;
    border-bottom: 1px dashed #e5e7eb;
}

/* =========================================================
   RESPONSIVE (MOBILE / SMALL SCREENS)
   ========================================================= */
@media (max-width: 980px) {
    .container {
        grid-template-columns: 1fr;
        padding: 1rem;
    }

    .metric-value {
        font-size: 1.5rem;
    }

    .total-value {
        font-size: 2rem;
    }

    canvas {
        height: 7rem !important;
    }
}

</style>
    </head>
    <body>
        <div class="navbar">Realtime Energy Monitoring</div>

        <div class="container">
            <main class="main">
                <!-- Motor 1 -->
                <section class="card motor-card">
                    <div class="card-header">
                        <div class="title">Motor 1 &ndash; Conveyor Belt</div>
                        <div class="badge">Healthy</div>
                    </div>

                    <div class="metrics large-metrics">
                        <div class="metric">
                            <div class="metric-value" id="m1V">0 V</div>
                            <div class="metric-label v">Voltage</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m1I">0 A</div>
                            <div class="metric-label i">Current</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m1P">0 kW</div>
                            <div class="metric-label p">Power</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m1T">0 °C</div>
                            <div class="metric-label t">Temp</div>
                        </div>
                    </div>

                    <div class="chart-wrap">
                        <canvas id="c1"></canvas>
                    </div>
                </section>

                <!-- Motor 2 -->
                <section class="card motor-card">
                    <div class="card-header">
                        <div class="title">Motor 2 &ndash; Hydraulic Pump</div>
                        <div class="badge">Healthy</div>
                    </div>

                    <div class="metrics large-metrics">
                        <div class="metric">
                            <div class="metric-value" id="m2V">0 V</div>
                            <div class="metric-label v">Voltage</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m2I">0 A</div>
                            <div class="metric-label i">Current</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m2P">0 kW</div>
                            <div class="metric-label p">Power</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m2T">0 °C</div>
                            <div class="metric-label t">Temp</div>
                        </div>
                    </div>

                    <div class="chart-wrap">
                        <canvas id="c2"></canvas>
                    </div>
                </section>

                <!-- Motor 3 -->
                <section class="card motor-card">
                    <div class="card-header">
                        <div class="title">Motor 3 &ndash; Cooling Fan</div>
                        <div class="badge">Healthy</div>
                    </div>

                    <div class="metrics large-metrics">
                        <div class="metric">
                            <div class="metric-value" id="m3V">0 V</div>
                            <div class="metric-label v">Voltage</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m3I">0 A</div>
                            <div class="metric-label i">Current</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m3P">0 kW</div>
                            <div class="metric-label p">Power</div>
                        </div>
                        <div class="metric">
                            <div class="metric-value" id="m3T">0 °C</div>
                            <div class="metric-label t">Temp</div>
                        </div>
                    </div>

                    <div class="chart-wrap">
                        <canvas id="c3"></canvas>
                    </div>
                </section>
            </main>

            <aside class="side">
                <div class="card total-card">
                    <div class="totalKW">
                        <div class="total-label">Total System Power</div>
                        <div class="total-value" id="totalPower">0.0 kW</div>
                    </div>
                    <div class="control-buttons">
                        <button class="btn start">START SYSTEM</button>
                        <button class="btn stop">STOP SYSTEM</button>
                    </div>
                </div>

                <div class="card log-card">
                    <div class="log-title">Event Log</div>
                    <div class="event-log" id="eventLog">
                        <div class="log-item">
                            09:00 AM - System Started (Info)
                        </div>
                        <div class="log-item">
                            10:42 AM - M2 Current Spike (Warning)
                        </div>
                    </div>
                    <button class="btn export">Export Data CSV</button>
                </div>
            </aside>
        </div>

        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <script>
/* app.js - updated
   - Uses motorCache to persist per-motor data
   - Adds autoscaling (smooth)
   - Adds colored ticks with units
   - Removes reliance on `latest` guard so UI updates
*/

window.addEventListener("DOMContentLoaded", () => {
    const UI_UPDATE_MS = 500;

    Chart.defaults.font.family =
        'system-ui, -apple-system, "Segoe UI", Roboto, Arial';

    Chart.defaults.font.size = 10; // 🔽 reduce overall chart font
    // Chart.defaults.font.weight = "500";
    // Chart.defaults.color = "#6b7280"; // default axis text color

    // Per-motor cache (always present)
    const motorCache = { M1: {}, M2: {}, M3: {} };

    // Autoscale config
    const SCALE_MARGIN = 1.15; // 15% headroom
    const scaleState = [
        {
            V: { min: 0, max: 250 },
            I: { min: 0, max: 2 },
            P: { min: 0, max: 1.5 },
            T: { min: 40, max: 80 },
        },
        {
            V: { min: 0, max: 250 },
            I: { min: 0, max: 2 },
            P: { min: 0, max: 1.5 },
            T: { min: 40, max: 80 },
        },
        {
            V: { min: 0, max: 250 },
            I: { min: 0, max: 2 },
            P: { min: 0, max: 1.5 },
            T: { min: 40, max: 80 },
        },
    ];

    function updateScale(scale, value, minLimit = 0) {
        // ignore non-positive values (keeps axis stable)
        if (value <= 0 || !isFinite(value)) return;
        // grow quickly if close to max
        if (value > scale.max * 0.9) {
            scale.max = Math.max(value * SCALE_MARGIN, scale.max);
        }
        // shrink slowly if much lower than current max
        if (value < scale.max * 0.3 && scale.max > minLimit) {
            scale.max = Math.max(value * SCALE_MARGIN, minLimit);
        }
    }

    // WebSocket: accepts per-motor msg { motor: "M1", V:..., current_A:..., temp_C:... }
    // or full message { motors: { M1:{...}, M2:{...} } }
    const ws = new WebSocket(`ws://${location.hostname}:81`);
    ws.onopen = () => console.log("ws open");
    ws.onclose = () => console.log("ws closed");
    ws.onerror = (e) => console.warn("ws err", e);
    ws.onmessage = (e) => {
        try {
            const msg = JSON.parse(e.data);

            // Case 1: single motor update
            if (msg.motor && motorCache[msg.motor]) {
                motorCache[msg.motor] = msg;
                return;
            }

            // Case 2: batch update
            if (msg.motors) {
                Object.keys(msg.motors).forEach((k) => {
                    if (motorCache[k]) {
                        motorCache[k] = msg.motors[k];
                    }
                });
            }
        } catch (err) {
            console.warn("ws parse error", err);
        }
    };

    // Chart creation helper - datasets bound to yV,yI,yP,yT
    function makeChart(id) {
        const ctx = document.getElementById(id).getContext("2d");
        return new Chart(ctx, {
            type: "line",
            data: {
                labels: [],
                datasets: [
                    {
                        label: "Voltage",
                        data: [],
                        borderColor: "#1f77b4",
                        yAxisID: "yV",
                        borderWidth: 2,
                        pointRadius: 0,
                        hitRadius: 15, // 👈 larger hover area
                        tension: 0.25,
                    },
                    {
                        label: "Current",
                        data: [],
                        borderColor: "#ff7f0e",
                        yAxisID: "yI",
                        borderWidth: 2,
                        pointRadius: 0,
                        hitRadius: 15, // 👈 larger hover area
                        tension: 0.25,
                    },
                    {
                        label: "Power",
                        data: [],
                        borderColor: "#6f42c1",
                        yAxisID: "yP",
                        borderWidth: 2,
                        pointRadius: 0,
                        hitRadius: 15, // 👈 larger hover area
                        tension: 0.25,
                    },
                    {
                        label: "Temp",
                        data: [],
                        borderColor: "#d62828",
                        yAxisID: "yT",
                        borderWidth: 2,
                        pointRadius: 0,
                        hitRadius: 15, // 👈 larger hover area
                        tension: 0.25,
                    },
                ],
            },
            options: {
                interaction: {
                    mode: "nearest",
                    intersect: false,
                    axis: "x",
                },
                hover: {
                    mode: "nearest",
                    intersect: false,
                },

                maintainAspectRatio: false,
                animation: false,
                plugins: { legend: { display: false } },
                scales: {
                    x: {
                        grid: { display: false },
                        ticks: {
                            font: { size: 9 },
                            maxTicksLimit: 10,
                            color: "#6b7280",
                        },
                    },
                    yV: {
                        min: 0,
                        max: 250,
                        position: "left",
                        grid: { color: "#e0e0e0" },
                        ticks: {
                            font: { size: 10 },
                            maxTicksLimit: 5,
                            color: "#1f77b4",
                            callback: (v) =>
                                typeof v === "number" ? `${v} V` : v,
                        },
                    },
                    yI: {
                        min: 0,
                        max: 2,
                        position: "left",
                        grid: { display: false },
                        ticks: {
                            font: { size: 10 },
                            maxTicksLimit: 5,
                            color: "#ff7f0e",
                            callback: (v) =>
                                typeof v === "number" ? `${v} A` : v,
                        },
                    },
                    yP: {
                        min: 0,
                        max: 4,
                        position: "right",
                        grid: { display: false },
                        ticks: {
                            font: { size: 10 },
                            maxTicksLimit: 5,
                            color: "#6f42c1",
                            callback: (v) =>
                                typeof v === "number" ? `${v} kW` : v,
                        },
                    },
                    yT: {
                        min: 40,
                        max: 80,
                        position: "right",
                        grid: { display: false },
                        ticks: {
                            font: { size: 10 },
                            maxTicksLimit: 5,
                            color: "#d62828",
                            callback: (v) =>
                                typeof v === "number" ? `${v} °C` : v,
                        },
                    },
                },
                elements: { point: { radius: 0 } },
            },
        });
    }

    const charts = [makeChart("c1"), makeChart("c2"), makeChart("c3")];
    const MAX_POINTS = 120;

    // formatting helpers used in UI text (not axis)
    function formatPower(n) {
        if (!isFinite(n)) return "0.00 kW";
        return n >= 1 ? n.toFixed(1) + " kW" : n.toFixed(2) + " kW";
    }
    function formatTemp(n) {
        if (!isFinite(n)) return "0.0 °C";
        return n.toFixed(1) + " °C";
    }

    // Main UI update loop (reads from motorCache)
    setInterval(() => {
        let totalPower = 0;

        ["M1", "M2", "M3"].forEach((key, idx) => {
            const M = motorCache[key] || {};

            const V = Number(M.V || 0);
            const I = Number(M.I || 0);
            // Backend sends Power in Watts, convert to kW
            const P = Number(M.P || 0) / 1000.0;
            const T = Number(M.T || 0);

            totalPower += isFinite(P) ? P : 0;

            // autoscale
            const s = scaleState[idx];
            updateScale(s.V, V, 50);
            updateScale(s.I, I, 0.5);
            updateScale(s.P, P, 0.05); // power typically small
            updateScale(s.T, T, 30);

            // update text metrics (safe DOM access)
            const elV = document.getElementById(`m${idx + 1}V`);
            const elI = document.getElementById(`m${idx + 1}I`);
            const elP = document.getElementById(`m${idx + 1}P`);
            const elT = document.getElementById(`m${idx + 1}T`);

            if (elV) elV.innerText = Math.round(V) + " V";
            if (elI) elI.innerText = I.toFixed(2) + " A";
            if (elP) elP.innerText = formatPower(P);
            if (elT) elT.innerText = isFinite(T) ? formatTemp(T) : "—";

            // update chart datasets
            const c = charts[idx];
            const now = new Date().toLocaleTimeString([], {
                hour: "2-digit",
                minute: "2-digit",
                second: "2-digit",
            });
            c.data.labels.push(now);
            c.data.datasets[0].data.push(V);
            c.data.datasets[1].data.push(I);
            c.data.datasets[2].data.push(P);
            c.data.datasets[3].data.push(isFinite(T) ? T : null);

            // trim
            if (c.data.labels.length > MAX_POINTS) {
                c.data.labels.shift();
                c.data.datasets.forEach((ds) => ds.data.shift());
            }

            // apply smooth axis limits (round to nice values) #CHANGED

            // c.options.scales.yV.max = Math.ceil(s.V.max / 10) * 10; #CHANGED
            // c.options.scales.yI.max = Number(s.I.max.toFixed(2)); #CHANGED
            // c.options.scales.yP.max = Number(s.P.max.toFixed(3)); #CHANGED
            // c.options.scales.yT.max = Math.ceil(s.T.max); #CHANGED

            // update chart without anim
            c.update("none");
        });

        // update total power display
        const tpEl = document.getElementById("totalPower");
        if (tpEl) {
            tpEl.innerText =
                totalPower >= 1
                    ? totalPower.toFixed(1) + " kW"
                    : totalPower.toFixed(2) + " kW";
        }

        // event log: check motorCache for warnings (high current)
        const logEl = document.getElementById("eventLog");
        if (logEl) {
            Object.keys(motorCache).forEach((k) => {
                const m = motorCache[k];
                if (m && m.current_A && m.current_A > 6) {
                    const time = new Date().toLocaleTimeString();
                    const div = document.createElement("div");
                    div.className = "log-item";
                    div.innerText = `${time} - ${k} Current Spike (Warning)`;
                    logEl.prepend(div);
                    while (logEl.children.length > 40)
                        logEl.removeChild(logEl.lastChild);
                }
            });
        }
    }, UI_UPDATE_MS);
});

</script>
    </body>
</html>

)rawliteral";
