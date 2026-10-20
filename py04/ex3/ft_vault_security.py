"""Access archives safely using context managers."""


def secure_archive(
    filename: str, action: str = "r", content: str = ""
) -> tuple[bool, str]:
    if action != "r" and action != "w":
        return (False, "Invalid action: use 'r' to read or 'w' to write")
    try:
        with open(filename, action, encoding="utf-8") as archive:
            if action == "r":
                return (True, archive.read())
            archive.write(content)
        return (True, "Content successfully written to file")
    except (OSError, UnicodeError, ValueError) as error:
        return (False, str(error))


def main() -> None:
    print("=== Cyber Archives Security ===")
    print("\nUsing 'secure_archive' to read from a nonexistent file:")
    print(secure_archive("/not/existing/file"))
    print("\nUsing 'secure_archive' to read from an inaccessible file:")
    print(secure_archive("/etc/master.passwd"))
    print("\nUsing 'secure_archive' to read from a regular file:")
    success, content = secure_archive("ancient_fragment.txt")
    print((success, content))
    if success:
        print("\nUsing 'secure_archive' to write previous content "
              "to a new file:")
        print(secure_archive("new_fragment.txt", "w", content))


if __name__ == "__main__":
    main()
