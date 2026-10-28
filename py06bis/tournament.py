"""Round-robin battles driven only by factories and abstract strategies."""

from ex3 import CreatureFactory, FlameFactory, TransformCreatureFactory
from ex4 import (
    AggressiveStrategy, BattleStrategy, InvalidStrategyError, NormalStrategy,
)


def battle(opponents: list[tuple[CreatureFactory, BattleStrategy]]) -> None:
    """Make every unordered pair fight once; abort on an invalid strategy."""
    print("*** Tournament ***")
    print(f"{len(opponents)} opponents involved")
    for index, (factory, strategy) in enumerate(opponents):
        for other_factory, other_strategy in opponents[index + 1:]:
            left = factory.create_base()
            right = other_factory.create_base()
            print("\n* Battle *")
            print(left.describe())
            print(" vs.")
            print(right.describe())
            print(" now fight!")
            try:
                strategy.act(left)
                other_strategy.act(right)
            except InvalidStrategyError as error:
                print(f"Battle error, aborting tournament: {error}")
                return


def main() -> None:
    flame = FlameFactory()
    transform = TransformCreatureFactory()
    normal = NormalStrategy()
    aggressive = AggressiveStrategy()
    print("Tournament 0 (basic)")
    print(" [ (Flameling+Normal), (Shiftling+Aggressive) ]")
    battle([(flame, normal), (transform, aggressive)])
    print("\nTournament 1 (error)")
    print(" [ (Shiftling+Aggressive), (Flameling+Aggressive) ]")
    battle([(transform, aggressive), (flame, aggressive)])


if __name__ == "__main__":
    main()
