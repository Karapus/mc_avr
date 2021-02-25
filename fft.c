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

ptype x_re[N];
ptype x_im[N];

ptype cosLUT[N/2];
ptype sinLUT[N/2];

void swap(uint8_t i1, uint8_t i2) {
	ptype tmp = x_re[i1];
	x_re[i1] = x_re[i2];
	x_re[i2] = tmp;
}

ptype pmul(ptype a, ptype b) {
	return a * b >> N_PS;
}


void fft() {
	//compute LUTS
	for (uint8_t i = 0; i < N/2; ++i) {
		cosLUT[i] = fromDouble(cos(2 * M_PI * i / N));
		sinLUT[i] = fromDouble(-sin(2 * M_PI * i / N));
	}
	//reordering
	#include "reorder.txt"
	
	//fft
	for (uint8_t k = 0; k < LOG_N; ++k) {
		uint8_t N_BLKS_STAGE = N / (2 * (k + 1));
		uint8_t N_BFS_BLOCK = 1 << k;
		uint8_t BF_SPAN = 1 << k;
		uint8_t BLK_STEP = 2 * (k + 1);
		uint8_t BF_STEP = 1;
		uint8_t TF_INDEX_STEP = N >> (k + 1);
		for (uint8_t m = 0; m < N_BLKS_STAGE; m++) {
                	uint8_t BLK_I = m *BLK_STEP;
			for (uint8_t n = 0; n < N_BFS_BLOCK; ++n) {
				uint8_t tf = n * TF_INDEX_STEP;
				uint8_t i1 = BLK_I + n * BF_STEP;
				uint8_t i2 = i1 + BF_SPAN;

				ptype resReCos = pmul(cosLUT[tf], x_re[i2]);
				ptype resReSin = pmul(sinLUT[tf], x_re[i2]);
				ptype resImCos = pmul(cosLUT[tf], x_im[i2]);
				ptype resImSin = pmul(sinLUT[tf], x_im[i2]);

				x_re[i2] = x_re[i1] - resReCos + resImSin;
				x_im[i2] = x_im[i1] - resReSin - resImCos;
				x_re[i1] = x_re[i1] + resReCos - resImSin;
				x_im[i1] = x_im[i1] + resReSin + resImCos;

			}
		}
	}
	//conversion to polar
}

double toDouble(ptype x) {
	return ((double) x) / (1 << N_PS);
}

int main() {
	x_re[1] = fromDouble(1);
	//printf("%f\n", toDouble(fromDouble(0.1)));
	for (int i = 0; i < N; ++i)
		printf("%f %f\n", toDouble(x_re[i]), toDouble(x_im[i]));

	fft();
	putchar('\n');
	for (int i = 0; i < N; ++i)
		printf("%f %f\n", toDouble(x_re[i]), toDouble(x_im[i]));
}
