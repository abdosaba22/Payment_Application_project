#ifndef HEADER_H_ 
#define HEADER_H_

#include <string.h>	// strlen, strcpy
#include <stdio.h>	// printf, scanf_s
#include <ctype.h> // isalpha ,isalnu
#include <stdlib.h> // malloc,free

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned long uint32_t;


#define SETBIT(REG,BIT) (REG |= 1<<BIT)
#define CLRBIT(REG,BIT) (REG &= ~(1<<BIT))
#define TOGBIT(REG,BIT) (REG ^= 1<<BIT)
#define READBIT(REG,BIT)  ((REG >> BIT) & 1)

#endif