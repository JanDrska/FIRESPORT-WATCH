import json
from pathlib import Path
from typing import Any

BANK_FUNC_CODE = {
    "coil": 1,
    "discrete": 2,
    "holding": 3,
    "input": 4,
}

SIGNAL_TYPE_TO_BANK = {
    "coil": "coil",
    "discrete": "discrete",
    "holding_bit": "holding",
    "holding_u16": "holding",
    "input_bit": "input",
    "input_u16": "input",
}

SIGNAL_TYPE_KIND = {
    "coil": "bit",
    "discrete": "bit",
    "holding_bit": "bit",
    "input_bit": "bit",
    "holding_u16": "u16",
    "input_u16": "u16",
}


def load_config(base_dir: Path) -> dict[str, Any]:
    cfg_path = base_dir / "signals.json"
    return json.loads(cfg_path.read_text(encoding="utf-8"))


def bank_size_from_signals(signals: list[dict[str, Any]], register_sizes: dict[str, Any], bank: str, minimum: int = 1) -> int:
    max_addr = -1
    for signal in signals:
        if SIGNAL_TYPE_TO_BANK.get(signal.get("type")) == bank:
            max_addr = max(max_addr, int(signal.get("address", 0)))
    configured = int(register_sizes.get(bank, 0) or 0)
    return max(minimum, configured, max_addr + 1)


def build_runtime_config(base_dir: Path) -> dict[str, Any]:
    cfg = load_config(base_dir)
    modbus_cfg = dict(cfg.get("modbus", {}))
    signals = list(cfg.get("signals", []))
    register_sizes = dict(cfg.get("register_sizes", {}))

    sizes = {
        "coil": bank_size_from_signals(signals, register_sizes, "coil", 1),
        "discrete": bank_size_from_signals(signals, register_sizes, "discrete", 1),
        "input": bank_size_from_signals(signals, register_sizes, "input", 16),
        "holding": bank_size_from_signals(signals, register_sizes, "holding", 16),
    }

    web_cfg = dict(cfg.get("web", {}))
    return {
        "base_dir": str(base_dir),
        "cfg_path": str(base_dir / "signals.json"),
        "unit_id": int(modbus_cfg.get("device_id", 3)),
        "signals": signals,
        "register_sizes": register_sizes,
        "sizes": sizes,
        "modbus": modbus_cfg,
        "web": {
            "host": str(web_cfg.get("host", "0.0.0.0")),
            "port": int(web_cfg.get("port", 8000)),
        },
    }


def get_values(runtime: dict[str, Any], bank: str, address: int, count: int = 1) -> list[int]:
    size = runtime["sizes"][bank]
    if address < 0:
        raise ValueError("address must be >= 0")
    if count < 0:
        raise ValueError("count must be >= 0")
    end = min(address + count, size)
    with runtime["lock"]:
        values = [int(runtime["state"][bank][i]) for i in range(address, end)]
    if len(values) < count:
        values.extend([0] * (count - len(values)))
    return values


def set_values(runtime: dict[str, Any], bank: str, address: int, values: list[int]) -> None:
    size = runtime["sizes"][bank]
    if address < 0:
        raise ValueError("address must be >= 0")
    with runtime["lock"]:
        for offset, value in enumerate(values):
            idx = address + offset
            if idx >= size:
                break
            runtime["state"][bank][idx] = int(value) & 0xFFFF


def preload_defaults(runtime: dict[str, Any]) -> None:
    for signal in runtime["signals"]:
        default = signal.get("default")
        if default is None:
            continue
        bank = SIGNAL_TYPE_TO_BANK.get(signal.get("type"))
        kind = SIGNAL_TYPE_KIND.get(signal.get("type"))
        if not bank or not kind:
            continue
        addr = int(signal.get("address", 0))
        if kind == "bit":
            bit = int(signal.get("bit", 0))
            current = get_values(runtime, bank, addr, 1)[0]
            if bank in ("coil", "discrete"):
                new_value = 1 if bool(default) else 0
            else:
                mask = 1 << bit
                new_value = (current | mask) if bool(default) else (current & ~mask)
            set_values(runtime, bank, addr, [new_value])
        elif kind == "u16":
            scale = float(signal.get("scale", 1.0) or 1.0)
            raw = int(round(float(default) / scale)) & 0xFFFF
            set_values(runtime, bank, addr, [raw])


def find_signal(runtime: dict[str, Any], name: str) -> dict[str, Any]:
    for signal in runtime["signals"]:
        if signal.get("name") == name:
            return signal
    raise KeyError(name)


def _decode_signal_from_snapshot(snapshot: dict[str, list[int]], signal: dict[str, Any]) -> Any:
    signal_type = signal.get("type")
    bank = SIGNAL_TYPE_TO_BANK.get(signal_type)
    kind = SIGNAL_TYPE_KIND.get(signal_type)
    if not bank or not kind:
        return None

    addr = int(signal.get("address", 0))
    bank_values = snapshot.get(bank, [])
    raw = int(bank_values[addr]) if 0 <= addr < len(bank_values) else 0

    if kind == "bit":
        if bank in ("coil", "discrete"):
            return bool(raw)
        bit = int(signal.get("bit", 0))
        return bool(raw & (1 << bit))

    scale = float(signal.get("scale", 1.0) or 1.0)
    return raw * scale


def decode_signal(runtime: dict[str, Any], signal: dict[str, Any]) -> Any:
    return _decode_signal_from_snapshot(snapshot_state(runtime, include_unused=True), signal)


def encode_u16(signal: dict[str, Any], value: float) -> int:
    scale = float(signal.get("scale", 1.0) or 1.0)
    raw = int(round(float(value) / scale))
    if raw < 0 or raw > 0xFFFF:
        raise ValueError("u16 out of range (0..65535)")
    return raw


def set_signal_value(runtime: dict[str, Any], signal: dict[str, Any], value: int | float | bool) -> dict[str, Any]:
    signal_type = signal.get("type")
    bank = SIGNAL_TYPE_TO_BANK.get(signal_type)
    kind = SIGNAL_TYPE_KIND.get(signal_type)
    if not bank or not kind:
        raise ValueError(f"unsupported signal type: {signal_type}")

    addr = int(signal.get("address", 0))

    if kind == "bit":
        bool_value = bool(int(value))
        if bank in ("coil", "discrete"):
            set_values(runtime, bank, addr, [1 if bool_value else 0])
            return {"ok": True, "name": signal["name"], "addr": addr, "value": bool_value}

        bit = int(signal.get("bit", 0))
        current = get_values(runtime, bank, addr, 1)[0]
        mask = 1 << bit
        new_raw = (current | mask) if bool_value else (current & ~mask)
        set_values(runtime, bank, addr, [new_raw])
        return {"ok": True, "name": signal["name"], "addr": addr, "bit": bit, "value": bool_value, "raw_after": new_raw}

    raw = encode_u16(signal, float(value))
    set_values(runtime, bank, addr, [raw])
    return {"ok": True, "name": signal["name"], "addr": addr, "value": float(value), "raw": raw}


def snapshot_state(runtime: dict[str, Any], include_unused: bool = False) -> dict[str, list[int]]:
    with runtime["lock"]:
        out = {
            "holding": [int(v) for v in runtime["state"]["holding"]],
            "input": [int(v) for v in runtime["state"]["input"]],
        }
        if include_unused:
            out["coil"] = [int(v) for v in runtime["state"]["coil"]]
            out["discrete"] = [int(v) for v in runtime["state"]["discrete"]]
    return out


def state_payload(runtime: dict[str, Any]) -> dict[str, Any]:
    snapshot = snapshot_state(runtime, include_unused=False)
    signals_out = {signal["name"]: _decode_signal_from_snapshot(snapshot, signal) for signal in runtime["signals"]}
    return {
        "connected": bool(runtime["status"]["modbus_running"]),
        "mode": "slave",
        "unit_id": int(runtime["unit_id"]),
        "signals": signals_out,
        "raw": {
            "input_registers": snapshot["input"],
            "holding_registers": snapshot["holding"],
            "coils": [],
            "discrete_inputs": [],
        },
        "error": runtime["status"].get("last_error") or None,
    }
