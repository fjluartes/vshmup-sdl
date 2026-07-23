/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 * 2026-07-23 fjluartes
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/hud.h"
#include "../game/player.h"
#include "../game/powerUpPod.h"
#include "../game/stars.h"
#include "../game/wave.h"
#include "../system/draw.h"
#include "../system/textures.h"
#include "stage.h"

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

	addPowerUpPod(SCREEN_WIDTH / 2, -50, PP_SIDEARM);

	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void logic(void)
{
	stage.numAliens = 0;

	backgroundY += app.deltaTime;

	if (backgroundY >= 0)
	{
		backgroundY = -SCREEN_HEIGHT;
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
			resetStage();

			initStage();
		}
	}
	else if (stage.numAliens == 0)
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
