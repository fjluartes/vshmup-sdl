/*
 * straightAlien.c
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/pointsPod.h"
#include "../system/atlas.h"
#include "../system/draw.h"
#include "../system/util.h"
#include "straightAlien.h"

extern App     app;
extern Entity *player;
extern Stage   stage;

static void tick(Entity *self);
static void die(Entity *self);
static void draw(Entity *self);
static void fireBullet(Entity *self);
static void takeDamege(Entity *self, int amount);

static AtlasImage *littleGreyAlienTexture = NULL;
static AtlasImage *bulletTexture = NULL;

void initStraightAlien(int startDelay, int x, int y, double dx, double dy)
{
    Entity        *e;
    StraightAlien *s;

    s = malloc(sizeof(StraightAlien));
    memset(s, 0, sizeof(StraightAlien));

    s->startDelay = startDelay;
    s->dx = dx;
    s->dy = dy;

    if (littleGreyAlienTexture == NULL)
    {
        littleGreyAlienTexture = getAtlasImage("gfx/littleGreyAlien.png", 1);
        bulletTexture = getAtlasImage("gfx/alienDownBullet.png");
    }

    e = spawnEntity(ET_ALIEN);
    e->texture = littleGreyAlienTexture;
    e->data = s;

    e->x = x - (e->texture->rect.w / 2);
    e->y = y;
    e->health = 2;

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void tick(Entity *self)
{
    StraightAlien *s;

    s = (StraightAlien *)self->data;

    s->startDelay -= app.deltaTime;

    if (s->startDelay <= 0)
    {
        self->x += s->dx * app.deltaTime;
        self->y += s->dy * app.deltaTime;
    }

    if ((s->dx < 0 && self->x < -75) || (s->dx > 0 && self->x > SCREEN_WIDTH))
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

        s->reload = FPS;
    }

    s->damageTimer = MAX(s->damageTimer - app.deltaTime, 0);

    if (player->health > 0 && 
        collision(self->x, self->y, 
            self->texture->rect.w, self->texture->rect.h, 
            player->x, player->y, 
            player->texture->rect.w, player->texture->rect.h))
    {
        self->health = 0;
        self->die(self);

        player->health = 0;
        player->die(player);
    }

    stage.hasAliens = 1;
}

static void draw(Entity *self)
{
    StraightAlien *s;

    s = (StraightAlien *)self->data;

    blitAtlasImage(self->texture, self->x, self->y, 0, SDL_FLIP_NONE);

    if (s->damageTimer > 0)
    {
        SDL_SetTextureBlendMode(self->texture->texture, SDL_BLENDMODE_ADD);
        blitAtlasImage(self->texture, self->x, self->y, 0, SDL_FLIP_NONE);
        SDL_SetTextureBlendMode(self->texture->texture, SDL_BLENDMODE_BLEND);
    }
}

static void takeDamage(Entity *self, int amount)
{
    self->health -= amount;

    if (self->health == 0)
    {
        self->die(self);
    }

    ((StraightAlien *)self->data)->damageTimer = 8;
}

static void die(Entity *self)
{
    stage.score++;

    addExplosion(self->x + (self->texture->rect.w / 2), self->y + (self->texture->rect.h / 2));

    if (--stage.numWaveAliens == 0)
    {
        addPointsPod(self->x, self->y);
    }
}

static void fireBullet(Entity *self)
{
    Bullet *b;

    b = spawnBullet(self);
    b->texture = bulletTexture;
    b->x = self->x + (self->texture->rect.w / 2) - (bulletTexture->rect.w / 2);
    b->y = self->y + self->texture->rect.h;
    b->dy = 10;
}