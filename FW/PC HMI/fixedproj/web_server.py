import json
from pathlib import Path
from typing import Any

import uvicorn
from fastapi import Body, FastAPI, HTTPException
from fastapi.staticfiles import StaticFiles

from shared_state import (
    BANK_FUNC_CODE,
    SIGNAL_TYPE_KIND,
    decode_signal,
    find_signal,
    set_signal_value,
    set_values,
    state_payload,
)


def create_app(runtime: dict[str, Any]) -> FastAPI:
    app = FastAPI()
    base_dir = Path(runtime["base_dir"])

    @app.get("/ping")
    def ping():
        return {"ok": True, "mode": "slave", "unit_id": int(runtime["unit_id"])}

    @app.get("/api/state")
    def get_state():
        return state_payload(runtime)

    @app.post("/api/set/{name}/{value}")
    def set_signal(name: str, value: int):
        if value not in (0, 1):
            raise HTTPException(status_code=400, detail="value must be 0 or 1")
        try:
            return set_signal_value(runtime, find_signal(runtime, name), value)
        except KeyError:
            raise HTTPException(status_code=404, detail=f"signal '{name}' not found")
        except ValueError as exc:
            raise HTTPException(status_code=400, detail=str(exc))

    @app.post("/api/toggle/{name}")
    def toggle_signal(name: str):
        try:
            signal = find_signal(runtime, name)
        except KeyError:
            raise HTTPException(status_code=404, detail=f"signal '{name}' not found")
        current = decode_signal(runtime, signal)
        if not isinstance(current, bool):
            raise HTTPException(status_code=400, detail="signal is not boolean")
        return set_signal_value(runtime, signal, 0 if current else 1)

    @app.post("/api/setpoint/{name}/{value}")
    def set_setpoint(name: str, value: float):
        try:
            signal = find_signal(runtime, name)
        except KeyError:
            raise HTTPException(status_code=404, detail=f"signal '{name}' not found")
        if SIGNAL_TYPE_KIND.get(signal.get("type")) != "u16":
            raise HTTPException(status_code=400, detail="signal is not u16")
        try:
            return set_signal_value(runtime, signal, value)
        except ValueError as exc:
            raise HTTPException(status_code=400, detail=str(exc))

    @app.post("/api/raw/{bank}/{addr}/{value}")
    def write_raw(bank: str, addr: int, value: int):
        bank = bank.lower()
        if bank not in BANK_FUNC_CODE:
            raise HTTPException(status_code=400, detail="bank must be one of: coil, discrete, input, holding")
        if addr < 0:
            raise HTTPException(status_code=400, detail="addr must be >= 0")
        if value < 0 or value > 0xFFFF:
            raise HTTPException(status_code=400, detail="value must be 0..65535")
        set_values(runtime, bank, addr, [value])
        return {"ok": True, "bank": bank, "addr": addr, "value": value}

    @app.post("/api/reset")
    def reset_registers():
        set_values(runtime, "holding", 0, [0] * runtime["sizes"]["holding"])
        set_values(runtime, "input", 0, [0] * runtime["sizes"]["input"])
        set_values(runtime, "coil", 0, [0] * runtime["sizes"]["coil"])
        set_values(runtime, "discrete", 0, [0] * runtime["sizes"]["discrete"])
        return {"ok": True}

    @app.post("/api/hmi_config")
    def save_hmi_config(payload: Any = Body(...)):
        if isinstance(payload, dict) and "config_text" in payload:
            try:
                cfg_obj = json.loads(payload["config_text"])
            except Exception as exc:
                raise HTTPException(status_code=400, detail=f"config_text must be valid JSON: {exc}")
        elif isinstance(payload, dict):
            cfg_obj = payload
        else:
            raise HTTPException(status_code=400, detail="payload must be JSON object or {config_text: ...}")

        out_js = f"window.MACHINE_CONFIG = {json.dumps(cfg_obj, ensure_ascii=False, indent=2)};\n"
        out_path = base_dir / "static" / "machine_config.js"
        out_path.write_text(out_js, encoding="utf-8")
        return {"ok": True, "path": str(out_path)}

    app.mount("/", StaticFiles(directory=str(base_dir / "static"), html=True), name="static")
    return app


def run_web_process(runtime: dict[str, Any]) -> None:
    app = create_app(runtime)
    web_cfg = runtime["web"]
    uvicorn.run(app, host=web_cfg["host"], port=int(web_cfg["port"]), log_level="info")
