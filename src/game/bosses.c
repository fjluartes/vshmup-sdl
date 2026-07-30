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
static Bullet *spawnBossBullet(Entity *self, SDL_Texture *texture, 
            float dx, float dy, int xOffset);
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
    Boss   *b;
    Entity *e;

    b = malloc(sizeof(Boss));
    memset(b, 0, sizeof(Boss));
    b->maxHealth = 100;
    b->fireBullets = fireGreenBossBullets;

    if (greenAlienTexture == NULL)
    {
        greenAlienTexture = getAtlasImage("gfx/greenAlien.png", 1);
        downBulletTexture = getAtlasImage("gfx/alienDownBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = greenAlienTexture;
    e->x = (SCREEN_WIDTH - e->texture->rect.w) / 2;
    e->y = -250;
    e->health = b->maxHealth;
    e->data = b;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void initYellowBoss(void)
{
    Boss   *b;
    Entity *e;

    b = malloc(sizeof(Boss));
    memset(b, 0, sizeof(Boss));
    b->maxHealth = 125;
    b->fireBullets = fireYellowBossBullets;

    if (yellowAlienTexture == NULL)
    {
        yellowAlienTexture = getAtlasImage("gfx/yellowAlien.png", 1);
        omniBulletTexture = getAtlasImage("gfx/alienBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = yellowAlienTexture;
    e->x = (SCREEN_WIDTH - e->texture->rect.w) / 2;
    e->y = -250;
    e->health = b->maxHealth;
    e->data = b;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void initBlueBoss(void)
{
    Boss   *b;
    Entity *e;

    b = malloc(sizeof(Boss));
    memset(b, 0, sizeof(Boss));
    b->maxHealth = 150;
    b->fireBullets = fireBlueBossBullets;

    if (blueAlienTexture == NULL)
    {
        blueAlienTexture = getAtlasImage("gfx/blueAlien.png", 1);
        omniBulletTexture = getAtlasImage("gfx/alienBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = blueAlienTexture;
    e->x = (SCREEN_WIDTH - e->texture->rect.w) / 2;
    e->y = -250;
    e->health = b->maxHealth;
    e->data = b;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void initBlueBoss(void)
{
    Boss   *b;
    Entity *e;

    b = malloc(sizeof(Boss));
    memset(b, 0, sizeof(Boss));
    b->maxHealth = 200;
    b->fireBullets = fireRedBossBullets;

    if (redAlienTexture == NULL)
    {
        redAlienTexture = getAtlasImage("gfx/redAlien.png", 1);
        omniBulletTexture = getAtlasImage("gfx/alienBullet.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = redAlienTexture;
    e->x = (SCREEN_WIDTH - e->texture->rect.w) / 2;
    e->y = -250;
    e->health = b->maxHealth;
    e->data = b;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
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

static Bullet *spawnBossBullet(Entity *self, SDL_Texture *texture,
                float dx, float dy, int xOffset)
{
    Bullet *b;
    b = spawnBullet(self);
    b->texture = texture;
    b->x = self->x + (self->texture->rect.w / 2)
           - (texture->rect.w / 2) + xOffset;
    b->y = self->y + self->texture->rect.h;
    b->dx = dx;
    b->dy = dy;
    return b;
}

static void fireGreenBossBullets(Entity *self)
{
    spawnBossBullet(self, downBulletTexture, 0, 10, -25);
    spawnBossBullet(self, downBulletTexture, 0, 10, 25);
}

static void fireYellowBossBullets(Entity *self)
{
    int x;
    for (x = -1; x <= 1; x++)
    {
        spawnBossBullet(self, omniBulletTexture, x * 3, 9, 0);
    }
}

static void fireBlueBossBullets(Entity *self)
{
    Bullet *b;
    float   dx, dy;

    if (player == NULL) return;

    calcSlope(player->x + (player->texture->rect.w / 2),
              player->y + (player->texture->rect.h / 2),
              self->x  + (self->texture->rect.w  / 2),
              self->y  + self->texture->rect.h,
              &dx, &dy);

    b = spawnBossBullet(self, omniBulletTexture, dx * 12, dy * 12, 0);
    (void)b;
}

static void fireRedBossBullets(Entity *self)
{
    int x;

    for (x = -5; x <= 5; x++)
    {
        spawnBossBullet(self, omniBulletTexture, x * 2, 7, 0);
    }

    if (rand() %2 == 0)
    {
        ((Boss *)self->data)->numShotsToFire++;
    }
}
