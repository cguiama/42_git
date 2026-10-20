import sys
from typing import IO


def main() -> None:
    if len(sys.argv) != 2:
        print("Usage: ft_ancient_text.py <file>")
        return

    filename = sys.argv[1]
    print("=== Cyber Archives Recovery ===")
    print(f"Accessing file '{filename}'")
    archive: IO[str]
    try:
        archive = open(filename, "r", encoding="utf-8")
        try:
            content = archive.read()
        finally:
            archive.close()
    except (OSError, UnicodeError, ValueError) as error:
        print(f"Error opening file '{filename}': {error}")
        return

    print("---\n")
    print(content, end="")
    print("\n---")
    print(f"File '{filename}' closed.")


if __name__ == "__main__":
    main()
