# EthernetConnectionChecker

A diagnostics-focused Arduino library for **Mega + Ethernet/SD shield** projects.

## Features

- Initializes Ethernet with DHCP or static IP.
- Probes multiple HTTP endpoints and validates status code ranges.
- Captures network metadata:
  - connection success/failure
  - latency
  - HTTP status code
  - short payload preview (when available)
- Saves data to SD in multiple formats:
  - `/netlog.csv` (human-readable rows)
  - `/netmetric.bin` (fixed-size binary records)
  - `/payloads.txt` (response snippets)
- LED flash status codes for headless diagnostics.
- Built-in management page served by the shield web server.
- Shared SPI bus arbitration between Ethernet and SD chip select lines.
- SD reinitialization support for transient startup failures.
- Built-in network diagnostics in dashboard and JSON APIs.

## Dashboard and APIs

- `/` : HTML dashboard
- `/run` : run endpoint probes immediately
- `/reinit-sd` : retry SD initialization
- `/clear` : clear in-memory logs/results
- `/api/diag` : JSON network/system diagnostics
- `/api/results` : JSON results with status ranges and redirect metadata

The dashboard now shows:
- local IP, gateway, subnet, DNS
- Ethernet hardware type and link status
- per-endpoint accepted status range
- redirect location for HTTP 3xx responses when present

## LED Flash Codes

- 1 flash: normal success state
- 2 flashes: one or more endpoint checks failed
- 3 flashes: SD initialization failed
- 4 flashes: DHCP initialization failed
- 5 flashes: checks requested while Ethernet is not ready

## Basic Usage

```cpp
#include <EthernetConnectionChecker.h>

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void setup() {
  checker.begin(mac);

  checker.addGatewayEndpoint("Gateway", "/", 80, 100, 599);
  checker.addEndpoint("IANA", "iana.org", "/", 80, 200, 399);
  checker.addReachabilityEndpoint("Arduino Reachability", "www.arduino.cc", "/", 80);

  checker.setCheckInterval(120000UL);
  checker.enableAutoChecks(true);
  checker.runChecks();
}

void loop() {
  checker.loop();
}
```

Open `http://<board-ip>/` to view the management dashboard.

## Endpoint Modes

- `addEndpoint(...)`: strict status acceptance with custom range.
- `addReachabilityEndpoint(...)`: accepts `100-599` for reachability-only checks.
- `addGatewayEndpoint(...)`: probes the active DHCP/static gateway IP.

## Notes

- This library uses HTTP checks (`EthernetClient`) and does not support HTTPS/TLS directly on classic W5100-style shields.
- For stable operation on shared SPI shields:
  - Ethernet CS: pin 10
  - SD CS: pin 4
- If SD fails at startup, call `reinitSd()` after Ethernet comes up.
