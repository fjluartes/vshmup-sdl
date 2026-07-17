/*
 * swingingAlien.c
 */
#include "common.h"

#include "../game/entities.h";
#include "../game/pointsPod.h";
#include "../system/atlas.h";
#include "swingingAlien.h"

extern App   app;
extern Stage stage;

static void tick(Entity *self);
static void die(Entity *self);

static AtlasImage *littleYellowAlienTexture = NULL;

void initSwingingAlien(int startDelay)
{
    Entity *e;
    SwingingAlien *s;

    s = malloc(sizeof(SwingingAlien));
    memset(s, 0, sizeof(SwingingAlien));

    s->startDelay = startDelay;

    if (littleYellowAlienTexture == NULL)
    {
        littleYellowAlienTexture = getAtlasImage("gfx/littleYellowAlien.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = littleYellowAlienTexture;
    e->data = s;

    e->x = (SCREEN_WIDTH - e->texture.rect.w) / 2;
    e->y = -150;

    e->tick = tick;
    e->die = die;
}

static void tick(Entity *self)
{
    SwingingAlien *s;

    s = (SwingingAlien *)self->data;

    s->startDelay -= app.deltaTime;

    if (s->startDelay <= 0)
    {
        s->swing += 0.05 * app.deltaTime;

        self->x += cos(s->swing) * (8 * app.deltaTime);
        self->y += 2 * app.deltaTime;
    }

    if (self->y >= SCREEN_HEIGHT)
    {
        self->health = 0;
    }

    stage.hasAliens = 1;
}

static void die(Entity *self)
{
    addPointsPod(self->x, self->y);
}