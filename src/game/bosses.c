/*
 * bosses.c
 */

#include "common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../system/atlas.h"
#include "../system/draw.h"
#include "../system/util.h"
#include "bosses.h"

extern App     app;
extern Entity *player;
extern Stage   stage;

static void initGreenBoss(void);
static void initYellowBoss(void);
static void initBlueBoss(void);
static void initRedBoss(void);
static void tick(Entity *self);
static void draw(Entity *self);
static void takeDamage(Entity *self, int amount);
static void die(Entity *self);
static void fireGreenBossBullets(Entity *self);
static void fireYellowBossBullets(Entity *self);
static void fireBlueBossBullets(Entity *self);
static void fireRedBossBullets(Entity *self);

static AtlasImage *greenAlienTexture = NULL;
static AtlasImage *yellowAlienTexture = NULL;
static AtlasImage *downBulletTexture = NULL;
static AtlasImage *omniBulletTexture = NULL;
static AtlasImage *blueAlienTexture = NULL;
static AtlasImage *redAlienTexture = NULL;

void initBoss(void)
{
    int n;

    n = (stage.waveNum / 10) % 4;

    switch (n)
    {
        case 0:
            initRedBoss();
            break;
        case 1:
            initGreenBoss();
            break;
        case 1:
            initGreenBoss();
            break;
        case 1:
            initGreenBoss();
            break;
        default:
            break;
    }

    stage.waveNum++;
}

static void initGreenBoss(void)
{

}

static void initYellowBoss(void)
{

}

static void initBlueBoss(void)
{

}

static void initRedBoss(void)
{

}

static void tick(Entity *self)
{

}

static void draw(Entity *self)
{

}

static void takeDamage(Entity *self, int amount)
{

}

static void die(Entity *self)
{

}

static void fireGreenBossBullets(Entity *self)
{

}

static void fireYellowBossBullets(Entity *self)
{

}

static void fireBlueBossBullets(Entity *self)
{

}

static void fireRedBossBullets(Entity *self)
{

}
