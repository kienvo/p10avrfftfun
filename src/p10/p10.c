/**
 * @file		p10.c
 * @author		github.com/kienvo (kienvo@kienlab.com)
 * @brief 		P10 utilites functions
 * @version		0.1
 * @date		May-04-2021
 * 
 * @copyright	Copyright (c) 2021 by kienvo@kienlab.com
 * 
 */
#include "p10.h"
#include "p10_driver.h"
//#include "../font/font.h"
#include "../../font8x8/font8x8_basic.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <avr/interrupt.h>
#include "../timer/atmega-timers.h"

#define P10XSIZE 32
#define P10YSIZE 16
// #define CHAIN_EN

static volatile uint8_t p10buf[3][4][16];
static const int red=0;
static const int green=1;
static const int blue=2;
static uint8_t X;
static uint8_t Y;

static void active_row(uint8_t r) {
	assert(r<8);
	if(r & _BV(0)) { p10setA(); } else p10clearA();
	if(r & _BV(1)) { p10setB(); } else p10clearB();
	if(r & _BV(2)) { p10setC(); } else p10clearC();
	p10clearOE();
}
/**
 * @brief 		8 bit shifting. Note: this function does not support chaining
 * 
 * @param		redh 8bit red HIGH side 
 * @param		redl 8bit red LOW side 
 * @param		blueh 8bit blue HIGH side
 * @param		bluel 8bit blue LOW side
 * @param		greenh 8bit green HIGH side
 * @param		greenl 8bit green LOW side
 */
static void sendbyte(uint8_t redh, uint8_t redl, uint8_t blueh, 
uint8_t bluel, uint8_t greenh, uint8_t greenl) { /* TODO: update chaining feature */
	// p10clearLAT();
	for(int8_t i=0; i<8; i++) {
		
		if(redh & _BV(i)) { p10setR1(); } else p10clearR1(); // red high-side
		if(redl & _BV(i)) { p10setR2(); } else p10clearR2();

		if(blueh & _BV(i)) { p10setB1(); } else p10clearB1();
		if(bluel & _BV(i)) { p10setB2(); } else p10clearB2();
		
		if(greenh & _BV(i)) { p10setG1(); } else p10clearG1();
		if(greenl & _BV(i)) { p10setG2(); } else p10clearG2();
		// Make clock
		p10clearCLK();
		p10setCLK();
	}
	// p10setLAT();
}

static void refresh() {
	cli();
	static int r;
	assert(r<8);
	p10setOE();
		p10clearLAT();
		sendbyte(p10buf[red][0][r], p10buf[red][0][r+8], p10buf[blue][0][r],\
			p10buf[blue][0][r+8], p10buf[green][0][r], p10buf[green][0][r+8]);
		sendbyte(p10buf[red][1][r], p10buf[red][1][r+8], p10buf[blue][1][r],\
			p10buf[blue][1][r+8], p10buf[green][1][r], p10buf[green][1][r+8]);
		sendbyte(p10buf[red][2][r], p10buf[red][2][r+8], p10buf[blue][2][r],\
			p10buf[blue][2][r+8], p10buf[green][2][r], p10buf[green][2][r+8]);
		sendbyte(p10buf[red][3][r], p10buf[red][3][r+8], p10buf[blue][3][r],\
			p10buf[blue][3][r+8], p10buf[green][3][r], p10buf[green][3][r+8]);
		p10setLAT();
		active_row(r);
		// _delay_ms(200);
	if(++r>=8) {
		r=0;
	}
	sei();
}

/* 
 * //TODO: draw a cordinate
 */
void p10gotoXY(uint8_t x, uint8_t y) { // TODO: done this
	assert(x<P10XSIZE);
	assert(y<P10YSIZE);
	X = x;
	Y = y;
}
static uint8_t is_there_red(uint8_t color) {
	return (color & RED)==RED;
}
static uint8_t is_there_green(uint8_t color) {
	return (color & GREEN)==GREEN;
}
static uint8_t is_there_blue(uint8_t color) {
	return (color & BLUE)==BLUE;
}
/* 
 * c: charater;
 * color: BLACK RED GREEN BLUE MAGENTA YELLOW CYAN WHITE;
 */
void p10putc8x8(uint8_t c, uint8_t color) {// TODO: done this
	c -= 32;
	if(is_there_red(color)) {
		if(0+Y<P10YSIZE) p10buf[red][X%4][0+Y]=pgm_read_byte(&font8x8_basic[c][0]);
		if(1+Y<P10YSIZE) p10buf[red][X%4][1+Y]=pgm_read_byte(&font8x8_basic[c][1]);
		if(2+Y<P10YSIZE) p10buf[red][X%4][2+Y]=pgm_read_byte(&font8x8_basic[c][2]);
		if(3+Y<P10YSIZE) p10buf[red][X%4][3+Y]=pgm_read_byte(&font8x8_basic[c][3]);
		if(4+Y<P10YSIZE) p10buf[red][X%4][4+Y]=pgm_read_byte(&font8x8_basic[c][4]);
		if(5+Y<P10YSIZE) p10buf[red][X%4][5+Y]=pgm_read_byte(&font8x8_basic[c][5]);
		if(6+Y<P10YSIZE) p10buf[red][X%4][6+Y]=pgm_read_byte(&font8x8_basic[c][6]);
		if(7+Y<P10YSIZE) p10buf[red][X%4][7+Y]=pgm_read_byte(&font8x8_basic[c][7]);
	}
	if(is_there_green(color)) {
		if(0+Y<P10YSIZE) p10buf[green][X%4][0+Y]=pgm_read_byte(&font8x8_basic[c][0]);
		if(1+Y<P10YSIZE) p10buf[green][X%4][1+Y]=pgm_read_byte(&font8x8_basic[c][1]);
		if(2+Y<P10YSIZE) p10buf[green][X%4][2+Y]=pgm_read_byte(&font8x8_basic[c][2]);
		if(3+Y<P10YSIZE) p10buf[green][X%4][3+Y]=pgm_read_byte(&font8x8_basic[c][3]);
		if(4+Y<P10YSIZE) p10buf[green][X%4][4+Y]=pgm_read_byte(&font8x8_basic[c][4]);
		if(5+Y<P10YSIZE) p10buf[green][X%4][5+Y]=pgm_read_byte(&font8x8_basic[c][5]);
		if(6+Y<P10YSIZE) p10buf[green][X%4][6+Y]=pgm_read_byte(&font8x8_basic[c][6]);
		if(7+Y<P10YSIZE) p10buf[green][X%4][7+Y]=pgm_read_byte(&font8x8_basic[c][7]);
	}
	if(is_there_blue(color)) {
		if(0+Y<P10YSIZE) p10buf[blue][X%4][0+Y]=pgm_read_byte(&font8x8_basic[c][0]);
		if(1+Y<P10YSIZE) p10buf[blue][X%4][1+Y]=pgm_read_byte(&font8x8_basic[c][1]);
		if(2+Y<P10YSIZE) p10buf[blue][X%4][2+Y]=pgm_read_byte(&font8x8_basic[c][2]);
		if(3+Y<P10YSIZE) p10buf[blue][X%4][3+Y]=pgm_read_byte(&font8x8_basic[c][3]);
		if(4+Y<P10YSIZE) p10buf[blue][X%4][4+Y]=pgm_read_byte(&font8x8_basic[c][4]);
		if(5+Y<P10YSIZE) p10buf[blue][X%4][5+Y]=pgm_read_byte(&font8x8_basic[c][5]);
		if(6+Y<P10YSIZE) p10buf[blue][X%4][6+Y]=pgm_read_byte(&font8x8_basic[c][6]);
		if(7+Y<P10YSIZE) p10buf[blue][X%4][7+Y]=pgm_read_byte(&font8x8_basic[c][7]);
	}
}

void p10puts(char *s){ // TODO: done this

}

void p10drawLineCol(uint8_t x, uint8_t size, uint8_t color)
{
	assert(x<32);
	// assert(size<16);
	if(size >16) size = 16;
	assert(color<0x0F);
	uint8_t i=0;
	// Clear line column
	for(; i<16; i++) {
		clearbit(p10buf[blue][x/8][i], x%8);
		clearbit(p10buf[green][x/8][i], x%8);
		clearbit(p10buf[red][x/8][i], x%8);
	}
	for(i=16-size; i<16; i++) {
		if(is_there_blue(color)) setbit(p10buf[blue][x/8][i], x%8);
		if(is_there_green(color)) setbit(p10buf[green][x/8][i], x%8);
		if(is_there_red(color)) setbit(p10buf[red][x/8][i], x%8);
	}

}

void p10init() {
	timer0(TIMER0_PRESCALER_1024, 1000/*us*//(1000000UL/(F_CPU/1024)), refresh);
	
	p10buf[red][0][0]=font8x8_basic['A'-32][0];
	p10buf[red][0][1]=font8x8_basic['A'-32][1];
	p10buf[red][0][2]=font8x8_basic['A'-32][2];
	p10buf[red][0][3]=font8x8_basic['A'-32][3];
	p10buf[red][0][4]=font8x8_basic['A'-32][4];
	p10buf[red][0][5]=font8x8_basic['A'-32][5];
	p10buf[red][0][6]=font8x8_basic['A'-32][6];
	p10buf[red][0][7]=font8x8_basic['A'-32][7];
	
	p10buf[red][1][8]=font8x8_basic['A'-32][0];
	p10buf[red][1][9]=font8x8_basic['A'-32][1];
	p10buf[red][1][10]=font8x8_basic['A'-32][2];
	p10buf[red][1][11]=font8x8_basic['A'-32][3];
	p10buf[red][1][12]=font8x8_basic['A'-32][4];
	p10buf[red][1][13]=font8x8_basic['A'-32][5];
	p10buf[red][1][14]=font8x8_basic['A'-32][6];
	p10buf[red][1][15]=font8x8_basic['A'-32][7];

	p10buf[blue][1][0]=font8x8_basic['B'-32][0];
	p10buf[blue][1][1]=font8x8_basic['B'-32][1];
	p10buf[blue][1][2]=font8x8_basic['B'-32][2];
	p10buf[blue][1][3]=font8x8_basic['B'-32][3];
	p10buf[blue][1][4]=font8x8_basic['B'-32][4];
	p10buf[blue][1][5]=font8x8_basic['B'-32][5];
	p10buf[blue][1][6]=font8x8_basic['B'-32][6];
	p10buf[blue][1][7]=font8x8_basic['B'-32][7];

	p10buf[green][2][0]=font8x8_basic['C'-32][0];
	p10buf[green][2][1]=font8x8_basic['C'-32][1];
	p10buf[green][2][2]=font8x8_basic['C'-32][2];
	p10buf[green][2][3]=font8x8_basic['C'-32][3];
	p10buf[green][2][4]=font8x8_basic['C'-32][4];
	p10buf[green][2][5]=font8x8_basic['C'-32][5];
	p10buf[green][2][6]=font8x8_basic['C'-32][6];
	p10buf[green][2][7]=font8x8_basic['C'-32][7];
	
	// p10buf[red][0][0]=_BV(0);
	// p10buf[red][0][1]=_BV(1);
	// p10buf[red][0][2]=_BV(2);
	// p10buf[red][0][3]=_BV(3);
	// p10buf[red][0][4]=_BV(4);
	// p10buf[red][0][5]=_BV(5);
	// p10buf[red][0][6]=_BV(6);
	// p10buf[red][0][7]=_BV(7);

	// p10buf[blue][1][0]=_BV(0);
	// p10buf[blue][1][1]=_BV(1);
	// p10buf[blue][1][2]=_BV(2);
	// p10buf[blue][1][3]=_BV(3);
	// p10buf[blue][1][4]=_BV(4);
	// p10buf[blue][1][5]=_BV(5);
	// p10buf[blue][1][6]=_BV(6);
	// p10buf[blue][1][7]=_BV(7);
	
	// p10buf[green][3][0]=_BV(0);
	// p10buf[green][3][1]=_BV(1);
	// p10buf[green][3][2]=_BV(2);
	// p10buf[green][3][3]=_BV(3);
	// p10buf[green][3][4]=_BV(4);
	// p10buf[green][3][5]=_BV(5);
	// p10buf[green][3][6]=_BV(6);
	// p10buf[green][3][7]=_BV(7);

	// p10buf[green][3][8]=_BV(0);
	// p10buf[green][3][9]=_BV(1);
	// p10buf[green][3][10]=_BV(2);
	// p10buf[green][3][11]=_BV(3);
	// p10buf[green][3][12]=_BV(4);
	// p10buf[green][3][13]=_BV(5);
	// p10buf[green][3][14]=_BV(6);
	// p10buf[green][3][15]=_BV(7);
	
	p10gotoXY(0,10);
	p10putc8x8('C', YELLOW);

	p10drawLineCol(8, 10, YELLOW);
	p10drawLineCol(2, 14, GREEN);
	p10drawLineCol(1, 5, BLUE);


	setbit(*(&p10B2P-1),p10B2);
	setbit(*(&p10R1P-1),p10R1);
	setbit(*(&p10R2P-1),p10R2);
	setbit(*(&p10B1P-1),p10B1);
	setbit(*(&p10G1P-1),p10G1);
	setbit(*(&p10G2P-1),p10G2);

	setbit(*(&p10AP-1),p10A);
	setbit(*(&p10BP-1),p10B);
	setbit(*(&p10CP-1),p10C);

	setbit(*(&p10CLKP-1),p10CLK);
	setbit(*(&p10LATP-1),p10LAT);
	setbit(*(&p10OEP-1),p10OE);
	
 	p10clearB1();
 	p10clearB2();
 	p10clearR1();
 	p10clearR2();
 	p10clearG1();
 	p10clearG2();

 	p10setA();
 	p10setB();
 	p10setC();

 	p10setCLK();
 	p10setLAT();
 	p10setOE();

}
