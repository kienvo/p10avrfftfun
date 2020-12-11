#ifndef __P10_DRIVER_H__
#include "../../cfg/p10_config.h"
#include "bit_utils.h"


// Set pins
#define p10setB1()		setbit(p10B1P,p10B1)
#define p10setB2()		setbit(p10B2P,p10B2)
#define p10setR1()		setbit(p10R1P,p10R1)
#define p10setR2()		setbit(p10R2P,p10R2)
#define p10setG1()		setbit(p10G1P,p10G1)
#define p10setG2()		setbit(p10G2P,p10G2)

#define p10setA()		setbit(p10AP,p10A)
#define p10setB()		setbit(p10BP,p10B)
#define p10setC()		setbit(p10CP,p10C)

#define p10setCLK() 	setbit(p10CLKP,p10CLK)
#define p10setLAT() 	setbit(p10LATP,p10LAT)
#define p10setOE()		setbit(p10OEP,p10OE)

// Clear pins
#define p10clearB2()	clearbit(p10B2P,p10B2)
#define p10clearR1()	clearbit(p10R1P,p10R1)
#define p10clearR2()	clearbit(p10R2P,p10R2)
#define p10clearB1()	clearbit(p10B1P,p10B1)
#define p10clearG1()	clearbit(p10G1P,p10G1)
#define p10clearG2()	clearbit(p10G2P,p10G2)

#define p10clearA()		clearbit(p10AP,p10A)
#define p10clearB()		clearbit(p10BP,p10B)
#define p10clearC()		clearbit(p10CP,p10C)

#define p10clearCLK() 	clearbit(p10CLKP,p10CLK)
#define p10clearLAT() 	clearbit(p10LATP,p10LAT)
#define p10clearOE()	clearbit(p10OEP,p10OE)


#endif /* __P10_DRIVER_H__ */