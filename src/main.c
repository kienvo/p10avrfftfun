#include <avr/io.h>
#include <util/delay.h>
#include "usart/usart.h"
#include "p10/p10.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "timer/atmega-timers.h"
#include "LightweightRingBuff.h"
#include <util/atomic.h>
#include "fft/ffft.h"


extern void print_reg(uint8_t byte, char *name, char *file, int line);
extern void print_io_reg(char *file, int line);
volatile uint16_t _dadc = 0;
int16_t samples[FFT_N];
complex_t bfly_buff[FFT_N];
uint16_t spectrum[FFT_N/2];
uint8_t log_bias[] = {
	100, 100, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void runled(uint8_t k)
{
	static uint8_t a[32];
	for (uint8_t i = 31; i > 0; i--)
	{
		a[i] = a[i - 1];
	}
	a[0] = k;
	for (uint8_t i = 0; i < 32; i++)
	{
		p10drawLineCol(i, a[i], RED);
	}
}
RingBuff_t ringbuff;
void readadc() 
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

		setbit(ADCSRA, ADSC);
		loop_until_bit_is_set(ADCSRA, ADIF);		
	}
	uint16_t tadc = ADCW;
	RingBuffer_Insert(&ringbuff, tadc);
	PORTB ^= _BV(7);
}
void adc_init()
{
	ADMUX = _BV(REFS1);
	ADCSRB = 0;
	ADCSRA |= _BV(ADEN) | _BV(ADPS1);// | _BV(ADPS0) | _BV(ADPS2);
	setbit(ADCSRA, ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	// setbit(ADCSRA, ADIF);
	timer1(TIMER1_PRESCALER_1, ( (100LL/*us*/*F_CPU)/(1000000UL)), readadc); // TODO: fix
	RingBuffer_InitBuffer(&ringbuff);
}
/* 
ISR(ADC_vect)
{
	static uint8_t first_conv = 0;
	static uint8_t i = 0;
	uint16_t d = 0;
	if (i >= 10)
	{
		i = 0;
		PORTB ^= _BV(7);
		_dadc = d / 10;
		fprintf(stderr, "get %d \n", d);
		d = 0;
	}
	d += ADCL;
	d += ADCH << 8;
	// setbit(ADCSRA, ADSC);
	i++;
} */
/* by Jim Ulery */
static unsigned julery_isqrt(unsigned long val) {
    unsigned long temp, g=0, b = 0x8000, bshft = 15;
    do {
        if (val >= (temp = (((g << 1) + b)<<bshft--))) {
			g += b;
			val -= temp;
        }
    } while (b >>= 1);
    return g;
}

void draw_spectrumcl(uint8_t x, uint8_t size)
{
	uint8_t color = size/4;
	if(color >= 8) color = 7;
	p10drawLineCol(x, size, color);
}

void draw_spectrum() 
{
	uint8_t ii=2;
	for(; ii<34; ii++) {
		p10drawLineCol(ii-2, (spectrum[ii]*spectrum[ii])>>4,RED);
	}
	// for(; ii<64-16; ii+=2) {
	// 	p10drawLineCol(ii/2+8,((((spectrum[ii]*spectrum[ii+1]))>>9)+1), RED);
	// 	// p10drawLineCol(ii/2,((((spectrum[ii]*spectrum[ii+1]))>>10)+1), RED);
	// }
}
// void draw_spectrum() 
// {

// 	for(uint8_t ii=0; ii<64; ii+=2) {
// 		p10drawLineCol(ii/2,((((spectrum[ii]*spectrum[ii+1])/log_bias[32-ii/2])>>9)+1), RED);
// 		// p10drawLineCol(ii/2,((((spectrum[ii]*spectrum[ii+1]))>>10)+1), RED);
// 	}
// }

int main()
{

	setbit(DDRB, 7);
	clearbit(PORTB, 7);

	uart0_init(BAUD_CALC(38400)); // 8n1

	stdout = &uart0_io;
	stderr = &uart0_io;
	stdin = &uart0_io;

	sei();
	printf("String from printf\n");
	print_io_reg(__FILE__, __LINE__);
	p10init();
	print_io_reg(__FILE__, __LINE__);
	adc_init();
	uint32_t adc;
	fprintf(stderr, "sqrt:%d\n", julery_isqrt(100000000));
	while (1)
	{
		while(RingBuffer_GetCount(&ringbuff) < FFT_N-1) {
			// fprintf(stderr, "leng %d\n",RingBuffer_GetCount(&ringbuff));
		}
		// fprintf(stderr, "ringbuf %d\n", *(ringbuff.Out));
		for(int i=0; i<FFT_N; i++) {
			samples[i]=*(ringbuff.Out) - 32768;
			RingBuffer_Remove(&ringbuff);
		}
		// fprintf(stderr, "sample %d\n", samples[1]);
		fft_input(samples, bfly_buff);
		fft_execute(bfly_buff);
		fft_output(bfly_buff, spectrum);

		draw_spectrum();
	}
}

void abort(void)
{
	fprintf(stderr, "[ERROR] abort()\n");
	setbit(DDRB, 7);
	cli();
	while (1)
	{
		setbit(PORTB, 7);
		_delay_ms(100);
		clearbit(PORTB, 7);
		_delay_ms(100);
	}
}