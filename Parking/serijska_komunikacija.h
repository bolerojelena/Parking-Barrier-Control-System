#ifndef SERIJSKA_KOMUNIKACIJA_H
#define SERIJSKA_KOMUNIKACIJA_H

#include "tipovi.h"

#define KAPACITET_BAFERA 20
#define KRAJ_PORUKE 0x00

// ********************************* FLAG-OVI *********************************
// ****************************************************************************

extern uint8_t* bafer_za_slanje;
extern uint8_t bafer_za_prijem[KAPACITET_BAFERA];

void init_serijska(uint8_t PCON_reg, uint8_t SCON_reg, uint8_t BDRCON_reg, uint8_t BRL_reg);
void omoguci_prekid_serijske(uint8_t omogucen_prekid);
void serijska_prekid(void);
void posalji_poruku(uint8_t* poruka, uint8_t duzina_poruke);
//void isprazni_bafer();

#endif