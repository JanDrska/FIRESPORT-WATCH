import time
from typing import Optional

import serial

from shared_state import get_values, set_values


def _crc16_modbus(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 0x0001:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def _append_crc(payload: bytes) -> bytes:
    crc = _crc16_modbus(payload)
    return payload + bytes((crc & 0xFF, (crc >> 8) & 0xFF))


def _valid_crc(frame: bytes) -> bool:
    if len(frame) < 4:
        return False
    return _crc16_modbus(frame[:-2]) == (frame[-2] | (frame[-1] << 8))


def _u16be(value: int) -> bytes:
    return bytes(((value >> 8) & 0xFF, value & 0xFF))


def _exception_response(unit_id: int, function: int, code: int) -> bytes:
    return _append_crc(bytes((unit_id, function | 0x80, code & 0xFF)))


def _parse_frame_length(buf: bytearray) -> Optional[int]:
    if len(buf) < 2:
        return None

    fc = buf[1]

    if fc in (3, 4):
        if len(buf) >= 8:
            req_len = 8
            if _valid_crc(bytes(buf[:req_len])):
                return req_len
        if len(buf) >= 5:
            byte_count = buf[2]
            resp_len = 5 + byte_count
            if byte_count % 2 == 0 and len(buf) >= resp_len and _valid_crc(bytes(buf[:resp_len])):
                return resp_len
        return None

    if fc == 16:
        if len(buf) >= 8 and _valid_crc(bytes(buf[:8])):
            return 8
        if len(buf) >= 7:
            byte_count = buf[6]
            req_len = 9 + byte_count
            if len(buf) >= req_len and _valid_crc(bytes(buf[:req_len])):
                return req_len
        return None

    if fc in (6,):
        if len(buf) >= 8 and _valid_crc(bytes(buf[:8])):
            return 8
        return None

    if fc & 0x80:
        if len(buf) >= 5 and _valid_crc(bytes(buf[:5])):
            return 5
        return None

    return None


def _build_response(runtime: dict, frame: bytes) -> Optional[bytes]:
    unit_id = int(runtime["unit_id"])
    if len(frame) < 4 or frame[0] != unit_id:
        return None

    fc = frame[1]

    if fc in (3, 4) and len(frame) == 8:
        address = (frame[2] << 8) | frame[3]
        quantity = (frame[4] << 8) | frame[5]
        if quantity < 1 or quantity > 125:
            return _exception_response(unit_id, fc, 3)

        bank = "holding" if fc == 3 else "input"
        size = int(runtime["sizes"][bank])
        if address + quantity > size:
            return _exception_response(unit_id, fc, 2)

        values = get_values(runtime, bank, address, quantity)
        payload = bytearray((unit_id, fc, quantity * 2))
        for value in values:
            payload.extend(_u16be(int(value) & 0xFFFF))
        return _append_crc(bytes(payload))

    if fc == 6 and len(frame) == 8:
        address = (frame[2] << 8) | frame[3]
        value = (frame[4] << 8) | frame[5]
        if address >= int(runtime["sizes"]["holding"]):
            return _exception_response(unit_id, fc, 2)
        set_values(runtime, "holding", address, [value])
        return frame

    if fc == 16 and len(frame) >= 11:
        address = (frame[2] << 8) | frame[3]
        quantity = (frame[4] << 8) | frame[5]
        byte_count = frame[6]
        if quantity < 1 or quantity > 123 or byte_count != quantity * 2:
            return _exception_response(unit_id, fc, 3)
        if address + quantity > int(runtime["sizes"]["holding"]):
            return _exception_response(unit_id, fc, 2)
        values = []
        idx = 7
        for _ in range(quantity):
            values.append((frame[idx] << 8) | frame[idx + 1])
            idx += 2
        set_values(runtime, "holding", address, values)
        return _append_crc(bytes((unit_id, fc, frame[2], frame[3], frame[4], frame[5])))

    return _exception_response(unit_id, fc, 1)


def run_modbus_process(runtime: dict) -> None:
    cfg = runtime["modbus"]
    runtime["status"]["modbus_running"] = True
    runtime["status"]["last_error"] = ""

    ser = None
    try:
        ser = serial.Serial(
            port=cfg.get("serial_port", cfg.get("port_name", "COM3")),
            baudrate=int(cfg.get("baudrate", 115200)),
            bytesize=int(cfg.get("bytesize", 8)),
            parity=cfg.get("parity", "N"),
            stopbits=int(cfg.get("stopbits", 1)),
            timeout=float(cfg.get("timeout", 0.01)),
            write_timeout=float(cfg.get("write_timeout", 0.01))
        )

        rx = bytearray()
        while True:
            chunk = ser.read(ser.in_waiting or 1)
            if chunk:
                rx.extend(chunk)
            else:
                time.sleep(0.0005)

            while rx:
                frame_len = _parse_frame_length(rx)
                if frame_len is None:
                    if len(rx) >= 8:
                        found = False
                        for offset in range(1, min(len(rx), 32)):
                            candidate_len = _parse_frame_length(rx[offset:])
                            if candidate_len is not None:
                                del rx[:offset]
                                found = True
                                break
                        if not found and len(rx) > 260:
                            del rx[:-8]
                    break

                frame = bytes(rx[:frame_len])
                del rx[:frame_len]

                response = _build_response(runtime, frame)
                if response:
                    ser.write(response)
                    ser.flush()
    except Exception as exc:
        runtime["status"]["last_error"] = repr(exc)
        raise
    finally:
        runtime["status"]["modbus_running"] = False
        if ser is not None:
            try:
                ser.close()
            except Exception:
                pass
