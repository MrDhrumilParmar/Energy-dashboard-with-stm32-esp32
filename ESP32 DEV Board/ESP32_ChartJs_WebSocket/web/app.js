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
