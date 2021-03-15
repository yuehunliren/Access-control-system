#include "../Re_commDrv.h"

#if mcu_stm32f1_hal

void myDelayUS(uint64_t t)
{
  uint32_t startval,tickn,delays,wait;
 
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  delays =t * 72; 
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {
 
        }
      wait = 72000 + startval - delays;
      while(wait < SysTick->VAL)
        {
 
        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}

void myDelayMS(uint64_t t)
{
	uint32_t i;
	for(i=0;i<t;i++) myDelayUS(1000);
}


uint32_t myGetMS()
{
   return HAL_GetTick();
}

void myCommonInit()
{

}

void myPrint(const char *str)
{

}
#endif
