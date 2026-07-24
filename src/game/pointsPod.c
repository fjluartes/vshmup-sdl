/*
 * pointsPod.c
 */
#include "../common.h"

#include "../game/entities.h"
#include "../system/atlas.h"
#include "../system/util.h"
#include "pointsPod.h"

extern App     app;
extern Entity *player;
extern Stage   stage;

static void tick(Entity *e);

static AtlasImage *texture;

void addPointsPod(int x, int y)
{
    Entity *e;
    PointsPod *p;

    p = malloc(sizeof(PointsPod));
    memset(p, 0, sizeof(PointsPod));

    p->dx = (1.0 * (-25 + rand() % 50)) * 0.2;
    p->dy = (1.0 * (-25 + rand() % 50)) * 0.2;
    p->timeout = FPS * 5;

    if (texture == NULL)
    {
        texture = getAtlasImage("gfx/points.png", 1);
    }

    e = spawnEntity(ET_POINTS);
    e->x = x;
    e->y = y;
    e->texture = texture;
    e->data = p;

    e->tick = tick;
}

static void tick(Entity *self)
{
    PointsPod *p;

    p = (PointsPod *)self->data;

    self->x += p->dx * app.deltaTime;
    self->y += p->dy * app.deltaTime;

    if (self->x < 0)
    {
        self->x = 0;
        p->dx = -p->dx;
    }

    if (self->y < 0)
    {
        self->y = 0;
        p->dy = -p->dy;
    }

    if (self->x >= SCREEN_WIDTH - self->texture->rect.w)
    {
        self->x = (SCREEN_WIDTH - self->texture->rect.w - 1);
        p->dx = -p->dx;
    }

    if (self->y >= SCREEN_HEIGHT - self->texture->rect.h)
    {
        self->y = (SCREEN_HEIGHT - self->texture->rect.h - 1);
        p->dy = -p->dy;
    }

    if (player->health > 0 && 
        collision(self->x, self->y, 
            self->texture->rect.w, self->texture->rect.h, 
            player->x, player->y, 
            player->texture->rect.w, player->texture->rect.h))
    {
        stage.score += 10;

        self->health = 0;
    }

    p->timeout -= app.deltaTime;

    if (p->timeout <= 0)
    {
        self->health = 0;
    }
}