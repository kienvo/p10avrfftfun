#ifndef __P10_H__
/**
 * @file		p10.h
 * @author		github.com/kienvo (kienvo@kienlab.com)
 * @brief 		P10 utilites functions
 * @version		0.1
 * @date		May-04-2021
 * 
 * @copyright	Copyright (c) 2021 by kienvo@kienlab.com
 * 
 */
#include "p10_driver.h"

#define BLACK 	0x00
#define RED 	0x01
#define GREEN 	0x02
#define BLUE 	0x04
#define MAGENTA (BLUE | RED)
#define YELLOW 	(GREEN | RED)
#define CYAN 	(BLUE | GREEN)
#define WHITE 	(BLUE | GREEN | RED)

void testscan(void);
void p10test1(void);
void p10test2(void);
void p10init(void);
void p10drawLineCol(uint8_t x, uint8_t size, uint8_t color);

#endif /* __P10_H__ */