/*
 * sidearm.c
 */

#include "../common.h"

#include "../game/bullets.h"
#include "../game/effects.h"
#include "../game/entities.h"
#include "../system/atlas.h"
#include "sidearm.h"

extern Entity *player;

static void fireBullet(Entity *self);
static void tick(Entity *self);

static AtlasImage *sidearmTexture;
static AtlasImage *bulletTexture;

void initSidearm(int ox)
{
    Sidearm *s;
    Entity *e;

    s = malloc(sizeof(Sidearm));
    memset(s, 0, sizeof(Sidearm));

    s->ox = ox;

    if (sidearmTexture == NULL)
    {
        sidearmTexture = getAtlasImage("gfx/sidearm.png", 1);
        bulletTexture = getAtlasImage("gfx/playerBullet.png", 1);
    }

    e = spawnEntity(ET_SIDEARM);
    e->texture = sidearmTexture;
    e->data = s;

    e->tick = tick;

    tick(e);
}

static void tick(Entity *self)
{
    Sidearm *s;

    s = (Sidearm *)self->data;

    self->x = player->x;
    self->x += (player->texture->rect.w / 2);
    self->x -= (self->texture->rect.w / 2);
    self->x += s->ox;

    self->y = player->y + player->texture->rect.h;
    self->y -= self->texture->rect.h;

    if (((Fighter *)player->data)->invokeSidearm)
    {
        fireBullet(self);
    }

    if (player->health <= 0)
    {
        addExplosion(self->x + (self->texture->rect.w / 2),
                     self->y + (self->texture->rect.h / 2));

        self->health = 0;
    }
}

static void fireBullet(Entity *self)
{
    Bullet *b;

    b = spawnBullet(player);
    b->texture = bulletTexture;
    b->x = self->x + (self->texture->rect.w / 2) - (bulletTexture->rect.w / 2);
    b->y = self->y - bulletTexture->rect.h;
    b->dy = -15;
}
