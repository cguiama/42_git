"""Demonstrate interchangeable creature factories."""

from ex3 import CreatureFactory, FlameFactory, TransformCreatureFactory


def test_factory(factory: CreatureFactory) -> None:
    print("Testing factory")
    for creature in [factory.create_base(), factory.create_evolved()]:
        print(creature.describe())
        print(creature.attack())
    print()


def battle(first: CreatureFactory, second: CreatureFactory) -> None:
    print("Testing battle")
    left = first.create_base()
    right = second.create_base()
    print(left.describe())
    print(" vs.")
    print(right.describe())
    print(" fight!")
    print(left.attack())
    print(right.attack())


def main() -> None:
    flame = FlameFactory()
    transform = TransformCreatureFactory()
    test_factory(flame)
    test_factory(transform)
    battle(flame, transform)


if __name__ == "__main__":
    main()
