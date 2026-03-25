/* Implementirati program za rad parking rampe. 

Rampa se sastoji od motora koji moze da se okrece u dva smera (podizanje i spustanje rampe) 
koji se kontrolise pomocu dva digitalna izlaza. 

Na samoj rampi se nalaze dva granicna prekidaca koji govore o krajnjim polozajima rampe (rampa spustena, rampa podignuta). 
Sistem takode poseduje i dva potpatosna prekidaca koja govore o tome da li automobil hoce da ude na parking ili izade. 

Parking može da primi maksimalno 15 automobila. 

Algoritam rada je sledeci: kada automobil pride rampi, aktivan je potpatosni prekidac na ulazu, rampa se podiže. 
Kada rampa stigne do kraja, ostaje u tom položaju 5 sec. da bi kola mogla nesmetano da produ ispod rampe. 
Posle 5sec rampa se spusta i ostaje spuštena dok se ne pojave vozila na ulazu ili izlazu. 

Stanje o zauzetosti parking mesta prikazati na displeju i slati serijskom komunikacijom na PC. */


#include "at89c51rc2.h"
#include "tipovi.h"
#include "gpio.h"

#include "timer.h"
#include "prekidi.h"
#include "serijska_komunikacija.h"

#include "display.h"

#include "poruka.h"

#define MAX_AUTOMOBILA 15

uint16_t broj_automobila = 0;

uint8_t ulazi = 0;
uint8_t izlazi = 0;
//========================== TIMER.H ==========================
uint8_t cekaj_5s = 0;
//====================//==================//===================
uint8_t proslo_5s_spusti = 0;

void main(void)
{
	uint8_t stanjeTasteraP0_0 = 1;
	uint8_t predhodnoStanjeTasteraP0_0 = 1;
	uint8_t stanjeTasteraP0_1 = 1;
	uint8_t predhodnoStanjeTasteraP0_1 = 1;
	uint8_t stanjeTasteraP0_2 = 1;
	uint8_t predhodnoStanjeTasteraP0_2 = 1;
	uint8_t stanjeTasteraP0_3 = 1;
	uint8_t predhodnoStanjeTasteraP0_3 = 1;
	
	uint16_t motor = 0;
	
	uint8_t granicni_prekidac_podignuta = 0;
	uint8_t granicni_prekidac_spustena = 0;
	uint8_t potpatosni_prekidac_ulazi = 0;
	uint8_t potpatosni_prekidac_izlazi = 0;
	
	uint8_t tmod = 0x20;
	//========================== INICIJALIZACIJA ==========================
	init_gpio();
	
	init_timer(tmod, 0x00, 0x47);
	omoguci_prekid_tajmera();
	omoguci_sve_prekide();
	start_timer();

	CKCON0 &= 0xFF;
	init_serijska(0x80, 0x50, 0x1C, 253);
	omoguci_prekid_serijske(1);
	
	initDisplay();
	clearDisplay();
	
	// samo da bi odmah ispisao - treba proveriti da li radi ********
//	clearDisplay();
//	writeLine("Paking: ");
//	newLine();
//	writeChar((broj_automobila / 10) + '0');
//	writeChar((broj_automobila % 10) + '0');
//	posalji_broj_automobila(broj_automobila);
		
	while(1)
	{
		//================= OCITAVANJE ULAZA - TASTERA =====================
		// debounce
		stanjeTasteraP0_0 = trenutnoSoftverskoStanjeP0_0;
		stanjeTasteraP0_1 = trenutnoSoftverskoStanjeP0_1;
		stanjeTasteraP0_2 = trenutnoSoftverskoStanjeP0_2;
		stanjeTasteraP0_3 = trenutnoSoftverskoStanjeP0_3;
		
		if(stanjeTasteraP0_0 < predhodnoStanjeTasteraP0_0)
		{
			granicni_prekidac_podignuta = granicni_prekidac_podignuta ? 0 : 1;
		}
		if(stanjeTasteraP0_1 < predhodnoStanjeTasteraP0_1)
		{
			granicni_prekidac_spustena = granicni_prekidac_spustena ? 0 : 1;
		}
		if(stanjeTasteraP0_2 < predhodnoStanjeTasteraP0_2)
		{
			potpatosni_prekidac_ulazi = potpatosni_prekidac_ulazi ? 0 : 1;
		}
		if(stanjeTasteraP0_3 < predhodnoStanjeTasteraP0_3)
		{
			potpatosni_prekidac_izlazi = potpatosni_prekidac_izlazi ? 0 : 1;
		}
		
		//========================== LOGIKA ==========================
		/* Algoritam rada je sledeci: kada automobil pride rampi, aktivan je potpatosni prekidac na ulazu, rampa se podiže. 
		Kada rampa stigne do kraja, ostaje u tom položaju 5 sec. da bi kola mogla nesmetano da produ ispod rampe. 
		Posle 5sec rampa se spusta i ostaje spuštena dok se ne pojave vozila na ulazu ili izlazu. */
		
		if(granicni_prekidac_podignuta)			// zastita prekidaca rampe
			granicni_prekidac_spustena = 0;
		if(granicni_prekidac_spustena)
			granicni_prekidac_podignuta = 0;
		
		if(motor == 1)	// zastita uslova rada motora
		{
			granicni_prekidac_spustena = 0;
		}
		if(motor == 2)
		{
			granicni_prekidac_podignuta = 0;
		}
		
		//====================== automatsko spustanje rampe =======================
		if(proslo_5s_spusti)
		{
			if(!granicni_prekidac_spustena)
			{
				motor = 2;
			}
			else
			{
				motor = 0;
				proslo_5s_spusti = 0;
				
				if(potpatosni_prekidac_ulazi)
				{
					potpatosni_prekidac_ulazi = 0;
					broj_automobila++;
				}
				if(potpatosni_prekidac_izlazi)
				{
					potpatosni_prekidac_izlazi = 0;
					broj_automobila--;
				}
				
				// ako je doslo do promene, ispisuje se izmena stanja
				// rst prosle akcijje
				potpatosni_prekidac_ulazi = 0;
				potpatosni_prekidac_izlazi = 0;
				
				//========================== DISPLAY I SK ==========================
				clearDisplay();
				
				writeLine("Paking: ");
				newLine();
				writeChar((broj_automobila / 10) + '0');
				writeChar((broj_automobila % 10) + '0');
				
				posalji_broj_automobila(broj_automobila);	// OVO PROVERITI - COBS
			}
		}
		//==========================//==========================//==============
		
		//========================== setovanje akcije ==========================
		if(potpatosni_prekidac_ulazi && !proslo_5s_spusti)
		{
			if(broj_automobila < MAX_AUTOMOBILA)
				ulazi = 1;
		}	// kraj ppu
		else
			ulazi = 0;
		
		if(potpatosni_prekidac_izlazi && !proslo_5s_spusti)
		{
			if(broj_automobila > 0)
				izlazi = 1;
		} // kraj ppi
		else
			izlazi = 0;
		
		//================= DOZVOLJENE AKCIJE ULAZI/IZLAZI =====================
		if((ulazi && !izlazi) || (izlazi && !ulazi))
		{
			if(!granicni_prekidac_podignuta)
			{
				motor = 1;
			}
			else
			{
				motor = 0;
				cekaj_5s = 1;	// pusti timer
				
				if(proslo_5s)
				{
					proslo_5s_spusti = 1;
					proslo_5s = 0;
					cekaj_5s = 0;
					
					if(ulazi)
						ulazi = 0;
					if(izlazi)
						izlazi = 0;
				}
			}
		}
		//======================= NEDOZVOLJENE AKCIJE =========================
		else	
		{	//	if(potpatosni_prekidac_ulazi && potpatosni_prekidac_izlazi || ulazi && izlazi)
			if(!proslo_5s_spusti)
			{
				cekaj_5s = 0;	// + rst brojaca
				brojacPrekida = 0;
				
				//motor = 0;		// + ako je u akciji
				if(motor != 0 || granicni_prekidac_podignuta)	// - akcije: motor_radi || motor_stoji__gore = rampa_podignuta
				{
					if(!granicni_prekidac_spustena)
						motor = 2;
					else
					{
						motor = 0;
					}
	  		}
 				else	// motor = 0;
				{
					granicni_prekidac_spustena = 1;
					granicni_prekidac_podignuta = 0;
				}
			}
		}
		//==========================//==========================//=================
		
		//========================== "Osvezavanje ulaza" ==========================
		predhodnoStanjeTasteraP0_0 = stanjeTasteraP0_0;
		predhodnoStanjeTasteraP0_1 = stanjeTasteraP0_1;
		predhodnoStanjeTasteraP0_2 = stanjeTasteraP0_2;
		predhodnoStanjeTasteraP0_3 = stanjeTasteraP0_3;
		
		//========================== OSVEZAVANJE IZLAZA ==========================
		if(motor == 0)	// motor je zaustavljen
		{
			P2_0 = 0;
			P2_1 = 0;
		}
		else if(motor == 1)	// motor podize rampu
		{
			P2_0 = 1;
			P2_1 = 0;
		}
		else if(motor == 2)	// motor spusta rampu
		{
			P2_0 = 0;
			P2_1 = 1;
		}
		
		if(cekaj_5s)	// indikatori
			P2_2 = 1;
		else
			P2_2 = 0;
		
		if(granicni_prekidac_podignuta)
			P2_4 = 1;
		else
			P2_4 = 0;
		if(granicni_prekidac_spustena)
			P2_6 = 1;
		else
			P2_6 = 0;
		if(potpatosni_prekidac_ulazi)
			P2_3 = 1;
		else
			P2_3 = 0;
		if(potpatosni_prekidac_izlazi)
			P2_7 = 1;
		else
			P2_7 = 0;
		//==========================//==========================//=================
	}
}
