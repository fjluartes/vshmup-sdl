/*
 * swingingAlien.c
 */
#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/pointsPod.h"
#include "../system/atlas.h"
#include "../system/util.h"
#include "swingingAlien.h"

extern App   app;
extern Stage stage;

static void tick(Entity *self);
static void die(Entity *self);
static void fireBullet(Entity *self);

static AtlasImage *littleYellowAlienTexture = NULL;
static AtlasImage *bulletTexture = NULL;

void initSwingingAlien(int startDelay, int x, double swingAmount, int sweepRange, int dy)
{
    Entity *e;
    SwingingAlien *s;

    s = malloc(sizeof(SwingingAlien));
    memset(s, 0, sizeof(SwingingAlien));

    s->startDelay = startDelay;
    s->reload = rand() % (int)FPS;
    s->swingAmount = swingAmount;
    s->sweepRange = sweepRange;
    s->dy = dy;

    if (littleYellowAlienTexture == NULL)
    {
        littleYellowAlienTexture = getAtlasImage("gfx/littleYellowAlien.png", 1);
        bulletTexture = getAtlasImage("gfx/alienDownBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = littleYellowAlienTexture;
    e->data = s;

    e->x = (SCREEN_WIDTH - e->texture->rect.w) / 2;
    e->y = -150;

    e->tick = tick;
    e->die = die;
}

static void tick(Entity* self)
{
    SwingingAlien* s;

    s = (SwingingAlien*)self->data;

    s->startDelay -= app.deltaTime;

    if (s->startDelay <= 0)
    {
        s->swing += s->swingAmount * app.deltaTime;

        self->x += cos(s->swing) * (s->sweepRange * app.deltaTime);
        self->y += s->dy * app.deltaTime;
    }

    if (self->y >= SCREEN_HEIGHT)
    {
        self->health = 0;
    }

    s->reload = MAX(s->reload - app.deltaTime, 0);

    if (s->reload == 0)
    {
        if (rand() % 5 == 0)
        {
            fireBullet(self);
        }
    }

    stage.numAliens++;
}

static void die(Entity *self)
{
    addExplosion(self->x + (self->texture->rect.w / 2),
                 self->y + (self->texture->rect.h / 2));

    if (--stage.numAliens == 0)
    {
        addPointsPod(self->x, self->y);
    }
}

static void fireBullet(Entity* self)
{
    Bullet *b;

    b = spawnBullet(self);
    b->texture = bulletTexture;
    b->x = self->x + (self->texture->rect.w / 2) - (bulletTexture->rect.w / 2);
    b->y = self->y - self->texture->rect.h;
    b->dy = 10;
}