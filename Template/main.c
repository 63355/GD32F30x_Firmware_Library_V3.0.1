/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

   \version 2024-12-20, V3.0.1, firmware for GD32F30x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "gd32f307c_eval.h"
#include "rgb_matrix.h"



int main(void)
{
	#ifdef __FIRMWARE_VERSION_DEFINE
		uint32_t fw_ver = 0;
	#endif	

	#ifdef __FIRMWARE_VERSION_DEFINE
		fw_ver = gd32f30x_firmware_version_get();
		/* print firmware version */
	//    printf("\r\nGD32F30x series firmware version: V%d.%d.%d", (uint8_t)(fw_ver >> 24), (uint8_t)(fw_ver >> 16), (uint8_t)(fw_ver >> 8));
	#endif /* __FIRMWARE_VERSION_DEFINE */

	systick_config();   

	full(10, 10, 10);


	led_matrix_init();	
	

    // --- 流水灯动画的状态变量 ---
    // 使用你定义的 r 和 c
    static uint8_t r = 0; // 当前亮灯的 y 坐标 (行)
    static uint8_t c = 0; // 当前亮灯的 x 坐标 (列)

    // 用于记录上一个像素位置的变量
    static uint8_t prev_r = 0;
    static uint8_t prev_c = 0;

    while (1)
	{
	
        // ==========================================================
        //         在单缓冲区上实现流水灯
        // ==========================================================

        // 1. 熄灭上一个位置的像素
        //    直接将前一个像素的颜色设置为黑色
        screen_buffer[prev_r][prev_c].r = 10;
        screen_buffer[prev_r][prev_c].g = 10;
        screen_buffer[prev_r][prev_c].b = 10;

        // 2. 点亮当前位置的像素
        //    这里使用你指定的亮度 31
        screen_buffer[r][c].r = 0;
        screen_buffer[r][c].g = 0;
        screen_buffer[r][c].b = 0;
        
        // 3. 更新“上一个位置”的记录，为下一次循环做准备
        //    将当前位置保存为下一次的“前一个位置”
        prev_r = r;
        prev_c = c;
		
        
        // 4. 计算下一个像素的位置
        c++; // X 坐标向右移动
        
        // 检查是否到达行尾
        if (c >= COL_NUM) {
            c = 0;       // 回到行首
            r++;     // 移动到下一行
            
            // 检查是否到达屏幕底部
            if (r >= ROW_NUM) {
                r = 0; // 回到屏幕顶部，重新开始
            }
        }

        // 5. 延时，控制流水灯的移动速度
        delay_ms(30);

    }
}






#ifdef GD_ECLIPSE_GCC
/* retarget the C library printf function to the USART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
#else
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));

    return ch;
}
#endif /* GD_ECLIPSE_GCC */
