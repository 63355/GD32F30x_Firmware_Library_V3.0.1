#include "rgb_matrix.h"

__attribute__((aligned(4))) volatile RGB_t screen_buffer[ROW_NUM][COL_NUM]; 

/*----------------------下面是移植需要实现部分----------------------*/

#define MCU_A_SET    (GPIO_BOP(MCU_A_GPIO_PORT) = (MCU_A_PIN))
#define MCU_A_RESET  (GPIO_BC(MCU_A_GPIO_PORT) = (MCU_A_PIN))

#define MCU_B_SET    (GPIO_BOP(MCU_B_GPIO_PORT) = (MCU_B_PIN))
#define MCU_B_RESET  (GPIO_BC(MCU_B_GPIO_PORT) = (MCU_B_PIN))

#define MCU_C_SET    (GPIO_BOP(MCU_C_GPIO_PORT) = (MCU_C_PIN))
#define MCU_C_RESET  (GPIO_BC(MCU_C_GPIO_PORT) = (MCU_C_PIN))

#define MCU_D_SET    (GPIO_BOP(MCU_D_GPIO_PORT) = (MCU_D_PIN))
#define MCU_D_RESET  (GPIO_BC(MCU_D_GPIO_PORT) = (MCU_D_PIN))

#define MCU_E_SET    (GPIO_BOP(MCU_E_GPIO_PORT) = (MCU_E_PIN))
#define MCU_E_RESET  (GPIO_BC(MCU_E_GPIO_PORT) = (MCU_E_PIN))

#define SM16306_SDI_R_SET    (GPIO_BOP(SM16306_SDI_R_GPIO_PORT) = (SM16306_SDI_R_PIN))
#define SM16306_SDI_R_RESET  (GPIO_BC(SM16306_SDI_R_GPIO_PORT) = (SM16306_SDI_R_PIN))

#define SM16306_SDI_G_SET    (GPIO_BOP(SM16306_SDI_G_GPIO_PORT) = (SM16306_SDI_G_PIN))
#define SM16306_SDI_G_RESET  (GPIO_BC(SM16306_SDI_G_GPIO_PORT) = (SM16306_SDI_G_PIN))

#define SM16306_SDI_B_SET    (GPIO_BOP(SM16306_SDI_B_GPIO_PORT) = (SM16306_SDI_B_PIN))
#define SM16306_SDI_B_RESET  (GPIO_BC(SM16306_SDI_B_GPIO_PORT) = (SM16306_SDI_B_PIN))

#define SM16306_CLK_SET      (GPIO_BOP(SM16306_CLK_GPIO_PORT) = (SM16306_CLK_PIN))
#define SM16306_CLK_RESET    (GPIO_BC(SM16306_CLK_GPIO_PORT) = (SM16306_CLK_PIN))

#define SM16306_LE_SET       (GPIO_BOP(SM16306_LE_GPIO_PORT) = (SM16306_LE_PIN))
#define SM16306_LE_RESET     (GPIO_BC(SM16306_LE_GPIO_PORT) = (SM16306_LE_PIN))

#define SM16306_OE_SET       (GPIO_BOP(SM16306_OE_GPIO_PORT) = (SM16306_OE_PIN))
#define SM16306_OE_RESET     (GPIO_BC(SM16306_OE_GPIO_PORT) = (SM16306_OE_PIN))

/**
* @brief 开一个(!!arr分辨率为0.1us!!)定时器中断,开出来的回调函数放在本文件的下面部分位置，里面执行“tim2_callback();”
 * @param period_us 期望周期 (单位：微秒)，0.1µs 分辨率
 *        例如：1.5 表示 1.5µs
 */
static void m_timer_init(float period_us)
{
    #define TIMER_INPUT_CLOCK_FREQ_HZ  120000000U // 假设定时器时钟为120MHz

    uint16_t prescaler_value = (uint16_t)((TIMER_INPUT_CLOCK_FREQ_HZ / 10000000U) - 1); // (120M/10M)-1=11

    // 把 period_us 转换为 0.1µs 计数单位
    uint32_t period_ticks = (uint32_t)(period_us * 10.0f) - 1; // ARR 值

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_parameter_struct timer_init_parameter;
    timer_deinit(TIMER2);

    timer_init_parameter.prescaler         = prescaler_value;
    timer_init_parameter.counterdirection  = TIMER_COUNTER_UP;
    timer_init_parameter.period            = period_ticks;
    timer_init_parameter.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init_parameter.repetitioncounter = 0;

    timer_init(TIMER2, &timer_init_parameter);

    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

    nvic_irq_enable(TIMER2_IRQn, 2, 0);

    timer_enable(TIMER2);
} 
/**
 * @brief uart0串口初始化函数
 * 波特率：1 500 000
 * 纯接收。接收开DMA和空闲中断。主机发完一个包（四分之一帧）进空闲中断后进DMA缓冲区把数据按照某个状态变量依次搬运到指定写缓冲区，独立存放
 */
static void m_uart_init(void)
{

}

/**
 * @brief 初始化对应的gpio和时钟
 */
void led_matrix_init(void)
{
	m_timer_init(TIME_BASE);
	//行片选部分的gpio初始化
	
	rcu_periph_clock_enable(MCU_A_GPIO_CLK);//开启时钟
	gpio_init(MCU_A_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,MCU_A_PIN);//初始化
	GPIO_BC(MCU_A_GPIO_PORT) = MCU_A_PIN;//默认输出低电平

	rcu_periph_clock_enable(MCU_B_GPIO_CLK);
	gpio_init(MCU_B_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,MCU_B_PIN);
	GPIO_BC(MCU_B_GPIO_PORT) = MCU_B_PIN;

	rcu_periph_clock_enable(MCU_C_GPIO_CLK);
	gpio_init(MCU_C_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,MCU_C_PIN);
	GPIO_BC(MCU_C_GPIO_PORT) = MCU_C_PIN;
	
	rcu_periph_clock_enable(MCU_D_GPIO_CLK);
	gpio_init(MCU_D_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,MCU_D_PIN);
	GPIO_BC(MCU_D_GPIO_PORT) = MCU_D_PIN;

	rcu_periph_clock_enable(MCU_E_GPIO_CLK);
	gpio_init(MCU_E_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,MCU_E_PIN);
	GPIO_BC(MCU_E_GPIO_PORT) = MCU_E_PIN;
	
	//SM16306横流驱动相关的用于 三路串行数据写入、数据时钟、锁存、使能的GPIO的初始化
	
	rcu_periph_clock_enable(RCU_AF);//使能AFIO的时钟
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);//使用AFIO将原本用于JTAG功能的PA15解放出来用作普通GPIO	

	rcu_periph_clock_enable(SM16306_SDI_R_GPIO_CLK);
	gpio_init(SM16306_SDI_R_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_SDI_R_PIN);
	GPIO_BC(SM16306_SDI_R_GPIO_PORT) = SM16306_SDI_R_PIN;

	rcu_periph_clock_enable(SM16306_SDI_G_GPIO_CLK);
	gpio_init(SM16306_SDI_G_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_SDI_G_PIN);
	GPIO_BC(SM16306_SDI_G_GPIO_PORT) = SM16306_SDI_G_PIN;

	rcu_periph_clock_enable(SM16306_SDI_B_GPIO_CLK);
	gpio_init(SM16306_SDI_B_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_SDI_B_PIN);
	GPIO_BC(SM16306_SDI_B_GPIO_PORT) = SM16306_SDI_B_PIN;

	rcu_periph_clock_enable(SM16306_CLK_GPIO_CLK);
	gpio_init(SM16306_CLK_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_CLK_PIN);
	GPIO_BC(SM16306_CLK_GPIO_PORT) = SM16306_CLK_PIN;

	rcu_periph_clock_enable(SM16306_LE_GPIO_CLK);
	gpio_init(SM16306_LE_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_LE_PIN);
	GPIO_BC(SM16306_LE_GPIO_PORT) = SM16306_LE_PIN;//LE默认

	rcu_periph_clock_enable(SM16306_OE_GPIO_CLK);
	gpio_init(SM16306_OE_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,SM16306_OE_PIN);
	GPIO_BOP(SM16306_OE_GPIO_PORT) = SM16306_OE_PIN;//OE默认高电平，即关闭所有输出
}

/*----------------------上面是移植需要实现部分----------------------*/

#define SM16306_UNLOCK  SM16306_LE_SET
#define SM16306_LOCK    SM16306_LE_RESET

#define SM16306_ON      SM16306_OE_RESET
#define SM16306_OFF     SM16306_OE_SET

/**
 * @brief 用于选中矩阵屏幕的某一行
 * @param row 希望选中的行号，范围0~ROW_NUM-1
 */
static inline void row_select(uint8_t row)
{
    if (row & 0x01) MCU_A_SET; else MCU_A_RESET;
    if (row & 0x02) MCU_B_SET; else MCU_B_RESET;
    if (row & 0x04) MCU_C_SET; else MCU_C_RESET;
    if (row & 0x08) MCU_D_SET; else MCU_D_RESET;
    if (row & 0x10) MCU_E_SET; else MCU_E_RESET;
}

///**
// * @brief 根据RGB888的某一位串行写入灯的数据
// */
//static inline void write_data(RGB_t *data, uint8_t mask)
//{	
//	//__NOP();
//	SM16306_CLK_SET;	
//	if(data->r & mask) SM16306_SDI_R_SET; else SM16306_SDI_R_RESET;
//	if(data->g & mask) SM16306_SDI_G_SET; else SM16306_SDI_G_RESET;
//	if(data->b & mask) SM16306_SDI_B_SET; else SM16306_SDI_B_RESET;	
//	SM16306_CLK_RESET;//时钟驱动
//}

#define write_data(data_ptr, data_mask) \
    do { \
		__NOP();__NOP();\
        SM16306_CLK_SET; \
        if((data_ptr)->r & (data_mask)) SM16306_SDI_R_SET; else SM16306_SDI_R_RESET; \
        if((data_ptr)->g & (data_mask)) SM16306_SDI_G_SET; else SM16306_SDI_G_RESET; \
        if((data_ptr)->b & (data_mask)) SM16306_SDI_B_SET; else SM16306_SDI_B_RESET; \
        SM16306_CLK_RESET; \
    } while(0)

/**
 * @param line 希望刷入的一组中的某一行，范围0~7
 * @param bit 刷入的这行的某一个位平面，范围0~7，0表示数据低位
 */
static inline void input_line(uint8_t line, uint8_t bit)
{
	uint8_t mask = 1 << bit;
	//-------------------------------------------------------------------------------------
	for(int8_t i=12; i>=0; i--)
	{		
		write_data(&screen_buffer[line+24][i], mask);
	}
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=28; i>=13; i--)
	{		
		write_data(&screen_buffer[line+24][i], mask);
	}	
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=41; i>=29; i--)
	{		
		write_data(&screen_buffer[line+24][i], mask);
	}	
	//-------------------------------------------------------------------------------------
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=41; i>=0; i--)
	{		
		write_data(&screen_buffer[line+16][i], mask);
	}	
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	//-------------------------------------------------------------------------------------
	for(int8_t i=12; i>=0; i--)
	{		
		write_data(&screen_buffer[line+8][i], mask);
	}
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=28; i>=13; i--)
	{		
		write_data(&screen_buffer[line+8][i], mask);
	}	
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=41; i>=29; i--)
	{		
		write_data(&screen_buffer[line+8][i], mask);
	}	
	//-------------------------------------------------------------------------------------
	write_data(&screen_buffer[0][0], 0);//插入三个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	for(int8_t i=41; i>=0; i--)
	{		
		write_data(&screen_buffer[line][i], mask);
	}	
	write_data(&screen_buffer[0][0], 0);//插入四个空数据
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	write_data(&screen_buffer[0][0], 0);
	//-------------------------------------------------------------------------------------
}

/**
 * @brief 向指定缓冲区全局刷入指定rgb数据
 */
void full(uint8_t r, uint8_t g, uint8_t b)
{
	for(uint8_t i=0; i<32; i++)
	{
		for(uint8_t j=0; j<42; j++)
		{
			screen_buffer[i][j].r = r;
			screen_buffer[i][j].g = g;
			screen_buffer[i][j].b = b;
		}
	}
}


///**
//* @brief 周期执行的灯珠刷新函数
//*/
//void tim2_callback(void)
//{
//	volatile static uint8_t current_group = 0;//当前组
//	volatile static uint8_t current_row = 0;//当前组的行
//	volatile static uint8_t current_plane = 0;//当前位平面等级
//	volatile static uint16_t cnt = 0;//运行计数器

//	static const uint16_t map[9][4] = {
//		{1,2,3,4},        //1
//		{5,7,9,11},       //2
//		{13,17,21,25},    //4
//		{29,37,45,53},    //8
//		{61,77,93,109},   //16
//		{125,157,189,221},//32
//		{253,317,381,445},//64
//		{509,637,765,893},//128
//		{1021,0,0,0}
//	};
//		
//	cnt++;

//	if (current_plane == RGB_LEVEL) 
//	{
//		if (cnt == map[RGB_LEVEL][0]) //哨兵
//		{ 
//			current_plane = 0;
//			current_row = (current_row + 1) % 8;
//			cnt = 0;
//		}
//	} 
//	else 
//	{
//		if (cnt == map[current_plane][current_group])
//		{
//			SM16306_OFF;
//			row_select(current_group * 8 + current_row);//选中某一行
//			if (current_group == 0) //仅在 group=0 时加载数据
//			{
//				timer_disable(TIMER2); //关定时器
//				SM16306_UNLOCK; //解锁，让数据流入锁存器
//				input_line(current_row, 8 - RGB_LEVEL + current_plane); //刷入该行的数据
//				SM16306_LOCK;//上锁，稳定数据
//				timer_enable(TIMER2); //开定时器				
//			}
//			SM16306_ON;
//			
//			// 状态更新
//			current_group++;
//			if (current_group >= 4)
//			{
//				current_group = 0;
//				current_plane++;
//			}
//		}
//		//timer_counter_value_config(TIMER2, 0);//清除tim2的cnt，让每一个平面的计时更加准确
//	}
//}


void tim2_callback(void)
{
	volatile static uint8_t current_group = 0;
	volatile static uint8_t current_row = 0;
	volatile static uint8_t current_plane = 0;

	static const uint16_t map[8][4] = 
	{
		{1*TIME_BASE,  1*TIME_BASE,  1*TIME_BASE,  1*TIME_BASE},        
		{2*TIME_BASE,  2*TIME_BASE,  2*TIME_BASE,  2*TIME_BASE},       
		{4*TIME_BASE,  4*TIME_BASE,  4*TIME_BASE,  4*TIME_BASE},      
		{8*TIME_BASE,  8*TIME_BASE,  8*TIME_BASE,  8*TIME_BASE},       
		{16*TIME_BASE, 16*TIME_BASE, 16*TIME_BASE, 16*TIME_BASE},  
		{32*TIME_BASE, 32*TIME_BASE, 32*TIME_BASE, 32*TIME_BASE},   
		{64*TIME_BASE, 64*TIME_BASE, 64*TIME_BASE, 64*TIME_BASE},   
		{128*TIME_BASE,128*TIME_BASE,128*TIME_BASE,128*TIME_BASE},
	};
	
    SM16306_OFF;
    if (current_group == 0) 
	{
		timer_disable(TIMER2); //关定时器
		SM16306_UNLOCK; //解锁，让数据流入锁存器
		input_line(current_row, 8 - RGB_LEVEL + current_plane); //刷入该行的数据
		SM16306_LOCK;//上锁，稳定数据
		timer_enable(TIMER2); //开定时器					
    }
    row_select(current_group * 8 + current_row);
    SM16306_ON;
    
	timer_autoreload_value_config(TIMER2, map[current_plane][current_group] - 1);//动态修改arr
	
	
    current_group++;//状态更新
    if (current_group >= 4) 
	{
        current_group = 0;
        current_plane++;
        if (current_plane >= RGB_LEVEL) 
		{
            current_plane = 0;
            current_row = (current_row + 1) % 8;
        }
    }
    timer_counter_value_config(TIMER2, 0);
}


/**
 * @brief 定时器中断回调函数
 */
void TIMER2_IRQHandler(void)
{
    // 检查是否是更新中断触发的
    if (SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP)) 
	{
		tim2_callback();  
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);//清除中断挂起标志位    	
    }
}

/**
 * @brief 阻塞若干个ms
 */
void delay_ms(volatile uint32_t nms)
{
    volatile uint32_t i, j;

    for (i = 0; i < nms; i++) {
        // 这个内层循环的计数值需要根据你的MCU主频进行粗略调整
        // 假设在某个主频下，这个循环大约消耗 1ms
        for (j = 0; j < 3500; j++) { 
            __NOP(); // 执行一条空指令，消耗一个时钟周期
        }
    }
}