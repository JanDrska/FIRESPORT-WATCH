/**
 * hmi.bundle.js
 * Unified HMI frontend script (no build system).
 *
 * This file is a readability-focused "single JS" variant:
 * - It concatenates the previous modular files in a fixed, safe order.
 * - It keeps clear section blocks so it's easy to navigate.
 *
 * Generated on 2026-02-13.
 */
/* eslint-disable no-unused-vars */
/* eslint-disable no-undef */



/* ==========================================================================
   SECTION: hmi.utils
   Source: js/hmi.utils.js
   ========================================================================== */

window.HMI = window.HMI || {};

function isIOS() {
  const ua = navigator.userAgent || "";
  const iDevice = /iPad|iPhone|iPod/.test(ua);
  const iPadOS13Plus = (navigator.platform === "MacIntel" && navigator.maxTouchPoints > 1);
  return iDevice || iPadOS13Plus;
}

function deepMerge(base, extra) {
  if (!extra) return structuredClone(base);
  const out = structuredClone(base);
  const isObj = (v) => v && typeof v === "object" && !Array.isArray(v);
  function mergeInto(dst, src) {
    for (const k of Object.keys(src)) {
      const sv = src[k];
      const dv = dst[k];
      if (isObj(dv) && isObj(sv)) mergeInto(dv, sv);
      else dst[k] = structuredClone(sv);
    }
  }
  mergeInto(out, extra);
  return out;
}

function mergeInto(dst, src) {
    for (const k of Object.keys(src)) {
      const sv = src[k];
      const dv = dst[k];
      if (isObj(dv) && isObj(sv)) mergeInto(dv, sv);
      else dst[k] = structuredClone(sv);
    }
  }

function clamp(v, lo, hi) { return Math.max(lo, Math.min(hi, v)); }

function parseLiteral(v, fallback) {
  if (typeof v === "boolean") return v;
  if (typeof v === "number" && Number.isFinite(v)) return v;
  if (typeof v === "string") {
    const s = v.trim().toLowerCase();
    if (s === "true") return true;
    if (s === "false") return false;
    const num = Number(s);
    if (Number.isFinite(num)) return num;
  }
  return fallback;
}

function ensureDefaults(b) {
  b.style = b.style || {};
  b.style.fontSize = Number(b.style.fontSize ?? 22);
  b.style.radius = Number(b.style.radius ?? 12);
  b.style.boxW = Number(b.style.boxW ?? 320);
  b.style.boxH = Number(b.style.boxH ?? 48);
  b.style.onColor = b.style.onColor ?? "#2ecc71";
  b.style.offColor = b.style.offColor ?? "#bbbbbb";
  b.style.textColor = b.style.textColor ?? "#ffffff";
  b.decimals = Number(b.decimals ?? 1);
  b.unit = b.unit ?? "";
  b.label = b.label ?? b.id ?? "";
  b.bind = b.bind || { key: "", type: "bool" };
  b.blink = b.blink || { enabled: false, key: "", color: "#f1c40f", hz: 2 };

  if (b.type === "button") {
    b.button = b.button || { text: b.label || "BTN", action: "toggle", key: "", stateKey: "", onValue: 1, offValue: 0, colorMode: "stroke", colorKey: "", visKey: "", visMode: "hideIfEqual", visValue: 1 };
    b.button.stateKey = (b.button.stateKey ?? "") + "";
    b.button.key = (b.button.key ?? "") + "";
    b.button.colorMode = (b.button.colorMode ?? "stroke") + "";
    b.button.colorKey = (b.button.colorKey ?? "") + "";
    b.button.visKey = (b.button.visKey ?? "") + "";
    b.button.visMode = (b.button.visMode ?? "hideIfEqual") + "";
    b.button.visValue = parseLiteral(b.button.visValue, 1);
    b.button.onValue = parseLiteral(b.button.onValue, 1);
    b.button.offValue = parseLiteral(b.button.offValue, 0);
  }

  b.style.fontWeight = b.style.fontWeight ?? "600";
  b.style.fontStyle  = b.style.fontStyle  ?? "normal";
  b.style.textAnchor = b.style.textAnchor ?? "start";
  b.style.strokeWidth = Number(b.style.strokeWidth ?? 2);

  if (b.type === "labelValue") {
    b.bind = b.bind || { key: "", type: "number" };
    b.bind.type = "number";
    b.style.boxW = Number(b.style.boxW ?? 320);
    b.style.boxH = Number(b.style.boxH ?? 48);
  }

  if (b.type === "text") {
    b.text = (b.text ?? b.label ?? b.id ?? "") + "";
    b.style.boxW = Number(b.style.boxW ?? 320);
    b.style.boxH = Number(b.style.boxH ?? 40);
  }

  if (b.type === "rect") {
    b.style.boxW = Number(b.style.boxW ?? 240);
    b.style.boxH = Number(b.style.boxH ?? 120);
    b.style.radius = Number(b.style.radius ?? 12);
    b.style.boxBg = b.style.boxBg ?? "#000000";
    b.style.offColor = b.style.offColor ?? "#bbbbbb";
    b.style.strokeWidth = Number(b.style.strokeWidth ?? 2);
  }

  if (b.type === "inputSend" || b.type === "inputEnter") {
    b.input = b.input || { mode: "setpoint", key: "", sig: "", placeholder: "", showType: true };
    if (!b.input.mode) b.input.mode = "setpoint";
    if (b.input.sig === undefined || b.input.sig === null) b.input.sig = "";
    if (b.input.addr === undefined || b.input.addr === null || b.input.addr === "") b.input.addr = 0;

    b.input.mode = (b.input.mode ?? "setpoint") + "";
    b.input.key = (b.input.key ?? "") + "";
    b.input.placeholder = (b.input.placeholder ?? "") + "";
    b.input.showType = (b.input.showType === undefined) ? true : !!b.input.showType;
    b.input.sig = (b.input.sig ?? "") + "";
    b.input.addr = Number(b.input.addr || 0);

    b.style.boxW = Number(b.style.boxW ?? 320);
    b.style.boxH = Number(b.style.boxH ?? 56);
  }
  return b;
}

function getScreenSvg(route) {
  if (route === "overview") return document.getElementById("overviewSvg");
  if (route === "control") return document.getElementById("controlSvg");
  if (route === "io") return document.getElementById("ioSvg");
  return null;
}

function getHtmlOverlay(route) {
  if (route === "overview") return document.getElementById("overviewHtml");
  if (route === "control") return document.getElementById("controlHtml");
  if (route === "io") return document.getElementById("ioHtml");
  return null;
}

function vbRectToOverlayPx(svg, overlay, x, y, w, h) {
  const ctm = svg.getScreenCTM();
  if (!ctm) return null;

  const pt1 = svg.createSVGPoint(); pt1.x = x;     pt1.y = y;
  const pt2 = svg.createSVGPoint(); pt2.x = x + w; pt2.y = y + h;

  const p1 = pt1.matrixTransform(ctm);
  const p2 = pt2.matrixTransform(ctm);

  const r = overlay.getBoundingClientRect();
  const left = p1.x - r.left;
  const top  = p1.y - r.top;
  const width  = (p2.x - p1.x);
  const height = (p2.y - p1.y);

  return { left, top, width, height };
}

function truthy(v) {
  if (typeof v === "boolean") return v;
  if (typeof v === "number") return v !== 0;
  if (typeof v === "string") {
    const s = v.trim().toLowerCase();
    if (["0","false","off","no","n"].includes(s)) return false;
    if (["1","true","on","yes","y"].includes(s)) return true;
    return s.length > 0;
  }
  return !!v;
}

function formatValue(b, val) {
  if (b.bind?.type === "number") {
    if (typeof val !== "number") return "—";
    const dec = Number.isFinite(b.decimals) ? b.decimals : 1;
    return val.toFixed(dec) + (b.unit ? " " + b.unit : "");
  }
  if (typeof val === "boolean") return val ? "ON" : "OFF";
  if (typeof val === "number") return val !== 0 ? "ON" : "OFF";
  return "—";
}

function baseColorFor(b, val) {
  if (b.bind?.type === "number") {
    return (typeof val === "number") ? (b.style.onColor) : (b.style.offColor);
  }
  return truthy(val) ? b.style.onColor : b.style.offColor;
}

function shouldBlink(b, signals, mainVal) {
  if (!backendOnline) return false;
  if (!b.blink?.enabled) return false;
  const k = (b.blink.key || "").trim();
  if (k) return truthy(signals?.[k]);
  if (b.bind?.type === "bool") return truthy(mainVal);
  if (b.bind?.type === "number") return (typeof mainVal === "number");
  return false;
}

function blinkPhase(hz, tMs) {
  const period = 1000 / Math.max(0.1, hz || 2);
  return Math.floor(tMs / period) % 2;
}

function normalizeVal(x) {
          if (typeof x === "string") {
            const s = x.trim().toLowerCase();
            if (s === "true") return true;
            if (s === "false") return false;
            const n = Number(s);
            if (!Number.isNaN(n) && s !== "") return n;
            return s;
          }
          return x;
        }

function equalLoose(a, b) {
          const aa = normalizeVal(a);
          const bb = normalizeVal(b);

          if (typeof aa === "number" && typeof bb === "number") return aa === bb;
          if (typeof aa === "boolean" && typeof bb === "boolean") return aa === bb;
          if (typeof aa === "number" && typeof bb === "boolean") return (aa !== 0) === bb;
          if (typeof aa === "boolean" && typeof bb === "number") return aa === (bb !== 0);
          return String(aa) === String(bb);
        }

function snap(v) {
  const on = document.getElementById("snapOn")?.checked;
  if (!on) return v;
  const g = Number(document.getElementById("gridSize")?.value || 10);
  return Math.round(v / g) * g;
}

function svgPoint(svg, e) {
  const pt = svg.createSVGPoint();
  pt.x = e.clientX; pt.y = e.clientY;
  return pt.matrixTransform(svg.getScreenCTM().inverse());
}

function applyScreenSizing() {
  const o = MACHINE.screens.overview;

  // overview: podle obrázku
  document.getElementById("overviewBg").src = o.background.src;
  document.getElementById("overviewSvg")
    .setAttribute("viewBox", `0 0 ${o.background.viewBox.w} ${o.background.viewBox.h}`);

  // control: podle canvas
  const c = MACHINE.screens.control?.canvas || { w: 1100, h: 260 };
  document.getElementById("controlSvg")
    .setAttribute("viewBox", `0 0 ${c.w} ${c.h}`);

  // io: podle canvas
  const i = MACHINE.screens.io?.canvas || { w: 1100, h: 220 };
  document.getElementById("ioSvg")
    .setAttribute("viewBox", `0 0 ${i.w} ${i.h}`);

  // iOS fix: drž wrapper aspect ratio podle viewBox
  const scene = document.querySelector("#screen-overview .scene");
  if (scene && o?.background?.viewBox?.w && o?.background?.viewBox?.h) {
    scene.style.aspectRatio = `${o.background.viewBox.w} / ${o.background.viewBox.h}`;
  }

}


/* ==========================================================================
   SECTION: hmi.globals
   Source: js/hmi.globals.js
   ========================================================================== */

/**
 * Global error hooks (stability): keep crashes visible and avoid silent freeze.
 */
(function installGlobalErrorHooks(){
  const markOffline = () => { try { if (typeof setStatus === 'function') setStatus(false, false); } catch {} };
  window.addEventListener('error', (ev) => {
    console.error('Uncaught error:', ev?.error || ev?.message || ev);
    markOffline();
  });
  window.addEventListener('unhandledrejection', (ev) => {
    console.error('Unhandled promise rejection:', ev?.reason || ev);
    markOffline();
  });
})();

/**
 * NOTE:
 * - API (fetch/write) helpers are in `hmi.api.js`.
 */

/**
 * NOTE:
 * - Pure helper functions are in `hmi.utils.js` (loaded before this file).
 * - This file keeps global constants + state + main app functions.
 */

/* ================================
   ADMIN
================================= */
const ADMIN_PASSWORD = "1234";
const ADMIN_FLAG_KEY = "hmi_adminUnlocked";

/** iOS detekce (včetně iPadOS, který se hlásí jako Mac) */


/** =========================================================
 *  Univerzální HMI – konfigurace v JS
 *  ========================================================= */
const DEFAULT_MACHINE = {
  title: "M.A.R.S.",
  subtitle: "Mobilní Automatická Rentgenovací Stanice",
  defaults: {
    boxBg: "#000000",
    boxAlpha: 0.55,
    palette: [
      { name:"červená", value:"#e74c3c" },
      { name:"zelená", value:"#2ecc71" },
      { name:"šedá", value:"#bbbbbb" },
      { name:"černá", value:"#000000" },
      { name:"bílá", value:"#ffffff" },
      { name:"oranžová", value:"#f39c12" },
      { name:"modrá", value:"#3498db" },
      { name:"žlutá", value:"#f1c40f" }
    ]
  },
  api: {
    stateUrl: "/api/state",
    writeTemplate: "/api/setpoint/{key}/{value}"
  },
  screens: {
    overview: {
      kind: "scene",
      background: { src: "img/stroj.png", viewBox: { w: 1600, h: 2000 } },
      blocks: []
    },
    control: {
      kind: "canvas",
      canvas: { w: 1100, h: 260 },
      blocks: []
    },
    io: {
      kind: "canvas",
      canvas: { w: 1100, h: 220 },
      blocks: []
    }
  }
};


const MACHINE = deepMerge(DEFAULT_MACHINE, window.MACHINE_CONFIG);

/** =========================================================
 *  Osy / IO / kamery
 *  ========================================================= */
const AXIS_NAMES = ["Řetězový dopravník","Rotace vozíku","Výtah detektor","Posun detektoru","Výtah rentgen","Posuv rentgenu"];
const AXIS_UNITS = ["mm","°","mm","mm","mm","mm"];
const AXIS_MAX = ["1000","360","300","250","200","136"];

const AXES = Array.from({length: 6}, (_, i) => {
  const n = i + 1;
  return {
    axis: n,
    name: AXIS_NAMES[i],
    setpoint: `Setpoint_axis_${n}`,
    feedback: `axis${n}_pos`,
    min: 0,
    max: Number(AXIS_MAX[i]),
    unit: AXIS_UNITS[i],
    speedDefault: 20,
    speedMin: 0.1,
    speedMax: 200
  };
});

const CAMERAS = [
  { name: "Kamera 1", url: "" },
  { name: "Kamera 2", url: "" },
  { name: "Kamera 3", url: "" },
  { name: "Kamera 4", url: "" }
];

let last = null;
let backendOnline = false;
let loadInFlight = false;
let loadSeq = 0;
let forcedOffline = false;
let activeCtrl = null;

const jogTimers = {};

/** =========================================================
 *  ONLINE/OFFLINE + hystereze + watchdog
 *  ========================================================= */
let okStreak = 0;
let failStreak = 0;

// kolik po sobě jdoucích výsledků musí nastat, než UI přepne stav
const FAIL_TO_OFFLINE = 3; // 3 chyby za sebou => offline
const OK_TO_ONLINE = 2;    // 2 úspěchy za sebou => online

// watchdog: když dlouho nepřišlo OK, spadni do offline
let lastOkAt = 0;
const OFFLINE_AFTER_MS = 1500;

// polling interval (fixed-rate)
const POLL_MS = 500;

/** =========================================================
 *  Routing
 *  ========================================================= */
function setStatus(apiOk, modbusOk) {
  window.__apiOk = !!apiOk;
  window.__modbusOk = (modbusOk === undefined) ? true : !!modbusOk;

  backendOnline = window.__apiOk && window.__modbusOk;

  const el = document.getElementById("status");
  if (!apiOk) {
    el.textContent = "OFFLINE";
    el.className = "pill err";
  } else if (!window.__modbusOk) {
    el.textContent = "API OK / MODBUS OFF";
    el.className = "pill err";
  } else {
    el.textContent = "OK";
    el.className = "pill ok";
  }

  // stop any jogging when Modbus link drops (or API drops)
  if (!window.__modbusOk || !window.__apiOk) {
    for (const k of Object.keys(jogTimers)) {
      if (jogTimers[k]) { clearInterval(jogTimers[k]); jogTimers[k] = null; }
    }
  }
}

function watchdog() {
  if (!lastOkAt) return;
  const age = performance.now() - lastOkAt;
  if (age > OFFLINE_AFTER_MS) {
    // status spadne i když se requesty zrovna "jen timeoutují"
    setStatus(false, false);
  }
}

function updateSignalDatalist(signals) {
  const dl = document.getElementById("signalKeys");
  if (!dl) return;
  const keys = Object.keys(signals || {}).sort();
  const fingerprint = keys.join("|");
  if (dl.dataset.fp === fingerprint) return;
  dl.dataset.fp = fingerprint;
  dl.innerHTML = "";
  for (const k of keys) {
    const opt = document.createElement("option");
    opt.value = k;
    dl.appendChild(opt);
  }
}

function setRoute(route) {
  if (route === "settings" && !isAdminUnlocked()) {
    if (!requireAdminOrRedirect()) return;
  }
  document.querySelectorAll(".tab").forEach(b => b.classList.toggle("active", b.dataset.route === route));
  document.querySelectorAll(".screen").forEach(s => s.classList.remove("active"));
  const scr = document.getElementById("screen-" + route);
  if (scr) scr.classList.add("active");
  syncEditorVisibility(route);
}

function currentRoute() {
  const h = (location.hash || "#overview").replace("#", "");
  return ["overview","control","io","cameras","settings"].includes(h) ? h : "overview";
}

/** =========================================================
 *  API write helper
 *  ========================================================= */
let _loadInFlight = false;


function ensureMsgEl(btn) {
  let el = btn?.nextElementSibling;
  if (!el || !el.classList?.contains("save-msg")) {
    el = document.createElement("span");
    el.className = "save-msg muted";
    btn?.insertAdjacentElement("afterend", el);
  }
  return el;
}

function flashSave(btn, ok, msg = "") {
  const el = ensureMsgEl(btn);
  el.textContent = msg || "";
  el.classList.toggle("err", !ok);
  el.classList.toggle("ok", ok);

  btn.classList.remove("flash-ok", "flash-err");
  btn.classList.add(ok ? "flash-ok" : "flash-err");

  window.clearTimeout(btn.__flashT);
  btn.__flashT = window.setTimeout(() => {
    btn.classList.remove("flash-ok", "flash-err");
    if (ok) el.textContent = "";
  }, 700);
}


/** =========================================================
 *  Osy
 *  ========================================================= */


function renderAxes() {
  const axes = document.getElementById("axes");
  axes.innerHTML = "";

  for (const cfg of AXES) {
    const label = document.createElement("div");
    label.className = "name";
    label.textContent = cfg.name || `Osa ${cfg.axis}`;

    const box = document.createElement("div");
    box.style.display = "flex";
    box.style.gap = "10px";
    box.style.alignItems = "center";
    box.style.flexWrap = "wrap";

    const fb = document.createElement("span");
    fb.id = `fb${cfg.axis}`;
    fb.className = "val";
    fb.textContent = "pos: —";

    const sp = document.createElement("input");
    sp.type = "number";
    sp.step = "0.1";
    sp.placeholder = cfg.setpoint;

    const speed = document.createElement("input");
    speed.type = "number";
    speed.step = "0.1";
    speed.value = cfg.speedDefault;
    speed.style.width = "120px";

    const btnSet = document.createElement("button");
    btnSet.textContent = "Nastavit";

    const btnHome = document.createElement("button");
    btnHome.textContent = "Home";

    const btnMinus = document.createElement("button");
    btnMinus.textContent = "Jog -";

    const btnPlus = document.createElement("button");
    btnPlus.textContent = "Jog +";

    async function doSetpoint(val) {
      const v = clamp(val, cfg.min, cfg.max);
      await apiWrite(cfg.setpoint, v);
      sp.value = v;
    }

    btnSet.addEventListener("click", async () => {
      const v = parseFloat(sp.value);
      if (isNaN(v)) return;
      await doSetpoint(v);
    });

    sp.addEventListener("keydown", async (e) => {
      if (e.key === "Enter") {
        const v = parseFloat(sp.value);
        if (isNaN(v)) return;
        await doSetpoint(v);
      }
    });

    btnHome.addEventListener("click", async () => { await doSetpoint(cfg.min); });

    function stopJog() {
      if (jogTimers[cfg.axis]) {
        clearInterval(jogTimers[cfg.axis]);
        jogTimers[cfg.axis] = null;
      }
    }
    function startJog(dir) {
      stopJog();
      let spVal = parseFloat(sp.value);
      if (isNaN(spVal)) {
        const fbVal = last?.signals?.[cfg.feedback];
        spVal = (typeof fbVal === "number") ? fbVal : cfg.min;
      }
      let spd = parseFloat(speed.value);
      if (isNaN(spd)) spd = cfg.speedDefault;
      spd = clamp(spd, cfg.speedMin, cfg.speedMax);
      speed.value = spd;
      const dt = 0.1;
      jogTimers[cfg.axis] = setInterval(async () => {
        spVal = clamp(spVal + dir * spd * dt, cfg.min, cfg.max);
        sp.value = spVal;
        await apiWrite(cfg.setpoint, spVal, { refresh: false });
      }, 100);
    }
    function bindHold(btn, dir) {
      btn.addEventListener("mousedown", () => startJog(dir));
      btn.addEventListener("mouseup", stopJog);
      btn.addEventListener("mouseleave", stopJog);

      btn.addEventListener("touchstart", (e) => { e.preventDefault(); startJog(dir); }, { passive: false });
      btn.addEventListener("touchend", stopJog);
      btn.addEventListener("touchcancel", stopJog);
    }
    bindHold(btnMinus, -1);
    bindHold(btnPlus, +1);

    box.appendChild(fb);
    box.appendChild(sp);
    box.appendChild(btnSet);
    box.appendChild(btnHome);
    box.appendChild(document.createTextNode("rychl.:"));
    box.appendChild(speed);
    box.appendChild(btnMinus);
    box.appendChild(btnPlus);

    axes.appendChild(label);
    axes.appendChild(box);
  }
}

/** =========================================================
 *  IO panel
 *  ========================================================= */
function isInputName(name) {
  const n = name.toLowerCase();
  return n.startsWith("di") || n.startsWith("ai") || n.startsWith("in_") || n.includes("input");
}
function isOutputName(name) {
  const n = name.toLowerCase();
  return n.startsWith("do") || n.startsWith("ao") || n.startsWith("coil") || n.startsWith("out_") || n.includes("output");
}
function addIoItem(container, name, val) {
  const dot = document.createElement("div");
  dot.className = "dot";
  if (typeof val === "boolean") dot.classList.add(val ? "on" : "off");
  else if (typeof val === "number") dot.classList.add(val !== 0 ? "on" : "off");
  else dot.classList.add("off");

  const lbl = document.createElement("div");
  lbl.className = "val";
  lbl.textContent = name;

  container.appendChild(dot);
  container.appendChild(lbl);
}
function renderIO(signals) {
  const ins = document.getElementById("inputsList");
  const outs = document.getElementById("outputsList");
  ins.innerHTML = "";
  outs.innerHTML = "";

  for (const [name, val] of Object.entries(signals || {})) {
    if (isOutputName(name) && !isInputName(name)) addIoItem(outs, name, val);
    else if (isInputName(name) && !isOutputName(name)) addIoItem(ins, name, val);
    else addIoItem(ins, name, val);
  }
  if (!ins.children.length) ins.innerHTML = "<div class='muted'>— žádné vstupy nenalezeny (DI*/AI*)</div>";
  if (!outs.children.length) outs.innerHTML = "<div class='muted'>— žádné výstupy nenalezeny (DO*/AO*/coil*)</div>";
}

/** =========================================================
 *  Kamery
 *  ========================================================= */
function renderCameras() {
  const grid = document.getElementById("camGrid");
  grid.innerHTML = "";

  for (const cam of CAMERAS) {
    const tile = document.createElement("div");
    tile.className = "camTile";

    const head = document.createElement("div");
    head.className = "camHead";

    const title = document.createElement("div");
    title.className = "name";
    title.textContent = cam.name;

    const badge = document.createElement("span");
    badge.className = "pill";
    badge.textContent = cam.url ? "ON" : "URL chybí";

    head.appendChild(title);
    head.appendChild(badge);

    const body = document.createElement("div");
    body.className = "camBody";

    if (cam.url) {
      const img = document.createElement("img");
      img.src = cam.url;
      img.alt = cam.name;
      body.appendChild(img);
    } else {
      const hint = document.createElement("div");
      hint.className = "camHint";
      hint.textContent = "Doplň URL do pole CAMERAS v JS";
      body.appendChild(hint);
    }

    tile.appendChild(head);
    tile.appendChild(body);
    grid.appendChild(tile);
  }
}

/** =========================================================
 *  Bloky – renderer (SVG)
 *  ========================================================= */
const SVG_NS = "http://www.w3.org/2000/svg";


function clearSvg(svg) { if (svg) svg.innerHTML = ""; }
function clearOverlay(route) {
  const ov = getHtmlOverlay(route);
  if (ov) ov.innerHTML = "";
}

/** Přepočet viewBox souřadnic na px v overlay (přes getScreenCTM – sedí i při letterbox) */


function drawInputInOverlay(route, svg, b) {
  const overlay = getHtmlOverlay(route);
  if (!overlay) return;

  const rect = vbRectToOverlayPx(svg, overlay, b.x, b.y, b.style.boxW, b.style.boxH);
  if (!rect) return;

  const wrap = document.createElement("div");
  wrap.className = "foWrap";
  wrap.style.position = "absolute";
  wrap.style.left = rect.left + "px";
  wrap.style.top = rect.top + "px";
  wrap.style.width = rect.width + "px";
  wrap.style.height = rect.height + "px";
  wrap.style.boxSizing = "border-box";

  const bg = (b.style?.boxBg ?? MACHINE.defaults?.boxBg ?? "#000000");
  const a  = (b.style?.boxAlpha ?? MACHINE.defaults?.boxAlpha ?? 0.55);

  wrap.style.background = bg;
  wrap.style.opacity = a;
  wrap.style.borderRadius = "12px";
  wrap.style.padding = "6px";
  wrap.style.display = "flex";
  wrap.style.gap = "8px";
  wrap.style.alignItems = "center";
  wrap.style.pointerEvents = "auto";

  if (b.input?.showType) {
    const pill = document.createElement("span");
    pill.className = "typePill";
    const mode = (b.input?.mode || "setpoint").toLowerCase();
    pill.textContent =
      (mode === "u16") ? "U16" :
      (mode === "s16") ? "S16" :
      (mode === "raw") ? "RAW" : "SET";
    wrap.appendChild(pill);
  }

  const inp = document.createElement("input");
  inp.type = "text";
  inp.className = "hmiInput";
  inp.dataset.route = route;
  inp.dataset.id = b.id;
  inp.placeholder = b.input?.placeholder || "";
  inp.style.flex = "1 1 auto";
  inp.style.width = "auto";
  inp.style.minWidth = "0";

  // iOS: vypne auto-zoom, když je font < 16px (pro jistotu držíme 16)
  inp.style.fontSize = "16px";

  wrap.appendChild(inp);

  if (b.type === "inputSend") {
    const btn = document.createElement("button");
    btn.type = "button";
    btn.className = "hmiInputSend";
    btn.textContent = "Odeslat";
    btn.dataset.route = route;
    btn.dataset.id = b.id;
    wrap.appendChild(btn);
  }

  overlay.appendChild(wrap);
}

function drawBlock(svg, route, block) {
  const b = ensureDefaults(structuredClone(block));
  const g = document.createElementNS(SVG_NS, "g");
  g.setAttribute("data-id", b.id);
  g.setAttribute("data-route", route);
  g.style.cursor = (editMode && isEditableRoute(route)) ? "move" : "default";

  const sel = document.createElementNS(SVG_NS, "rect");
  sel.setAttribute("id", `sel_${route}_${b.id}`);
  sel.setAttribute("x", b.x - 8);
  sel.setAttribute("y", b.y - 40);
  sel.setAttribute("width", Math.max(60, b.style.boxW + 60));
  sel.setAttribute("height", Math.max(60, b.style.boxH + 60));
  sel.setAttribute("rx", 12);
  sel.setAttribute("fill", "none");
  sel.setAttribute("stroke", "transparent");
  sel.setAttribute("stroke-width", 2);
  g.appendChild(sel);

  if (b.type === "dotOnly") {
    const dot = document.createElementNS(SVG_NS, "circle");
    dot.setAttribute("id", `dot_${route}_${b.id}`);
    dot.setAttribute("cx", b.x);
    dot.setAttribute("cy", b.y);
    dot.setAttribute("r", b.style.radius);
    dot.setAttribute("fill", b.style.offColor);
    dot.setAttribute("stroke", "#fff");
    dot.setAttribute("stroke-width", 2);
    g.appendChild(dot);
  }

  if (b.type === "dotLabel" || b.type === "valueDot") {
    const dot = document.createElementNS(SVG_NS, "circle");
    dot.setAttribute("id", `dot_${route}_${b.id}`);
    dot.setAttribute("cx", b.x);
    dot.setAttribute("cy", b.y);
    dot.setAttribute("r", b.style.radius);
    dot.setAttribute("fill", b.style.offColor);
    dot.setAttribute("stroke", "#fff");
    dot.setAttribute("stroke-width", 2);

    const box = document.createElementNS(SVG_NS, "rect");
    box.setAttribute("id", `box_${route}_${b.id}`);
    box.setAttribute("x", b.x + b.style.radius + 10);
    box.setAttribute("y", b.y - (b.style.boxH / 2));
    box.setAttribute("width", b.style.boxW);
    box.setAttribute("height", b.style.boxH);
    box.setAttribute("rx", 10);
    box.setAttribute("fill", b.style.boxBg || "#000");
    box.setAttribute("opacity", Number.isFinite(b.style.boxAlpha) ? b.style.boxAlpha : 0.55);
    box.setAttribute("stroke", b.style.offColor);
    box.setAttribute("stroke-width", 2);

    const txt = document.createElementNS(SVG_NS, "text");
    txt.setAttribute("id", `txt_${route}_${b.id}`);
    txt.setAttribute("x", b.x + b.style.radius + 22);
    txt.setAttribute("y", b.y + (b.style.fontSize * 0.35));
    txt.setAttribute("font-size", b.style.fontSize);
    txt.setAttribute("fill", b.style.textColor);
    txt.textContent = b.label;

    g.appendChild(dot);
    g.appendChild(box);
    g.appendChild(txt);
  }

  if (b.type === "button") {
    const box = document.createElementNS(SVG_NS, "rect");
    box.setAttribute("id", `btn_${route}_${b.id}`);
    box.setAttribute("x", b.x);
    box.setAttribute("y", b.y);
    box.setAttribute("width", b.style.boxW);
    box.setAttribute("height", b.style.boxH);
    box.setAttribute("rx", 14);
    box.setAttribute("fill", "rgba(0,0,0,0.45)");
    box.setAttribute("stroke", "#ddd");
    box.setAttribute("stroke-width", 2);

    const txt = document.createElementNS(SVG_NS, "text");
    txt.setAttribute("id", `btntxt_${route}_${b.id}`);
    txt.setAttribute("x", b.x + b.style.boxW/2);
    txt.setAttribute("y", b.y + b.style.boxH/2 + (b.style.fontSize*0.35));
    txt.setAttribute("font-size", b.style.fontSize);
    txt.setAttribute("fill", b.style.textColor);
    txt.setAttribute("text-anchor", "middle");
    txt.textContent = b.button?.text || b.label || b.id;

    g.appendChild(box);
    g.appendChild(txt);
  }

  if (b.type === "labelValue") {
    const box = document.createElementNS(SVG_NS, "rect");
    box.setAttribute("id", `box_${route}_${b.id}`);
    box.setAttribute("x", b.x);
    box.setAttribute("y", b.y);
    box.setAttribute("width", b.style.boxW);
    box.setAttribute("height", b.style.boxH);
    box.setAttribute("rx", 10);
    box.setAttribute("fill", b.style.boxBg || "#000");
    box.setAttribute("opacity", Number.isFinite(b.style.boxAlpha) ? b.style.boxAlpha : 0.55);
    box.setAttribute("stroke", b.style.offColor);
    box.setAttribute("stroke-width", 2);

    const txt = document.createElementNS(SVG_NS, "text");
    txt.setAttribute("id", `txt_${route}_${b.id}`);
    txt.setAttribute("x", b.x + 14);
    txt.setAttribute("y", b.y + (b.style.boxH/2) + (b.style.fontSize*0.35));
    txt.setAttribute("font-size", b.style.fontSize);
    txt.setAttribute("fill", b.style.textColor);
    txt.textContent = b.label || "";

    g.appendChild(box);
    g.appendChild(txt);
  }

  if (b.type === "text") {
    const txt = document.createElementNS(SVG_NS, "text");
    txt.setAttribute("id", `txt_${route}_${b.id}`);
    txt.setAttribute("x", b.x);
    txt.setAttribute("y", b.y);
    txt.setAttribute("font-size", b.style.fontSize);
    txt.setAttribute("fill", b.style.textColor);
    txt.setAttribute("font-weight", b.style.fontWeight);
    txt.setAttribute("font-style", b.style.fontStyle);
    txt.setAttribute("text-anchor", b.style.textAnchor);
    txt.textContent = (b.text ?? b.label ?? b.id ?? "");
    g.appendChild(txt);
  }

  if (b.type === "rect") {
    const r = document.createElementNS(SVG_NS, "rect");
    r.setAttribute("id", `rect_${route}_${b.id}`);
    r.setAttribute("x", b.x);
    r.setAttribute("y", b.y);
    r.setAttribute("width", b.style.boxW);
    r.setAttribute("height", b.style.boxH);
    r.setAttribute("rx", b.style.radius);
    r.setAttribute("fill", b.style.boxBg || "#000");
    r.setAttribute("opacity", Number.isFinite(b.style.boxAlpha) ? b.style.boxAlpha : 0.25);
    r.setAttribute("stroke", b.style.offColor || "#bbb");
    r.setAttribute("stroke-width", b.style.strokeWidth || 2);
    g.appendChild(r);
  }

  if (b.type === "inputSend" || b.type === "inputEnter") {
    // iOS FIX: nepoužívat foreignObject (Safari ho škáluje/posouvá), ale HTML overlay nad SVG
    if (isIOS()) {
      // (volitelné) jemná linka v SVG pro vizuální "kotvu" v edit režimu
      const box = document.createElementNS(SVG_NS, "rect");
      box.setAttribute("x", b.x);
      box.setAttribute("y", b.y);
      box.setAttribute("width", b.style.boxW);
      box.setAttribute("height", b.style.boxH);
      box.setAttribute("rx", 12);
      box.setAttribute("fill", "transparent");
      box.setAttribute("stroke", "transparent");
      g.appendChild(box);

      // skutečné ovládací prvky do overlay (px přes getScreenCTM)
      // (overlay se čistí v renderBlocks)
      // pozor: musíme je kreslit až když je svg už v DOM -> tady už je
      drawInputInOverlay(route, svg, b);
    } else {
      // ostatní prohlížeče: foreignObject OK
      const fo = document.createElementNS(SVG_NS, "foreignObject");
      fo.setAttribute("x", b.x);
      fo.setAttribute("y", b.y);
      fo.setAttribute("width", b.style.boxW);
      fo.setAttribute("height", b.style.boxH);

      const div = document.createElement("div");
      div.setAttribute("xmlns", "http://www.w3.org/1999/xhtml");
      div.className = "foWrap";
      const bg = (b.style?.boxBg ?? MACHINE.defaults?.boxBg ?? "#000000");
      const a  = (b.style?.boxAlpha ?? MACHINE.defaults?.boxAlpha ?? 0.55);
      div.style.background = bg;
      div.style.opacity = a;
      div.style.borderRadius = "12px";
      div.style.padding = "6px";

      const inp = document.createElement("input");
      inp.type = "text";
      inp.className = "hmiInput";
      inp.dataset.route = route;
      inp.dataset.id = b.id;
      inp.placeholder = b.input?.placeholder || "";

      const hasPill = !!b.input?.showType;
      const minus = (b.type === "inputSend") ? 80 : 20;
      const minusPill = hasPill ? 52 : 0;
      inp.style.width = Math.max(70, b.style.boxW - minus - minusPill) + "px";

      if (b.input?.showType) {
        const pill = document.createElement("span");
        pill.className = "typePill";
        const mode = (b.input?.mode || "setpoint").toLowerCase();
        pill.textContent =
          (mode === "u16") ? "U16" :
          (mode === "s16") ? "S16" :
          (mode === "raw") ? "RAW" : "SET";
        div.appendChild(pill);
      }

      div.appendChild(inp);

      if (b.type === "inputSend") {
        const btn = document.createElement("button");
        btn.type = "button";
        btn.className = "hmiInputSend";
        btn.textContent = "Odeslat";
        btn.dataset.route = route;
        btn.dataset.id = b.id;
        div.appendChild(btn);
      }

      fo.appendChild(div);
      g.appendChild(fo);
    }
  }

  const interactive = (b.type === "button" || b.type === "inputSend" || b.type === "inputEnter");
  g.style.pointerEvents = (editMode && isEditableRoute(route)) ? "auto" : (interactive ? "auto" : "none");

  if (selected && selected.route === route && selected.id === b.id) {
    sel.setAttribute("stroke", "#333");
  }

  svg.appendChild(g);
}

function renderBlocks(route) {
  const cfg = MACHINE.screens[route];
  const svg = getScreenSvg(route);
  if (!cfg || !svg) return;

  clearSvg(svg);
  clearOverlay(route); // <<< důležité: vždy přepočítat HTML inputy

  for (const blk of cfg.blocks) drawBlock(svg, route, blk);
}

function renderAllBlocks() {
  renderBlocks("overview");
  renderBlocks("control");
  renderBlocks("io");
}

/** =========================================================
 *  Výpočet hodnot, barvy, blikání
 *  ========================================================= */


function updateBlocks(signals, tMs) {
  for (const route of ["overview","control","io"]) {
    const cfg = MACHINE.screens[route];
    if (!cfg) continue;

    for (const blk0 of cfg.blocks) {
      const b = ensureDefaults(structuredClone(blk0));
      const mainVal = signals?.[b.bind?.key];

      const dot = document.getElementById(`dot_${route}_${b.id}`);
      const box = document.getElementById(`box_${route}_${b.id}`);
      const txt = document.getElementById(`txt_${route}_${b.id}`);

      if (b.type === "button") {
        const rect = document.getElementById(`btn_${route}_${b.id}`);
        const label = document.getElementById(`btntxt_${route}_${b.id}`);
        const gEl = document.querySelector(`g[data-route="${route}"][data-id="${b.id}"]`);

        const vKey = (b.button?.visKey || "").trim();

        


        


        if (gEl && vKey) {
          const vVal = signals?.[vKey];
          const target = b.button?.visValue;
          const eq = equalLoose(vVal, target);
          const mode = (b.button?.visMode || "hideIfEqual");
          const hide = (mode === "showOnlyIfEqual") ? (!eq) : (eq);
          gEl.style.display = hide ? "none" : "";
        } else if (gEl) {
          gEl.style.display = "";
        }

        const ck = (b.button?.colorKey || b.bind?.key || b.button?.stateKey || b.button?.key || "").trim();
        const stateVal = ck ? signals?.[ck] : null;
        const baseCol = ck ? baseColorFor(b, stateVal) : "#666";

        let col = baseCol;
        if (shouldBlink(b, signals, stateVal)) {
          col = (blinkPhase(b.blink.hz, tMs) === 0) ? baseCol : (b.blink.color || "#f1c40f");
        }

        const mode = (b.button?.colorMode || "stroke");

        if (rect) {
          rect.setAttribute("stroke", col);
          if (mode === "strokeFill") {
            rect.setAttribute("fill", col);
            rect.setAttribute("opacity", Number.isFinite(b.style.boxAlpha) ? b.style.boxAlpha : 0.55);
          } else {
            rect.setAttribute("fill", b.style.boxBg || "#000");
            rect.setAttribute("opacity", Number.isFinite(b.style.boxAlpha) ? b.style.boxAlpha : 0.55);
          }
        }

        if (label) {
          label.setAttribute("fill", b.style.textColor);
          label.setAttribute("font-size", b.style.fontSize);
        }

        continue;
      }

      if (b.type === "labelValue") {
        const baseCol = baseColorFor(b, mainVal);
        let col = baseCol;

        if (shouldBlink(b, signals, mainVal)) {
          col = (blinkPhase(b.blink.hz, tMs) === 0) ? baseCol : (b.blink.color || "#f1c40f");
        }

        if (box) box.setAttribute("stroke", col);

        if (txt) {
          const vtxt = formatValue(b, mainVal);
          txt.textContent = (b.label ? (b.label + ": ") : "") + vtxt;
          txt.setAttribute("fill", b.style.textColor);
          txt.setAttribute("font-size", b.style.fontSize);
        }

        continue;
      }

      if (!backendOnline) {
        if (dot) dot.setAttribute("fill", "#bbb");
        if (box) box.setAttribute("stroke", "#bbb");
        if (txt) {
          if (b.type === "valueDot") {
            txt.textContent = (b.label ? (b.label + ": ") : "") + "—" + (b.unit ? " " + b.unit : "");
          } else {
            txt.textContent = b.label || b.id;
          }
          txt.setAttribute("fill", b.style.textColor);
          txt.setAttribute("font-size", b.style.fontSize);
        }
        continue;
      }

      const baseCol = baseColorFor(b, mainVal);
      let col = baseCol;
      if (shouldBlink(b, signals, mainVal)) {
        col = (blinkPhase(b.blink.hz, tMs) === 0) ? baseCol : (b.blink.color || "#f1c40f");
      }

      if (dot) dot.setAttribute("fill", col);
      if (box) box.setAttribute("stroke", col);
      if (txt) {
        if (b.type === "valueDot") {
          const vtxt = formatValue(b, mainVal);
          txt.textContent = (b.label ? (b.label + ": ") : "") + vtxt;
        } else {
          txt.textContent = b.label || b.id;
        }
        txt.setAttribute("fill", b.style.textColor);
        txt.setAttribute("font-size", b.style.fontSize);
      }
    }
  }
}

/** =========================================================
 *  Editor
 *  ========================================================= */
function isEditableRoute(route) { return ["overview","control","io"].includes(route); }

let editAllowed = (localStorage.getItem("hmi_editAllowed") === "1");
let editMode = false;
let selected = null;
let drag = null;


function syncEditorVisibility(route) {
  const ed = document.getElementById("editor");
  if (!ed) return;
  ed.style.display = (editAllowed && isEditableRoute(route)) ? "grid" : "none";
}

function setEdit(on) {
  if (!editAllowed) { editMode = false; on = false; }

  editMode = !!on;
  document.getElementById("editToggle").textContent = "EDIT: " + (editMode ? "ON" : "OFF");
  document.getElementById("duplicateBtn").disabled = !editMode;
  document.getElementById("deleteBtn").disabled = !editMode || !selected;
  document.getElementById("applyProps").disabled = !editMode || !selected;

  renderAllBlocks();
}

function findBlock(route, id) {
  const blocks = MACHINE.screens[route]?.blocks || [];
  return blocks.find(b => b.id === id) || null;
}
function findBlockIndex(route, id) {
  const blocks = MACHINE.screens[route]?.blocks || [];
  return blocks.findIndex(b => b.id === id);
}

function setSelected(route, id) {
  selected = (route && id) ? { route, id } : null;
  document.getElementById("deleteBtn").disabled = !editMode || !selected;
  document.getElementById("applyProps").disabled = !editMode || !selected;
  renderAllBlocks();
  fillProps();
}

function nearestBlock(route, x, y, maxDist=50) {
  const blocks = MACHINE.screens[route]?.blocks || [];
  let best = null, bestD = Infinity;
  for (const b of blocks) {
    const dx = b.x - x, dy = b.y - y;
    const d = Math.hypot(dx, dy);
    if (d < bestD) { bestD = d; best = b; }
  }
  return (bestD <= maxDist) ? best : null;
}


function makeNewBlock(type, x, y) {
  const base = {
    id: `B${Math.floor(Math.random()*90000+10000)}`,
    type,
    x, y,
    label: (type === "button") ? "Tlačítko" : (type === "inputSend" || type === "inputEnter") ? "Hodnota" : "Indikátor",
    bind: { key: "", type: "bool" },
    unit: "",
    decimals: 1,
    style: { fontSize: 22, radius: 12, boxW: 320, boxH: 48, onColor:"#2ecc71", offColor:"#bbbbbb", textColor:"#ffffff", boxBg:(MACHINE.defaults?.boxBg ?? "#000000"), boxAlpha:(MACHINE.defaults?.boxAlpha ?? 0.55) },
    blink: { enabled:false, key:"", color:"#f1c40f", hz:2 }
  };

  if (type === "dotOnly") {
    base.style.boxW = 0; base.style.boxH = 0;
  }
  if (type === "valueDot") {
    base.bind.type = "number";
  }
  if (type === "button") {
    base.bind = { key: "", type: "bool" };
    base.style.boxW = 220;
    base.style.boxH = 60;
    base.button = { text: "BTN", action:"toggle", key:"", stateKey:"", onValue:1, offValue:0 };
  }

  if (type === "inputSend" || type === "inputEnter") {
    base.bind = { key: "", type: "number" };
    base.style.boxW = 320;
    base.style.boxH = 56;
    base.input = { mode:"setpoint", key: "", addr: 0, placeholder: "Zadej hodnotu...", showType: true };
  }

  if (type === "labelValue") {
    base.label = "Popisek";
    base.bind = { key: "", type: "number" };
    base.unit = "";
    base.decimals = 1;
    base.style.boxW = 320;
    base.style.boxH = 48;
  }

  if (type === "text") {
    base.label = "Text";
    base.text = "Text";
    base.bind = { key: "", type: "bool" };
    base.style.fontSize = 22;
    base.style.textColor = "#ffffff";
    base.style.fontWeight = "600";
    base.style.fontStyle = "normal";
    base.style.textAnchor = "start";
    base.style.boxW = 320;
    base.style.boxH = 40;
  }

  if (type === "rect") {
    base.label = "";
    base.bind = { key: "", type: "bool" };
    base.style.boxW = 240;
    base.style.boxH = 120;
    base.style.radius = 12;
    base.style.boxBg = "#000000";
    base.style.boxAlpha = 0.25;
    base.style.offColor = "#bbbbbb";
    base.style.strokeWidth = 2;
  }

  return base;
}

function addBlock(route, block) {
  MACHINE.screens[route].blocks.push(block);
  renderBlocks(route);
  setSelected(route, block.id);
}

function deleteSelected() {
  if (!selected) return;
  const idx = findBlockIndex(selected.route, selected.id);
  if (idx >= 0) MACHINE.screens[selected.route].blocks.splice(idx, 1);
  setSelected(null, null);
  renderBlocks(currentRoute());
  exportConfigToBox();
}

function duplicateSelected() {
  if (!selected) return;
  const src = findBlock(selected.route, selected.id);
  if (!src) return;
  const copy = structuredClone(src);
  copy.id = `D${Math.floor(Math.random()*90000+10000)}`;
  copy.x = snap((copy.x ?? 0) + 10);
  copy.y = snap((copy.y ?? 0) + 10);
  MACHINE.screens[selected.route].blocks.push(copy);
  renderBlocks(selected.route);
  setSelected(selected.route, copy.id);
  exportConfigToBox();
}

function fillProps() {
  const bp = document.getElementById("buttonProps");
  const ip = document.getElementById("inputProps");
  if (!selected) {
    ["p_id","p_type","p_x","p_y","p_label","p_key","p_unit","p_decimals","p_font","p_radius","p_boxw","p_boxh","p_onColor","p_offColor","p_textColor",
     "p_blinkEnabled","p_blinkKey","p_blinkColor","p_blinkHz","p_boxBg","p_boxAlpha","p_btnText","p_btnAction","p_btnOn","p_btnOff","p_btnKey","p_btnStateKey","p_btnColorMode","p_btnColorKey","p_btnVisKey","p_btnVisMode","p_btnVisValue","p_inpMode","p_inpKey","p_inpSig","p_inpShowType","p_inpPlaceholder"].forEach(id=>{
      const el=document.getElementById(id);
      if(!el) return;
      if(el.type === "checkbox") el.checked = false;
      else el.value = "";
    });
    if (bp) bp.style.display = "none";
    if (ip) ip.style.display = "none";
    return;
  }

  const b = ensureDefaults(structuredClone(findBlock(selected.route, selected.id)));
  document.getElementById("p_id").value = b.id;
  document.getElementById("p_type").value = b.type;
  document.getElementById("p_x").value = b.x;
  document.getElementById("p_y").value = b.y;
  document.getElementById("p_label").value = b.label || "";
  document.getElementById("p_key").value = b.bind?.key || "";
  document.getElementById("p_bindType").value = b.bind?.type || "bool";
  document.getElementById("p_unit").value = b.unit || "";
  document.getElementById("p_decimals").value = b.decimals ?? 1;

  document.getElementById("p_font").value = b.style.fontSize;
  document.getElementById("p_radius").value = b.style.radius;
  document.getElementById("p_boxw").value = b.style.boxW;
  document.getElementById("p_boxh").value = b.style.boxH;

  document.getElementById("p_onColor").value = b.style.onColor;
  document.getElementById("p_offColor").value = b.style.offColor;
  document.getElementById("p_textColor").value = b.style.textColor;
  document.getElementById("p_boxBg").value = b.style.boxBg || (MACHINE.defaults?.boxBg ?? "#000000");
  document.getElementById("p_boxAlpha").value = Number(b.style.boxAlpha ?? (MACHINE.defaults?.boxAlpha ?? 0.55));

  document.getElementById("p_blinkEnabled").checked = !!b.blink.enabled;
  document.getElementById("p_blinkKey").value = b.blink.key || "";
  document.getElementById("p_blinkColor").value = b.blink.color || "#f1c40f";
  document.getElementById("p_blinkHz").value = b.blink.hz || 2;

  if (bp) bp.style.display = (b.type === "button") ? "block" : "none";
  if (ip) ip.style.display = (b.type === "inputSend" || b.type === "inputEnter") ? "block" : "none";
  if (b.type === "button") {
    document.getElementById("p_btnText").value = b.button?.text || "";
    document.getElementById("p_btnAction").value = b.button?.action || "toggle";
    document.getElementById("p_btnOn").value = b.button?.onValue ?? 1;
    document.getElementById("p_btnOff").value = b.button?.offValue ?? 0;
    document.getElementById("p_btnKey").value = b.button?.key || "";
    document.getElementById("p_btnStateKey").value = b.button?.stateKey || "";
    document.getElementById("p_btnColorMode").value = b.button?.colorMode || "stroke";
    document.getElementById("p_btnColorKey").value = b.button?.colorKey || "";
    document.getElementById("p_btnVisKey").value = b.button?.visKey || "";
    document.getElementById("p_btnVisMode").value = b.button?.visMode || "hideIfEqual";
    document.getElementById("p_btnVisValue").value = (b.button?.visValue ?? 1);
  }

  if (b.type === "inputSend" || b.type === "inputEnter") {
    document.getElementById("p_inpMode").value = (b.input?.mode || "setpoint");
    document.getElementById("p_inpKey").value = b.input?.key || "";
    document.getElementById("p_inpSig").value = (b.input?.sig || "") + "";
    document.getElementById("p_inpShowType").checked = (b.input?.showType === undefined) ? true : !!b.input?.showType;
    document.getElementById("p_inpPlaceholder").value = b.input?.placeholder || "";
    syncInputPropsUI();
  }
}

function syncInputPropsUI() {
  const modeEl = document.getElementById("p_inpMode");
  const keyEl  = document.getElementById("p_inpKey");
  const addrEl = document.getElementById("p_inpSig");
  if (!modeEl || !keyEl || !addrEl) return;

  const mode = (modeEl.value || "setpoint").toLowerCase();
  const isSet = (mode === "setpoint");

  keyEl.disabled = !isSet;
  keyEl.style.opacity = isSet ? "1" : "0.6";

  addrEl.disabled = isSet;
  addrEl.style.opacity = isSet ? "0.6" : "1";
}

function applyProps() {
  if (!selected) return;
  const idx = findBlockIndex(selected.route, selected.id);
  if (idx < 0) return;

  const b = MACHINE.screens[selected.route].blocks[idx];
  const newId = (document.getElementById("p_id").value || b.id).trim();
  if (newId && newId !== b.id) {
    if (findBlock(selected.route, newId)) {
      alert("ID už existuje na této obrazovce.");
      return;
    }
    b.id = newId;
    selected.id = newId;
  }

  b.x = snap(Number(document.getElementById("p_x").value || b.x || 0));
  b.y = snap(Number(document.getElementById("p_y").value || b.y || 0));

  b.label = document.getElementById("p_label").value || "";
  b.bind = b.bind || { key:"", type:"bool" };
  b.bind.key = document.getElementById("p_key").value || "";
  b.bind.type = document.getElementById("p_bindType").value || "bool";
  b.unit = document.getElementById("p_unit").value || "";
  b.decimals = Number(document.getElementById("p_decimals").value || 1);

  b.style = b.style || {};
  b.style.fontSize = Number(document.getElementById("p_font").value || 22);
  b.style.radius = Number(document.getElementById("p_radius").value || 12);
  b.style.boxW = Number(document.getElementById("p_boxw").value || 320);
  b.style.boxH = Number(document.getElementById("p_boxh").value || 48);
  b.style.onColor = document.getElementById("p_onColor").value || "#2ecc71";
  b.style.offColor = document.getElementById("p_offColor").value || "#bbbbbb";
  b.style.textColor = document.getElementById("p_textColor").value || "#ffffff";
  b.style.boxBg = document.getElementById("p_boxBg").value || (MACHINE.defaults?.boxBg ?? "#000000");
  b.style.boxAlpha = Number(document.getElementById("p_boxAlpha").value || (MACHINE.defaults?.boxAlpha ?? 0.55));

  b.blink = b.blink || {};
  b.blink.enabled = !!document.getElementById("p_blinkEnabled").checked;
  b.blink.key = document.getElementById("p_blinkKey").value || "";
  b.blink.color = document.getElementById("p_blinkColor").value || "#f1c40f";
  b.blink.hz = Number(document.getElementById("p_blinkHz").value || 2);

  if (b.type === "button") {
    b.button = b.button || {};
    b.button.text = document.getElementById("p_btnText").value || b.button.text || "BTN";
    b.button.action = document.getElementById("p_btnAction").value || "toggle";
    b.button.onValue = parseLiteral(document.getElementById("p_btnOn").value, 1);
    b.button.offValue = parseLiteral(document.getElementById("p_btnOff").value, 0);
    b.button.key = document.getElementById("p_btnKey").value || "";
    b.button.stateKey = document.getElementById("p_btnStateKey").value || "";
    b.button.colorMode = document.getElementById("p_btnColorMode").value || "stroke";
    b.button.colorKey = document.getElementById("p_btnColorKey").value || "";
    b.button.visKey = document.getElementById("p_btnVisKey").value || "";
    b.button.visMode = document.getElementById("p_btnVisMode").value || "hideIfEqual";
    b.button.visValue = parseLiteral(document.getElementById("p_btnVisValue").value, 1);
  }

  if (b.type === "inputSend" || b.type === "inputEnter") {
    b.input = b.input || {};
    b.input.mode = (document.getElementById("p_inpMode").value || "setpoint");
    b.input.key = document.getElementById("p_inpKey").value || "";
    b.input.sig = (document.getElementById("p_inpSig").value || "").trim();
    const _maybeN = Number(b.input.sig);
    b.input.addr = Number.isFinite(_maybeN) && b.input.sig !== "" ? _maybeN : Number(b.input.addr || 0);
    b.input.showType = !!document.getElementById("p_inpShowType").checked;
    b.input.placeholder = document.getElementById("p_inpPlaceholder").value || "";
  }

  renderBlocks(selected.route);
  exportConfigToBox();

  const btn = document.getElementById("applyProps");
  const cfgText = document.getElementById("configBox").value || "";

  saveConfigToServer(cfgText).then(function(r) {
    if (r.ok) flashSave(btn, true, "Uloženo");
    else flashSave(btn, false, "Chyba: " + r.error);
  });
}

function exportConfigToBox(copyToClipboard=true) {
  const obj = {
    title: MACHINE.title,
    subtitle: MACHINE.subtitle,
    screens: {
      overview: MACHINE.screens.overview,
      control: MACHINE.screens.control,
      io: MACHINE.screens.io
    }
  };
  const txt = JSON.stringify(obj, null, 2);
  document.getElementById("configBox").value = txt;
  if (copyToClipboard) navigator.clipboard?.writeText(txt).catch(()=>{});
}

function applyConfigFromBox() {
  try {
    const parsed = JSON.parse(document.getElementById("configBox").value || "{}");
    if (parsed.screens?.overview) MACHINE.screens.overview = parsed.screens.overview;
    if (parsed.screens?.control) MACHINE.screens.control = parsed.screens.control;
    if (parsed.screens?.io) MACHINE.screens.io = parsed.screens.io;
    applyScreenSizing();
    renderAllBlocks();
    exportConfigToBox(false);
  } catch (e) {
    alert("Neplatný JSON: " + e);
  }
}

let activeColorInput = null;
function initColorPaletteUI(){
  const swWrap = document.getElementById("colorSwatches");
  if (!swWrap) return;

  const palette = (MACHINE.defaults && Array.isArray(MACHINE.defaults.palette)) ? MACHINE.defaults.palette : [
    { name:"červená", value:"#e74c3c" },
    { name:"zelená", value:"#2ecc71" },
    { name:"šedá", value:"#bbbbbb" },
    { name:"černá", value:"#000000" },
    { name:"bílá", value:"#ffffff" },
    { name:"oranžová", value:"#f39c12" },
    { name:"modrá", value:"#3498db" },
    { name:"žlutá", value:"#f1c40f" },
  ];

  swWrap.innerHTML = "";
  for (const c of palette){
    const s = document.createElement("button");
    s.type = "button";
    s.className = "swatch";
    s.style.background = c.value;
    s.title = c.name + " " + c.value;
    s.setAttribute("aria-label", c.name);
    s.addEventListener("click", () => {
      if (!activeColorInput) return;
      activeColorInput.value = c.value;
      activeColorInput.dispatchEvent(new Event("input", { bubbles:true }));
    });
    swWrap.appendChild(s);
  }

  const ids = ["p_onColor","p_offColor","p_textColor","p_blinkColor","p_boxBg"];
  for (const id of ids){
    const el = document.getElementById(id);
    if (!el) continue;
    el.addEventListener("focus", ()=>{ activeColorInput = el; });
    el.addEventListener("click", ()=>{ activeColorInput = el; });
  }
}

function initEditor() {
  const editBtn = document.getElementById("editToggle");
  if (editBtn) {
    editBtn.addEventListener("click", async function () {
      const wasOn = !!editMode;
      setEdit(!editMode);

      if (wasOn && !editMode) {
        exportConfigToBox(true);
        const cfgText = (document.getElementById("configBox") && document.getElementById("configBox").value) ? document.getElementById("configBox").value : "";
        try {
          const r = await saveConfigToServer(cfgText);
          if (r && r.ok === false) flashSave(editBtn, false, "Chyba: " + (r.error || ""));
          else flashSave(editBtn, true, "Uloženo");
        } catch (e) {
          flashSave(editBtn, false, "Chyba: " + e);
        }
      }
    });
  }

  document.getElementById("exportBtn")?.addEventListener("click", () => exportConfigToBox(true));
  document.getElementById("applyBtn")?.addEventListener("click", () => applyConfigFromBox());
  document.getElementById("duplicateBtn")?.addEventListener("click", () => duplicateSelected());
  document.getElementById("deleteBtn")?.addEventListener("click", () => deleteSelected());
  document.getElementById("applyProps")?.addEventListener("click", () => applyProps());
  document.getElementById("p_inpMode")?.addEventListener("change", () => syncInputPropsUI());

  window.addEventListener("keydown", (e) => {
    if (!editMode) return;
    if (e.key === "Delete") { e.preventDefault(); deleteSelected(); }
    if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === "d") { e.preventDefault(); duplicateSelected(); }
  });

  for (const route of ["overview","control","io"]) {
    const svg = getScreenSvg(route);
    if (!svg) continue;

    svg.addEventListener("mousedown", (e) => {
      if (!editMode || !isEditableRoute(route)) return;
      const p = svgPoint(svg, e);
      const x = snap(Math.round(p.x));
      const y = snap(Math.round(p.y));

      const targetG = e.target?.closest?.("g");
      const targetId = targetG?.getAttribute?.("data-id");
      if (targetId) {
        setSelected(route, targetId);
        const b = findBlock(route, targetId);
        if (b) drag = { route, id: targetId, dx: (b.x - x), dy: (b.y - y) };
        return;
      }

      if (e.shiftKey) {
        const nb = nearestBlock(route, x, y);
        if (nb) {
          setSelected(route, nb.id);
          deleteSelected();
        }
        return;
      }

      const type = document.getElementById("toolSelect").value;
      const b = makeNewBlock(type, x, y);
      addBlock(route, b);
      exportConfigToBox(false);
      drag = { route, id: b.id, dx: 0, dy: 0 };
    });

    svg.addEventListener("mousemove", (e) => {
      if (!editMode || !drag) return;
      const svg2 = getScreenSvg(drag.route);
      if (!svg2) return;
      const p = svgPoint(svg2, e);
      const x = snap(Math.round(p.x));
      const y = snap(Math.round(p.y));
      const idx = findBlockIndex(drag.route, drag.id);
      if (idx < 0) return;
      const b = MACHINE.screens[drag.route].blocks[idx];
      b.x = x + drag.dx;
      b.y = y + drag.dy;
      renderBlocks(drag.route);
      fillProps();
    });

    window.addEventListener("mouseup", () => {
      if (drag) { drag = null; exportConfigToBox(false); }
    });
  }

  exportConfigToBox(false);
}

/** =========================================================
 *  Button interactions (runtime)
 *  ========================================================= */
function bindButtonsRuntime() {
  
  

  for (const route of ["overview","control","io"]) {
    const svg = getScreenSvg(route);
    if (!svg) continue;

    svg.addEventListener("click", async (e) => {
      if (editMode) return;
      const g = e.target?.closest?.("g");
      const id = g?.getAttribute?.("data-id");
      if (!id) return;

      const blk = findBlock(route, id);
      if (!blk || blk.type !== "button") return;

      const action = blk.button?.action || "toggle";
      const key = (blk.button?.key || "").trim();
      if (!key) return;

      if (action === "toggle") await apiToggleCoil(key);
      else if (action === "set") await apiSetCoil(key, 1);
      else if (action === "reset") await apiSetCoil(key, 0);
    });

    const start = async (e) => {
      if (editMode) return;
      const g = e.target?.closest?.("g");
      const id = g?.getAttribute?.("data-id");
      if (!id) return;
      const blk = findBlock(route, id);
      if (!blk || blk.type !== "button") return;
      if ((blk.button?.action || "") !== "momentary") return;

      const key = (blk.button?.key || "").trim();
      if (!key) return;
      e.preventDefault();
      await apiSetCoil(key, 1);
    };

    const stop = async (e) => {
      if (editMode) return;
      const g = e.target?.closest?.("g");
      const id = g?.getAttribute?.("data-id");
      if (!id) return;
      const blk = findBlock(route, id);
      if (!blk || blk.type !== "button") return;
      if ((blk.button?.action || "") !== "momentary") return;

      const key = (blk.button?.key || "").trim();
      if (!key) return;
      e.preventDefault();
      await apiSetCoil(key, 0);
    };

    svg.addEventListener("mousedown", start);
    svg.addEventListener("mouseup", stop);
    svg.addEventListener("mouseleave", stop);
    svg.addEventListener("touchstart", start, { passive:false });
    svg.addEventListener("touchend", stop, { passive:false });
    svg.addEventListener("touchcancel", stop, { passive:false });
  }
}

/** =========================================================
 *  Input blocks runtime (foreignObject + iOS HTML overlay)
 *  ========================================================= */
function bindInputsRuntime() {
  for (const route of ["overview","control","io"]) {
    const svg = getScreenSvg(route);
    if (!svg || svg.dataset.inputsBound) continue;
    svg.dataset.inputsBound = "1";

    async function writeByInputBlock(blk, value) {
      if (!backendOnline) return;

      const mode = (blk.input?.mode || "setpoint").trim();
      if (mode === "setpoint") {
        const key = (blk.input?.key || "").trim();
        if (!key) return;
        await apiWrite(key, value);
        return;
      }

      const sig = (blk.input?.sig || "").trim();
      if (sig) {
        const res = await fetch(`/api/write_signal/${encodeURIComponent(mode)}/${encodeURIComponent(sig)}/${encodeURIComponent(value)}`, { method: "POST" });
        if (!res.ok) {
          const t = await res.text().catch(()=>"");
          console.warn("write_signal failed", res.status, t);
        }
        return;
      }

      const addr = Number(blk.input?.addr || 0);
      if (!Number.isFinite(addr) || addr < 0) return;

      const res = await fetch(`/api/write/${encodeURIComponent(mode)}/${encodeURIComponent(addr)}/${encodeURIComponent(value)}`, { method: "POST" });
      if (!res.ok) {
        const t = await res.text().catch(()=>"");
        console.warn("write failed", res.status, t);
      }
    }

    // klik na "Odeslat" (funguje pro foreignObject i pro iOS overlay)
    svg.addEventListener("click", async (e) => {
      if (editMode) return;

      // iOS overlay je mimo SVG, takže klik na button přijde sem jen u foreignObject.
      // Pro iOS overlay řešíme ještě níž delegací na document.
      const btn = e.target?.closest?.("button.hmiInputSend");
      if (!btn) return;

      const id = btn.dataset.id;
      const r = btn.dataset.route || route;
      const blk = findBlock(r, id);
      if (!blk || blk.type !== "inputSend") return;

      const inp = document.querySelector(`input.hmiInput[data-route="${r}"][data-id="${id}"]`);
      const value = (inp?.value ?? "").trim();
      if (value === "") return;

      await writeByInputBlock(blk, value);
    });

    svg.addEventListener("keydown", async (e) => {
      if (editMode) return;
      const inp = e.target?.closest?.("input.hmiInput");
      if (!inp) return;
      if (e.key !== "Enter") return;

      const id = inp.dataset.id;
      const r = inp.dataset.route || route;
      const blk = findBlock(r, id);
      if (!blk || blk.type !== "inputEnter") return;

      const value = (inp.value || "").trim();
      if (value === "") return;

      e.preventDefault();
      await writeByInputBlock(blk, value);
      inp.value = "";
    });

    // iOS overlay: delegace na document (protože input/button jsou mimo SVG)
    if (!document.body.dataset.hmiOverlayBound) {
      document.body.dataset.hmiOverlayBound = "1";

      document.addEventListener("click", async (e) => {
        if (editMode) return;
        const btn = e.target?.closest?.("button.hmiInputSend");
        if (!btn) return;

        const id = btn.dataset.id;
        const r = btn.dataset.route;
        if (!id || !r) return;

        const blk = findBlock(r, id);
        if (!blk || blk.type !== "inputSend") return;

        const inp = document.querySelector(`input.hmiInput[data-route="${r}"][data-id="${id}"]`);
        const value = (inp?.value ?? "").trim();
        if (value === "") return;

        await writeByInputBlock(blk, value);
      }, true);

      document.addEventListener("keydown", async (e) => {
        if (editMode) return;
        const inp = e.target?.closest?.("input.hmiInput");
        if (!inp) return;
        if (e.key !== "Enter") return;

        const id = inp.dataset.id;
        const r = inp.dataset.route;
        if (!id || !r) return;

        const blk = findBlock(r, id);
        if (!blk || blk.type !== "inputEnter") return;

        const value = (inp.value || "").trim();
        if (value === "") return;

        e.preventDefault();
        await writeByInputBlock(blk, value);
        inp.value = "";
      }, true);
    }
  }
}

/** =========================================================
 *  Load loop
 *  ========================================================= */


/** =========================================================
 *  Blink/render loop
 *  ========================================================= */
function tick() {
  watchdog();
  const t = performance.now();
  const signals = (backendOnline ? (last?.signals || {}) : {});
  updateBlocks(signals, t);
  requestAnimationFrame(tick);
}

/** =========================================================
 *  Init
 *  ========================================================= */
document.querySelectorAll(".tab").forEach(b => b.addEventListener("click", () => {
  const r = b.dataset.route;
  if (r === "settings") {
    if (!requireAdminOrRedirect()) return;
  }
  location.hash = "#" + r;
}));
window.addEventListener("hashchange", () => setRoute(currentRoute()));

function initSettings() {
  const edEnable = document.getElementById("settingsEditEnable");
  const sSnap = document.getElementById("settingsSnapOn");
  const sGrid = document.getElementById("settingsGridSize");
  const sBox = document.getElementById("settingsConfigBox");
  const sExport = document.getElementById("settingsExport");
  const sApply = document.getElementById("settingsApply");
  const sSave = document.getElementById("settingsSaveServer");

  if (edEnable) edEnable.checked = editAllowed;
  if (sSnap) sSnap.checked = document.getElementById("snapOn").checked;
  if (sGrid) sGrid.value = document.getElementById("gridSize").value;

  function syncToSettingsBox() {
    if (!sBox) return;
    sBox.value = document.getElementById("configBox").value || "";
  }
  function syncFromSettingsBox() {
    if (!sBox) return;
    document.getElementById("configBox").value = sBox.value;
  }

  syncToSettingsBox();

  edEnable && edEnable.addEventListener("change", () => {
    editAllowed = !!edEnable.checked;
    localStorage.setItem("hmi_editAllowed", editAllowed ? "1" : "0");
    setEdit(false);
    syncEditorVisibility(currentRoute());
    setSelected(null, null);
  });

  sSnap && sSnap.addEventListener("change", () => {
    document.getElementById("snapOn").checked = !!sSnap.checked;
  });

  sGrid && sGrid.addEventListener("change", () => {
    document.getElementById("gridSize").value = sGrid.value;
  });

  sExport && sExport.addEventListener("click", () => {
    exportConfigToBox(true);
    syncToSettingsBox();
  });

  sApply && sApply.addEventListener("click", () => {
    syncFromSettingsBox();
    applyConfigFromBox();
    syncToSettingsBox();
  });

  sSave && sSave.addEventListener("click", async () => {
    exportConfigToBox(true);
    syncToSettingsBox();
    const cfgText = sBox ? sBox.value : document.getElementById("configBox").value;

    const r = await saveConfigToServer(cfgText);
    if (r.ok) flashSave(sSave, true, "Uloženo");
    else      flashSave(sSave, false, "Chyba: " + r.error);
  });
}

/** =========================================================
 *  Admin locking
 *  ========================================================= */
function isAdminUnlocked() {
  return sessionStorage.getItem(ADMIN_FLAG_KEY) === "1";
}

function setAdminUnlocked(on) {
  if (on) sessionStorage.setItem(ADMIN_FLAG_KEY, "1");
  else sessionStorage.removeItem(ADMIN_FLAG_KEY);
  updateAdminUI();
}

function requireAdminOrRedirect() {
  if (isAdminUnlocked()) return true;

  const pw = prompt("Admin heslo pro vstup do Nastavení:");
  if (pw === null) return false;

  if (pw === ADMIN_PASSWORD) {
    setAdminUnlocked(true);
    return true;
  }

  alert("Špatné heslo.");
  location.hash = "#overview";
  return false;
}

function updateAdminUI() {
  const badge = document.getElementById("adminBadge");
  const btn = document.getElementById("adminLogoutBtn");

  if (isAdminUnlocked()) {
    badge && (badge.style.display = "inline-block");
    btn && (btn.style.display = "inline-block");
  } else {
    badge && (badge.style.display = "none");
    btn && (btn.style.display = "none");
  }
}

/** =========================================================
 *  Polling: fixed-rate (NE await+sleep)
 *  ========================================================= */
let nextAt = 0;
async function loadLoopFixedRate(){
  const now = performance.now();
  if (!nextAt) nextAt = now;
  nextAt += POLL_MS;

  const t0 = performance.now();
  await load();
  const dt = performance.now() - t0;
  console.log("load dt ms:", dt.toFixed(0));

  const wait = Math.max(0, nextAt - performance.now());
  setTimeout(loadLoopFixedRate, wait);
}


/* ==========================================================================
   SECTION: hmi.api
   Source: js/hmi.api.js
   ========================================================================== */

async function fetchJsonWithTimeout(url, timeoutMs = 2000) {
  const controller = new AbortController();
  const t = setTimeout(() => controller.abort(), timeoutMs);
  try {
    const res = await fetch(url, { signal: controller.signal, cache: "no-store" });
    if (!res.ok) {
      const txt = await res.text().catch(() => "");
      throw new Error(`HTTP ${res.status} ${txt}`.trim());
    }
    return await res.json();
  } catch (e) {
    if (e && (e.name === "AbortError" || String(e).includes("AbortError"))) {
      throw new Error(`timeout after ${timeoutMs}ms`);
    }
    throw e;
  } finally {
    clearTimeout(t);
  }
}

async function load() {
  if (loadInFlight) {
    console.log("[HMI] load skipped (in flight)");
    return;
  }
  loadInFlight = true;

  const mySeq = ++loadSeq;
  const payload = document.getElementById("payload");

  try {
    if (forcedOffline || navigator.onLine === false) {
      forcedOffline = true;
      failStreak++;
      okStreak = 0;
      if (failStreak >= FAIL_TO_OFFLINE) setStatus(false, false);
      if (payload) payload.textContent = "browser offline";
      return;
    }

    const url = (MACHINE && MACHINE.api && MACHINE.api.stateUrl) ? MACHINE.api.stateUrl : "/api/state";
    const data = await fetchJsonWithTimeout(url, 400);

    if (mySeq !== loadSeq) return;

    last = data;

    // úspěch
    okStreak++;
    failStreak = 0;
    lastOkAt = performance.now();

    const connected = (typeof data.connected === "boolean") ? data.connected : true;

    // online až po OK_TO_ONLINE úspěších za sebou (hystereze)
    if (okStreak >= OK_TO_ONLINE) {
      setStatus(true, connected);
    }

    if (payload) payload.textContent = JSON.stringify(data, null, 2);

    const signals = (connected ? (data.signals || {}) : {});
    if (connected) updateSignalDatalist(signals);
    renderIO(signals);

    for (const cfg of AXES) {
      const pos = signals?.[cfg.feedback];
      const fbEl = document.getElementById(`fb${cfg.axis}`);
      if (fbEl && typeof pos === "number") fbEl.textContent = `pos: ${pos.toFixed(1)} ${cfg.unit}`;
      else if (fbEl) fbEl.textContent = `pos: —`;
    }

  } catch (e) {
    const msg = String(e || "");
    failStreak++;
    okStreak = 0;

    // OFFLINE až po FAIL_TO_OFFLINE chybách za sebou (nebude to stroboskop)
    if (failStreak >= FAIL_TO_OFFLINE) {
      setStatus(false, false);
    }

    if (payload) payload.textContent = "load failed: " + msg;
  } finally {
    loadInFlight = false;
  }
}

async function triggerLoad() {
  if (_loadInFlight) return;
  _loadInFlight = true;
  try { await load(); } finally { _loadInFlight = false; }
}

async function apiWrite(key, value, opts = { refresh: true }) {
  if (!backendOnline) return;

  const tpl =
    (MACHINE && MACHINE.api && MACHINE.api.writeTemplate)
      ? MACHINE.api.writeTemplate
      : "/api/setpoint/{key}/{value}";

  const url = tpl
    .replace("{key}", encodeURIComponent(key))
    .replace("{value}", encodeURIComponent(value));

  const r = await fetch(url, { method: "POST" });

  if (r.ok && (opts?.refresh ?? true)) {
    await triggerLoad();
  }
}

async function saveConfigToServer(cfgText) {
  let payload;
  try { payload = JSON.parse(cfgText); }
  catch { payload = { config_text: String(cfgText || "") }; }

  const res = await fetch("/api/hmi_config", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload)
  });

  if (!res.ok) {
    const t = await res.text().catch(() => "");
    return { ok: false, error: `${res.status} ${t}`.trim() };
  }

  const out = await res.json().catch(() => ({}));
  return { ok: true, path: out.path || "static/machine_config.js" };
}

async function apiSetCoil(key, v01) {
    if (!backendOnline) return;
    const r = await fetch(`/api/set/${encodeURIComponent(key)}/${v01}`, { method: "POST" });
    if (r.ok) await triggerLoad();
  }

async function apiToggleCoil(key) {
    if (!backendOnline) return;
    const r = await fetch(`/api/toggle/${encodeURIComponent(key)}`, { method: "POST" });
    if (r.ok) await triggerLoad();
  }


/* ==========================================================================
   SECTION: hmi.render
   Source: js/hmi.render.js
   ========================================================================== */

// (Render code was not clearly separable; kept in hmi.core.js)


/* ==========================================================================
   SECTION: hmi.editor
   Source: js/hmi.editor.js
   ========================================================================== */

// (Editor code was not clearly separable; kept in hmi.core.js)


/* ==========================================================================
   SECTION: hmi.core
   Source: js/hmi.core.js
   ========================================================================== */

function init() {
  MACHINE.title && (document.getElementById("title").textContent = MACHINE.title);
  MACHINE.subtitle && (document.getElementById("subtitle").textContent = MACHINE.subtitle);

  applyScreenSizing();
  renderAxes();
  renderCameras();

  initEditor();
  initColorPaletteUI();
  initSettings();
  bindButtonsRuntime();
  bindInputsRuntime();
  updateAdminUI();

  setRoute(currentRoute());
  renderAllBlocks();
  exportConfigToBox(false);

  // při změně velikosti (rotace iOS, zoom, atd.) přepočítat overlay inputy
  window.addEventListener("resize", () => {
    // malý debounce přes RAF
    window.cancelAnimationFrame(window.__hmiResizeRaf);
    window.__hmiResizeRaf = window.requestAnimationFrame(() => renderAllBlocks());
  });

  window.addEventListener("offline", () => {forcedOffline = true; setStatus(false, false);});
  window.addEventListener("online", () => { forcedOffline = false; triggerLoad(); });

  loadLoopFixedRate();
  requestAnimationFrame(tick);

  const logoutBtn = document.getElementById("adminLogoutBtn");
  logoutBtn && logoutBtn.addEventListener("click", () => {
    setEdit(false);
    setSelected(null, null);

    editAllowed = false;
    localStorage.setItem("hmi_editAllowed", "0");

    const edEnable = document.getElementById("settingsEditEnable");
    if (edEnable) edEnable.checked = false;

    location.hash = "#overview";
    syncEditorVisibility("overview");
    setAdminUnlocked(false);
  });
}

// Boot after DOM is ready (stability)
document.addEventListener('DOMContentLoaded', () => {
  try {
    if (typeof MACHINE === 'undefined' || !MACHINE) {
      console.error('MACHINE config missing. Check machine_config.js + script order.');
      if (typeof setStatus === 'function') setStatus(false, false);
      return;
    }
    init();
  } catch (e) {
    console.error('HMI init failed:', e);
    if (typeof setStatus === 'function') setStatus(false, false);
  }
});
