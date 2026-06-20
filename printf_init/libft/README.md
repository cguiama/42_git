*This project has been created as part of the 42 curriculum by guilamar.*

# Libft

## Description
Libft is the foundational project of the 42 curriculum. Its primary goal is to recreate a custom C standard library from scratch. By rewriting these basic functions, we gain a deep understanding of their internal mechanics, data structures, pointer arithmetic, and dynamic memory allocation. This library will serve as a core dependency for most of the subsequent projects in the Common Core.

## The Library (Function Inventory)
The project is divided into the implementation of three specific modules:

### Part 1: Libc Functions
Replicas of standard C functions.
* **Character classification:** `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`
* **Character manipulation:** `ft_toupper`, `ft_tolower`
* **String manipulation:** `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strdup`
* **Memory manipulation:** `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc`
* **Conversion:** `ft_atoi`

### Part 2: Additional Functions
Utility functions for complex string manipulation and file descriptor outputs.
* **String generation/formatting:** `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`
* **Function application:** `ft_strmapi`, `ft_striteri`
* **File Descriptor output:** `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`

### Part 3: Linked Lists
Data structure management functions to create, iterate, and destroy linked lists (`t_list`).
* **Node creation:** `ft_lstnew`
* **List manipulation:** `ft_lstadd_front`, `ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`
* **Iteration & Information:** `ft_lstsize`, `ft_lstlast`, `ft_lstiter`, `ft_lstmap`

## Instructions

To compile the library, you must have `gcc` (or `cc`) and `make` installed. The compilation follows strict 42 rules using the flags `-Wall -Wextra -Werror`.

### 1. Compilation
Clone the repository and run `make` at the root of the directory to compile the `libft.a` static library.

**Available Make Rules:**
* `make`: Compiles the standard functions (Part 1 , 2 & 3).
* `make clean`: Removes all object (`.o`) files.
* `make fclean`: Removes the object files and the `libft.a` binary.
* `make re`: Recompiles the entire project from scratch.

### 2. Usage (Quick Start)
To use the library in your own projects, include the header in your `.c` files:

    #include "libft.h"

Then, compile your project linking the generated library. Use the `-L` flag to specify the directory path of the library, and the `-l` flag to link it:

    cc main.c -L. -lft -o my_program

## Resources
* Standard C Library Manual Pages (`man 3`).
* **AI Usage:** AI was utilized strictly as an architectural tutor. It was not used to generate ready-to-use code, but rather to break down complex pointer mechanics (like the double pointer arrays in `ft_split`) and assist in structural timeblocking to balance the curriculum demands with professional routines.
