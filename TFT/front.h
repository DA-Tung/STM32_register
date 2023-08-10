#ifndef _FRONT_H_
#define _FRONT_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include <stdint.h>

typedef struct {
    uint8_t 	  	width;
    uint8_t 	  	height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;


#endif //_FRONT_H_

