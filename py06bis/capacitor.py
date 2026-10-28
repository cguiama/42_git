"""Demonstrate a capability independently of concrete entity types."""

from ex3 import TransformCreatureFactory
from ex3.capabilities import TransformCapability


def main() -> None:
    factory = TransformCreatureFactory()
    print("Testing Creature with transform capability")
    for label, creature in [("base", factory.create_base()),
                            ("evolved", factory.create_evolved())]:
        print(f" {label}:")
        print(creature.describe())
        print(creature.attack())
        if not isinstance(creature, TransformCapability):
            raise TypeError("Entity does not support transformation")
        print(creature.transform())
        try:
            print(creature.attack())
        finally:
            print(creature.revert())


if __name__ == "__main__":
    main()
