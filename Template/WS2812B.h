/*******************************************************************************
 * @file    WS2812B.h
 * @author  King
 * @version V1.00
 * @date    13-Dec-2021
 * @brief   ......
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WS2812B_H__

#include <stdint.h>
	
#define __WS2812B_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __WS2812B_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes ------------------------------------------------------------------*/
//#include stdint.h

/* Exported constants --------------------------------------------------------*/
#define WS2812B_GPIO_CLK_L    RCU_GPIOA
#define WS2812B_GPIO_PORT_L   GPIOA
#define WS2812B_GPIO_PIN_L    GPIO_PIN_2


#define WS2812B_GPIO_CLK_R    RCU_GPIOA
#define WS2812B_GPIO_PORT_R   GPIOA
#define WS2812B_GPIO_PIN_R    GPIO_PIN_4

#define GRB  24   
#define PIXEL_NUM 256  //
#define LED_DATA_SIZE ((PIXEL_NUM+3) * 3)                // 每个 LED 3 字节（RGB）
#define PACKET_TOTAL_LEN 775                             // 串口接收 2+2+1+1+768+1 = 775字节
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
EXTERN void DMA_config(void);    
EXTERN void DMA_config2(void); 
EXTERN void DMA_config3(void); 
EXTERN void DMA_config4(void); 
EXTERN void Fixed_Color_Display(uint32_t grb);	
EXTERN void Set_Row_Color_From_Payload(uint8_t row, const uint8_t* payload);																
EXTERN void Draw_Image(const uint32_t img[16][16]);
EXTERN void WS2812B_Running_Light(uint8_t pos, uint32_t color);
EXTERN void Show_Image_By_Single_Buffer(void);

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2021 *************************END OF FILE***/

