"""Export processor output through structurally compatible plugins."""

from typing import Any, Protocol

from data_processor import LogProcessor, NumericProcessor, TextProcessor
from data_stream import DataStream as InputDataStream, sample_stream


class ExportPlugin(Protocol):
    def process_output(self, data: list[tuple[int, str]]) -> None:
        """Export ranked strings."""
        ...


class DataStream(InputDataStream):
    def output_pipeline(self, nb: int, plugin: ExportPlugin) -> None:
        """Export up to nb items per processor; restore a failed batch."""
        if isinstance(nb, bool) or not isinstance(nb, int) or nb < 0:
            raise ValueError("Output count must be a non-negative integer")
        if nb == 0:
            return
        for processor in self._processors:
            data = [processor.output()
                    for _ in range(min(nb, processor.remaining))]
            if not data:
                continue
            try:
                plugin.process_output(data.copy())
            except Exception:
                # Restore consumed values with their original processing ranks.
                processor._items[0:0] = data
                raise


class CSVExportPlugin:
    """A plugin needs the right method, without inheriting ExportPlugin."""

    def process_output(self, data: list[tuple[int, str]]) -> None:
        fields: list[str] = []
        for _, value in data:
            if any(char in value for char in ',"\r\n'):
                value = '"' + value.replace('"', '""') + '"'
            fields.append(value)
        print("CSV Output:")
        print(",".join(fields))


def json_string(value: str) -> str:
    """Escape a JSON string without importing a serialization library."""
    escaped: list[str] = []
    for char in value:
        if char == '"':
            escaped.append('\\"')
        elif char == "\\":
            escaped.append("\\\\")
        elif ord(char) < 32 or 0xD800 <= ord(char) <= 0xDFFF:
            escaped.append(f"\\u{ord(char):04x}")
        else:
            escaped.append(char)
    return '"' + "".join(escaped) + '"'


class JSONExportPlugin:
    def process_output(self, data: list[tuple[int, str]]) -> None:
        fields = [f'{json_string(f"item_{rank}")}: {json_string(value)}'
                  for rank, value in data]
        print("JSON Output:")
        print("{" + ", ".join(fields) + "}")


def main() -> None:
    print("=== Code Nexus - Data Pipeline ===\n")
    stream = DataStream()
    stream.print_processors_stats()
    for processor in [NumericProcessor(), TextProcessor(), LogProcessor()]:
        stream.register_processor(processor)
    print("\nSend first batch of data")
    stream.process_stream(sample_stream())
    stream.print_processors_stats()
    print("\nSend 3 processed data from each processor to a CSV plugin:")
    stream.output_pipeline(3, CSVExportPlugin())
    stream.print_processors_stats()
    batch: list[Any] = [
        21, ["I love AI", "LLMs are wonderful", "Stay healthy"],
        [{"log_level": "ERROR", "log_message": "500 server crash"},
         {"log_level": "NOTICE",
          "log_message": "Certificate expires in 10 days"}],
        [32, 42, 64, 84, 128, 168], "World hello",
    ]
    print(f"\nSend another batch of data: {batch}")
    stream.process_stream(batch)
    stream.print_processors_stats()
    print("\nSend 5 processed data from each processor to a JSON plugin:")
    stream.output_pipeline(5, JSONExportPlugin())
    stream.print_processors_stats()


if __name__ == "__main__":
    main()
