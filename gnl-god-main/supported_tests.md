# 🛠 Supported Tests & Logic

**GNL-GOD** organizes its tests by `BUFFER_SIZE` and project parts to ensure complete coverage of the `get_next_line` logic.

---

## 🟢 Part 1: Mandatory
Focuses on reading from a single file descriptor with various buffer sizes.

| Scenario | BUFFER_SIZE | Description |
| :--- | :--- | :--- |
| `mandatory_bs_1` | 1 | Tests the most basic unit-by-unit reading. |
| `mandatory_bs_42` | 42 | Standard buffer size test. |
| `mandatory_bs_9999` | 9999 | Large buffer size test. |
| `mandatory_bs_10M` | 10,000,000 | Extreme buffer size test for performance and overflow. |

**Files Tested:**
- `empty`: An empty file (should return NULL).
- `nl`: A file with only a newline.
- `41_no_nl`: 41 characters without a newline.
- `41_with_nl`: 41 characters followed by a newline and more data.
- `multiple_nlx5`: Multiple consecutive newlines.
- `big_line_no_nl`: A very long line without a newline.
- `stdin`: Reading from standard input.

---

## 🟡 Part 2: Bonus
Tests multiple file descriptors management and static variable efficiency.

| Scenario | BUFFER_SIZE | Description |
| :--- | :--- | :--- |
| `bonus_bs_1` | 1 | Basic unit reading with multiple FDs. |
| `bonus_bs_42` | 42 | Standard multi-FD test. |
| `bonus_bs_9999` | 9999 | Large buffer multi-FD test. |
| `bonus_bs_10M` | 10,000,000 | Extreme buffer multi-FD test. |

**Bonus Features Validated:**
- **Multiple FDs:** Switching between different file descriptors without losing the reading thread of any of them.
- **Static Variables:** Proper use of a single static variable to manage multiple FDs.
- **Memory Management:** Ensuring no leaks occur when switching or closing FDs.

---

## 🔬 Test Methodology
Every single test follows this internal flow:
1. Compiles your `get_next_line` files with a specific `-D BUFFER_SIZE=N`.
2. Links with the C++ testing framework.
3. Forks a process for each test to handle Segmentation Faults or Timeouts gracefully.
4. Validates the string returned by `get_next_line` against the expected line.
5. Monitors memory allocations in real-time to detect leaks immediately.
6. Generates a detailed report in `deepthought`.
