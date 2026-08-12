#!/usr/bin/env python3
"""
Networking handler for UnoQBridgeService.

Supported ops (called as "networking.<op>" from the bridge):
  ping, dns_resolve, tcp_check, http_get, http_post

All network ops use the Linux side's network stack — no Arduino shields needed.
"""

from __future__ import annotations

import socket
import ssl
import urllib.error
import urllib.request
from typing import Any, Dict, Optional


# ---------------------------------------------------------------------------
# Backend
# ---------------------------------------------------------------------------

class NetworkingBackend:
    def ping(self) -> Dict[str, Any]:
        return {"service": "networking", "status": "ok"}

    def net_ping(self, host: str, timeout: float = 3.0) -> Dict[str, Any]:
        """TCP reachability check — ICMP is blocked in Docker without NET_ADMIN."""
        for port in (53, 80, 443):
            try:
                with socket.create_connection((host, port), timeout=timeout):
                    pass
                return {"host": host, "reachable": True, "via_port": port}
            except OSError:
                continue
        return {"host": host, "reachable": False}

    def dns_resolve(self, host: str) -> Dict[str, Any]:
        try:
            address = socket.gethostbyname(host)
            return {"host": host, "address": address, "resolved": True}
        except socket.gaierror as exc:
            return {"host": host, "resolved": False, "reason": str(exc)}

    def tcp_check(self, host: str, port: int, timeout: float = 3.0) -> Dict[str, Any]:
        try:
            with socket.create_connection((host, port), timeout=timeout):
                pass
            return {"host": host, "port": port, "open": True}
        except OSError as exc:
            return {"host": host, "port": port, "open": False, "reason": str(exc)}

    def http_get(
        self,
        url: str,
        timeout: float = 10.0,
        max_bytes: int = 2048,
    ) -> Dict[str, Any]:
        req = urllib.request.Request(url, headers={"User-Agent": "UnoQBridge/1.0"})
        is_https = url.lower().startswith("https://")

        if not is_https:
            # Plain HTTP — passing an SSL context raises ValueError in urllib
            try:
                with urllib.request.urlopen(req, timeout=timeout) as resp:
                    status = resp.status
                    body = resp.read(max_bytes).decode("utf-8", errors="replace")
                return {"url": url, "status": status, "body": body,
                        "truncated": len(body) == max_bytes}
            except urllib.error.HTTPError as exc:
                return {"url": url, "status": exc.code, "body": "", "error": str(exc)}
            except Exception as exc:
                return {"url": url, "status": 0, "body": "", "error": str(exc)}

        # HTTPS — try verified first, fall back to unverified for corp/self-signed CAs
        for ctx in (ssl.create_default_context(), ssl._create_unverified_context()):
            try:
                with urllib.request.urlopen(req, timeout=timeout, context=ctx) as resp:
                    status = resp.status
                    body = resp.read(max_bytes).decode("utf-8", errors="replace")
                return {"url": url, "status": status, "body": body,
                        "truncated": len(body) == max_bytes}
            except urllib.error.HTTPError as exc:
                return {"url": url, "status": exc.code, "body": "", "error": str(exc)}
            except ssl.SSLError:
                continue
            except Exception as exc:
                return {"url": url, "status": 0, "body": "", "error": str(exc)}
        return {"url": url, "status": 0, "body": "", "error": "ssl_failed"}

    def http_post(
        self,
        url: str,
        body: str = "",
        content_type: str = "application/json",
        timeout: float = 10.0,
        max_bytes: int = 2048,
    ) -> Dict[str, Any]:
        try:
            data = body.encode("utf-8")
            req = urllib.request.Request(
                url,
                data=data,
                headers={"Content-Type": content_type, "User-Agent": "UnoQBridge/1.0"},
                method="POST",
            )
            with urllib.request.urlopen(req, timeout=timeout) as resp:
                status = resp.status
                response_body = resp.read(max_bytes).decode("utf-8", errors="replace")
            return {"url": url, "status": status, "body": response_body, "truncated": len(response_body) == max_bytes}
        except urllib.error.HTTPError as exc:
            return {"url": url, "status": exc.code, "body": "", "error": str(exc)}
        except Exception as exc:
            return {"url": url, "status": 0, "body": "", "error": str(exc)}


# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------

def _err(request_id: Any, code: str, message: str) -> Dict[str, Any]:
    return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok(request_id: Any, result: Any) -> Dict[str, Any]:
    return {"id": request_id, "ok": True, "result": result}


def dispatch(backend: NetworkingBackend, request: Dict[str, Any]) -> Dict[str, Any]:
    request_id = request.get("id")
    op = request.get("op")
    if not isinstance(op, str) or not op:
        return _err(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

    try:
        if op == "ping":
            result = backend.ping()
        elif op == "net_ping":
            result = backend.net_ping(
                host=str(request["host"]),
                timeout=float(request.get("timeout", 3.0)),
            )
        elif op == "dns_resolve":
            result = backend.dns_resolve(host=str(request["host"]))
        elif op == "tcp_check":
            result = backend.tcp_check(
                host=str(request["host"]),
                port=int(request["port"]),
                timeout=float(request.get("timeout", 3.0)),
            )
        elif op == "http_get":
            result = backend.http_get(
                url=str(request["url"]),
                timeout=float(request.get("timeout", 10.0)),
                max_bytes=int(request.get("max_bytes", 2048)),
            )
        elif op == "http_post":
            result = backend.http_post(
                url=str(request["url"]),
                body=str(request.get("body", "")),
                content_type=str(request.get("content_type", "application/json")),
                timeout=float(request.get("timeout", 10.0)),
                max_bytes=int(request.get("max_bytes", 2048)),
            )
        else:
            return _err(request_id, "UNKNOWN_OP", f"Unsupported networking op: {op}")

        return _ok(request_id, result)

    except KeyError as exc:
        return _err(request_id, "MISSING_ARG", f"Missing required field: {exc}")
    except Exception as exc:  # pragma: no cover
        return _err(request_id, "INTERNAL", str(exc))
