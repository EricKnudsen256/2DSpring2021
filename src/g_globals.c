#include "simple_logger.h"

#include "g_globals.h"


void globals_init()
{
	totalKills = 0;


	highScoreList = (int *)gfc_allocate_array(sizeof (int), 100);


	globals_load_scores();


	atexit(globals_save_scores);
}

void globals_save_scores()
{
	SJson *json, *array, *score, *jsonOld;


	json = sj_object_new();

	jsonOld = sj_load("high_scores.save");

	if (jsonOld)
	{
		array = sj_object_get_value(jsonOld, "scores");

		score = sj_new_int(totalKills);

		sj_array_append(array, score);


		sj_object_insert(json, "scores", array);

		sj_save(json, "high_scores.save");
	}
	else
	{
		array = sj_array_new();

		score = sj_new_int(totalKills);

		sj_array_append(array, score);

		sj_object_insert(json, "scores", array);

		sj_save(json, "high_scores.save");
	}
	






	sj_free(json);

	sj_array_free(array);



}

SJson *globals_load_scores()
{
	SJson *json, *scores;
	int i, j, count;

	json = sj_load("high_scores.save");

	if (!json)return;


	

	scores = sj_object_get_value(json, "scores");
	count = sj_array_get_count(scores);

	for (i = 0; i < count; i++)
	{
		
		sj_get_integer_value(sj_array_get_nth(scores, i), &highScoreList[i]);

		slog("%i", highScoreList[i]);
	}


	//sorting shit

	for (i = 0; i < count; i++)
	{
		//slog("%i", highScoreList[i]);
		for (j = 0; j < count; j++)
		{
			if (highScoreList[j] < highScoreList[i])
			{
				int tmp = highScoreList[i];
				highScoreList[i] = highScoreList[j];
				highScoreList[j] = tmp;
			}
		}
	}


	return json;

}
