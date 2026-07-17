/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 */

#include "../common.h"

#include "../system/draw.h"
#include "../system/util.h"
#include "bullets.h"

extern App     app;
extern Entity *player;
extern Stage   stage;

static void doAlienCollisions(Bullet *b);

void initBullets(void)
{
	memset(&stage.bulletHead, 0, sizeof(Bullet));
	stage.bulletTail = &stage.bulletHead;
}

Bullet *spawnBullet(Entity *owner)
{
	Bullet *b;

	b = malloc(sizeof(Bullet));
	memset(b, 0, sizeof(Bullet));
	stage.bulletTail->next = b;
	stage.bulletTail = b;

	b->owner = owner;

	return b;
}

void doBullets(void)
{
	Bullet *b, *prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx * app.deltaTime;
		b->y += b->dy * app.deltaTime;

		if (!collision(b->x, b->y, 
			           b->texture->rect.w, b->texture->rect.h, 
			           0, 0, SCREEN_WIDTH, SCREEN_HEIGHT))
		{
			b->dead = 1;
		}
		else if (b->owner == player)
		{
			doAlienCollisions(b);
		}

		if (b->dead)
		{
			prev->next = b->next;

			if (b == stage.bulletTail)
			{
				stage.bulletTail = prev;
			}

			free(b);

			b = prev;
		}

		prev = b;
	}
}

static void doAlienCollisions(Bullet *b)
{
	Entity *e;

	for (e = stage.entityHead.next; e != NULL; e = e->next)
	{
		if (e->type == ET_ALIEN && 
				collision(e->x, e->y, 
			              e->texture->rect.w, e->texture->rect.h, 
				          b->x, b->y, 
			              b->texture->rect.w, b->texture->rect.h))
		{
			e->health--;

			if (e->health == 0)
			{
				e->die(e);
			}

			b->dead = 1;
		}
	}
}

void drawBullets(void)
{
	Bullet *b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blitAtlasImage(b->texture, b->x, b->y, 0, SDL_FLIP_NONE);
	}
}
