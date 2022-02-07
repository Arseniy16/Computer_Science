/* --------------------------------------------------- */
/* TEST va_list and va_parameters(va_start, va_end...) */
/* --------------------------------------------------- */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define START(x, y) va_start(x, y)
#define END(x) va_end(x)

void minprintf(char * fmt, ...);

int main()
{
	int x = 87;
	char line[20];

	scanf("%s", line);
	minprintf("x = %d", x);
	minprintf("str = %s", line);

	return 0;
}

void minprintf(char * fmt, ...)
{
	va_list ap;
	char *p, *sval;
	int ival;

	START(ap, fmt);
	//va_start(ap, fmt);
	for (p = fmt; *p; p++)
	{
		if (*p != '%')
		{
			putchar(*p);
			continue;
		}

		switch (*++p)
		{
			case 'd': 
				ival = va_arg(ap, int);
				printf("%d\n", ival);
				break;

			case 's':
				for (sval = va_arg(ap, char*); *sval; sval++)
					putchar(*sval);
				break;

			default:
				putchar(*p);
				break;
		}
	}

	END(ap);
	//va_end(ap);
}