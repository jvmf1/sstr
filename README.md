# example

```c
#include "sstr.h"
#include <stdio.h>

// the code above does not checks for errors

int main() {
	sstr *string = sstr_create(NULL); // creates empty string
	sstr *time = sstr_create(NULL);

	FILE *p = popen("date", "r");
	sstr_fgetsx(time, p, EOF, 32); // reads p until 'EOF'. increasing capacity by 32 everytime when needed
	pclose(p);

	sstr_printf(string, "Today's date is %s", time->data);

	printf("%s", string->data);

	sstr_free(time);
	sstr_free(string);
}
```
