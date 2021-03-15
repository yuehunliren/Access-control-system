#include "Re_all_config.h"
#include "drv\commDrv\Re_commDrv.h"
#if service_input_keypad
void keypad_init_hardware(Pin_input_mode index);
char keypad_readrow(char index);
void keypad_writecol(char index, char state);
#endif
