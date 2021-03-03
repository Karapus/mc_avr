#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#define LOG_N 8 
#define N (1 << LOG_N)
#define N_PS 7
typedef int16_t ptype;

ptype fromDouble(double x) {
	return x * (1 << N_PS);
}

ptype x_re[N];
ptype x_im[N];

ptype cosLUT[N/2];
ptype sinLUT[N/2];

int main() {
	for (uint8_t i = 0; i < N/2; ++i)
		printf("%d ", fromDouble(cos(2 * M_PI * i / N)));
	putchar('\n');
	for (uint8_t i = 0; i < N/2; ++i)
		printf("%d ", fromDouble(-sin(2 * M_PI * i / N)));
}
