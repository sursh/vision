#include <stdio.h>

int main()
{

	int j[] = {0, 1, 129};
	int i;

	for(i=0; i<=2; i++) {
		j[i] > 0 ? (j[i] < 128 ? printf("first\n") : printf("second\n")) : printf("third\n");
	}

	return 0;
}