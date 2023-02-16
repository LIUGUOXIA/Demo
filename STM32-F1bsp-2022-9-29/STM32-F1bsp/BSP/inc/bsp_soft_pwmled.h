#ifndef  _PWMLED_H_
#define  _PWMLED_H_


#include "stdint.h"

typedef enum
{
	LED_ONO = 0,
	LED_NUM
}Pwmlednum_ennum;



typedef struct
{
	const uint16_t freq;
	const uint16_t duty_gear;
 uint16_t duty;
	uint16_t duty_time_cnt;
	const uint16_t peroid;
	uint16_t peroid_cnt;
	uint8_t state;
	void (*pwmledon)();
	void (*pwmledoff)();
}PwmLed_t;




extern void PwmLed_SetDuty(Pwmlednum_ennum ledpos,uint16_t duty);
extern void PwmLed_1msMainfunction(void);

#endif

