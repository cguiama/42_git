*This project has been created as part of the 42 curriculum by guilamar.*

# Get Next Line

## Description
Get Next Line is a 42 curriculum project that challenges us to implement a function capable of reading a file descriptor line by line. The primary goal is to understand **static variables** in C — a mechanism that allows a function to retain state between successive calls without relying on global variables. Rather than reading the entire file at once, `get_next_line` reads in fixed-size chunks (`BUFFER_SIZE`) and accumulates content across calls, returning exactly one line per invocation.

## Algorithm & Technical Choices

The implementation is structured around one central idea: **a static pointer that survives between function calls**.

### 1. The Static Variable (`over`)
A `static char *over` is declared inside `get_next_line`. Unlike a local variable, it is not destroyed when the function returns — it persists in the data segment of the process for the entire program lifetime. This allows the function to "remember" bytes read beyond the last `\n` and reuse them in the next call, without re-reading the file.

### 2. Read and Accumulate (`read_and_accumulate`)
Each call reads up to `BUFFER_SIZE` bytes from the file descriptor using `read()`. The result is concatenated onto `over` via `ft_strjoin`. This loop repeats until either a `\n` is found in `over` or `read()` returns 0 (EOF). The key insight: we never read more than necessary, and we never discard unprocessed bytes.

### 3. Extract the Line (`extract_line`)
Once a `\n` is found (or EOF is reached), `ft_substr` extracts everything from the start of `over` up to and including the `\n`. This is the string returned to the caller.

### 4. Update the Remainder (`update_over`)
After extraction, everything after the `\n` is saved back into `over` for the next call. If nothing remains, `over` is set to `NULL`.

**Edge cases handled:**
- `fd < 0` or `BUFFER_SIZE <= 0` → returns `NULL` immediately
- Empty file or EOF with no trailing `\n` → returns last content without `\n`, then `NULL`
- `read()` error → frees all memory and returns `NULL`
- Works with any `BUFFER_SIZE` from 1 to 10000000

## Function Inventory

### Core (`get_next_line.c`)
* `get_next_line` — Main entry point. Manages the static variable and orchestrates the three helper functions.
* `read_and_accumulate` *(static)* — Reads from fd in chunks and concatenates onto the accumulated buffer until `\n` or EOF.
* `extract_line` *(static)* — Extracts everything up to and including `\n` from the buffer.
* `update_over` *(static)* — Saves the remainder after `\n` for the next call.

### Utilities (`get_next_line_utils.c`)
* `ft_strlen` — Returns the length of a string.
* `ft_strchr` — Searches for a character in a string and returns its pointer.
* `ft_strjoin` — Concatenates two strings into a newly allocated string.
* `ft_substr` — Returns a newly allocated substring from a given start and length.

## Instructions

To compile, `gcc` (or `cc`) and `make` must be installed. The `BUFFER_SIZE` macro can be defined at compile time with `-D BUFFER_SIZE=n`.

### 1. Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```

To test with edge-case buffer sizes:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main_gnl_test.c -o gnl_test
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main_gnl_test.c -o gnl_test
```

### 2. Usage

Include the header in your `.c` files:

```c
#include "get_next_line.h"
```

Use it in a loop to read line by line:

```c
int     fd;
char    *line;

fd = open("file.txt", O_RDONLY);
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```

**Important:** always `free()` the returned line after use. The caller is responsible for managing that memory.

### 3. Files to submit

```
get_next_line.c
get_next_line_utils.c
get_next_line.h
```

## Resources
* `man 2 read` — System call used to read from a file descriptor.
* `man 3 malloc` / `man 3 free` — Memory management.
* [Static variables in C — GeeksForGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
* **AI Usage:** AI was used as a Socratic tutor during the conceptual phases of this project — explaining static variables, the memory model behind them, and the difference between stack and data segment. The utility functions (`ft_strlen`, `ft_strchr`, `ft_strjoin`, `ft_substr`) were adapted from the student's own libft. The main logic of `get_next_line` was developed with AI guidance on structure and edge cases, with the student reviewing, correcting, and understanding each decision before submission.
