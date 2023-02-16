
// 高级定时器TIMx,x[2,3,4,5]定时应用
#include "stm32f10x.h"
#include "bsp.h"



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	
//	int str[3];
	bsp_Init();
	
	

  while(1)
  {
		bsp_Idle();
	//	play_music();
    // comSendBuf(COM1, str, 3);//_ucaBuf是指针，要定义一个变量，将变量的地址作为实参传入   长度是根据实际数组大小来定的
		//printf("11111\r\n");
  }
}
/*********************************************END OF FILE**********************/

