/*
 * powerUpPod.c
 */

#include "../common.h"

#include "../game/entities.h"
#include "../game/sidearm.h"
#include "../system/atlas.h"
#include "../system/util.h"
#include "powerUpPod.h"

#define CHANGE_TIMER (FPS * 3)

extern App     app;
extern Entity *player;

static void activatePowerUp(Entity *self, PowerUpPod *p);
static void updateTexture(Entity *e, PowerUpPod *p);
static void tick(Entity *self);

static AtlasImage *sidearmPodTexture = NULL;

void addPowerUpPod(int x, int y, int type)
{
    PowerUpPod *p;
    Entity     *e;

    p = malloc(sizeof(PowerUpPod));
    memset(p, 0, sizeof(PowerUpPod));

    p->type = type;
    p->changeTimer = CHANGE_TIMER;

    e = spawnEntity(ET_POWER_UP_POD);
    e->x = x;
    e->y = y;
    e->data = p;

    e->tick = tick;

    if (sidearmPodTexture == NULL)
    {
        sidearmPodTexture = getAtlasImage("gfx/sidearmPowerUpPod.png", 1);
    }

    updateTexture(e, p);
}

static void tick(Entity *self)
{
    PowerUpPod *p;

    p = (PowerUpPod *)self->data;

    p->changeTimer -= app.deltaTime;

    if (p->changeTimer <= 0)
    {
        p->type = (p->type + 1) % PP_MAX;
        
        p->changeTimer = CHANGE_TIMER;

        updateTexture(self, p);
    }

    if (collision(self->x, self->y,
                  self->texture->rect.w, self->texture->rect.h,
                  player->x, player->y,
                  player->texture->rect.w, player->texture->rect.h))
    {
        activatePowerUp(self, p);
    }

    self->y += 3 * app.deltaTime;

    if (self->y >= SCREEN_HEIGHT)
    {
        self->health = 0;
    }
}

static void updateTexture(Entity *e, PowerUpPod *p)
{
    switch (p->type)
    {
        case PP_SIDEARM:
            e->texture = sidearmPodTexture;
            break;
        default:
            break;
    }
}

static void activatePowerUp(Entity *self, PowerUpPod *p)
{
    switch (p->type)
    {
        case PP_SIDEARM:
            initSidearm(-48);
            initSidearm(48);
            break;
        default:
            break;
    }

    self->health = 0;
}
