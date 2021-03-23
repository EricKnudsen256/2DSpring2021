#include "random.h"

void init_random()
{

	srand((unsigned)time(0));
}

float random()
{
	return (float)rand() / (float)RAND_MAX;
}



int random_int_range(int min, int max)
{
	return rand() % (min - max + 1) + min;
}