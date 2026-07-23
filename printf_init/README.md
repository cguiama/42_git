*This project has been created as part of the 42 curriculum by guilamar.*

# ft_printf

## Description
ft_printf is a 42 curriculum project that challenges us to recreate the `printf()` function from the C standard library. The primary goal is to understand **variadic functions** in C — functions that accept a variable number of arguments of different types. Rather than relying on the original `printf()`, we implement our own parsing logic, type dispatch, and base conversion from scratch. Once approved, `ft_printf` can be added directly to the `libft` and used across all subsequent 42 projects.

## Algorithm & Technical Choices

The implementation is structured around three core responsibilities:

### 1. Format String Parser (`ft_printf.c`)
The main function iterates character by character over the format string. When it encounters a `%`, it reads the next character (the conversion specifier) and delegates to a dispatcher function. All other characters are written directly to `stdout`. The function accumulates the total count of characters written and returns it, mirroring the behavior of the original `printf()`.

### 2. Type Dispatcher (`get_type` in `ft_aux_functions.c`)
A chain of `if / else if` blocks maps each specifier (`c`, `s`, `p`, `d`, `i`, `u`, `x`, `X`, `%`) to its dedicated handler. The `va_list` is passed directly to each handler, which is responsible for calling `va_arg` internally with the correct type. This avoids double-consuming arguments and keeps each handler self-contained.

### 3. Generic Base Converter (`ft_putnbr_base_fd` / `ft_nlemb`)
Instead of writing separate functions for decimal, hexadecimal lowercase, and hexadecimal uppercase, a single recursive function handles all numeric bases. The base is passed as a string (e.g., `"0123456789abcdef"`), and the character at index `n % base_length` is used to select the correct digit. This design handles `%u`, `%x`, `%X`, and `%p` with the same two functions, differing only in the base string and cast type.

**Edge cases handled:**
- `%s` with `NULL` → prints `(null)`
- `%p` with `NULL` → prints `(nil)`
- `INT_MIN` → cast through `(unsigned long)` before negation to avoid overflow
- `%%` → prints a literal `%` without consuming any argument
- Format string ending with a lone `%` → safely ignored

## The Library (Function Inventory)

### Core
* `ft_printf` — Main entry point. Parses the format string and dispatches conversions.

### Auxiliary (`ft_aux_functions.c`)
* `get_type` — Dispatches to the correct handler based on the conversion specifier.
* `ft_handle_int` *(static)* — Handles `%c`, `%s`, `%d`, `%i`.
* `ft_handle_base` *(static)* — Handles `%u`, `%x`, `%X`.
* `ft_putnbr_base_fd` — Recursive printer for any numeric base.
* `ft_nlemb` — Counts the number of digits of a number in a given base.

### Conversions Implemented
| Specifier | Description |
|-----------|-------------|
| `%c` | Single character |
| `%s` | String (`NULL`-safe) |
| `%p` | Pointer address in hexadecimal (`NULL`-safe) |
| `%d` | Signed decimal integer |
| `%i` | Signed integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal (lowercase) |
| `%X` | Unsigned hexadecimal (uppercase) |
| `%%` | Literal percent sign |

## Instructions

To compile the library, `gcc` (or `cc`) and `make` must be installed. Compilation follows strict 42 rules with `-Wall -Wextra -Werror`.

### 1. Compilation
Clone the repository (with the `libft` submodule) and run `make` at the root:

**Available Make Rules:**
* `make` — Compiles `libft` and then `libftprintf.a`.
* `make clean` — Removes all object (`.o`) files.
* `make fclean` — Removes object files and the `libftprintf.a` binary.
* `make re` — Recompiles the entire project from scratch.

### 2. Usage
Include the header in your `.c` files:

    #include "ft_printf.h"

Compile linking both libraries:

    cc main.c -L. -lftprintf -L./libft -lft -o my_program

Or, since `libftprintf.a` already embeds `libft`:

    cc main.c -L. -lftprintf -o my_program

## Resources
* `man 3 printf` — Original function specification.
* `man 3 stdarg` — Documentation for `va_list`, `va_start`, `va_arg`, `va_end`.
* [cppreference — printf format string](https://en.cppreference.com/w/c/io/fprintf)
* **AI Usage:** AI was used strictly as a Socratic tutor throughout this project — it was never asked to generate ready-to-use code. Instead, it guided the reasoning process through questions: pointing out bugs, explaining why a given approach was wrong, and asking the student to identify and fix issues independently. AI assisted with conceptual clarification of variadic functions, `va_list` internals, and type promotion rules in C. All code was written by the student.
