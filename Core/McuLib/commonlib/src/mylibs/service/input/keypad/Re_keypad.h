#ifndef __KEYPAD_H__
#define __KEYPAD_H__
#if service_input_keypad
#include "drv\commDrv\Re_commDrv.h"
void keypad_init(
    Pin_input_mode mode,
    short cnt_trig,
    void (*callback)(unsigned char x, unsigned char y));
void keypad_scan();
#endif
#endif
