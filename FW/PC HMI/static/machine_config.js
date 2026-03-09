window.MACHINE_CONFIG = {
  "title": "M.A.R.S.",
  "subtitle": "Mobilní Automatická Rentgenovací Stanice",
  "defaults": {
    "boxBg": "#000000",
    "boxAlpha": 0.55,
    "palette": [
      {
        "name": "červená",
        "value": "#e74c3c"
      },
      {
        "name": "zelená",
        "value": "#2ecc71"
      },
      {
        "name": "šedá",
        "value": "#bbbbbb"
      },
      {
        "name": "černá",
        "value": "#000000"
      },
      {
        "name": "bílá",
        "value": "#ffffff"
      },
      {
        "name": "oranžová",
        "value": "#f39c12"
      },
      {
        "name": "modrá",
        "value": "#3498db"
      },
      {
        "name": "žlutá",
        "value": "#f1c40f"
      }
    ]
  },
  "api": {
    "stateUrl": "/api/state",
    "writeTemplate": "/api/setpoint/{key}/{value}"
  },
  "screens": {
    "overview": {
      "kind": "scene",
      "background": {
        "src": "img/stroj.png",
        "viewBox": {
          "w": 2000,
          "h": 1414
        }
      },
      "blocks": [
        {
          "id": "B84562",
          "type": "dotLabel",
          "x": 1710,
          "y": 560,
          "label": "STAV",
          "bind": {
            "key": "target_l_light_blink",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#e74c3c",
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
          "y": 500,
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
            "text": "PŘIPRAVIT",
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
          "id": "D89557",
          "type": "button",
          "x": 70,
          "y": 620,
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
            "text": "RESTART MĚŘENÍ",
            "action": "reset",
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
          "id": "D65357",
          "type": "button",
          "x": 70,
          "y": 380,
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
            "text": "START",
            "action": "momentary",
            "key": "start_btn",
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
          "x": 310,
          "y": 500,
          "label": "Tlačítko",
          "bind": {
            "key": "horn",
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
            "text": "HOUKAČKA",
            "action": "momentary",
            "key": "stop_btn",
            "stateKey": "horn",
            "onValue": 1,
            "offValue": 0,
            "colorMode": "stroke",
            "colorKey": "horn",
            "visKey": "",
            "visMode": "hideIfEqual",
            "visValue": 1
          }
        },
        {
          "id": "B88323",
          "type": "button",
          "x": 310,
          "y": 380,
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
            "action": "momentary",
            "key": "stop_btn",
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
          "x": 1360,
          "y": 1090,
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
            "text": "VYPRÁZDNIT TER.",
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
          "id": "D26352",
          "type": "button",
          "x": 1100,
          "y": 1090,
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
            "text": "VYNULOVAT",
            "action": "reset",
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
          "id": "B63156",
          "type": "labelValue",
          "x": 1300,
          "y": 580,
          "label": "",
          "bind": {
            "key": "casTERC1_S",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 100,
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
          }
        },
        {
          "id": "D33367",
          "type": "labelValue",
          "x": 1440,
          "y": 580,
          "label": "",
          "bind": {
            "key": "casTERC1_ms",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 150,
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
          }
        },
        {
          "id": "D82647",
          "type": "labelValue",
          "x": 1300,
          "y": 900,
          "label": "",
          "bind": {
            "key": "casTERC2_S",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 100,
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
          }
        },
        {
          "id": "D18870",
          "type": "labelValue",
          "x": 1440,
          "y": 900,
          "label": "",
          "bind": {
            "key": "casTERC2_ms",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 150,
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
          }
        },
        {
          "id": "D28385",
          "type": "dotLabel",
          "x": 1710,
          "y": 620,
          "label": "PLNÝ",
          "bind": {
            "key": "target_l_full",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
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
          "id": "D58819",
          "type": "dotLabel",
          "x": 1710,
          "y": 680,
          "label": "PRÁZDNÝ",
          "bind": {
            "key": "target_l_empty",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
            "boxH": 48,
            "onColor": "#e74c3c",
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
          "id": "D98514",
          "type": "dotLabel",
          "x": 1700,
          "y": 860,
          "label": "STAV",
          "bind": {
            "key": "target_r_light_blink",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
            "boxH": 48,
            "onColor": "#2ecc71",
            "offColor": "#e74c3c",
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
          "id": "D69502",
          "type": "dotLabel",
          "x": 1700,
          "y": 920,
          "label": "PLNÝ",
          "bind": {
            "key": "target_r_full",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
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
          "id": "D43294",
          "type": "dotLabel",
          "x": 1700,
          "y": 980,
          "label": "PRÁZDNÝ",
          "bind": {
            "key": "target_r_empty",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 160,
            "boxH": 48,
            "onColor": "#e74c3c",
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
        },
        {
          "id": "B50377",
          "type": "text",
          "x": 1320,
          "y": 560,
          "label": "s",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 320,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D88487",
          "type": "text",
          "x": 1320,
          "y": 880,
          "label": "s",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 320,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D54182",
          "type": "text",
          "x": 1460,
          "y": 560,
          "label": "ms",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 320,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D63716",
          "type": "text",
          "x": 1460,
          "y": 880,
          "label": "ms",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 320,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D40987",
          "type": "dotLabel",
          "x": 90,
          "y": 860,
          "label": "OPTICKÁ BRÁNA",
          "bind": {
            "key": "horn",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 200,
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
          "id": "D76548",
          "type": "dotLabel",
          "x": 90,
          "y": 920,
          "label": "START POVOLEN",
          "bind": {
            "key": "horn",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 200,
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
          "id": "D16882",
          "type": "button",
          "x": 310,
          "y": 620,
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
            "boxBg": "#3498db",
            "boxAlpha": 0.8
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "button": {
            "text": "INICIALIZACE",
            "action": "reset",
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
          "id": "D45096",
          "type": "button",
          "x": 90,
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
            "boxW": 180,
            "boxH": 60,
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
            "text": "START ENABLE",
            "action": "toggle",
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
          "id": "D48826",
          "type": "button",
          "x": 90,
          "y": 1100,
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
            "boxW": 180,
            "boxH": 60,
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
            "text": "START SENZOR",
            "action": "momentary",
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
          "id": "B36223",
          "type": "dotOnly",
          "x": 320,
          "y": 1050,
          "label": "Indikátor",
          "bind": {
            "key": "sensor_enable",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 0,
            "boxH": 0,
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
          "id": "D17320",
          "type": "dotOnly",
          "x": 320,
          "y": 1120,
          "label": "Indikátor",
          "bind": {
            "key": "start_sensor",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 22,
            "radius": 12,
            "boxW": 0,
            "boxH": 0,
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
          "id": "B23486",
          "type": "labelValue",
          "x": 1350,
          "y": 1330,
          "label": "Napětí baterie terčů",
          "bind": {
            "key": "target_vbatt",
            "type": "number"
          },
          "unit": "V",
          "decimals": 2,
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
          "id": "D24459",
          "type": "text",
          "x": 1410,
          "y": 960,
          "label": ":",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D59535",
          "type": "text",
          "x": 1410,
          "y": 650,
          "label": ":",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D19756",
          "type": "labelValue",
          "x": 1160,
          "y": 900,
          "label": "",
          "bind": {
            "key": "casTERC2_M",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 100,
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
          }
        },
        {
          "id": "D97115",
          "type": "text",
          "x": 1270,
          "y": 960,
          "label": ":",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D26462",
          "type": "text",
          "x": 1270,
          "y": 640,
          "label": ":",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D39262",
          "type": "text",
          "x": 1180,
          "y": 880,
          "label": "M",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D62572",
          "type": "text",
          "x": 1180,
          "y": 560,
          "label": "M",
          "bind": {
            "key": "",
            "type": "bool"
          },
          "unit": "",
          "decimals": 1,
          "style": {
            "fontSize": 64,
            "radius": 12,
            "boxW": 50,
            "boxH": 40,
            "onColor": "#2ecc71",
            "offColor": "#bbbbbb",
            "textColor": "#000000",
            "boxBg": "#000000",
            "boxAlpha": 0.55,
            "fontWeight": "600",
            "fontStyle": "normal",
            "textAnchor": "start"
          },
          "blink": {
            "enabled": false,
            "key": "",
            "color": "#f1c40f",
            "hz": 2
          },
          "text": "Text"
        },
        {
          "id": "D70050",
          "type": "labelValue",
          "x": 1160,
          "y": 580,
          "label": "",
          "bind": {
            "key": "casTERC1_M",
            "type": "number"
          },
          "unit": "",
          "decimals": 0,
          "style": {
            "fontSize": 45,
            "radius": 12,
            "boxW": 100,
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
