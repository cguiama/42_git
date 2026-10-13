#!/usr/bin/env python3

import random
from typing import Generator


def gen_event() -> Generator[tuple[str, str], None, None]:
    """Generate random player actions on demand, without an event buffer."""
    players = ["alice", "bob", "charlie", "dylan"]
    actions = ["run", "eat", "sleep", "grab", "move", "climb", "swim",
               "release", "use"]
    while True:
        yield (random.choice(players), random.choice(actions))


def consume_event(
    events: list[tuple[str, str]],
) -> Generator[tuple[str, str], None, None]:
    """Remove and yield random entries until the original list is empty."""
    while events:
        index = random.randrange(len(events))
        yield events.pop(index)


def main() -> None:
    print("=== Game Data Stream Processor ===")
    stream = gen_event()
    for index in range(1000):
        name, action = next(stream)
        print(f"Event {index}: Player {name} did action {action}")

    events: list[tuple[str, str]] = []
    for index in range(10):
        events.append(next(stream))
    print(f"Built list of 10 events: {events}")
    for event in consume_event(events):
        print(f"Got event from list: {event}")
        print(f"Remains in list: {events}")


if __name__ == "__main__":
    main()
