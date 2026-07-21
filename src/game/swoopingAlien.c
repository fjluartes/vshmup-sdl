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

}
