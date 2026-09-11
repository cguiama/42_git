#!/usr/bin/env python3

class Plant:
    class Stats:
        def __init__(self):
            self._grow_count = 0
            self._age_count = 0
            self._show_count = 0

        def increment_grow(self) -> None:
            self._grow_count += 1

        def increment_age(self) -> None:
            self._age_count += 1

        def increment_show(self) -> None:
            self._show_count += 1

        def get_grow_count(self) -> int:
            return self._grow_count

        def get_age_count(self) -> int:
            return self._age_count

        def get_show_count(self) -> int:
            return self._show_count

        def display(self, name: str) -> None:
            print(f"[statistics for {name.capitalize()}]")
            print(f"Stats: {self._grow_count} grow, {self._age_count} age, "
                  f"{self._show_count} show")

    def __init__(self, name: str, height: float, age: int) -> None:
        self._name = name
        self._height = 0.0
        self._age = 0
        self._type = ""
        self._stats = self.Stats()

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
            self._age += age
            if announce:
                print(f"Age updated: {age} days")
        else:
            print(f"{self._name.capitalize()}: Error, age can't be negative"
                  "\nAge update rejected")

    def get_height(self) -> float:
        return self._height

    def get_age(self) -> int:
        return self._age

    def get_name(self) -> str:
        return self._name

    def get_stats(self) -> Stats:
        return self._stats

    def show(self) -> None:
        print(f"{self._name.capitalize()}: {round(self.get_height(), 2)}cm, "
              f"{self.get_age()} days old")
        self._stats.increment_show()

    def grow(self, growing_size: float, announce: bool = True) -> None:
        self.set_height(self._height + growing_size, announce)
        self._stats.increment_grow()

    def age(self, ages: int, announce: bool = True) -> None:
        self.set_age(ages, announce)
        self._stats.increment_age()

    def print_type(self) -> None:
        print(f"=== {self._type}")

    @staticmethod
    def is_older_than_year(age: int) -> bool:
        return age > 365

    @classmethod
    def anonymous(cls, name: str = "Unknown plant", height: float = 0.0,
                  age: int = 0):
        plant = cls(name, height, age)
        plant._type = "Anonymous"
        return plant


class Flower(Plant):
    def __init__(
            self, name: str, height: float, age: int, color: str
    ) -> None:
        super().__init__(name, height, age)
        self._color = color
        self._bloomed = False
        self._type = "Flower"

    def bloom(self) -> None:
        print(f"[asking the {self._name} to grow and bloom]")

        self._bloomed = True

    def show(self) -> None:
        super().show()
        print(f" Color: {self._color}")
        if not self._bloomed:
            print(f" {self._name.capitalize()} has not bloomed yet")
        else:
            print(f" {self._name.capitalize()} is blooming beautifully!")


class Seed(Flower):
    def __init__(self, name: str, height: float, age: int, color: str) -> None:
        super().__init__(name, height, age, color)
        self._seeds = 0
        self._bloomed = False
        self._type = "Seed"

    def bloom(self) -> None:
        print(f"[make {self._name} grow, age and bloom]")
        self._bloomed = True
        self._seeds += 42

    def show(self) -> None:
        super().show()
        print(f" Seeds: {self._seeds}")


class Tree(Plant):
    class TreeStats(Plant.Stats):
        def __init__(self):
            super().__init__()
            self._shade_count = 0

        def increment_shade_count(self) -> None:
            self._shade_count += 1

        def display(self, name: str) -> None:
            super().display(name)
            print(f" {self._shade_count} shade")

    def __init__(
            self, name: str, height: float, age: int, trunk_diameter: float
    ) -> None:
        super().__init__(name, height, age)
        self._trunk_diameter = trunk_diameter
        self._type = "Tree"
        self._stats = self.TreeStats()

    def produce_shade(self) -> None:
        self._stats.increment_shade_count()
        print(f"[asking the {self._name} to produce shade]")
        print(f"{self._type.capitalize()} {self._name.capitalize()} now"
              f" produces a shade of {self.get_height()}cm long and"
              f" {self._trunk_diameter}cm wide.")

    def get_stats(self) -> TreeStats:
        return self._stats

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

    def age(self, ages: int, announce: bool = True) -> None:
        super().age(ages, announce)
        self._nutritional_value += 0.5

    def update_nutritional_value(self, days: int, growing_size: float,
                                 announce: bool = False) -> None:
        print(f"[make {self._name} grow and age for"
              f" {days} days]")
        for _ in range(days):
            self.grow(growing_size, announce)
            self.age(1, announce)


def show_plant_statistics(plant: Plant) -> None:
    plant.get_stats().display(plant.get_name())


if __name__ == "__main__":
    print("=== Garden statistics ===")
    print("=== Check year-old")
    print(f"Is 30 days more than a year? -> "
          f"{Plant.is_older_than_year(300)}")
    print(f"Is 400 days more than a year? -> "
          f"{Plant.is_older_than_year(400)}\n")
    rose = Flower("rose", 15.0, 10, "Red")
    rose.print_type()
    rose.show()
    show_plant_statistics(rose)
    rose.grow(8, False)
    rose.bloom()
    rose.show()
    show_plant_statistics(rose)
    oak = Tree("oak", 200.0, 365, 5.0)
    print("")
    oak.print_type()
    oak.show()
    show_plant_statistics(oak)
    oak.produce_shade()
    show_plant_statistics(oak)
    print("")
    sunflower = Seed("sunflower", 80.0, 45, "yellow")
    sunflower.print_type()
    sunflower.show()
    sunflower.bloom()
    sunflower.grow(30, False)
    sunflower.age(20, False)
    sunflower.show()
    show_plant_statistics(sunflower)
    print("")
    unknow = Plant.anonymous()
    unknow.print_type()
    unknow.show()
    show_plant_statistics(unknow)
