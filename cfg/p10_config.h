#ifndef __P10_CONFIG_H__
#include <avr/io.h>
#define p10R1 	PC2	// blue up-side data pin
#define p10R2 	PA6 // blue down-side data pin
#define p10B1	PC0 // red up-side data pin
#define p10B2	PC4 // red down-side data pin
#define p10G1	PE3	//5!green up-side data pin
#define p10G2	PG5 //4!green down-side data pin

#define p10A	PA7 // select line
#define p10B	PE5	//4!select line
#define p10C	PA5 // select line

#define p10CLK 	PA3 // CLOCK
#define p10LAT 	PE4	//!LATCH
#define p10OE	PA1 // Output Enable

#define p10R1P 	PORTC
#define p10R2P 	PORTA
#define p10B1P	PORTC
#define p10B2P	PORTC
#define p10G1P	PORTE
#define p10G2P	PORTG

#define p10AP	PORTA
#define p10BP	PORTE
#define p10CP	PORTA

#define p10CLKP PORTA
#define p10LATP PORTE
#define p10OEP	PORTA
#endif /*__P10_CONFIG_H__*/