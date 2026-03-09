# HMI + Modbus RTU slave odděleně od webu

Tahle verze běží jako **2 samostatné procesy**:

- `modbus_server` = Modbus RTU slave na sériové lince
- `web_server` = FastAPI + web HMI

Web už neleze do stejného event loopu jako Modbus server. Oba procesy sdílí stejné registry v RAM, takže:

- master pořád čte celé registry
- web pořád čte celé registry
- web může do registrů i zapisovat
- coils a discrete inputs jsou ponechané jen kvůli kompatibilitě, ale HMI používá hlavně `holding` a `input`

## Spuštění

```bat
cd "C:\cesta\k\HMI - STOPKY"
python -m pip install --upgrade pip
python -m pip install fastapi uvicorn pymodbus pyserial
python main.py
```

Pak otevři:

```text
http://127.0.0.1:8000
```

## Modbus RTU

V `signals.json` je sériovka nastavená přes:

```json
"modbus": {
  "serial_port": "COM8",
  "baudrate": 115200,
  "bytesize": 8,
  "parity": "N",
  "stopbits": 1,
  "timeout": 0.02,
  "device_id": 3
}
```

Doporučený timeout pro Python slave je tady už snížený na `0.02` s, aby zbytečně neblokoval linku.

## Poznámka

Registry se dál vrací celé přes `/api/state`:

- `raw.input_registers`
- `raw.holding_registers`

Takže se chovají stejně jako předtím, jen Modbus a web už běží odděleně.
