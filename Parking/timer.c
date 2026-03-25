#include "timer.h"
#include "at89c51rc2.h"
#include "serijska_komunikacija.h"

// deklaracija
uint8_t proslo_5s = 0;

uint16_t brojacPrekida = 0;

// debounce
// P0_0
uint8_t trenutnoHardverskoStanjeP0_0 = 1;
uint8_t prethodnoHardverskoStanjeP0_0 = 1;
uint8_t brojacStanjeP0_0 = 0;
uint8_t trenutnoSoftverskoStanjeP0_0 = 1;
// P0_1
uint8_t trenutnoHardverskoStanjeP0_1 = 1;
uint8_t prethodnoHardverskoStanjeP0_1 = 1;
uint8_t brojacStanjeP0_1 = 0;
uint8_t trenutnoSoftverskoStanjeP0_1 = 1;
// P0_2
uint8_t trenutnoHardverskoStanjeP0_2 = 1;
uint8_t prethodnoHardverskoStanjeP0_2 = 1;
uint8_t brojacStanjeP0_2 = 0;
uint8_t trenutnoSoftverskoStanjeP0_2 = 1;
// P0_3
uint8_t trenutnoHardverskoStanjeP0_3 = 1;
uint8_t prethodnoHardverskoStanjeP0_3 = 1;
uint8_t brojacStanjeP0_3 = 0;
uint8_t trenutnoSoftverskoStanjeP0_3 = 1;

void prekid_tajmera(void) interrupt 3
{
	if(cekaj_5s)	// samo tada treba da ulazi u prekid
	{
		if(++brojacPrekida == 25000)
		{
			proslo_5s = 1;
			brojacPrekida = 0;
		}
	}
	
	// P0_0
	trenutnoHardverskoStanjeP0_0 = P0_0;		
	if(trenutnoHardverskoStanjeP0_0 == prethodnoHardverskoStanjeP0_0)	
	{
	   brojacStanjeP0_0++;
	   if(brojacStanjeP0_0 == 5)
	   	trenutnoSoftverskoStanjeP0_0 = trenutnoHardverskoStanjeP0_0;
	}
	else
	   brojacStanjeP0_0 = 0; //nisu bila dva ista uzastopna stanja
	
	prethodnoHardverskoStanjeP0_0 = trenutnoHardverskoStanjeP0_0;
	
	// P0_1
	trenutnoHardverskoStanjeP0_1 = P0_1;		
	if(trenutnoHardverskoStanjeP0_1 == prethodnoHardverskoStanjeP0_1)	
	{
	   brojacStanjeP0_1++;
	   if(brojacStanjeP0_1 == 5)
	   	trenutnoSoftverskoStanjeP0_1 = trenutnoHardverskoStanjeP0_1;
	}
	else
	   brojacStanjeP0_1 = 0;
	
	prethodnoHardverskoStanjeP0_1 = trenutnoHardverskoStanjeP0_1;
	
	// P0_2
	trenutnoHardverskoStanjeP0_2 = P0_2;		
	if(trenutnoHardverskoStanjeP0_2 == prethodnoHardverskoStanjeP0_2)	
	{
	   brojacStanjeP0_2++;
	   if(brojacStanjeP0_2 == 5)
	   	trenutnoSoftverskoStanjeP0_2 = trenutnoHardverskoStanjeP0_2;
	}
	else
	   brojacStanjeP0_2 = 0;
	
	prethodnoHardverskoStanjeP0_2 = trenutnoHardverskoStanjeP0_2;
	
	// P0_3
	trenutnoHardverskoStanjeP0_3 = P0_3;		
	if(trenutnoHardverskoStanjeP0_3 == prethodnoHardverskoStanjeP0_3)	
	{
	   brojacStanjeP0_3++;
	   if(brojacStanjeP0_3 == 5)
	   	trenutnoSoftverskoStanjeP0_3 = trenutnoHardverskoStanjeP0_3;
	}
	else
	   brojacStanjeP0_3 = 0;
	
	prethodnoHardverskoStanjeP0_3 = trenutnoHardverskoStanjeP0_3;
	
}

void init_timer(uint8_t tmod, uint8_t tl, uint8_t th)
{
	TH1 = th;
	TL1 = tl;
	
	TMOD &= 0x2F;
	TMOD |= tmod;
}

void start_timer(void)
{
	TR1 = 1;
}

//void stop_timer(void)
//{
//	TR1 = 0;
//}

//void rst_timer(uint8_t tl, uint8_t th)
//{
//	TH1 = th;
//	TL1 = tl;
//}

void omoguci_prekid_tajmera()
{
	ET1 = 1;
}

//void onemoguci_prekid_tajmera()
//{
//	ET1 = 0;
//}