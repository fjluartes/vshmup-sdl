/*
 * wave.c
 */
#include "../common.h";

#include "../game/swingingAlien.h"
#include "wave.h"

extern App app;

static void addSwingingAliens(void);

static unsigned long waveSeed;
static int           setupNextWave;
static double        waveStartTimer;

void initWave(void)
{
    waveSeed = 77191;

    setupNextWave = 0;

    nextWave();
}

void nextWave(void)
{
    if (!setupNextWave)
    {
        setupNextWave = 1;

        waveStartTimer = FPS;
    }
    else
    {
        waveStartTimer = MAX(waveStartTimer - app.deltaTime, 0);

        if (waveStartTimer <= 0)
        {
            srand(waveSeed);

            addSwingingAliens();

            waveSeed = rand() % 99999;

            setupNextWave = 0;
        }
    }
}

static void addSwingingAliens(void)
{
    int n, i, delay;

    n = 6 + rand() % 7;
    delay = 10 + rand() % 20;

    for (i = 0; i < n; i++)
    {
        initSwingingAlien(i * delay);
    }
}
