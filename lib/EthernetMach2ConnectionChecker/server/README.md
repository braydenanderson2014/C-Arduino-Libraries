# EthernetMach2ConnectionChecker Server

This is the PC-side FastAPI service for the `EthernetMach2ConnectionChecker` Arduino agent.

## What It Does

- persists device registration, heartbeats, configs, and result batches in SQLite
- persists agent-uploaded network snapshots including speed tests, device scans, and path traces
- serves a browser dashboard at `/`
- accepts device registration and heartbeat packets from Mega agents
- stores the latest result batch plus recent history for each device
- exposes per-device config for agents to poll and for operators to edit
- lets operators queue guided commands for each Mega agent from the server UI

## Quick Start

```powershell
cd lib/EthernetMach2ConnectionChecker/server
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
uvicorn app:app --reload --host 0.0.0.0 --port 8080
```

Then open `http://localhost:8080/` for the dashboard.

The SQLite database is created automatically at `server/data/mach2_server.db`.

## Endpoints

- `GET /`
- `GET /health`
- `GET /api/v1/overview`
- `POST /api/v1/register`
- `POST /api/v1/heartbeat`
- `POST /api/v1/results/batch`
- `POST /api/v1/network/snapshot`
- `GET /api/v1/config/{deviceId}`
- `PUT /api/v1/config/{deviceId}`
- `GET /api/v1/devices`
- `GET /api/v1/device/{deviceId}`
- `GET /api/v1/results/{deviceId}`
- `POST /api/v1/commands/{deviceId}`
- `GET /api/v1/commands/next/{deviceId}`
- `POST /api/v1/commands/{commandId}/complete`
- `GET /api/v1/commands/{deviceId}`

## Dashboard Workflow

1. Start the FastAPI service on the PC.
2. Point the Mega agent at the PC LAN IP and port `8080`.
3. Wait for the agent to call `register` and `heartbeat`.
4. Open the dashboard to inspect latest checks, snapshots, device inventory, and queued commands.
5. Queue actions such as `run_checks`, `run_speed_test`, `scan_subnet`, `run_path_trace`, or `run_full_survey` from the dashboard.
6. The Mega polls `GET /api/v1/commands/next/{deviceId}`, executes the action, uploads new results/snapshots, and completes the command.
7. The agent can later fetch its config from `GET /api/v1/config/{deviceId}`.

## Folder Layout

- `app.py`: FastAPI app, SQLite schema, API handlers.
- `static/index.html`: operator dashboard.
- `data/`: runtime SQLite database location.
- `requirements.txt`: Python dependencies.

## Notes

- This server currently uses SQLite only; there is no auth layer yet.
- Use the PC's LAN IP address in the Arduino example so the Mega can reach the server.
- The Mega now supports server-guided commands for checks, snapshots, speed tests, subnet scans, and a lightweight path trace.
- The path trace is application-level and HTTP-based; it is not a true TTL/ICMP traceroute.
- The current Arduino agent scaffold still needs config parsing on-device to fully consume edited probe lists.
