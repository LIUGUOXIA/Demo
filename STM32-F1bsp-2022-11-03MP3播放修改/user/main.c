
// �߼���ʱ��TIMx,x[2,3,4,5]��ʱӦ��
#include "stm32f10x.h"
#include "bsp.h"



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	
//	int str[3];
	bsp_Init();
	
	

  while(1)
  {
		bsp_Idle();
	//	play_music();
    // comSendBuf(COM1, str, 3);//_ucaBuf��ָ�룬Ҫ����һ���������������ĵ�ַ��Ϊʵ�δ���   �����Ǹ���ʵ�������С������
		//printf("11111\r\n");
  }
}
/*********************************************END OF FILE**********************/

