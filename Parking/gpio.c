#include "gpio.h"
#include "at89c51rc2.h"

void init_gpio(void)
{
	P0 = 0xFF;
	P2 = 0x00;
}