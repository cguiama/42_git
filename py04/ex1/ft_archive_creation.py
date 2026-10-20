import sys
from typing import IO


def main() -> None:
    if len(sys.argv) != 2:
        print("Usage: ft_archive_creation.py <file>")
        return

    filename = sys.argv[1]
    print("=== Cyber Archives Recovery & Preservation ===")
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

    transformed = ""
    for line in content.splitlines(keepends=True):
        if line.endswith("\n"):
            transformed += line[:-1] + "#\n"
        else:
            transformed += line + "#"

    print("\nTransform data:")
    print("---\n")
    print(transformed, end="")
    print("\n---")
    try:
        destination = input("Enter new file name (or empty): ")
    except (EOFError, KeyboardInterrupt):
        print("\nNot saving data.")
        return
    if not destination:
        print("Not saving data.")
        return

    print(f"Saving data to '{destination}'")
    try:
        archive = open(destination, "w", encoding="utf-8")
        try:
            archive.write(transformed)
        finally:
            archive.close()
    except (OSError, UnicodeError, ValueError) as error:
        print(f"Error opening file '{destination}': {error}")
        print("Data not saved.")
        return
    print(f"Data saved in file '{destination}'.")


if __name__ == "__main__":
    main()
