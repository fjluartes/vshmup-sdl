/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 */

#include "../common.h"

#include "../system/draw.h"
#include "../system/text.h"
#include "hud.h"

#define BOSS_BAR_LENGTH (SCREEN_WIDTH - 140)

extern Game  game;
extern Stage stage;

static void drawScoreBar(void);
static void drawBossBar(void);

void drawHud(void)
{
	drawScoreBar();

	if (stage.boss != NULL)
	{
		drawBossBar();
	}
}

void drawScoreBar(void)
{
	char text[16];

	sprintf(text, "Score: %03d", stage.score);
	drawText(text, 10, 0, 255, 255, 255, TEXT_ALIGN_LEFT, 0);

	if (stage.score > game.highscores[0].score)
	{
		sprintf(text, "Highscore: %03d", stage.score);
		drawText(text, SCREEN_WIDTH - 10, 0, 128, 255, 128, TEXT_ALIGN_RIGHT, 0);
	}
	else
	{
		sprintf(text, "Highscore: %03d", game.highscores[0].score);
		drawText(text, SCREEN_WIDTH - 10, 0, 255, 255, 255, TEXT_ALIGN_RIGHT, 0);
	}
}

static void drawBossBar(void)
{
	double w;
	Boss  *b;

	b = (Boss *)stage.boss->data;

	w = (1.0 * stage.boss->health) / b->maxHealth;

	w *= BOSS_BAR_LENGTH;

	drawText("Boss", 10, SCREEN_HEIGHT - 60,
		255, 255, 255, TEXT_ALIGN_LEFT, 0);

	drawRect(125, SCREEN_HEIGHT - 40, w, 25, 255, 0, 0, 192);
	drawOutlineRect(125, SCREEN_HEIGHT - 40, BOSS_BAR_LENGTH,
		25, 255, 255, 25, 192);	
}
