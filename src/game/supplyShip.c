/*
 * supplyShip.c
 */

#include "../common.h"

#include "../game/effects.h"
#include "../game/entities.h"
#include "../game/powerUpPod.h"
#include "../system/atlas.h"
#include "../system/draw.h"
#include "../system/util.h"
#include "supplyShip.h"

extern App     app;
extern Entity *player;

static void tick(Entity *self);
static void die(Entity *self);
static void draw(Entity *self);
static void takeDamage(Entity *self, int amount);

static AtlasImage *supplyShipTexture = NULL;

void initSupplyShip(void)
{
    Entity     *e;
    SupplyShip *s;

    s = malloc(sizeof(SupplyShip));
    memset(s, 0, sizeof(SupplyShip));

    if (supplyShipTexture == NULL)
    {
        supplyShipTexture = getAtlasImage("gfx/supplyShip.png", 1);
    }

    e = spawnEntity(ET_ALIEN);
    e->y = 50;
    e->health = 5;
    e->texture = supplyShipTexture;
    e->data = s;

    if (rand() % 2 == 0)
    {
        e->x = -supplyShipTexture->rect.w;
        s->dx = 3;
    }
    else
    {
        e->x = SCREEN_WIDTH;
        s->dx = -3;
    }

    e->tick = tick;
    e->draw = draw;
    e->takeDamage = takeDamage;
    e->die = die;
}

static void tick(Entity *self)
{
    SupplyShip *s;

    s = (SupplyShip *)self->data;

    self->x += (s->dx * app.deltaTime);

    if ((s->dx < 0 && self->x < -75) || (s->dx > 0 && self->x > SCREEN_WIDTH))
    {
        self->health = 0;
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
}

static void draw(Entity *self)
{
    SupplyShip *s;

    s = (SupplyShip *)self->data;

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

    ((SupplyShip *)self->data)->damageTimer = 8;
}

static void die(Entity *self)
{
    addExplosion(self->x + (self->texture->rect.w / 2), 
                 self->y + (self->texture->rect.h / 2));

    addPowerUpPod(self->x + (self->texture->rect.w / 2), 
                  self->y + (self->texture->rect.h / 2));
}