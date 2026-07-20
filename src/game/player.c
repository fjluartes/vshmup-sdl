/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../system/atlas.h"
#include "player.h"

extern App     app;
extern Entity *player;

static void doPlayer(Entity *self);
static void fireBullet(void);
static void die(Entity *self);

static AtlasImage *bulletTexture;

void initPlayer(void)
{
	Fighter *f;

	f = malloc(sizeof(Fighter));
	memset(f, 0, sizeof(Fighter));

	f->speed = 4.0;
	f->reloadRate = 20;

	player = spawnEntity(ET_PLAYER);
	player->texture = getAtlasImage("gfx/player.png", 1);
	player->x = (SCREEN_WIDTH - player->texture->rect.w) / 2;
	player->y = SCREEN_HEIGHT - 100;
	player->data = f;

	player->tick = doPlayer;
	player->die = die;

	bulletTexture = getAtlasImage("gfx/playerBullet.png", 1);
}

static void doPlayer(Entity *self)
{
	Fighter *f;

	f = (Fighter *)player->data;

	f->reload = MAX(f->reload - app.deltaTime, 0);
	f->invokeSidearm = 0;

	if (app.keyboard[SDL_SCANCODE_LEFT])
	{
		player->x -= f->speed * app.deltaTime;
	}

	if (app.keyboard[SDL_SCANCODE_RIGHT])
	{
		player->x += f->speed * app.deltaTime;
	}

	if (app.keyboard[SDL_SCANCODE_UP])
	{
		player->y -= f->speed * app.deltaTime;
	}

	if (app.keyboard[SDL_SCANCODE_DOWN])
	{
		player->y += f->speed * app.deltaTime;
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] && f->reload == 0)
	{
		fireBullet();

		f->reload = f->reloadRate;
		f->invokeSidearm = 1;
	}

	player->x = MIN(MAX(player->x, 0), SCREEN_WIDTH - player->texture->rect.w);
	player->y = MIN(MAX(player->y, 0), SCREEN_HEIGHT - player->texture->rect.h);
}

static void die(Entity *self)
{
	addExplosion(self->x + (self->texture->rect.w / 2),
		         self->y + (self->texture->rect.h / 2));
}

static void fireBullet(void)
{
	Bullet *b;

	b = spawnBullet(player);
	b->texture = bulletTexture;
	b->x = player->x + (player->texture->rect.w / 2) - (bulletTexture->rect.w / 2);
	b->y = player->y - bulletTexture->rect.h;
	b->dy = -15;
}
