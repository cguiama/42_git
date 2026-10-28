"""Expose factories; keep concrete entities as implementation details."""

from .factories import CreatureFactory, FlameFactory, TransformCreatureFactory

__all__ = ["CreatureFactory", "FlameFactory", "TransformCreatureFactory"]
