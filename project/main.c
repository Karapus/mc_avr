﻿#include "oled.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define LOG_N 6 
#define N (1 << LOG_N)
#define N_PS 7

typedef int16_t ptype;

ptype x_re[N];
ptype x_im[N];

#include "luts6.txt"

void swap(uint8_t i1, uint8_t i2) {
	ptype tmp = x_re[i1];
	x_re[i1] = x_re[i2];
	x_re[i2] = tmp;
}

ptype pmul(ptype a, ptype b) {
	return a * b >> N_PS;
}

ptype abs(ptype x) {
	return ((x < 0) ? -x : x) >> 8;
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
	for (int i = 0; i < N; ++i)
		x_re[i] = magnLUT[abs(x_re[i])][abs(x_im[i])];
}

ptype ADC_read() {
	ADMUX = 0x40;        // channel A0 selected
	ADCSRA|=(1<<ADSC);   // start conversion
	// wait for ADIF conversion complete return
	while(!(ADCSRA & (1<<ADIF)))
		;
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	return (ADC); //return calculated ADC value
}

int main() {
	OLED_Init();
	// Selecting internal reference voltage
	ADMUX = (1<<REFS0);
	// Enable ADC also set Prescaler as 128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	TCNT0 = 0;
	TCCR0 = (1<<CS00);

	while (1) {
		for (int i = 0; i < N; ++i) {
			while ((TIFR & 0x01) == 0)
				;
			x_re[i] = ADC_read() << 5;
			TCNT0 = 0;
			TIFR = 0x01;
		}
		fft();
		OLED_Graph(x_re);
	}
}
