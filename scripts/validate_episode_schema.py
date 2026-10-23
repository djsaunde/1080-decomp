#!/usr/bin/env python3
"""Validate canonical Episode/Step JSON files used for decomp episodes."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path


def _is_number(value: object) -> bool:
    return isinstance(value, (int, float)) and not isinstance(value, bool)


def _expect_type(
    errors: list[str],
    data: object,
    path: str,
    expected: type | tuple[type, ...],
) -> None:
    if not isinstance(data, expected):
        if isinstance(expected, tuple):
            type_names = ", ".join(t.__name__ for t in expected)
        else:
            type_names = expected.__name__
        errors.append(f"{path}: expected {type_names}, got {type(data).__name__}")


def validate_episode(path: Path, *, require_match: bool) -> list[str]:
    errors: list[str] = []

    try:
        data = json.loads(path.read_text())
    except OSError as exc:
        return [f"{path}: failed to read file: {exc}"]
    except json.JSONDecodeError as exc:
        return [f"{path}: invalid JSON: {exc}"]

    required_top = {
        "function_name",
        "project",
        "model",
        "start_time",
        "end_time",
        "steps",
        "outcome",
        "final_match_percent",
        "best_match_percent",
        "total_tokens",
        "instruction_count",
        "initial_m2c_source",
        "final_source",
    }
    missing_top = sorted(required_top - data.keys())
    for key in missing_top:
        errors.append(f"{path}: missing top-level key '{key}'")
    if missing_top:
        return errors

    _expect_type(errors, data["function_name"], "function_name", str)
    _expect_type(errors, data["project"], "project", str)
    _expect_type(errors, data["model"], "model", str)
    _expect_type(errors, data["start_time"], "start_time", str)
    if data["end_time"] is not None:
        _expect_type(errors, data["end_time"], "end_time", str)
    _expect_type(errors, data["steps"], "steps", list)
    _expect_type(errors, data["outcome"], "outcome", str)
    if not _is_number(data["final_match_percent"]):
        errors.append("final_match_percent: expected int or float")
    if not _is_number(data["best_match_percent"]):
        errors.append("best_match_percent: expected int or float")
    _expect_type(errors, data["total_tokens"], "total_tokens", int)
    _expect_type(errors, data["instruction_count"], "instruction_count", int)
    if data["initial_m2c_source"] is not None:
        _expect_type(errors, data["initial_m2c_source"], "initial_m2c_source", str)
    if data["final_source"] is not None:
        _expect_type(errors, data["final_source"], "final_source", str)
    if "metadata" in data and not isinstance(data["metadata"], dict):
        errors.append("metadata: expected dict when present")

    valid_outcomes = {"match", "partial", "failed", "incomplete"}
    if data["outcome"] not in valid_outcomes:
        errors.append(
            f"outcome: expected one of {sorted(valid_outcomes)}, "
            f"got {data['outcome']!r}"
        )

    steps = data["steps"]
    for idx, step in enumerate(steps):
        label = f"steps[{idx}]"
        if not isinstance(step, dict):
            errors.append(f"{label}: expected object, got {type(step).__name__}")
            continue

        required_step = {
            "step_number",
            "timestamp",
            "assistant_text",
            "tool_calls",
            "match_percent",
            "compiled",
            "token_usage",
        }
        missing_step = sorted(required_step - step.keys())
        for key in missing_step:
            errors.append(f"{label}: missing key '{key}'")
        if missing_step:
            continue

        _expect_type(errors, step["step_number"], f"{label}.step_number", int)
        _expect_type(errors, step["timestamp"], f"{label}.timestamp", str)
        if step["assistant_text"] is not None:
            _expect_type(
                errors,
                step["assistant_text"],
                f"{label}.assistant_text",
                str,
            )
        _expect_type(errors, step["tool_calls"], f"{label}.tool_calls", list)
        if step["match_percent"] is not None and not _is_number(step["match_percent"]):
            errors.append(f"{label}.match_percent: expected int or float")
        if step["compiled"] is not None and not isinstance(step["compiled"], bool):
            errors.append(f"{label}.compiled: expected bool")
        if step["token_usage"] is not None and not isinstance(
            step["token_usage"], dict
        ):
            errors.append(f"{label}.token_usage: expected dict")

        for tc_idx, tool_call in enumerate(step["tool_calls"]):
            tc_label = f"{label}.tool_calls[{tc_idx}]"
            if not isinstance(tool_call, dict):
                errors.append(
                    f"{tc_label}: expected object, got {type(tool_call).__name__}"
                )
                continue
            required_tc = {"name", "input", "output", "duration_ms"}
            missing_tc = sorted(required_tc - tool_call.keys())
            for key in missing_tc:
                errors.append(f"{tc_label}: missing key '{key}'")
            if missing_tc:
                continue
            _expect_type(errors, tool_call["name"], f"{tc_label}.name", str)
            _expect_type(errors, tool_call["input"], f"{tc_label}.input", dict)
            _expect_type(errors, tool_call["output"], f"{tc_label}.output", str)
            _expect_type(
                errors,
                tool_call["duration_ms"],
                f"{tc_label}.duration_ms",
                int,
            )

    if require_match:
        if data["outcome"] != "match":
            errors.append("outcome: expected 'match'")
        if data["final_match_percent"] != 100.0:
            errors.append("final_match_percent: expected 100.0")
        if data["best_match_percent"] != 100.0:
            errors.append("best_match_percent: expected 100.0")
        if not steps:
            errors.append("steps: expected at least one step for exact match episode")
        else:
            last_step = steps[-1]
            if last_step.get("compiled") is not True:
                errors.append("steps[-1].compiled: expected true")
            if last_step.get("match_percent") != 100.0:
                errors.append("steps[-1].match_percent: expected 100.0")
        if not data["final_source"]:
            errors.append("final_source: expected non-empty source text")

    return errors


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate a decomp episode JSON against the canonical schema."
    )
    parser.add_argument("paths", nargs="+", type=Path, help="Episode JSON file(s)")
    parser.add_argument(
        "--require-match",
        action="store_true",
        help="Require an exact-match terminal episode",
    )
    args = parser.parse_args()

    any_errors = False
    for path in args.paths:
        errors = validate_episode(path, require_match=args.require_match)
        if errors:
            any_errors = True
            for error in errors:
                print(error, file=sys.stderr)

    return 1 if any_errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
