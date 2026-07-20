/*
 * effects.c
 */
#include "../common.h"

#include "../system/atlas.h"
#include "../system/draw.h"
#include "effects.h"

#define NUM_SMALL_EXPLOSION_FRAMES 9
#define NUM_EXPLOSION_FRAMES       9

extern App app;

static Explosion explosionHead, *explosionTail;
static AtlasImage *smallExplosionTextures[NUM_SMALL_EXPLOSION_FRAMES];
static AtlasImage *explosionTextures[NUM_EXPLOSION_FRAMES];
static int hasTextures = 0;

void initEffects(void)
{
	int i;
	char filename[MAX_FILENAME_LENGTH];

	memset(&explosionHead, 0, sizeof(Explosion));
	explosionTail = &explosionHead;

	if (!hasTextures)
	{
		hasTextures = 1;

		for (i = 0; i < NUM_SMALL_EXPLOSION_FRAMES; i++)
		{
			sprintf(filename, "gfx/smallExplosion%02d.png", i + 1);

			smallExplosionTextures[i] = getAtlasImage(filename, 1);
		}

		for (i = 0; i < NUM_EXPLOSION_FRAMES; i++)
		{
			sprintf(filename, "gfx/explosion%02d.png", i + 1);

			explosionTextures[i] = getAtlasImage(filename, 1);
		}
	}
}

void doEffects(void)
{
	Explosion *e, *prev;

	prev = &explosionHead;

	for (e = explosionHead.next; e != NULL; e = e->next)
	{
		e->frameTime -= app.deltaTime;

		if (e->frameTime <= 0)
		{
			e->frame++;

			e->frameTime = e->frameSpeed;

			if (e->frame < e->numFrames)
			{
				e->texture++;
			}
			else
			{
				prev->next = e->next;

				if (e == explosionTail)
				{
					explosionTail = prev;
				}

				free(e);

				e = prev;
			}
		}

		prev = e;
	}
}

void drawEffects(void)
{
	Explosion *e;

	for (e = explosionHead.next; e != NULL; e = e->next)
	{
		blitAtlasImage(*e->texture, e->x, e->y, 1, SDL_FLIP_NONE);
	}
}

void addSmallExplosion(int x, int y)
{
	Explosion *e;

	e = malloc(sizeof(Explosion));
	memset(e, 0, sizeof(Explosion));
	explosionTail->next = e;
	explosionTail = e;

	e->x = x;
	e->y = y;
	e->numFrames = NUM_SMALL_EXPLOSION_FRAMES;
	e->frameSpeed = 1;
	e->frameTime = e->frameSpeed;
	e->texture = smallExplosionTextures;
}

void addExplosion(int x, int y)
{
	Explosion* e;

	e = malloc(sizeof(Explosion));
	memset(e, 0, sizeof(Explosion));
	explosionTail->next = e;
	explosionTail = e;

	e->x = x;
	e->y = y;
	e->numFrames = NUM_EXPLOSION_FRAMES;
	e->frameSpeed = 4;
	e->frameTime = e->frameSpeed;
	e->texture = smallExplosionTextures;
}

void clearEffects(void)
{
	Explosion *e;

	while (explosionHead.next)
	{
		e = explosionHead.next;

		explosionHead.next = e->next;

		free(e);
	}

	explosionTail = &explosionHead;
}
