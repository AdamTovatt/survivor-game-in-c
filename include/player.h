#include "raylib.h"

// this scales with player speed
#define ACCELERATION_SPEED 5.0f
#define RETARDATION_SPEED 10.0f
#define PLAYER_COLOR DARKGREEN

typedef struct
{
    Vector2 pos;
    Vector2 velocity;
    float speed;
    float size;
} Player;

#define INITIAL_PLAYER (Player){.speed = 420, .size = 20};

void TickPlayer(Player *player);
void DrawPlayer(Player *player);