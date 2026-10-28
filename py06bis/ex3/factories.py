"""Create related entities without exposing concrete classes to callers."""

from abc import ABC, abstractmethod

from . import entities as _entities
from .creature import Creature


class CreatureFactory(ABC):
    @abstractmethod
    def create_base(self) -> Creature:
        """Create a fresh base entity from this family."""

    @abstractmethod
    def create_evolved(self) -> Creature:
        """Create a fresh evolved entity from this family."""


class FlameFactory(CreatureFactory):
    def create_base(self) -> Creature:
        return _entities.Flameling()

    def create_evolved(self) -> Creature:
        return _entities.Pyrodon()


class TransformCreatureFactory(CreatureFactory):
    def create_base(self) -> Creature:
        return _entities.Shiftling()

    def create_evolved(self) -> Creature:
        return _entities.Morphagon()
