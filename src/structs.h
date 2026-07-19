/*
 * Copyright (C) 2021-2022 Parallel Realities. All rights reserved.
 */

typedef struct Texture    Texture;
typedef struct Bullet     Bullet;
typedef struct AtlasImage AtlasImage;
typedef struct Star       Star;
typedef struct Entity     Entity;
typedef struct Explosion  Explosion;

struct Texture
{
	char         name[MAX_FILENAME_LENGTH];
	SDL_Texture *texture;
	Texture     *next;
};

struct AtlasImage
{
	char         filename[MAX_FILENAME_LENGTH];
	SDL_Texture *texture;
	SDL_Rect     rect;
	int          rotated;
	AtlasImage  *next;
};

struct Entity
{
	int         type;
	double      x;
	double      y;
	AtlasImage *texture;
	int         health;
	void(*data);
	void (*tick)(Entity *self);
	void (*die)(Entity *self);
	Entity *next;
};

typedef struct
{
	double reload;
	int    reloadRate;
	double speed;
	int    invokeSidearm;
} Fighter;

typedef struct
{
	double swing;
	double startDelay;
	double reload;
	double swingAmount;
	int    sweepRange;
	int dy;
} SwingingAlien;

typedef struct
{
	double dx;
	double dy;
	double timeout;
} PointsPod;

typedef struct
{
	int    type;
	double changeTimer;
} PowerUpPod;

typedef struct
{
	int ox;
} Sidearm;

struct Explosion
{
	int          x;
	int          y;
	int          frame;
	int          numFrames;
	double       frameSpeed;
	double       frameTime;
	AtlasImage **texture;
	Explosion   *next;
};

struct Bullet
{
	double      x;
	double      y;
	double      dx;
	double      dy;
	int         dead;
	AtlasImage *texture;
	Entity     *owner;
	Bullet     *next;
};

struct Star
{
	double x;
	double y;
	double speed;
};

typedef struct
{
	Entity entityHead, *entityTail;
	Bullet bulletHead, *bulletTail;
	int    hasAliens;
	int    score;
	int    highscore;
} Stage;

typedef struct
{
	struct
	{
		void (*logic)(void);
		void (*draw)(void);
	} delegate;
	SDL_Renderer *renderer;
	SDL_Window   *window;
	int           keyboard[MAX_KEYBOARD_KEYS];
	double        deltaTime;
	struct
	{
		int fps;
		int showFPS;
	} dev;
} App;
