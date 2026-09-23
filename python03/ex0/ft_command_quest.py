#!/usr/bin/env python3
import sys


def no_arguments() -> None:
    print("No arguments provided!", file=sys.stderr)
    print(f"Total arguments: {len(sys.argv)}", file=sys.stderr)
    sys.exit(1)


def with_arguments() -> None:
    print(f"Arguments received: {len(sys.argv) - 1}")
    i = 1
    for arg in sys.argv[1:]:
        print(f"Argument {i}: {arg}")
        i += 1
    print(f"Total arguments: {len(sys.argv)}")


def main() -> None:
    path = sys.argv[0]
    program_name = path.split('/')[-1]

    print("=== Command Quest ===")
    print(f"Program name: {program_name}")
    if len(sys.argv) == 1:
        no_arguments()
    else:
        with_arguments()


if __name__ == "__main__":
    main()
