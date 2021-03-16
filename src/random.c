#include "random.h"

float random()
{
	return (float)rand() / (float)RAND_MAX;
}



int random_int_range(int min, int max)
{
	return min + (int)(rand() / (float)(RAND_MAX + 1) * (max - min + 1));
}