"""Interchangeable battle algorithms selected by capabilities."""

from abc import ABC, abstractmethod

from ex3.capabilities import TransformCapability
from ex3.creature import Creature


class InvalidStrategyError(Exception):
    """The selected strategy cannot act on this creature."""


class BattleStrategy(ABC):
    @abstractmethod
    def is_valid(self, creature: Creature) -> bool:
        """Check suitability without changing the creature."""

    @abstractmethod
    def act(self, creature: Creature) -> None:
        """Perform an action or raise InvalidStrategyError."""


class NormalStrategy(BattleStrategy):
    def is_valid(self, creature: Creature) -> bool:
        return isinstance(creature, Creature)

    def act(self, creature: Creature) -> None:
        if not self.is_valid(creature):
            raise InvalidStrategyError("Invalid Creature for normal strategy")
        print(creature.attack())


class AggressiveStrategy(BattleStrategy):
    def is_valid(self, creature: Creature) -> bool:
        return isinstance(creature, TransformCapability)

    def act(self, creature: Creature) -> None:
        if not isinstance(creature, TransformCapability):
            raise InvalidStrategyError(
                f"Invalid Creature '{creature.name}' "
                "for this aggressive strategy"
            )
        print(creature.transform())
        try:
            print(creature.attack())
        finally:
            print(creature.revert())
