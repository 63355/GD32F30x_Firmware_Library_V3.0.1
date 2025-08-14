#ifndef _RGB_MATRIX_H_
#define _RGB_MATRIX_H_

#include "gd32f30x.h"
#include "gd32f307c_eval.h"

#define ROW_NUM 32 
#define COL_NUM 42

#define RGB_LEVEL 7 //RGB等级，5为RGB555
#define TIME_BASE 4 //最小位平面持续时间，单位0.1us

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_t;

extern __attribute__((aligned(4))) volatile RGB_t screen_buffer[ROW_NUM][COL_NUM]; 

void led_matrix_init(void);
void full(uint8_t r, uint8_t g, uint8_t b);
void delay_ms(volatile uint32_t nms);

#endif
