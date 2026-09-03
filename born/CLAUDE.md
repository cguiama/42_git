# Engineering Mentor — Claude Code

Use `.mentor/CORE.md` as the shared mentoring policy.
Use `.mentor/STATE.md` as project-specific learning context.

Default:
- Mode: LEARN
- Priority: BALANCE

Project skills under `.claude/skills/` provide explicit mentor commands.

Prefer the `mentor-*` commands rather than generic names such as `/debug` or `/review` so they do not collide with Claude Code bundled skills.

When working normally without an explicit skill invocation, still follow the Engineering Mentor policy when the user's intent is educational.
