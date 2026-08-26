*This activity has been created as part of the 42 curriculum by guilamar, jhcosta-.*

# push_swap

## Description

`push_swap` is a 42 curriculum project that challenges us to sort data on a stack with a highly limited set of instructions, using the lowest possible number of operations. To succeed, we must manipulate various types of algorithms and choose the most appropriate solution for optimized data sorting.

In this version (v1.1), the project goes beyond simple sorting: we must implement a **disorder metric** to evaluate the initial state of the stack, and provide **four distinct sorting strategies** (`simple`, `medium`, `complex`, and `adaptive`) that can be selected at runtime via command-line flags. The adaptive strategy automatically chooses the best algorithm based on the calculated disorder to optimize performance.

---

## Project Status

This README describes the final architecture and behavior of version 1.1.

| Module | Status | Notes |
|---|---|---|
| Build and Libft | Done | Compiles with `-Wall -Wextra -Werror`. |
| Stack structure | Done | Doubly linked list, creation, insertion, size and cleanup. |
| Parsing | Done | Quoted input, integer range validation, duplicate checks and flags before numbers. |
| Stack operations | Done | Swap, push, rotate and reverse rotate are implemented. |
| `assign_index` | Done | Assigns a relative index without changing stack order. |
| Small sorts | Done | `sort_3` and `sort_5` are implemented. |
| Disorder and strategies | Done | All four strategies select and sort correctly. |
| CLI and benchmark | Done | All flags work; metrics are sent exclusively to `stderr`. |
| Main integration | Done | Coordinates parsing, selection, sorting, benchmark and cleanup. |

---

## Final Architecture & Technical Choices

The final implementation is structured around data structure management,
disorder calculation and four sorting strategies.

### 1. Data Structure — Doubly Linked List

Instead of using a standard array, the stacks are implemented as doubly linked lists (`t_stack`).

Each node contains:

- An `int value`.
- An `int index` used for radix sorting.
- A `prev` pointer.
- A `next` pointer.

This allows O(1) time complexity for both top and bottom stack operations, which is crucial for the `rra` and `rrb` instructions.

---

### 2. Project Architecture

The project is organized by responsibility. Each layer has one job, which keeps
pointer management isolated from the sorting algorithms:

```text
.
├── include/
│   └── push_swap.h
├── src/
│   ├── main.c
│   ├── stack/
│   │   ├── stack_creation_tools.c
│   │   └── stack_manipulation_tools.c
│   ├── parsing/
│   │   ├── flags.c
│   │   ├── input_normalization.c
│   │   └── input_validation.c
│   ├── operations/
│   │   ├── swap.c
│   │   ├── push.c
│   │   ├── rotate.c
│   │   └── reverse_rotate.c
│   ├── metrics/
│   │   ├── index.c
│   │   └── disorder.c
│   └── sorting/
│       ├── small_sort.c
│       ├── simple_sort.c
│       ├── strategy.c
│       ├── medium_blocks.c
│       ├── medium_restore.c
│       └── complex_sort.c
├── libft/
├── Makefile
├── README.md
└── ROADMAP.md
```

#### Layer responsibilities

| Layer | Responsibility | Functions |
|---|---|---|
| `main.c` | Coordinates the program flow only. | `main` |
| `stack/stack_creation_tools.c` | Creates, builds and frees stacks. | `ft_new_node`, `init_stack`, `init_stack_from_args`, `ft_stacksize`, `ft_freestack` |
| `stack/stack_manipulation_tools.c` | Maintains and inspects the doubly linked-list structure. | `ft_stacklast`, `ft_stackadd_back`, `ft_stackadd_front`, `ft_stackis_sorted` |
| `parsing/input_validation.c` | Validates numeric input, range and duplicates. | `ft_str_is_digit`, `ft_check_args`, `parse_int`, `validate_numbers`, `ft_check_duplicates` |
| `operations/*.c` | Performs and prints the allowed `push_swap` instructions. | `sa`/`sb`/`ss`, `pa`/`pb`, `ra`/`rb`/`rr`, `rra`/`rrb`/`rrr` |
| `metrics/*.c` | Builds relative indexes and measures disorder. | `assign_index`, `compute_disorder` |
| `sorting/*.c` | Selects and runs algorithms using only the operations layer. | `execute_strategy`, `sort_3`, `sort_5`, `simple_sort`, `medium_sort`, `complex_sort`, `adaptive_sort` |

The `operations` layer uses the stack helpers to perform the allowed
instructions. Sorting algorithms must use those operations rather than
manipulating the linked list directly.

---

### 3. Push_swap Instruction Set

The program may print only the following instructions to `stdout`. The `ft_*`
functions are internal helpers; the names below are the actual Push_swap
language consumed by the checker.

| Instruction | Meaning |
|---|---|
| `sa` | Swaps the first two elements of A. |
| `sb` | Swaps the first two elements of B. |
| `ss` | Executes `sa` and `sb` together. |
| `pa` | Moves the top element of B to the top of A. |
| `pb` | Moves the top element of A to the top of B. |
| `ra` | Moves the top of A to its bottom. |
| `rb` | Moves the top of B to its bottom. |
| `rr` | Executes `ra` and `rb` together. |
| `rra` | Moves the bottom of A to its top. |
| `rrb` | Moves the bottom of B to its top. |
| `rrr` | Executes `rra` and `rrb` together. |

An instruction that cannot change its target stack, such as `sa` on a stack
with fewer than two elements, leaves that stack unchanged.

### 4. How the Algorithms Use the Instructions

The algorithms build a program from the instruction set above. They never
change list pointers directly.

- `simple_sort` repeatedly brings the minimum value of A to the top with `ra`
  or `rra`, sends it to B with `pb`, sorts the final three elements, then uses
  `pa` to rebuild A in ascending order.
- `medium_sort` divides the indexes into blocks. Each block is sent from A to
  B with `pb` and `ra`; B is then recovered in descending-index order with
  `rb`/`rrb` and `pa`.
- `complex_sort` uses Radix LSD on `index`: a `0` bit uses `pb`, a `1` bit
  uses `ra`, and every pass ends by bringing B back with `pa`.
- `adaptive_sort` selects one of the strategies based on the disorder metric.

---

### 5. Disorder Metric — `compute_disorder`

Before any sorting occurs, the program calculates a disorder metric represented by a `double` between `0` and `1`.

The algorithm iterates through all possible pairs of numbers in stack A. If a larger number precedes a smaller one, it counts as a "mistake".

The metric is calculated as:

```text
disorder = mistakes / total_pairs
```

This value dictates which strategy the `adaptive` mode will choose.

A value closer to `0` represents a stack that is closer to being sorted, while a value closer to `1` represents a more disordered stack.

---

### 6. Indexing — `assign_index`

For the Radix Sort to handle negative numbers properly, the original values are mapped to positive indexes.

The smallest number receives index `0`, the next smallest receives index `1`, and so on.

For example:

```text
Values:  42  -10  7  100  0

Indexes:  3    0  2    4  1
```

The Radix algorithm then operates on the binary representation of these indexes instead of the original values.

---

### 7. The Four Strategies

#### Simple — O(n²)

A Selection Sort adaptation.

The algorithm:

1. Finds the smallest element in stack A.
2. Pushes it to stack B.
3. Repeats until A is empty.
4. Pushes everything back to A.

This strategy prioritizes simplicity over the number of operations.

Status: implemented in `src/sorting/simple_sort.c`.

---

#### Medium — O(n√n)

A block-based sorting strategy.

The stack is divided into blocks with a size based on `sqrt(n)`.

Each block is pushed to stack B and then organized while being returned to stack A.

This strategy provides a balance between implementation complexity and number of operations.

---

#### Complex — O(n log n)

A Radix Sort adaptation using the LSD (Least Significant Digit) approach.

The algorithm checks the bits of the indexed numbers from right to left.

For each bit:

- If the bit is `0`, the element is pushed to B using `pb`.
- If the bit is `1`, the element is rotated in A using `ra`.
- After processing the current bit, everything in B is pushed back to A using `pa`.

Because the algorithm works with the indexes generated by `assign_index`, negative input values are handled correctly.

Status: implemented in `src/sorting/complex_sort.c` using LSD Radix Sort.

---

#### Adaptive

`adaptive_sort` calculates the regime before any move and chooses the strategy
that matches it. This is also the default strategy when no selector is given.

The current thresholds are:

```text
disorder < 0.2  → Simple
disorder < 0.5  → Medium
disorder >= 0.5 → Complex
```

This allows the program to adapt its behavior to the initial state of the input rather than always using the same algorithm.

---

## Edge Cases for the Final Version

The final program must handle the following cases:

- Non-integer arguments.
- Values exceeding `INT_MAX`.
- Values below `INT_MIN`.
- Duplicate values.
- Empty input.
- Inputs with `argc < 2`.
- Already sorted stacks.
- Negative numbers.
- Inputs containing only 2 or 3 elements.

For invalid input, the final program prints:

```text
Error
```

to `stderr`.

Empty input or `argc < 2` exits silently.

For inputs of size 2 or 3, short dedicated sequences minimize the number of
operations. Inputs up to five values use `sort_5` when appropriate.

---

# Instructions

To compile the program, `gcc` (or `cc`) and `make` must be installed.

Compilation follows the strict 42 rules with:

```text
-Wall -Wextra -Werror
```

## 1. Compilation

Clone the repository and run:

```bash
make
```

at the project root.

### Available Make Rules

| Command | Description |
|---|---|
| `make` | Compiles `libft` and then the `push_swap` executable. |
| `make clean` | Removes all object (`.o`) files. |
| `make fclean` | Removes object files and the `push_swap` binary. |
| `make re` | Recompiles the entire project from scratch. |

---

## 2. Usage

Run the program with a list of integers as arguments.

The input may be split into normal arguments, one quoted string, or a mixture
of both:

```bash
./push_swap 4 67 3
./push_swap "4 67 3"
./push_swap "4 67" 3
```

Optional flags can be passed before the numbers.

The default strategy is `adaptive`.

### Flags

| Flag | Description |
|---|---|
| `--simple` | Forces the O(n²) algorithm. |
| `--medium` | Forces the O(n√n) algorithm. |
| `--complex` | Forces the O(n log n) algorithm. |
| `--adaptive` | Chooses a strategy from the measured disorder. This is the default. |
| `--bench` | Sends disorder, chosen strategy and operation counters to `stderr`. |

### Examples

Default behavior using the adaptive strategy:

```bash
./push_swap 4 67 3 87 23
```

Force the complex strategy and count operations:

```bash
./push_swap --complex 4 67 3 87 23 | wc -l
```

Run benchmark mode:

```bash
./push_swap --bench --adaptive 4 67 3 87 23
```

Test invalid input:

```bash
./push_swap --bench 0 one 2 3
```

---

## 3. Validation with the Provided Checkers

The provided checkers validate the operation stream generated by `push_swap`.
They receive the original numbers as arguments and read the operations from
standard input. They print `OK` when stack A ends sorted in ascending order and
stack B is empty; otherwise, they print `KO`.

Choose the checker for the operating system where you are testing:

| File | Platform |
|---|---|
| `checker_linux` | GNU/Linux x86_64 |
| `fedora_checker` | Fedora Linux x86_64 |
| `checker_Mac` | macOS Intel x86_64 |

Only one checker is needed for a test. On Linux, use `checker_linux` unless
you are specifically testing on Fedora. The macOS checker cannot run on Linux;
on Apple Silicon Macs it may require Rosetta.

Set the path to the suitable checker and pipe the operations into it:

```bash
ARG="4 67 3 87 23"
CHECKER=/path/to/checker_linux
./push_swap $ARG | $CHECKER $ARG
```

The expected result is:

```text
OK
```

To count operations while validating a strategy, run:

```bash
ARG="4 67 3 87 23"
./push_swap --complex $ARG | tee /tmp/push_swap_operations | $CHECKER $ARG
wc -l /tmp/push_swap_operations
```

The checker verifies the final stack state; it does not replace tests for
parsing, memory leaks or operation counts. Benchmark metrics already go to
`stderr`, so they do not interfere with the operation stream consumed by the
checker.

---

# Resources

- **`man 3 atoi`** — Original function specification used as a reference during parsing.
- **`man 3 write`** — Used as a reference for outputting instructions and errors to `stdout` and `stderr`.
- [**Radix Sort Explanation**](https://en.wikipedia.org/wiki/Radix_sort) — Reference for the bitwise sorting strategy.

---

## Contributions

This project was developed by exactly two learners, who collaborated on the
integration and understand the complete codebase.

| Learner | Main responsibilities |
|---|---|
| `guilamar` | Stack construction, input normalization and validation, swap/push operations, indexing, `simple_sort`, `complex_sort`, block distribution for `medium_sort`, benchmark integration and documentation. |
| `jhcosta-` | Rotate/reverse-rotate operations, disorder metric, `sort_5`, flag parsing, restoration of B in `medium_sort`, and algorithm integration/review. |

Both learners contributed to final testing, memory verification, strategy
integration and the README.

---

## AI Usage

AI assisted the project as a tutor and code-review companion by:

- Explaining data structures, stack operations and algorithmic trade-offs.
- Reviewing pointer handling, memory management, error handling and Norminette.
- Assisting with the layered project structure, benchmark design and README.
- Helping validate the program with checker binaries and Valgrind.

All final implementation choices were reviewed and understood by both learners.
