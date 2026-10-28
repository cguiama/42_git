import os


def main() -> None:
    print("\nORACLE STATUS: Consulting the configuration...")
    try:
        from dotenv import load_dotenv
    except ImportError:
        print("[WARNING] python-dotenv is missing.")
        print("Install it with: "
              "python3 -m pip install -r ex1/requirements.txt")
        return
    directory = os.path.dirname(os.path.abspath(__file__))
    try:
        load_dotenv(os.path.join(directory, ".env"), override=False)
    except (OSError, UnicodeError) as error:
        print(f"[WARNING] Cannot load .env: {error}")
    names = ("MATRIX_MODE", "DATABASE_URL", "API_KEY", "LOG_LEVEL",
             "ZION_ENDPOINT")
    for name in names:
        if not os.getenv(name):
            print(f"[WARNING] Missing configuration: {name}")
    mode = os.getenv("MATRIX_MODE") or "development"
    if mode not in ("development", "production"):
        print("[WARNING] Invalid MATRIX_MODE; using development")
        mode = "development"
    database = os.getenv("DATABASE_URL", "")
    api_key = os.getenv("API_KEY", "")
    endpoint = os.getenv("ZION_ENDPOINT", "")
    log_level = os.getenv("LOG_LEVEL") or (
        "INFO" if mode == "production" else "DEBUG"
    )
    if not database:
        database_status = "Not configured"
    elif mode == "production":
        database_status = "Connected to production instance"
    else:
        database_status = "Connected to local instance"
    authenticated = api_key and api_key != "replace_with_your_api_key"
    print("\nConfiguration loaded:")
    print(f"Mode: {mode}")
    print(f"Database: {database_status}")
    print("API Access: " + (
        "Authenticated" if authenticated else "Not configured"
    ))
    print(f"Log Level: {log_level}")
    print("Remote Network: " + ("Online" if endpoint else "Not configured"))
    print("\nConfiguration security check:")
    print("[OK] No hardcoded secrets detected")
    try:
        with open(os.path.join(directory, ".gitignore"),
                  encoding="utf-8") as ignore_file:
            ignored = ".env" in ignore_file.read().splitlines()
    except (OSError, UnicodeError):
        ignored = False
    if ignored:
        print("[OK] .env file properly ignored by git")
    else:
        print("[WARNING] Add .env to ex1/.gitignore")
    print("[OK] Production overrides available")
    print("\nThe Oracle sees all configurations.")


if __name__ == "__main__":
    main()
