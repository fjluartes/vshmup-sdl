/*
 * wave.c
 */

#include "../common.h"

#include "../game/swingingAlien.h"
#include "wave.h"

extern App   app;
extern Stage stage;

static void addSwingingAliens(void);
static void addSwoopingAliens(void);
static void addStraightAliens(void);

static unsigned long waveSeed;
static int           setupNextWave;
static double        waveStartTimer;

void initWave(void)
{
    waveSeed = 26244;

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

            switch (rand() % 3)
            {
                case 0:
                    addSwingingAliens();
                    break;
                case 1:
                    addSwoopingAliens();
                    break;
                case 2:
                    addStraightAliens();
                    break;
                default:
                    break;
            }

            waveSeed = rand() % 99999;

            setupNextWave = 0;
        }
    }
}

static void addSwingingAliens(void)
{
    int n, i, x, delay, sweepRange, dy;
    double swingAmount;

    n = 6 + rand() % 7;
    delay = 10 + rand() % 20;
    swingAmount = 1.0 * (5 + rand() % 6) * 0.01;
    sweepRange = 5 + rand() % 10;
    dy = 1 + rand() % 5;
    x = (SCREEN_WIDTH / 2);

    for (i = 0; i < n; i++)
    {
        initSwingingAlien(i * delay, x, swingAmount, sweepRange, dy);
    }

    stage.numWaveAliens = n;
}

static void addSwoopingAliens(void)
{
    int    i, n, delay, x;
    double dx, swoopAmount;

    n = 5 + rand() % 8;
    delay = 25;
    dx = (1.0 * (50 + rand() % 150)) * 0.01;
    swoopAmount = (1.0 * (100 + rand() % 100)) * 0.0001;
    x = -75;

    if (rand() % 2 == 0)
    {
        dx = -dx;
        x = SCREEN_WIDTH + 75;
    }

    for (i = 0; i < n; i++)
    {
        initSwoopingAlien(i * delay, x, dx, swoopAmount);
    }

    stage.numWaveAliens = n;
}

static void addStraightAliens(void)
{
    int    n, i, x, y, delay;
    double dx, dy;

    n = 10 + rand() % 6;
    delay = 10 + rand() % 20;
    dx = (1.0 * (100 + rand() % 300)) * 0.02;
    dy = (1.0 * (rand() % 30)) * 0.01;
    x = -75;
    y = (SCREEN_HEIGHT * 0.1) + (SCREEN_HEIGHT * 0.25);

    if (rand() % 2 == 0)
    {
        x = SCREEN_WIDTH + 75;
        dx = -dx;
    }

    if (rand() % 2 == 0)
    {
        dy = -dy;
    }

    for (i = 0; i < n; i++)
    {
        initStraightAlien(i * delay, x, y, dx, dy);
    }

    stage.numWaveAliens = n;
}
