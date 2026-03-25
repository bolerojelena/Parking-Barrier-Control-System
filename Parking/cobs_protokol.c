#include "cobs_protokol.h"
#include "serijska_komunikacija.h"
//#include "main.h"

uint8_t odgovor_OK[] = {0x01};
uint8_t odgovor_GRESKA[] = {0x00};

uint8_t* kodiraj_poruku(uint8_t* poruka, uint8_t duzina_poruke) {
	uint8_t i, indeks_za_upis_nule = 0;
	uint8_t kodirana_poruka[MAKS_DUZINA_PORUKE];

	for(i = 0; i < duzina_poruke; i++){
		// Ukoliko je trenutni bajt 0, na mesto prethodne 0 se upisuje broj bajtova izmedju prethodne i trenutne 0 
		if(poruka[i] == 0){
			kodirana_poruka[indeks_za_upis_nule] = (i + 1) - indeks_za_upis_nule;
			indeks_za_upis_nule = i + 1;
		}
		else {
			kodirana_poruka[i + 1] = poruka[i];
		}
	}
	kodirana_poruka[indeks_za_upis_nule] = (duzina_poruke + 1) - indeks_za_upis_nule;
	kodirana_poruka[duzina_poruke+1] = 0;

	return kodirana_poruka;
}

/* bez komandi
uint8_t* dekodiraj_poruku(uint8_t* kodirana_poruka) {
	uint8_t dekodirana_poruka[MAKS_DUZINA_PORUKE];
	uint8_t duzina_poruke = 0, i = 0, j = 0, pozicija_nule;

	while (kodirana_poruka[i] != KRAJ_PORUKE) {
		duzina_poruke++;
		i++;
	}
	duzina_poruke++;

	pozicija_nule = kodirana_poruka[0];
	for (i = 1; i < duzina_poruke; i++) {
		if (i == pozicija_nule) {
			dekodirana_poruka[j++] = 0x00;
			pozicija_nule = i + kodirana_poruka[i];
		}
		else {
			dekodirana_poruka[j++] = kodirana_poruka[i];
		}
	}

	return dekodirana_poruka;
} */


/*
void parsiraj_poruku(uint8_t* poruka) {
	uint8_t* dekodirana_poruka = dekodiraj_poruku(poruka);
	
	if (dekodirana_poruka[0] == 0x01 && dekodirana_poruka[1] == 0x00) {	// 0x01 0x00 -> stopiran
		start_flag = 0;
		posalji_poruku(odgovor_OK, 1);
	}
	else if (dekodirana_poruka[0] == 0x01 && dekodirana_poruka[1] == 0x01) {	// 0x01 0x01 -> startovan
		start_flag = 1;
		posalji_poruku(odgovor_OK, 1);
	}
	else {
		posalji_poruku(odgovor_GRESKA, 1);
	}
} */