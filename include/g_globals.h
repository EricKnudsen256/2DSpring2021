#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "simple_json.h"


int totalKills;
int *highScoreList;


void globals_init();

void globals_save_scores();

SJson *globals_load_scores();

#endif
