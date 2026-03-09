import multiprocessing as mp
import os
import signal
import time
from pathlib import Path

from modbus_server import run_modbus_process
from shared_state import build_runtime_config, preload_defaults
from web_server import run_web_process


BASE_DIR = Path(__file__).resolve().parent


def build_shared_runtime(ctx: mp.context.BaseContext) -> dict:
    runtime = build_runtime_config(BASE_DIR)
    runtime["state"] = {
        "coil": ctx.Array("H", runtime["sizes"]["coil"], lock=False),
        "discrete": ctx.Array("H", runtime["sizes"]["discrete"], lock=False),
        "input": ctx.Array("H", runtime["sizes"]["input"], lock=False),
        "holding": ctx.Array("H", runtime["sizes"]["holding"], lock=False),
    }
    runtime["lock"] = ctx.RLock()
    manager = ctx.Manager()
    runtime["status"] = manager.dict(modbus_running=False, last_error="")
    preload_defaults(runtime)
    return runtime


def main() -> None:
    mp.freeze_support()
    ctx = mp.get_context("spawn")
    runtime = build_shared_runtime(ctx)

    modbus_proc = ctx.Process(target=run_modbus_process, args=(runtime,), name="modbus_server")
    web_proc = ctx.Process(target=run_web_process, args=(runtime,), name="web_server")

    modbus_proc.start()
    web_proc.start()

    try:
        while True:
            if not modbus_proc.is_alive() or not web_proc.is_alive():
                break
            time.sleep(0.2)
    except KeyboardInterrupt:
        pass
    finally:
        for proc in (web_proc, modbus_proc):
            if proc.is_alive():
                proc.terminate()
        for proc in (web_proc, modbus_proc):
            proc.join(timeout=3)


if __name__ == "__main__":
    main()
