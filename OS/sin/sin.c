#include <math.h>
#include <stdio.h>

int main() {
	int i, error = 0;
	for (i = 0; i < 100000000; i++) {
		double a = cos(i) * cos(i) + sin(i) * sin(i);
		if (a > 1.0005 || a < 0.9995) {
			error = 1;
			break;
		}
	}

	printf("%d\n", error);
}
