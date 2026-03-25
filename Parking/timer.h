#ifndef TIMER_H
#define TIMER_H

#include "tipovi.h"

//========= TAJMER 1 AUTO-RELOAD MOD =========

//extern uint8_t ulazi;
//extern uint8_t izlazi;
extern uint8_t cekaj_5s;
extern uint16_t brojacPrekida;

// debounce
extern uint8_t trenutnoSoftverskoStanjeP0_0;
extern uint8_t trenutnoSoftverskoStanjeP0_1;
extern uint8_t trenutnoSoftverskoStanjeP0_2;
extern uint8_t trenutnoSoftverskoStanjeP0_3;

// incijaliizacija
extern uint8_t proslo_5s;

void prekid_tajmera(void);
void init_timer(uint8_t tmod, uint8_t tl, uint8_t th);

void start_timer(void);
//void stop_timer(void);
//void rst_timer(uint8_t tl, uint8_t th);

void omoguci_prekid_tajmera();
//void onemoguci_prekid_tajmera();

#endif