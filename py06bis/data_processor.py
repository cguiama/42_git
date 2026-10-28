"""Abstract processors with validated ingestion and FIFO output."""

from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    """Keep individual string values and their lifetime processing ranks."""

    def __init__(self) -> None:
        self._items: list[tuple[int, str]] = []
        self.total_processed: int = 0

    @property
    def remaining(self) -> int:
        return len(self._items)

    @abstractmethod
    def validate(self, data: Any) -> bool:
        """Return whether the entire input can be ingested."""

    @abstractmethod
    def ingest(self, data: Any) -> None:
        """Validate and store the entire input, or raise ValueError."""

    def _store(self, values: list[str]) -> None:
        for value in values:
            self._items.append((self.total_processed, value))
            self.total_processed += 1

    def output(self) -> tuple[int, str]:
        """Remove the oldest item; raise IndexError if the queue is empty."""
        if not self._items:
            raise IndexError("No processed data available")
        return self._items.pop(0)


class NumericProcessor(DataProcessor):
    """Accept numbers and homogeneous or mixed numeric lists."""

    def validate(self, data: Any) -> bool:
        values = data if isinstance(data, list) else [data]
        return all(type(value) in (int, float) for value in values)

    def ingest(self, data: int | float | list[int | float]) -> None:
        if not self.validate(data):
            raise ValueError("Improper numeric data")
        values = data if isinstance(data, list) else [data]
        self._store([str(value) for value in values])


class TextProcessor(DataProcessor):
    """Accept a string or a list of strings."""

    def validate(self, data: Any) -> bool:
        values = data if isinstance(data, list) else [data]
        return all(isinstance(value, str) for value in values)

    def ingest(self, data: str | list[str]) -> None:
        if not self.validate(data):
            raise ValueError("Improper text data")
        values = data if isinstance(data, list) else [data]
        self._store(list(values))


class LogProcessor(DataProcessor):
    """Accept string dictionaries, formatting standard log fields specially."""

    def validate(self, data: Any) -> bool:
        values = data if isinstance(data, list) else [data]
        return all(
            isinstance(value, dict)
            and all(isinstance(key, str) and isinstance(item, str)
                    for key, item in value.items())
            for value in values
        )

    def ingest(self, data: dict[str, str] | list[dict[str, str]]) -> None:
        if not self.validate(data):
            raise ValueError("Improper log data")
        values = data if isinstance(data, list) else [data]
        formatted: list[str] = []
        for value in values:
            if "log_level" in value and "log_message" in value:
                formatted.append(
                    f"{value['log_level']}: {value['log_message']}"
                )
            else:
                formatted.append(
                    ", ".join(f"{key}: {item}" for key, item in value.items())
                )
        self._store(formatted)


def main() -> None:
    print("=== Code Nexus - Data Processor ===")
    numeric = NumericProcessor()
    text = TextProcessor()
    log = LogProcessor()
    print("\nTesting Numeric Processor...")
    print(f" Trying to validate input '42': {numeric.validate(42)}")
    print(f" Trying to validate input 'Hello': {numeric.validate('Hello')}")
    print(" Test invalid ingestion of string 'foo' without prior validation:")
    try:
        # Deliberate mypy error required by the subject, exercise 0.
        numeric.ingest("foo")
    except ValueError as error:
        print(f" Got exception: {error}")
    print(" Processing data: [1, 2, 3, 4, 5]")
    numeric.ingest([1, 2, 3, 4, 5])
    for _ in range(3):
        rank, value = numeric.output()
        print(f" Numeric value {rank}: {value}")
    print("\nTesting Text Processor...")
    print(f" Trying to validate input 42: {text.validate(42)}")
    print(f" Trying to validate input 'Hello': {text.validate('Hello')}")
    text.ingest(["Hello", "Nexus", "World"])
    rank, value = text.output()
    print(f" Text value {rank}: {value}")
    print("\nTesting Log Processor...")
    print(f" Trying to validate input 'Hello': {log.validate('Hello')}")
    entries = [
        {"log_level": "NOTICE", "log_message": "Connection to server"},
        {"log_level": "ERROR", "log_message": "Unauthorized access!!"},
    ]
    print(f" Trying to validate logs: {log.validate(entries)}")
    log.ingest(entries)
    for _ in range(2):
        rank, value = log.output()
        print(f" Log entry {rank}: {value}")


if __name__ == "__main__":
    main()
