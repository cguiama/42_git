#!/usr/bin/env python3

def input_temperature(temp_str: str) -> int | None:
    print(f"Input data is '{temp_str}'")
    try:
        temp_nbr = int(temp_str)
    except ValueError as erro:
        print(f"Caught input_temperature error: {erro}")
    else:
        print(f"Temperature is now {temp_nbr}°C")
        return temp_nbr


def test_temperature() -> None:
    input_temperature("25")
    print("")
    input_temperature("abc")


if __name__ == "__main__":
    print("=== Garden Temperature ===\n")
    test_temperature()
    print("\nAll tests completed - program didn't crash!")
