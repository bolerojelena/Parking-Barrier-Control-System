#include "prekidi.h"
#include "at89c51rc2.h"

void omoguci_sve_prekide(void)
{
	EA = 1;
}