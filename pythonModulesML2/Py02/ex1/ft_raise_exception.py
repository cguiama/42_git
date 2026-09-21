#!/usr/bin/env python3


def input_temperature(temp_str: str) -> int:
    temperature = int(temp_str)

    if temperature < 0:
        raise ValueError(
            f"{temperature}°C is too cold for plants (min 0°C)"
        )
    if temperature > 40:
        raise ValueError(
            f"{temperature}°C is too hot for plants (max 40°C)"
        )

    return temperature


def test_temperature() -> None:
    for entrada in ("25", "abc", "100", "-50"):
        print(f"Input data is '{entrada}'")
        try:
            temperature = input_temperature(entrada)
        except ValueError as erro:
            print(f"Caught input_temperature error: {erro}")
        else:
            print(f"Temperature is now {temperature}°C")
        print("")


if __name__ == "__main__":
    print("=== Garden Temperature Checker ===\n")
    test_temperature()
    print("All tests completed - program didn't crash!")
