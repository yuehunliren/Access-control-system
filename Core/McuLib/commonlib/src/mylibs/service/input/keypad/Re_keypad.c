#include "Re_keypad.h"
#include "Re_keypadDrv.h"
#if service_input_keypad
#include "drv\commDrv\Re_commDrv.h"
Pin_input_mode configMode;
short keypad_cnt;
short keypad_cnt_trig;
static uint32_t cnt[36] = {0};
static flag[36] = {0};
void (*keypadCallback)(unsigned char x, unsigned char y);
void keypad_init(
    Pin_input_mode mode,
    short cnt_trig,
    void (*callback)(unsigned char x, unsigned char y))
{
    keypad_init_hardware(mode);
    configMode = mode;
    keypad_cnt = cnt_trig;
    keypadCallback = callback;
    if (configMode == Pullup)
    {
        for (char i = 0; i < 4; i++)
        {
            keypad_writecol(i, 1);
        }
    }
    else
    {
        for (char i = 0; i < 4; i++)
        {
            keypad_writecol(i, 0);
        }
    }
}
void keypad_scan()
{
    for (char x = 0; x < 4; x++)
    {
        if (configMode == Pullup)
        {
            keypad_writecol(x, 0);
            for (char y = 0; y < 4; y++)
            {
                if (!keypad_readrow(y) && (flag[4*x+y] == 0))
                {
                	cnt[4*x+y] = myGetMS();
                	flag[4*x+y] = 1;
                }
                 if(myGetMS() - cnt[4*x+y] >=  200 && flag[4*x+y] == 1)
               {
                	 if(!keypad_readrow(y))
                	{
                     keypadCallback(y, x);
                     flag[4*x+y] = 0;
                	}
               }
            }
            keypad_writecol(x, 1);  
        }
        else
        {
            keypad_writecol(x, 1);
            for (char y = 0; y < 4; y++)
            {
                if (!keypad_readrow(y))
                {
                    keypad_cnt = 0;
                }
                else
                {
                    if (keypad_cnt < 1000)
                    {
                        keypad_cnt++;
                    }
                    if (keypad_cnt == keypad_cnt_trig)
                    {
                        keypadCallback(x, y);
                    }
                }
            }
            keypad_writecol(x, 0);
        }
    }
}
#endif
