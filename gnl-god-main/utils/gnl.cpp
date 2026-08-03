extern "C"
{
#define new tripouille
#ifdef GNL_BONUS
# include "get_next_line_bonus.h"
#else
# include "get_next_line.h"
#endif
#undef new
}

#include "gnl.hpp"
#include "color.hpp"
#include "check.hpp"

#include <string.h>

extern int iTest;

using namespace std;

void gnl(int fd, char const * expectedReturn)
{
	char *  gnlReturn = get_next_line(fd);
	if (expectedReturn == NULL)
		check(gnlReturn == NULL);
	else
		check(!strcmp(gnlReturn, expectedReturn));
	free(gnlReturn);
	iTest++;
}