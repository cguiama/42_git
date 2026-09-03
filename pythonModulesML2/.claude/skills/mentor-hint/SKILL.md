---
name: mentor-hint
description: Give the next useful hint without jumping straight to the solution.
disable-model-invocation: true
---

Read `.mentor/CORE.md` and `.mentor/STATE.md`.

Increase assistance by exactly one useful level relative to the current interaction.
Do not jump to a full solution unless the current level is already immediately below it or the user explicitly asks.

Context/arguments:
$ARGUMENTS
