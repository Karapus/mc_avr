#include <inttypes.h>
#include <avr/pgmspace.h>

#define LOG_N 6 
#define N (1 << LOG_N)
#define N_PS 7
typedef int16_t ptype;

ptype x_re[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
ptype x_im[N];

#include "luts6.txt"

inline void swap(uint8_t i1, uint8_t i2) {
	ptype tmp = x_re[i1];
	x_re[i1] = x_re[i2];
	x_re[i2] = tmp;
}

inline ptype pmul(ptype a, ptype b) {
	return a * b >> N_PS;
}


void fft() {
	//reordering
	#include "swap6.txt"
	
	//fft
	for (uint8_t i = 1; i <= LOG_N; ++i) {
		const uint8_t m = 1 << i;
		for (uint8_t j = 0; j < N; j += m) {
			uint8_t span = m >> 1;
			uint8_t tf_k = (1 << LOG_N) >> i;
			for (uint8_t k = 0; k < span; ++k) {
				uint8_t tf = k * tf_k;
				uint8_t i1 = k + j;
				uint8_t i2 = i1 + span;

				ptype resReCos = pmul(pgm_read_byte(&cosLUT[tf]), x_re[i2]);
				ptype resReSin = pmul(pgm_read_byte(&sinLUT[tf]), x_re[i2]);
				ptype resImCos = pmul(pgm_read_byte(&cosLUT[tf]), x_im[i2]);
				ptype resImSin = pmul(pgm_read_byte(&sinLUT[tf]), x_im[i2]);

				x_re[i2] = x_re[i1] - resReCos + resImSin;
				x_im[i2] = x_im[i1] - resReSin - resImCos;
				x_re[i1] = x_re[i1] + resReCos - resImSin;
				x_im[i1] = x_im[i1] + resReSin + resImCos;
			}
		}
	}
	//conversion to polar
}
