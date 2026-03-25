#include "poruka.h"
#include "serijska_komunikacija.h"

#include "cobs_protokol.h"

//uint8_t odgovor_OK[] = {0x01};
//uint8_t odgovor_GRESKA[] = {0x00};

uint8_t* kodirana_poruka; // maks duzina = 4 <- 2 cifre + begin i end
uint8_t* poruka; // maks 2 cifre + null terminator 0x00

void posalji_broj_automobila(uint16_t broj_automobila)
{
	uint16_t duzina_poruke = 0;
	uint16_t duzina_kodirane_poruke = 0;
	
//	if (broj_automobila > 15) // realno ne treba
//	{
//		posalji_poruku(odgovor_GRESKA, 1);
//  }

  if (broj_automobila < 10) 
	{
		poruka[0] = '0' + broj_automobila;
    duzina_poruke = 1;
  }
	else 
	{
		poruka[0] = '0' + (broj_automobila / 10);	// posto je BINARNO
    poruka[1] = '0' + (broj_automobila % 10);
		duzina_poruke = 2;
  }
	
  kodirana_poruka = kodiraj_poruku(poruka, duzina_poruke);	// duzina_kodirane = duzina_poruke + 2 karaktera (begin i end)
	duzina_kodirane_poruke = duzina_poruke + 2;
	
  posalji_poruku(kodirana_poruka, duzina_kodirane_poruke);
	posalji_poruku(odgovor_OK, 1);
	
}