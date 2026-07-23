/*
 * swoopingAlien.c
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/pointsPod.h"
#include "../system/atlas.h"
#include "../system/atlas.h"
#include "../system/atlas.h"
#include "swoopingAlien.h"

extern App     app;
extern Entity *player;
extern Stage   stage;

static void tick(Entity *self);
static void die(Entity *self);
static void fireBullet(Entity *self);
static void draw(Entity *self);
static void takeDamage(Entity *self, int amount);

static AtlasImage *littleBlueAlienTexture = NULL;
static AtlasImage *bulletTexture = NULL;

void initSwoopingAlien(int startDelay, int x, double dx, double swoopAmount)
{
    Entity        *e;
    SwoopingAlien *s;

    s = malloc(sizeof(SwoopingAlien));
    memset(s, 0, sizeof(SwoopingAlien));

    s->startDelay = startDelay;
    s->swoopAmount = swoopAmount;
    s->dx = dx;

    if (littleBlueAlienTexture == NULL)
    {
        littleBlueAlienTexture = getAtlasImage("gfx/littleBlueAlien.png", 1);
        bulletTexture = getAtlasImage("gfx/alienDownBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = littleBlueAlienTexture;
    e->data = s;

    e->x = x - (e->texture->rect.w / 2);
    e->y = 0;
    e->health = 2;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void tick(Entity *self)
{
    SwoopingAlien *s;

    s = (SwoopingAlien*)self->data;

    s->startDelay -= app.deltaTime;

    if (s->startDelay <= 0)
    {
        s->swoop += s->swoopAmount * app.deltaTime;
    }
}

static void die(Entity *self)
{
    
}

static void fireBullet(Entity *self)
{

}

static void draw(Entity *self)
{

}

static void takeDamage(Entity *self, int amount)
{

}
