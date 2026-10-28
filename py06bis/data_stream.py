"""Route heterogeneous input through the abstract processor interface."""

from typing import Any

from data_processor import (
    DataProcessor, LogProcessor, NumericProcessor, TextProcessor,
)


class DataStream:
    """Select the first registered processor that validates each element."""

    def __init__(self) -> None:
        self._processors: list[DataProcessor] = []

    def register_processor(self, proc: DataProcessor) -> None:
        if proc not in self._processors:
            self._processors.append(proc)

    def process_stream(self, stream: list[Any]) -> None:
        for element in stream:
            try:
                for processor in self._processors:
                    if processor.validate(element):
                        processor.ingest(element)
                        break
                else:
                    print("DataStream error - Can't process element in "
                          f"stream: {element}")
            except (ValueError, TypeError) as error:
                print(f"DataStream error - {error}")

    def print_processors_stats(self) -> None:
        print("== DataStream statistics ==")
        if not self._processors:
            print("No processor found, no data")
        for processor in self._processors:
            name = type(processor).__name__.replace("Processor", " Processor")
            print(f"{name}: total {processor.total_processed} items "
                  f"processed, remaining {processor.remaining} on processor")


def sample_stream() -> list[Any]:
    return [
        "Hello world", [3.14, -1, 2.71],
        [{"log_level": "WARNING",
          "log_message": "Telnet access! Use ssh instead"},
         {"log_level": "INFO", "log_message": "User wil is connected"}],
        42, ["Hi", "five"],
    ]


def main() -> None:
    print("=== Code Nexus - Data Stream ===\n")
    stream = DataStream()
    stream.print_processors_stats()
    numeric = NumericProcessor()
    text = TextProcessor()
    log = LogProcessor()
    print("\nRegistering Numeric Processor")
    stream.register_processor(numeric)
    batch = sample_stream()
    print(f"\nSend first batch of data on stream: {batch}")
    stream.process_stream(batch)
    stream.print_processors_stats()
    print("\nRegistering other data processors; send the same batch again")
    stream.register_processor(text)
    stream.register_processor(log)
    stream.process_stream(batch)
    stream.print_processors_stats()
    print("\nConsume elements: Numeric 3, Text 2, Log 1")
    for processor, count in [(numeric, 3), (text, 2), (log, 1)]:
        for _ in range(count):
            rank, value = processor.output()
            print(f" Item {rank}: {value}")
    stream.print_processors_stats()


if __name__ == "__main__":
    main()
