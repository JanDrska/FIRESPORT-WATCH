window.MACHINE_CONFIG = {
  "title": "M.A.R.S.",
  "subtitle": "Mobilní Automatická Rentgenovací Stanice",
  "screens": {
    "overview": {
      "kind": "scene",
      "background": {
        "src": "img/stroj.png",
        "viewBox": {
          "w": 1600,
          "h": 2000
        }
      },
      "blocks": [
        {
          "id": "B52353",
          "type": "dotLabel",
          "x": 600,
          "y": 1460,
          "label": "M1",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": true,
            "key": "alarm",
            "color": "#e74c3c",
            "hz": 2
          }
        },
        {
          "id": "D90973",
          "type": "dotLabel",
          "x": 750,
          "y": 520,
          "label": "M2",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D81206",
          "type": "dotLabel",
          "x": 590,
          "y": 1030,
          "label": "M5",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D35087",
          "type": "dotLabel",
          "x": 960,
          "y": 1030,
          "label": "M6",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D59076",
          "type": "dotLabel",
          "x": 950,
          "y": 390,
          "label": "M4",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D29052",
          "type": "dotLabel",
          "x": 800,
          "y": 140,
          "label": "M3",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": true,
            "key": "alarm",
            "color": "#e74c3c",
            "hz": 2
          }
        },
        {
          "id": "D36122",
          "type": "dotLabel",
          "x": 440,
          "y": 130,
          "label": "M7",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 50,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.5
          },
          "blink": {
            "enabled": true,
            "key": "alarm",
            "color": "#e74c3c",
            "hz": 2
          }
        },
        {
          "id": "D33328",
          "type": "dotLabel",
          "x": 340,
          "y": 710,
          "label": "S14",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D92604",
          "type": "dotLabel",
          "x": 340,
          "y": 790,
          "label": "S15",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D23138",
          "type": "dotLabel",
          "x": 340,
          "y": 610,
          "label": "S13",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D87432",
          "type": "dotLabel",
          "x": 350,
          "y": 1040,
          "label": "S16",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D70274",
          "type": "dotLabel",
          "x": 350,
          "y": 1160,
          "label": "S17",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D29591",
          "type": "dotLabel",
          "x": 350,
          "y": 1290,
          "label": "S18",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "B84562",
          "type": "dotLabel",
          "x": 750,
          "y": 1030,
          "label": "RTG",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 80,
            "boxH": 48,
            "onColor": "#e74c3c",
            "offColor": "#2ecc71",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": true,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "B37247",
          "type": "button",
          "x": 70,
          "y": 780,
          "label": "Tlačítko",
          "bind": {
            "key": "DO1",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#f1c40f",
            "boxAlpha": 0.8
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "TOGGLE",
            "action": "toggle",
            "key": "start_sensor",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D89557",
          "type": "button",
          "x": 70,
          "y": 660,
          "label": "Tlačítko",
          "bind": {
            "key": "DO1",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#e74c3c",
            "boxAlpha": 0.8
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "RESET",
            "action": "reset",
            "key": "DO1",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D65357",
          "type": "button",
          "x": 70,
          "y": 540,
          "label": "Tlačítko",
          "bind": {
            "key": "DO1",
            "type": "bool"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#2ecc71",
            "boxAlpha": 0.8
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "SET",
            "action": "set",
            "key": "sensor_enable",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D66742",
          "type": "button",
          "x": 70,
          "y": 900,
          "label": "Tlačítko",
          "bind": {
            "key": "DO1",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "HOLD",
            "action": "momentary",
            "key": "DO1",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "B88323",
          "type": "button",
          "x": 70,
          "y": 100,
          "label": "",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#e74c3c",
            "boxAlpha": 1
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "STOP",
            "action": "reset",
            "key": "motor_running",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "B93365",
          "type": "button",
          "x": 70,
          "y": 1020,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": true,
            "key": "motor_running",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BLIK",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "strokeFill",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "B87425",
          "type": "button",
          "x": 70,
          "y": 1140,
          "label": "Tlačítko",
          "bind": {
            "key": "DO1",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "DO1",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "B83922",
          "type": "valueDot",
          "x": 390,
          "y": 1790,
          "label": "Rychlost",
          "bind": {
            "key": "speed",
            "type": "number"
          },
          "unit": "m/s",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 320,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "B84683",
          "type": "inputSend",
          "x": 780,
          "y": 1830,
          "label": "Hodnota",
          "bind": {
            "key": "Setpoint_axis_1",
            "type": "number"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 320,
            "boxH": 56,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "input": {
            "mode": "u16",
            "key": "Setpoint_axis_1",
            "addr": 0,
            "placeholder": "Zadej hodnotu...",
            "showType": true,
            "sig": "Setpoint_axis_1"
          }
        },
        {
          "id": "B38063",
          "type": "labelValue",
          "x": 410,
          "y": 1830,
          "label": "Popisek",
          "bind": {
            "key": "Setpoint_axis_1",
            "type": "number"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 320,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "B28978",
          "type": "inputEnter",
          "x": 780,
          "y": 1770,
          "label": "Hodnota",
          "bind": {
            "key": "speed",
            "type": "number"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 320,
            "boxH": 56,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "input": {
            "mode": "setpoint",
            "key": "speed",
            "addr": 0,
            "placeholder": "Zadej hodnotu...",
            "showType": true,
            "sig": ""
          }
        },
        {
          "id": "D72762",
          "type": "button",
          "x": 70,
          "y": 300,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D40478",
          "type": "button",
          "x": 70,
          "y": 420,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D51863",
          "type": "button",
          "x": 70,
          "y": 1260,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D18728",
          "type": "button",
          "x": 70,
          "y": 1380,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D59337",
          "type": "button",
          "x": 70,
          "y": 1500,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D72141",
          "type": "button",
          "x": 70,
          "y": 1620,
          "label": "Tlačítko",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 220,
            "boxH": 100,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "BTN",
            "action": "toggle",
            "key": "",
            "stateKey": "",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "D41620",
          "type": "dotLabel",
          "x": 1190,
          "y": 650,
          "label": "S9",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D37304",
          "type": "dotLabel",
          "x": 1190,
          "y": 740,
          "label": "S8",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D81075",
          "type": "dotLabel",
          "x": 1190,
          "y": 830,
          "label": "S7",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D76763",
          "type": "dotLabel",
          "x": 1190,
          "y": 1040,
          "label": "S6",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D19398",
          "type": "dotLabel",
          "x": 1190,
          "y": 1160,
          "label": "S5",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D39544",
          "type": "dotLabel",
          "x": 1190,
          "y": 1290,
          "label": "S4",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D82063",
          "type": "dotLabel",
          "x": 890,
          "y": 1670,
          "label": "S3",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D48588",
          "type": "dotLabel",
          "x": 760,
          "y": 1670,
          "label": "S2",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D26511",
          "type": "dotLabel",
          "x": 640,
          "y": 1670,
          "label": "S1",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D31127",
          "type": "dotLabel",
          "x": 880,
          "y": 290,
          "label": "S10",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D89108",
          "type": "dotLabel",
          "x": 750,
          "y": 290,
          "label": "S11",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        },
        {
          "id": "D33235",
          "type": "dotLabel",
          "x": 620,
          "y": 290,
          "label": "S12",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 65,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#ff0000",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        }
      ]
    },
    "control": {
      "kind": "canvas",
      "canvas": {
        "w": 1100,
        "h": 260
      },
      "blocks": [
        {
          "id": "B41566",
          "type": "dotLabel",
          "x": 170,
          "y": 80,
          "label": "Indikátor",
          "bind": {
            "key": "motor_running",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 320,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#ffffff",
            "boxBg": "#000000",
            "boxAlpha": 0.55
          },
          "blink": {
            "enabled": true,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          }
        }
      ]
    },
    "io": {
      "kind": "canvas",
      "canvas": {
        "w": 1100,
        "h": 220
      },
      "blocks": []
    }
  }
};
