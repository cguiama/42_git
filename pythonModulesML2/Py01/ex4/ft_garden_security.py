#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, age: int) -> None:
        self._name = name
        self._height = 0.0
        self._age = 0

        self.set_height(height, False)
        self.set_age(age, False)

    def set_height(self, height: float, announce: bool = True) -> None:
        if height >= 0:
            self._height = height
            if announce:
                print(f"Height updated: {height}cm")
        else:
            print(f"{self._name.capitalize()}: Error, height can't be negative"
                  "\nHeight update rejected")

    def set_age(self, age: int, announce: bool = True) -> None:
        if age >= 0:
            self._age = age
            if announce:
                print(f"Age updated: {age} days")
        else:
            print(f"{self._name.capitalize()}: Error, age can't be negative"
                  "\nAge update rejected")

    def get_height(self) -> float:
        return self._height

    def get_age(self) -> int:
        return self._age

    def show(self) -> None:
        print(f"{self._name}: {round(self.get_height(), 2)}cm, "
              f"{self.get_age()} days old")

    def grow(self, growing_size: float) -> None:
        self.set_height(self._height + growing_size)

    def age(self) -> None:
        self.set_age(self._age + 1)


if __name__ == "__main__":
    print("=== Garden Security System ===")
    rose = Plant("Rose", 15, 10)
    print("Plant created: ", end="")
    rose.show()
    print("")
    rose.set_height(25)
    rose.set_age(30)
    print("")
    rose.set_height(-5)
    rose.set_age(-5)
    print("\nCurrent state: ", end="")
    rose.show()
