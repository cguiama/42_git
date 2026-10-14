#!/usr/bin/env python3

import random


def main() -> None:
    """Transform player names and filter scores using comprehensions."""
    print("=== Game Data Alchemist ===")
    players = ["Alice", "bob", "Charlie", "dylan", "Emma", "Gregory",
               "john", "kevin", "Liam"]
    print(f"Initial list of players: {players}")

    capitalized = [name.capitalize() for name in players]
    initially_capitalized = [name for name in players if name[0].isupper()]
    print(f"New list with all names capitalized: {capitalized}")
    print(f"New list of capitalized names only: {initially_capitalized}")

    scores = {name: random.randint(0, 1000) for name in capitalized}
    average = sum(scores.values()) / len(scores)
    high_scores = {
        name: score for name, score in scores.items() if score > average
    }
    print(f"\nScore dict: {scores}")
    print(f"Score average is {round(average, 2)}")
    print(f"High scores: {high_scores}")


if __name__ == "__main__":
    main()
