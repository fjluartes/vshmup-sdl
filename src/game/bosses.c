/*
 * bosses.c
 */

#include "../common.h"

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
static Bullet *spawnBossBullet(Entity *self, AtlasImage *texture, 
            double dx, double dy, int xOffset);
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
        case 2:
            initGreenBoss();
            break;
        case 3:
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

static void initRedBoss(void)
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
    Boss *b;

    stage.hasAliens = 1;

    if (self->y < 120)
    {
        self->y += app.deltaTime;
        return;
    }

    b = (Boss *)self->data;

    self->x += b->dx;
    // clamp boss texture to walls
    self->x = MAX(MIN(self->x, SCREEN_WIDTH - self->texture->rect.w), 0);

    // reverse direction when boss hits wall
    if ((self->x == 0 && b->dx < 0) ||
        (self->x == SCREEN_WIDTH - self->texture->rect.w && b->dx > 0))
    {
        b->dx = -b->dx;
    }

    self->y = 120;

    b->thinkTime = MAX(b->thinkTime - app.deltaTime, 0);

    if (b->thinkTime == 0)
    {
        b->dx = 0;

        if (rand() % 5 != 0)
        {
            b->dx = (1.0 * (rand() % 500 - rand() % 500)) * 0.001;
        }

        b->thinkTime = FPS * (25 + (rand() % 75));
        b->thinkTime *= 0.01;
    }

    b->attackTime = MAX(b->attackTime - app.deltaTime, 0);

    if (b->attackTime == 0)
    {
        b->numShotsToFire = 3 + rand() % 4;

        b->attackTime = FPS * (2 + (rand() % 3));
    }

    b->reload = MAX(b->reload - app.deltaTime, 0);

    if (b->reload == 0 && b->numShotsToFire > 0)
    {
        b->numShotsToFire--;

        b->reload = 12;

        b->fireBullets(self);
    }

    b->damageTimer = MAX(b->damageTimer - app.deltaTime, 0);

    if (player->health > 0 &&
        collision(self->x, self->y, 
            self->texture->rect.w, self->texture->rect.h, 
            player->x, player->y, 
            player->texture->rect.w, player->texture->rect.h))
    {
        player->health = 0;
        player->die(player);
    }

    stage.boss = self;
}

static void draw(Entity *self)
{
    Boss *b;

    b = (Boss *)self->data;

    blitAtlasImage(self->texture, self->x, self->y, 0, SDL_FLIP_NONE);

    if (b->damageTimer > 0)
    {
        SDL_SetTextureBlendMode(self->texture->texture, SDL_BLENDMODE_ADD);
        blitAtlasImage(self->texture, self->x, self->y, 0, SDL_FLIP_NONE);
        SDL_SetTextureBlendMode(self->texture->texture, SDL_BLENDMODE_BLEND);
    }
}

static void takeDamage(Entity *self, int amount)
{
    if (self->y >= 120)
    {
        self->health -= amount;

        if (self->health <= 0)
        {
            self->die(self);
        }

        ((Boss *)self->data)->damageTimer = 8;
    }
}

static void die(Entity *self)
{
    int i, x, y;

    stage.score += 100;

    for (i = 0; i < 25; i++)
    {
        x = self->x + (self->texture->rect.w / 2);
        x -= rand() % self->texture->rect.w;
        x += rand() % self->texture->rect.w;

        y = self->y + (self->texture->rect.h / 2);
        y -= rand() % self->texture->rect.h;
        y += rand() % self->texture->rect.h;

        addExplosion(x, y);
    }
}

static Bullet *spawnBossBullet(Entity *self, AtlasImage *texture,
                double dx, double dy, int xOffset)
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
    double  dx, dy;

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
