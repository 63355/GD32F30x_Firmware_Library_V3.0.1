#ifndef _RGB_MATRIX_H_
#define _RGB_MATRIX_H_

#include "gd32f30x.h"
#include "gd32f307c_eval.h"

#define ROW_NUM 32 
#define COL_NUM 42

#define RGB_LEVEL 5 //RGB等级，6为RGB555
#define TIME_BASE 5 //驱动定时器中断的周期（单位us，分辨率0.1us），推荐值1~10，越小cpu负载越大

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
