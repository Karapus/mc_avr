#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#define LOG_N 6 
#define N (1 << LOG_N)
#define N_PS 7
typedef int16_t ptype;

ptype fromDouble(double x) {
	return x * (1 << N_PS);
}

int main() {
	puts("const ptype cosLUT[N/2] PROGMEM =");
	putchar('{');
	for (uint8_t i = 0; i < (N/2 - 1); ++i)
		printf("%d, ", fromDouble(cos(2 * M_PI * i / N)));
	printf("%d};\n", fromDouble(cos(M_PI)));
	puts("const ptype sinLUT[N/2] PROGMEM =");
	putchar('{');
	for (uint8_t i = 0; i < (N/2 - 1); ++i)
		printf("%d, ", fromDouble(sin(2 * M_PI * i / N)));
	printf("%d};\n", fromDouble(sin(M_PI)));
}
