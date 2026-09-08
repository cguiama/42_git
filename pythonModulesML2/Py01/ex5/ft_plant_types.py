#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, age: int) -> None:
        self._name = name
        self._height = 0.0
        self._age = 0
        self._type = ""

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

    def grow(self, growing_size: float, announce: bool = True) -> None:
        self.set_height(self._height + growing_size, announce)

    def age(self, announce: bool = True) -> None:
        self.set_age(self._age + 1, announce)

    def print_type(self) -> None:
        print(f"=== {self._type}")


class Flower(Plant):
    def __init__(
            self, name: str, height: float, age: int, color: str
    ) -> None:
        super().__init__(name, height, age)
        self._color = color
        self._bloomed = False
        self._type = "Flower"

    def bloom(self) -> None:
        print(f"[asking the {self._name} to bloom]")
        self._bloomed = True

    def show(self) -> None:
        super().show()
        print(f" Color: {self._color}")
        if not self._bloomed:
            print(f" {self._name} has not bloomed yet")
        else:
            print(f" {self._name} is blooming beautifully!\n")


class Tree(Plant):
    def __init__(
            self, name: str, height: float, age: int, trunk_diameter: float
    ) -> None:
        super().__init__(name, height, age)
        self._trunk_diameter = trunk_diameter
        self._type = "Tree"

    def produce_shade(self) -> None:
        print(f"[asking the {self._name} to produce shade]")
        print(f"{self._type.capitalize()} {self._name.capitalize()} now"
              f" produces a shade of {self.get_height()}cm long and"
              f" {self._trunk_diameter}cm wide.\n")

    def show(self) -> None:
        super().show()
        print(f" Trunk diameter: {self._trunk_diameter}cm")


class Vegetable(Plant):
    def __init__(self, name: str, height: float, age: int,
                 harvest_season: str, nutritional_value: float = 0) -> None:
        super().__init__(name, height, age)
        self._harvest_season = harvest_season
        self._nutritional_value = nutritional_value
        self._type = "Vegetable"

    def show(self) -> None:
        super().show()
        print(f" Harvest season: {self._harvest_season}")
        print(f" Nutritional value: {self._nutritional_value:.0f}")

    def grow(self, growing_size: float, announce: bool = True) -> None:
        super().grow(growing_size, announce)
        self._nutritional_value += 0.5

    def age(self, announce: bool = True) -> None:
        super().age(announce)
        self._nutritional_value += 0.5

    def update_nutritional_value(self, days: int, growing_size: float,
                                 announce: bool = False) -> None:
        print(f"[make {self._name} grow and age for"
              f" {days} days]")
        for day in range(days):
            self.grow(growing_size, announce)
            self.age(announce)


if __name__ == "__main__":
    print("=== Garden Plant Types ===")
    rose = Flower("Rose", 15.0, 10, "red")
    rose.print_type()
    rose.show()
    rose.bloom()
    rose.show()
    oak = Tree("Oak", 200.0, 365, 5.0)
    oak.print_type()
    oak.show()
    oak.produce_shade()
    tomato = Vegetable("Tomato", 5.0, 10, "April")
    tomato.print_type()
    tomato.show()
    tomato.update_nutritional_value(20, 2.1)
    tomato.show()
