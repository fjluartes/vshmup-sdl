/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 * 2026-07-24 fjluartes
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/highscores.h"
#include "../game/hud.h"
#include "../game/player.h"
#include "../game/stars.h"
#include "../game/supplyShip.h"
#include "../game/wave.h"
#include "../system/draw.h"
#include "../system/textures.h"
#include "stage.h"

#define SUPPLY_SHIP_INTERVAL (FPS * 30)

extern App     app;
extern Entity *player;
extern Stage   stage;

static void logic(void);
static void draw(void);
static void drawBackground(void);
static void resetStage(void);

static SDL_Texture *background;
static double       backgroundY;
static double       gameOverTimer;
static double       nextSupplyShipTimer;

void initStage(void)
{
	memset(&stage, 0, sizeof(Stage));

	initEntities();

	initPlayer();

	initStars();

	initBullets();

	initEffects();

	initWave();

	background = loadTexture("gfx/background.jpg");

	backgroundY = -SCREEN_HEIGHT;

	gameOverTimer = FPS * 2;

	nextSupplyShipTimer = SUPPLY_SHIP_INTERVAL / 2;

	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void logic(void)
{
	stage.hasAliens = 0;

	backgroundY += app.deltaTime;

	if (backgroundY >= 0)
	{
		backgroundY = -SCREEN_HEIGHT;
	}

	nextSupplyShipTimer -= app.deltaTime;

	if (nextSupplyShipTimer <= 0)
	{
		initSupplyShip();

		nextSupplyShipTimer = SUPPLY_SHIP_INTERVAL;
	}

	doStars();

	doEntities();

	doBullets();

	doEffects();

	if (player->health <= 0)
	{
		gameOverTimer -= app.deltaTime;

		if (gameOverTimer <= 0)
		{
			updateHighscores();

			resetStage();

			initStage();
		}
	}
	else if (!stage.hasAliens)
	{
		clearDeadEntities();

		nextWave();
	}
}

static void draw(void)
{
	drawBackground();

	drawStars();

	drawEntities();

	drawBullets();

	drawEffects();

	drawHud();
}

static void drawBackground(void)
{
	double y;

	for (y = backgroundY; y < SCREEN_HEIGHT; y += SCREEN_HEIGHT)
	{
		blit(background, 0, y, 0, SDL_FLIP_NONE);
	}
}

static void resetStage(void)
{
	clearEntities();

	clearDeadEntities();

	clearBullets();

	clearEffects();
}
