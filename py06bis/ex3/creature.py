"""Entity interface shared by factories and battle strategies."""

from abc import ABC, abstractmethod


class Creature(ABC):
    def __init__(self, name: str, creature_type: str) -> None:
        self.name: str = name
        self.type: str = creature_type

    @abstractmethod
    def attack(self) -> str:
        """Return the entity's current attack."""

    def describe(self) -> str:
        return f"{self.name} is a {self.type} type Creature"
