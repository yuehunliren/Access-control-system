#ifndef __RE_COMMDRV_H__
#define __RE_COMMDRV_H__

#include "Re_all_config.h"
#include "../Re_nativeInterface.h"
typedef enum
{
    Pullup,
    Pulldown
} Pin_input_mode;
void myDelayMS(uint64_t t);

void myDelayUS(uint64_t t);

uint32_t myGetMS();

void myCommonInit();

void myPrint(const char *);

#endif // __RE_COMMDRV_H__