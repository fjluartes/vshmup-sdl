/*
 * highscores.c
 */

#include "../common.h"

#include "highscores.h"

extern Game game;
extern Stage stage;

static int highscoreComparator(const void *a, const void *b);

void initHighscores(void)
{
    int i;

    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        memset(&game.highscores[i], 0, sizeof(Highscore));

        STRCPY(game.highscores[i].name, "ANON");
        game.highscores[i].score = 550 - (i * 50);
    }
}

void updateHighscores(void)
{
    game.highscores[NUM_HIGHSCORES - 1].score = stage.score;

    qsort(game.highscores, NUM_HIGHSCROES, sizeof(Highscore), highscoreComparator);
}

static int highscoreComparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore *)a);
    Highscore *h2 = ((Highscore *)b);

    return h2->score - h1->score;
}