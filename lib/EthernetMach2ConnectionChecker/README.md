# EthernetMach2ConnectionChecker

`EthernetMach2ConnectionChecker` is the Mach 2 version of the original Ethernet checker concept.

Instead of hosting a dashboard on the Arduino Mega itself, the Mega becomes a lightweight probe agent that:

- initializes Ethernet
- runs HTTP reachability and status checks
- sends heartbeat and result batches to a PC-hosted server
- polls the server for configuration updates

The PC server owns the dashboard, storage, and operator controls.

## Folder Layout

- `src/`: Arduino library source
- `examples/`: starter Arduino sketches
- `server/`: FastAPI starter service for ingest, config, and dashboard APIs

## Planned Agent Workflow

1. Boot and acquire network access.
2. Register with the server.
3. Poll for configuration.
4. Run probe batches on a timer.
5. Send heartbeat and result payloads back to the server.

## Planned Server Endpoints

- `POST /api/v1/register`
- `POST /api/v1/heartbeat`
- `POST /api/v1/results/batch`
- `GET /api/v1/config/{deviceId}`
- `GET /health`

## Example Usage

```cpp
#include <EthernetMach2ConnectionChecker.h>

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
EM2ServerTarget server("192.168.1.20", 8080, "/api/v1");
EthernetMach2ConnectionChecker agent(10);

void setup() {
  Serial.begin(115200);
  if (!agent.begin(mac, server)) {
    return;
  }

  agent.addProbe("Google Connectivity", "connectivitycheck.gstatic.com", "/generate_204", 80, 204, 204);
  agent.addProbe("Gateway", "192.168.1.1", "/", 80, 100, 599);
  agent.registerAgent();
  agent.sendHeartbeat();
}

void loop() {
  agent.loop();
}
```

## Notes

- This scaffold is intentionally small and is meant to be expanded iteratively.
- The current server is an in-memory FastAPI starter, not a production deployment.
- The Mega remains HTTP-only when used with classic W5100/W5500 style shields.
