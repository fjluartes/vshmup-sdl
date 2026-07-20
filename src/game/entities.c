/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 */

#include "../common.h"

#include "../system/draw.h"
#include "entities.h"

extern Stage stage;

static Entity deadHead, *deadTail;

void initEntities(void)
{
	memset(&stage.entityHead, 0, sizeof(Entity));
	stage.entityTail = &stage.entityHead;

	memset(&deadHead, 0, sizeof(Entity));
	deadTail = &deadHead;
}

Entity *spawnEntity(int type)
{
	Entity *e;

	e = malloc(sizeof(Entity));
	memset(e, 0, sizeof(Entity));
	stage.entityTail->next = e;
	stage.entityTail = e;

	e->type = type;
	e->health = 1;

	return e;
}

void doEntities(void)
{
	Entity *e, *prev;

	prev = &stage.entityHead;

	for (e = stage.entityHead.next; e != NULL; e = e->next)
	{
		if (e->tick != NULL)
		{
			e->tick(e);
		}

		if (e->health <= 0)
		{
			if (e == stage.entityTail)
			{
				stage.entityTail = prev;
			}

			prev->next = e->next;

			// move to dead queue
			deadTail->next = e;
			deadTail = e;
			deadTail->next = NULL;

			e = prev;
		}

		prev = e;
	}
}

void drawEntities(void)
{
	Entity *e;

	for (e = stage.entityHead.next; e != NULL; e = e->next)
	{
		blitAtlasImage(e->texture, e->x, e->y, 0, SDL_FLIP_NONE);
	}
}

void clearEntities(void)
{
	Entity *e;

	while (stage.entityHead.next)
	{
		e = stage.entityHead.next;

		stage.entityHead.next = e->next;

		if (e->data != NULL)
		{
			free(e->data);
		}

		free(e);
	}

	stage.entityTail = &stage.entityHead;
}

void clearDeadEntities(void)
{
	Entity *e;

	while (deadHead.next)
	{
		e = deadHead.next;

		deadHead.next = e->next;

		if (e->data != NULL)
		{
			free(e->data);
		}

		free(e);
	}

	deadTail = &deadHead;
}
