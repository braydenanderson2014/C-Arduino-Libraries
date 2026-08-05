from contextlib import asynccontextmanager
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import sqlite3
from typing import Any, Dict, List

from fastapi import FastAPI, HTTPException
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel, Field


BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
STATIC_DIR = BASE_DIR / "static"
DB_PATH = DATA_DIR / "mach2_server.db"
STALE_AFTER_SECONDS = 120


def utc_now_iso() -> str:
    return datetime.now(timezone.utc).isoformat()


def get_connection() -> sqlite3.Connection:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    connection = sqlite3.connect(DB_PATH)
    connection.row_factory = sqlite3.Row
    return connection


def init_db() -> None:
    with get_connection() as connection:
        connection.executescript(
            """
            CREATE TABLE IF NOT EXISTS devices (
                device_id TEXT PRIMARY KEY,
                ip TEXT NOT NULL DEFAULT '',
                agent_name TEXT NOT NULL DEFAULT '',
                firmware TEXT NOT NULL DEFAULT '',
                registered_at TEXT NOT NULL,
                last_seen_at TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS heartbeats (
                device_id TEXT PRIMARY KEY,
                ip TEXT NOT NULL DEFAULT '',
                heartbeat_at_ms INTEGER NOT NULL DEFAULT 0,
                result_count INTEGER NOT NULL DEFAULT 0,
                event_count INTEGER NOT NULL DEFAULT 0,
                received_at TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS device_configs (
                device_id TEXT PRIMARY KEY,
                config_json TEXT NOT NULL,
                updated_at TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS latest_results (
                device_id TEXT PRIMARY KEY,
                ip TEXT NOT NULL DEFAULT '',
                checked_at_ms INTEGER NOT NULL DEFAULT 0,
                received_at TEXT NOT NULL,
                results_json TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS result_history (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id TEXT NOT NULL,
                ip TEXT NOT NULL DEFAULT '',
                checked_at_ms INTEGER NOT NULL DEFAULT 0,
                received_at TEXT NOT NULL,
                results_json TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS latest_network_snapshots (
                device_id TEXT PRIMARY KEY,
                received_at TEXT NOT NULL,
                snapshot_json TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS network_snapshot_history (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id TEXT NOT NULL,
                received_at TEXT NOT NULL,
                snapshot_json TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS control_commands (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id TEXT NOT NULL,
                action TEXT NOT NULL,
                host TEXT NOT NULL DEFAULT '',
                path TEXT NOT NULL DEFAULT '',
                port INTEGER NOT NULL DEFAULT 80,
                start_octet INTEGER NOT NULL DEFAULT 1,
                end_octet INTEGER NOT NULL DEFAULT 30,
                status TEXT NOT NULL DEFAULT 'queued',
                summary TEXT NOT NULL DEFAULT '',
                created_at TEXT NOT NULL,
                updated_at TEXT NOT NULL
            );
            """
        )


@asynccontextmanager
async def lifespan(_: FastAPI):
    init_db()
    yield


app = FastAPI(
    title="EthernetMach2ConnectionChecker Server",
    version="0.2.0",
    description="Persistent ingest, config, and dashboard service for Arduino Mega network agents.",
    lifespan=lifespan,
)
app.mount("/static", StaticFiles(directory=STATIC_DIR, check_dir=False), name="static")


class ProbeConfig(BaseModel):
    name: str
    host: str
    path: str = "/"
    port: int = Field(default=80, ge=1, le=65535)
    minAcceptedStatus: int = Field(default=200, ge=100, le=599)
    maxAcceptedStatus: int = Field(default=399, ge=100, le=599)


class DeviceConfig(BaseModel):
    checkIntervalMs: int = Field(default=60000, ge=0)
    heartbeatIntervalMs: int = Field(default=30000, ge=0)
    probes: List[ProbeConfig] = Field(default_factory=list)


class RegisterRequest(BaseModel):
    deviceId: str
    ip: str = ""
    agentName: str = "EthernetMach2ConnectionChecker"
    firmware: str = ""


class HeartbeatRequest(BaseModel):
    deviceId: str
    ip: str = ""
    heartbeatAtMs: int = 0
    resultCount: int = 0
    eventCount: int = 0


class ProbeResultPayload(BaseModel):
    name: str
    host: str
    path: str = "/"
    status: int = -1
    connected: bool = False
    accepted: bool = False
    latencyMs: int = 0
    checkedAtMs: int = 0
    note: str = ""


class ResultsBatchRequest(BaseModel):
    deviceId: str
    ip: str = ""
    checkedAtMs: int = 0
    results: List[ProbeResultPayload] = Field(default_factory=list)


class ResultsSummaryPayload(BaseModel):
    totalCount: int = 0
    acceptedCount: int = 0
    failedCount: int = 0


class SpeedTestPayload(BaseModel):
    success: bool = False
    connectMs: int = 0
    durationMs: int = 0
    bytesReceived: int = 0
    downloadBps: int = 0
    host: str = ""
    path: str = ""
    port: int = 80


class NetworkDevicePayload(BaseModel):
    host: str = ""
    port: int = 80
    isUp: bool = False
    latencyMs: int = 0
    lastSeenMs: int = 0


class TraceHopPayload(BaseModel):
    label: str = ""
    target: str = ""
    port: int = 80
    status: int = -1
    connected: bool = False
    accepted: bool = False
    latencyMs: int = 0
    note: str = ""


class NetworkSnapshotRequest(BaseModel):
    deviceId: str
    ip: str = ""
    gateway: str = ""
    dns: str = ""
    subnet: str = ""
    link: str = ""
    hardware: str = ""
    serverHost: str = ""
    serverPort: int = 80
    lastCheckAtMs: int = 0
    resultsSummary: ResultsSummaryPayload = Field(default_factory=ResultsSummaryPayload)
    speedTest: SpeedTestPayload = Field(default_factory=SpeedTestPayload)
    devices: List[NetworkDevicePayload] = Field(default_factory=list)
    pathTrace: List[TraceHopPayload] = Field(default_factory=list)


class CommandEnqueueRequest(BaseModel):
    action: str
    host: str = ""
    path: str = ""
    port: int = Field(default=80, ge=0, le=65535)
    startOctet: int = Field(default=1, ge=1, le=254)
    endOctet: int = Field(default=30, ge=1, le=254)


class CommandCompletionRequest(BaseModel):
    deviceId: str
    status: str = "completed"
    summary: str = ""


def serialize_config(config: DeviceConfig) -> str:
    return json.dumps(config.model_dump(), separators=(",", ":"))


def deserialize_config(raw: str) -> DeviceConfig:
    try:
        data = json.loads(raw)
        return DeviceConfig.model_validate(data)
    except (json.JSONDecodeError, ValueError, TypeError):
        return DeviceConfig()


def decode_results(raw: str) -> List[Dict[str, Any]]:
    try:
        data = json.loads(raw)
    except json.JSONDecodeError:
        return []

    if not isinstance(data, list):
        return []
    return [item for item in data if isinstance(item, dict)]


def decode_snapshot(raw: str) -> Dict[str, Any]:
    try:
        data = json.loads(raw)
    except json.JSONDecodeError:
        return {}

    if not isinstance(data, dict):
        return {}
    return data


def upsert_registered_device(
    connection: sqlite3.Connection,
    device_id: str,
    ip: str,
    agent_name: str,
    firmware: str,
    seen_at: str,
) -> None:
    connection.execute(
        """
        INSERT INTO devices (
            device_id, ip, agent_name, firmware, registered_at, last_seen_at
        ) VALUES (?, ?, ?, ?, ?, ?)
        ON CONFLICT(device_id) DO UPDATE SET
            ip = excluded.ip,
            agent_name = excluded.agent_name,
            firmware = excluded.firmware,
            last_seen_at = excluded.last_seen_at
        """,
        (device_id, ip, agent_name, firmware, seen_at, seen_at),
    )


def touch_device(
    connection: sqlite3.Connection,
    device_id: str,
    ip: str,
    seen_at: str,
) -> None:
    connection.execute(
        """
        INSERT INTO devices (
            device_id, ip, agent_name, firmware, registered_at, last_seen_at
        ) VALUES (?, ?, '', '', ?, ?)
        ON CONFLICT(device_id) DO UPDATE SET
            ip = CASE WHEN excluded.ip != '' THEN excluded.ip ELSE devices.ip END,
            last_seen_at = excluded.last_seen_at
        """,
        (device_id, ip, seen_at, seen_at),
    )


def ensure_config(connection: sqlite3.Connection, device_id: str) -> DeviceConfig:
    row = connection.execute(
        "SELECT config_json FROM device_configs WHERE device_id = ?",
        (device_id,),
    ).fetchone()
    if row is not None:
        return deserialize_config(row["config_json"])

    config = DeviceConfig()
    connection.execute(
        "INSERT INTO device_configs (device_id, config_json, updated_at) VALUES (?, ?, ?)",
        (device_id, serialize_config(config), utc_now_iso()),
    )
    return config


def fetch_config(connection: sqlite3.Connection, device_id: str) -> Dict[str, Any]:
    ensure_config(connection, device_id)
    row = connection.execute(
        "SELECT config_json, updated_at FROM device_configs WHERE device_id = ?",
        (device_id,),
    ).fetchone()
    config = deserialize_config(row["config_json"] if row is not None else "{}")
    updated_at = row["updated_at"] if row is not None else utc_now_iso()
    return {
        "config": config,
        "updatedAt": updated_at,
    }


def save_config(connection: sqlite3.Connection, device_id: str, config: DeviceConfig) -> str:
    updated_at = utc_now_iso()
    connection.execute(
        """
        INSERT INTO device_configs (device_id, config_json, updated_at)
        VALUES (?, ?, ?)
        ON CONFLICT(device_id) DO UPDATE SET
            config_json = excluded.config_json,
            updated_at = excluded.updated_at
        """,
        (device_id, serialize_config(config), updated_at),
    )
    return updated_at


def seconds_since(timestamp: str) -> int | None:
    try:
        parsed = datetime.fromisoformat(timestamp)
    except ValueError:
        return None
    delta = datetime.now(timezone.utc) - parsed
    return max(0, int(delta.total_seconds()))


def summarize_results(results: List[Dict[str, Any]]) -> Dict[str, int]:
    total_count = len(results)
    accepted_count = 0
    failed_count = 0
    for result in results:
        if bool(result.get("accepted", False)):
            accepted_count += 1
        else:
            failed_count += 1
    return {
        "totalCount": total_count,
        "acceptedCount": accepted_count,
        "failedCount": failed_count,
    }


def row_to_dict(row: sqlite3.Row | None) -> Dict[str, Any] | None:
    if row is None:
        return None
    return {key: row[key] for key in row.keys()}


def command_row_to_dict(row: sqlite3.Row) -> Dict[str, Any]:
    return {
        "commandId": row["id"],
        "deviceId": row["device_id"],
        "action": row["action"],
        "host": row["host"],
        "path": row["path"],
        "port": row["port"],
        "startOctet": row["start_octet"],
        "endOctet": row["end_octet"],
        "status": row["status"],
        "summary": row["summary"],
        "createdAt": row["created_at"],
        "updatedAt": row["updated_at"],
    }


def device_status(
    device_row: sqlite3.Row,
    heartbeat_row: sqlite3.Row | None,
    result_row: sqlite3.Row | None,
) -> Dict[str, Any]:
    age_seconds = seconds_since(device_row["last_seen_at"])
    if heartbeat_row is None and result_row is None:
        return {
            "label": "registered",
            "ageSeconds": age_seconds,
            "isOnline": False,
        }

    if age_seconds is None or age_seconds > STALE_AFTER_SECONDS:
        return {
            "label": "stale",
            "ageSeconds": age_seconds,
            "isOnline": False,
        }

    summary = summarize_results(decode_results(result_row["results_json"])) if result_row else {
        "totalCount": 0,
        "acceptedCount": 0,
        "failedCount": 0,
    }
    if summary["failedCount"] > 0:
        return {
            "label": "warn",
            "ageSeconds": age_seconds,
            "isOnline": True,
        }

    return {
        "label": "ok",
        "ageSeconds": age_seconds,
        "isOnline": True,
    }


def build_overview() -> Dict[str, Any]:
    with get_connection() as connection:
        device_rows = connection.execute(
            "SELECT * FROM devices ORDER BY last_seen_at DESC, device_id ASC"
        ).fetchall()
        heartbeat_rows = {
            row["device_id"]: row
            for row in connection.execute("SELECT * FROM heartbeats").fetchall()
        }
        result_rows = {
            row["device_id"]: row
            for row in connection.execute("SELECT * FROM latest_results").fetchall()
        }
        config_rows = {
            row["device_id"]: row
            for row in connection.execute("SELECT * FROM device_configs").fetchall()
        }
        snapshot_rows = {
            row["device_id"]: row
            for row in connection.execute("SELECT * FROM latest_network_snapshots").fetchall()
        }
        pending_command_counts = {
            row["device_id"]: row["pending_count"]
            for row in connection.execute(
                """
                SELECT device_id, COUNT(*) AS pending_count
                FROM control_commands
                WHERE status IN ('queued', 'issued')
                GROUP BY device_id
                """
            ).fetchall()
        }

        devices: List[Dict[str, Any]] = []
        warn_count = 0
        stale_count = 0
        online_count = 0
        for device_row in device_rows:
            device_id = device_row["device_id"]
            heartbeat_row = heartbeat_rows.get(device_id)
            result_row = result_rows.get(device_id)
            config_row = config_rows.get(device_id)
            snapshot_row = snapshot_rows.get(device_id)
            status = device_status(device_row, heartbeat_row, result_row)
            latest_results = decode_results(result_row["results_json"]) if result_row else []
            result_summary = summarize_results(latest_results)
            config = deserialize_config(config_row["config_json"]) if config_row else DeviceConfig()
            snapshot = decode_snapshot(snapshot_row["snapshot_json"]) if snapshot_row else {}
            speed_test = snapshot.get("speedTest", {}) if isinstance(snapshot.get("speedTest", {}), dict) else {}
            snapshot_devices = snapshot.get("devices", []) if isinstance(snapshot.get("devices", []), list) else []
            path_trace = snapshot.get("pathTrace", []) if isinstance(snapshot.get("pathTrace", []), list) else []

            if status["label"] == "warn":
                warn_count += 1
            if status["label"] == "stale":
                stale_count += 1
            if status["isOnline"]:
                online_count += 1

            devices.append(
                {
                    "deviceId": device_id,
                    "ip": device_row["ip"],
                    "agentName": device_row["agent_name"],
                    "firmware": device_row["firmware"],
                    "registeredAt": device_row["registered_at"],
                    "lastSeenAt": device_row["last_seen_at"],
                    "status": status,
                    "latestHeartbeat": row_to_dict(heartbeat_row),
                    "latestResultsSummary": {
                        **result_summary,
                        "checkedAtMs": result_row["checked_at_ms"] if result_row else 0,
                        "receivedAt": result_row["received_at"] if result_row else None,
                    },
                    "networkSnapshotSummary": {
                        "receivedAt": snapshot_row["received_at"] if snapshot_row else None,
                        "link": snapshot.get("link", ""),
                        "hardware": snapshot.get("hardware", ""),
                        "deviceCount": len(snapshot_devices),
                        "traceHopCount": len(path_trace),
                        "speedBps": speed_test.get("downloadBps", 0),
                    },
                    "pendingCommandCount": pending_command_counts.get(device_id, 0),
                    "config": config.model_dump(),
                }
            )

        history_count = connection.execute(
            "SELECT COUNT(*) AS total FROM result_history"
        ).fetchone()["total"]
        snapshot_history_count = connection.execute(
            "SELECT COUNT(*) AS total FROM network_snapshot_history"
        ).fetchone()["total"]
        queued_command_count = connection.execute(
            "SELECT COUNT(*) AS total FROM control_commands WHERE status IN ('queued', 'issued')"
        ).fetchone()["total"]

    return {
        "serverTime": utc_now_iso(),
        "databasePath": str(DB_PATH),
        "stats": {
            "deviceCount": len(devices),
            "onlineCount": online_count,
            "warnCount": warn_count,
            "staleCount": stale_count,
            "resultHistoryCount": history_count,
            "snapshotHistoryCount": snapshot_history_count,
            "queuedCommandCount": queued_command_count,
        },
        "devices": devices,
    }


def build_device_detail(device_id: str) -> Dict[str, Any] | None:
    with get_connection() as connection:
        device_row = connection.execute(
            "SELECT * FROM devices WHERE device_id = ?",
            (device_id,),
        ).fetchone()
        if device_row is None:
            return None

        heartbeat_row = connection.execute(
            "SELECT * FROM heartbeats WHERE device_id = ?",
            (device_id,),
        ).fetchone()
        result_row = connection.execute(
            "SELECT * FROM latest_results WHERE device_id = ?",
            (device_id,),
        ).fetchone()
        snapshot_row = connection.execute(
            "SELECT * FROM latest_network_snapshots WHERE device_id = ?",
            (device_id,),
        ).fetchone()
        config_snapshot = fetch_config(connection, device_id)
        history_rows = connection.execute(
            """
            SELECT received_at, checked_at_ms, results_json
            FROM result_history
            WHERE device_id = ?
            ORDER BY id DESC
            LIMIT 10
            """,
            (device_id,),
        ).fetchall()
        command_rows = connection.execute(
            """
            SELECT *
            FROM control_commands
            WHERE device_id = ?
            ORDER BY id DESC
            LIMIT 12
            """,
            (device_id,),
        ).fetchall()

        latest_results = decode_results(result_row["results_json"]) if result_row else []
        status = device_status(device_row, heartbeat_row, result_row)
        snapshot = decode_snapshot(snapshot_row["snapshot_json"]) if snapshot_row else {}

        history = []
        for row in history_rows:
            decoded = decode_results(row["results_json"])
            history.append(
                {
                    "receivedAt": row["received_at"],
                    "checkedAtMs": row["checked_at_ms"],
                    "summary": summarize_results(decoded),
                }
            )

        return {
            "serverTime": utc_now_iso(),
            "device": {
                "deviceId": device_row["device_id"],
                "ip": device_row["ip"],
                "agentName": device_row["agent_name"],
                "firmware": device_row["firmware"],
                "registeredAt": device_row["registered_at"],
                "lastSeenAt": device_row["last_seen_at"],
                "status": status,
            },
            "heartbeat": row_to_dict(heartbeat_row),
            "config": {
                "updatedAt": config_snapshot["updatedAt"],
                "value": config_snapshot["config"].model_dump(),
            },
            "latestResults": {
                "checkedAtMs": result_row["checked_at_ms"] if result_row else 0,
                "receivedAt": result_row["received_at"] if result_row else None,
                "summary": summarize_results(latest_results),
                "results": latest_results,
            },
            "networkSnapshot": {
                "receivedAt": snapshot_row["received_at"] if snapshot_row else None,
                "value": snapshot,
            },
            "commands": [command_row_to_dict(row) for row in command_rows],
            "history": history,
        }


@app.get("/", include_in_schema=False)
def dashboard() -> FileResponse:
    dashboard_file = STATIC_DIR / "index.html"
    if not dashboard_file.exists():
        raise HTTPException(status_code=404, detail="Dashboard file not found")
    return FileResponse(dashboard_file)


@app.get("/health")
def health() -> Dict[str, Any]:
    overview = build_overview()
    return {
        "status": "ok",
        "serverTime": overview["serverTime"],
        "deviceCount": overview["stats"]["deviceCount"],
        "onlineCount": overview["stats"]["onlineCount"],
        "warnCount": overview["stats"]["warnCount"],
        "staleCount": overview["stats"]["staleCount"],
    }


@app.get("/api/v1/overview")
def overview() -> Dict[str, Any]:
    return build_overview()


@app.post("/api/v1/register")
def register_device(payload: RegisterRequest) -> Dict[str, Any]:
    seen_at = utc_now_iso()
    with get_connection() as connection:
        upsert_registered_device(
            connection,
            payload.deviceId,
            payload.ip,
            payload.agentName,
            payload.firmware,
            seen_at,
        )
        ensure_config(connection, payload.deviceId)
        connection.commit()

    return {
        "accepted": True,
        "deviceId": payload.deviceId,
        "serverTime": seen_at,
        "configPath": f"/api/v1/config/{payload.deviceId}",
        "dashboardPath": "/",
    }


@app.post("/api/v1/heartbeat")
def heartbeat(payload: HeartbeatRequest) -> Dict[str, Any]:
    received_at = utc_now_iso()
    with get_connection() as connection:
        touch_device(connection, payload.deviceId, payload.ip, received_at)
        ensure_config(connection, payload.deviceId)
        connection.execute(
            """
            INSERT INTO heartbeats (
                device_id, ip, heartbeat_at_ms, result_count, event_count, received_at
            ) VALUES (?, ?, ?, ?, ?, ?)
            ON CONFLICT(device_id) DO UPDATE SET
                ip = excluded.ip,
                heartbeat_at_ms = excluded.heartbeat_at_ms,
                result_count = excluded.result_count,
                event_count = excluded.event_count,
                received_at = excluded.received_at
            """,
            (
                payload.deviceId,
                payload.ip,
                payload.heartbeatAtMs,
                payload.resultCount,
                payload.eventCount,
                received_at,
            ),
        )
        connection.commit()

    return {
        "accepted": True,
        "deviceId": payload.deviceId,
        "serverTime": received_at,
    }


@app.post("/api/v1/results/batch")
def ingest_results(payload: ResultsBatchRequest) -> Dict[str, Any]:
    received_at = utc_now_iso()
    results_json = json.dumps([result.model_dump() for result in payload.results], separators=(",", ":"))
    with get_connection() as connection:
        touch_device(connection, payload.deviceId, payload.ip, received_at)
        ensure_config(connection, payload.deviceId)
        connection.execute(
            """
            INSERT INTO latest_results (
                device_id, ip, checked_at_ms, received_at, results_json
            ) VALUES (?, ?, ?, ?, ?)
            ON CONFLICT(device_id) DO UPDATE SET
                ip = excluded.ip,
                checked_at_ms = excluded.checked_at_ms,
                received_at = excluded.received_at,
                results_json = excluded.results_json
            """,
            (payload.deviceId, payload.ip, payload.checkedAtMs, received_at, results_json),
        )
        connection.execute(
            """
            INSERT INTO result_history (
                device_id, ip, checked_at_ms, received_at, results_json
            ) VALUES (?, ?, ?, ?, ?)
            """,
            (payload.deviceId, payload.ip, payload.checkedAtMs, received_at, results_json),
        )
        connection.commit()

    return {
        "accepted": True,
        "deviceId": payload.deviceId,
        "receivedCount": len(payload.results),
        "serverTime": received_at,
    }


@app.post("/api/v1/network/snapshot")
def ingest_network_snapshot(payload: NetworkSnapshotRequest) -> Dict[str, Any]:
    received_at = utc_now_iso()
    snapshot_json = json.dumps(payload.model_dump(), separators=(",", ":"))
    with get_connection() as connection:
        touch_device(connection, payload.deviceId, payload.ip, received_at)
        ensure_config(connection, payload.deviceId)
        connection.execute(
            """
            INSERT INTO latest_network_snapshots (device_id, received_at, snapshot_json)
            VALUES (?, ?, ?)
            ON CONFLICT(device_id) DO UPDATE SET
                received_at = excluded.received_at,
                snapshot_json = excluded.snapshot_json
            """,
            (payload.deviceId, received_at, snapshot_json),
        )
        connection.execute(
            """
            INSERT INTO network_snapshot_history (device_id, received_at, snapshot_json)
            VALUES (?, ?, ?)
            """,
            (payload.deviceId, received_at, snapshot_json),
        )
        connection.commit()

    return {
        "accepted": True,
        "deviceId": payload.deviceId,
        "serverTime": received_at,
    }


@app.get("/api/v1/config/{device_id}")
def get_device_config(device_id: str) -> Dict[str, Any]:
    with get_connection() as connection:
        config_snapshot = fetch_config(connection, device_id)
        connection.commit()

    return {
        "deviceId": device_id,
        "serverTime": utc_now_iso(),
        "config": config_snapshot["config"].model_dump(),
        "updatedAt": config_snapshot["updatedAt"],
    }


@app.put("/api/v1/config/{device_id}")
def put_device_config(device_id: str, config: DeviceConfig) -> Dict[str, Any]:
    with get_connection() as connection:
        touch_device(connection, device_id, "", utc_now_iso())
        updated_at = save_config(connection, device_id, config)
        connection.commit()

    return {
        "accepted": True,
        "deviceId": device_id,
        "serverTime": utc_now_iso(),
        "config": config.model_dump(),
        "updatedAt": updated_at,
    }


@app.get("/api/v1/devices")
def list_devices() -> Dict[str, Any]:
    overview_snapshot = build_overview()
    return {
        "serverTime": overview_snapshot["serverTime"],
        "stats": overview_snapshot["stats"],
        "devices": overview_snapshot["devices"],
    }


@app.get("/api/v1/device/{device_id}")
def get_device_detail(device_id: str) -> Dict[str, Any]:
    detail = build_device_detail(device_id)
    if detail is None:
        raise HTTPException(status_code=404, detail="Device not found")
    return detail


@app.get("/api/v1/results/{device_id}")
def get_latest_results(device_id: str) -> Dict[str, Any]:
    detail = build_device_detail(device_id)
    if detail is None:
        raise HTTPException(status_code=404, detail="Device not found")
    return {
        "deviceId": device_id,
        "latestResults": detail["latestResults"],
        "history": detail["history"],
    }


@app.post("/api/v1/commands/{device_id}")
def enqueue_command(device_id: str, payload: CommandEnqueueRequest) -> Dict[str, Any]:
    created_at = utc_now_iso()
    with get_connection() as connection:
        touch_device(connection, device_id, "", created_at)
        command_cursor = connection.execute(
            """
            INSERT INTO control_commands (
                device_id, action, host, path, port, start_octet, end_octet,
                status, summary, created_at, updated_at
            ) VALUES (?, ?, ?, ?, ?, ?, ?, 'queued', '', ?, ?)
            """,
            (
                device_id,
                payload.action,
                payload.host,
                payload.path,
                payload.port,
                payload.startOctet,
                payload.endOctet,
                created_at,
                created_at,
            ),
        )
        connection.commit()

    return {
        "accepted": True,
        "deviceId": device_id,
        "commandId": command_cursor.lastrowid,
        "serverTime": created_at,
    }


@app.get("/api/v1/commands/next/{device_id}")
def get_next_command(device_id: str) -> Dict[str, Any]:
    with get_connection() as connection:
        row = connection.execute(
            """
            SELECT *
            FROM control_commands
            WHERE device_id = ? AND status IN ('queued', 'issued')
            ORDER BY id ASC
            LIMIT 1
            """,
            (device_id,),
        ).fetchone()

        if row is None:
            return {
                "available": False,
                "deviceId": device_id,
                "serverTime": utc_now_iso(),
            }

        updated_at = utc_now_iso()
        if row["status"] == "queued":
            connection.execute(
                "UPDATE control_commands SET status = 'issued', updated_at = ? WHERE id = ?",
                (updated_at, row["id"]),
            )
            connection.commit()
            row = connection.execute(
                "SELECT * FROM control_commands WHERE id = ?",
                (row["id"],),
            ).fetchone()

    response = command_row_to_dict(row)
    response["available"] = True
    response["serverTime"] = utc_now_iso()
    return response


@app.post("/api/v1/commands/{command_id}/complete")
def complete_command(command_id: int, payload: CommandCompletionRequest) -> Dict[str, Any]:
    updated_at = utc_now_iso()
    with get_connection() as connection:
        connection.execute(
            """
            UPDATE control_commands
            SET status = ?, summary = ?, updated_at = ?
            WHERE id = ?
            """,
            (payload.status, payload.summary, updated_at, command_id),
        )
        connection.commit()

    return {
        "accepted": True,
        "commandId": command_id,
        "deviceId": payload.deviceId,
        "serverTime": updated_at,
    }


@app.get("/api/v1/commands/{device_id}")
def list_commands(device_id: str) -> Dict[str, Any]:
    with get_connection() as connection:
        rows = connection.execute(
            """
            SELECT *
            FROM control_commands
            WHERE device_id = ?
            ORDER BY id DESC
            LIMIT 20
            """,
            (device_id,),
        ).fetchall()

    return {
        "deviceId": device_id,
        "commands": [command_row_to_dict(row) for row in rows],
    }


if __name__ == "__main__":
    try:
        import uvicorn
    except ImportError as exc:
        raise SystemExit(
            "uvicorn is required to run app.py directly. Install requirements.txt first."
        ) from exc

    host = os.getenv("EM2_SERVER_HOST", "0.0.0.0")
    port = int(os.getenv("EM2_SERVER_PORT", "8080"))
    reload_enabled = os.getenv("EM2_SERVER_RELOAD", "0") == "1"

    uvicorn.run("app:app", host=host, port=port, reload=reload_enabled)
