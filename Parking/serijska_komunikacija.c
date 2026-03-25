#include "serijska_komunikacija.h"
#include "tipovi.h"
#include "at89c51rc2.h"
#include "cobs_protokol.h"

uint8_t* bafer_za_slanje;
uint8_t bafer_za_prijem[KAPACITET_BAFERA];
uint8_t indeks_upis = 0;
uint8_t kraj = 0;

void init_serijska(uint8_t PCON_reg, uint8_t SCON_reg, uint8_t BDRCON_reg, uint8_t BRL_reg) {
	PCON &= 0x3F;
	PCON |= PCON_reg & 0xC0;

	SCON &= 0x0F;
	SCON |= SCON_reg & 0xF0;

	BDRCON &= 0xE0;
	BDRCON |= BDRCON_reg & 0x1F;

	BRL = BRL_reg;
}

void omoguci_prekid_serijske(uint8_t omogucen_prekid) {
	ES = omogucen_prekid & 0x01;
}

void serijska_prekid(void) interrupt 4 {
	if (TI) {
		TI = 0;
		
		if (!kraj) {
			if (*(++bafer_za_slanje) == KRAJ_PORUKE) {
				kraj = 1;
			}
			
			SBUF = *bafer_za_slanje;
		}
	}
	else {
		uint8_t primljeni_bajt;

		RI = 0;

		primljeni_bajt = SBUF;

		bafer_za_prijem[indeks_upis] = primljeni_bajt;
		
		// overflow zastita
		indeks_upis = (indeks_upis + 1) % KAPACITET_BAFERA;
		
		if(primljeni_bajt == KRAJ_PORUKE)	// nista
		{
			bafer_za_prijem[indeks_upis] = '\0';
			indeks_upis = 0;
		}

//		if (primljeni_bajt == KRAJ_PORUKE) {
//			if (bafer_za_prijem[0] != KRAJ_PORUKE) {
//				parsiraj_poruku(bafer_za_prijem);
//			}
//			isprazni_bafer();
//		}
//		else {
//			indeks_upis = (indeks_upis + 1) % KAPACITET_BAFERA;
//		}
	}
}

void posalji_poruku(uint8_t* poruka, uint8_t duzina_poruke) {
	kraj = 0;
	bafer_za_slanje = kodiraj_poruku(poruka, duzina_poruke);
	SBUF = *bafer_za_slanje;
}

//void isprazni_bafer() {
//	uint8_t i = 0;
//	for (i = 0; i < KAPACITET_BAFERA; i++) {
//		bafer_za_prijem[i] = 0x00;
//	}

//	indeks_upis = 0;
//}