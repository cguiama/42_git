#!/usr/bin/env python3

import random


BASE_ACHIEVEMENTS = ["Crafting Genius", "Strategist", "World Savior",
                     "Speed Runner", "Survivor", "Master Explorer",
                     "Treasure Hunter", "Unstoppable", "Hidden Path Finder",
                     "First Steps", "Collector Supreme", "Untouchable",
                     "Sharp Mind", "Boss Slayer"]


class Player():
    def __init__(self, name: str):
        self._name = name
        self._achievements: set[str] = set()

    def set_achievements(self, achievements: set[str]) -> None:
        self._achievements = achievements

    def get_achievements(self) -> set[str]:
        return self._achievements

    def get_name(self) -> str:
        return self._name


def gen_player_achievements() -> set[str]:
    qnt = random.randint(5, 9)
    achievements = random.sample(BASE_ACHIEVEMENTS, k=qnt)
    return set(achievements)


def main() -> None:
    print("=== Achievement Tracker System ===\n")
    all_distinct: set[str] = set()
    players = [
        Player("Alice"),
        Player("Bob"),
        Player("Charlie"),
        Player("Dylan")
    ]

    for player in players:
        player.set_achievements(gen_player_achievements())
        print(f"Player {player.get_name()}: {player.get_achievements()}")
        all_distinct = all_distinct.union(player.get_achievements())

    common: set[str] = players[0].get_achievements()

    for player in players[1:]:
        common = common.intersection(player.get_achievements())
    print(f"\nAll distinct achievements: {all_distinct}\n")
    print(f"Common achievements: {common}\n")
    for player in players:
        unique = player.get_achievements()
        for other in players:
            if player is not other:
                unique = unique.difference(other.get_achievements())
        print(f"Only {player.get_name()} has: {unique}")
    print("")
    for player in players:
        all_achievements = set(BASE_ACHIEVEMENTS)
        missing = all_achievements.difference(player.get_achievements())
        print(f"{player.get_name()} is missing: {missing}")


if __name__ == "__main__":
    main()
