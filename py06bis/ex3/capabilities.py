"""Independent abilities, without a dependency on the Creature hierarchy."""

from abc import ABC, abstractmethod


class TransformCapability(ABC):
    def __init__(self) -> None:
        self.transformed: bool = False

    @abstractmethod
    def transform(self) -> str:
        """Enter the transformed state."""

    @abstractmethod
    def revert(self) -> str:
        """Return to the normal state."""
