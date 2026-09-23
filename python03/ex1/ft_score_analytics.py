#!/usr/bin/env python3

import sys


def print_results(args: list[int]) -> None:
    low = min(args)
    high = max(args)
    total = sum(args)
    total_players = len(args)
    average_score = total/total_players

    print(f"Scores processed: {args}")
    print(f"Total players: {total_players}")
    print(f"Average score: {average_score:.1f}")
    print(f"Total score: {total}")
    print(f"High score: {high}")
    print(f"Low score: {low}")
    print(f"Score range: {high - low}")


def input_parser(args: list[str]) -> None:
    scores: list[int] = []
    for arg in args:
        try:
            value = int(arg)
        except ValueError:
            print(f"Invalid parameter: '{arg}'")
        else:
            scores += [value]

    if len(args) < 1 or len(scores) < 1:
        print("No scores provided. Usage: python3 "
              "ft_score_analytics.py <score1> <score2> ...")
    else:
        print_results(scores)


def main(args: list[str]) -> None:
    print("=== Player Score Analytics ===")
    input_parser(args)


if __name__ == "__main__":
    main(sys.argv[1:])
