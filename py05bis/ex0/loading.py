import importlib


def main() -> None:
    print("\nREAGENT STATUS: Loading reagents...")
    print("\nChecking dependencies:")
    missing = False
    for name, description in (
        ("pandas", "Data manipulation ready"),
        ("numpy", "Numerical computation ready"),
    ):
        try:
            reagent = importlib.import_module(name)
            print(f"[OK] {name} ({reagent.__version__}) - {description}")
        except ImportError:
            missing = True
            print(f"[MISSING] {name} - Reagent unavailable")
    print("\npip vs Poetry:")
    print("  requirements.txt declares what to install; "
          "pip resolves it at install time.")
    print("  pyproject.toml declares the same constraints, "
          "and Poetry pins the resolved")
    print("  versions in poetry.lock so every install is identical.")
    if missing:
        print("\nInstall missing reagents with pip:")
        print("python3 -m pip install -r ex0/requirements.txt")
        print("\nOr with Poetry:")
        print("poetry -C ex0 install")
        print("poetry -C ex0 run python loading.py")
    else:
        print("\nAll reagents present and accounted for.")


if __name__ == "__main__":
    main()
