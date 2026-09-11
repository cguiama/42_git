#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, age: int) -> None:
        self._name = name
        self._height = height
        self._age = age

    def show(self) -> None:
        print(f"{self._name.capitalize()}: {round(self._height, 2)}cm, "
              f"{self._age} days old")

    def grow(self, growing_size: float) -> None:
        self._height += growing_size

    def age(self) -> None:
        self._age += 1


if __name__ == "__main__":
    print("=== Garden Plant Growth ===")
    rose = Plant("Rose", 25, 30)
    rose.show()
    initial_height = rose._height
    for day in range(1, 8):
        rose.grow(0.8)
        rose.age()
        print(f"=== Day {day} ===")
        rose.show()
    print(f"Growth this week: {round(rose._height - initial_height, 2)}cm")
