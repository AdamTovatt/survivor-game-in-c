#include "player.h"

#include "enemy.h"
#include "helpers.h"
#include "level.h"
#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include "weapon.h"

extern Camera2D camera;
extern EnemySpawner enemySpawner;
extern Level level;

Player InitialPlayer() {
  return (Player){
      .body =
          (PhysicsBody){
              .pos = Vector2Zero(),
              .velocity = Vector2Zero(),
              .acceleration = Vector2Zero(),
              .mass = 400,
              .radius = 20,
          },
      .speed = 200,
      .weapon = InitialWeapon(),
  };
}

Vector2 GetDesiredVeloctiy() {
  Vector2 delta = Vector2Zero();
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) delta.y -= 1;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) delta.y += 1;

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) delta.x -= 1;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) delta.x += 1;
  // we cast float to int. Because length is either 0 (0,0), 1 (vertical or
  // horizontal) or 2 (diagonal)
  int lengthSquared = Vector2LengthSqr(delta);  // faster than length, no sqrt
  if (lengthSquared == 0) return delta;
  if (lengthSquared == 1) return delta;
  return Vector2Normalize(delta);
}

void MovePlayer(Player *player) {
  Vector2 force = Vector2Scale(GetDesiredVeloctiy(), player->speed);
  if (Vector2LengthSqr(force) != 0) ApplyAcceleration(&player->body, force);
  MoveBodyWithWeights(&player->body, ACCELERATION_SPEED, RETARDATION_SPEED);
}

void TickPlayer(Player *player) {
  MovePlayer(player);
  // stop if hitting object
  for (int i = 0; i < level.treeCount; ++i) {
    PhysicsBody treeBody = GetTreeBody(level.trees[i]);
    if (CheckCollision(player->body, treeBody)) {
      RigidCollision(&player->body, &treeBody);
    }
  }
  TickWeapon(&player->weapon, player->body.pos);
}

void DrawPlayer(Player *player) {
  DrawCircleV(player->body.pos, player->body.radius, PLAYER_COLOR);
  for (int i = 0; i < player->weapon.bulletCapacity; ++i) {
    if (player->weapon.bullets[i].spawned)
      DrawBullet(&player->weapon.bullets[i]);
  }
}