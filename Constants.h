#pragma once

// ---------------------------------------------------------------------------------------------------------------------------------------
//  STAR WARS HORDE  ---  Constants
// ---------------------------------------------------------------------------------------------------------------------------------------

// Screen
constexpr int   SCREEN_WIDTH = 1280;
constexpr int   SCREEN_HEIGHT = 720;
constexpr int   TARGET_FPS = 60;

// Player
constexpr float PLAYER_SPEED = 280.0f;
constexpr float PLAYER_MAX_HEALTH = 100.0f;
constexpr float PLAYER_RADIUS = 16.0f;
constexpr float PLAYER_IFRAMES = 0.5f;    // invincibility after hit

// Blaster (Pistol-style) --- auto-fires toward mouse
constexpr float BLASTER_FIRE_RATE = 0.20f;   // seconds between shots
constexpr float BLASTER_BULLET_SPEED = 650.0f;
constexpr float BLASTER_BULLET_DMG = 20.0f;
constexpr float BLASTER_BULLET_LIFE = 2.0f;
constexpr float BLASTER_BULLET_RAD = 5.0f;

// Rapid Cannon (Shotgun-style) --- fires in movement direction
constexpr float CANNON_FIRE_RATE = 0.55f;
constexpr float CANNON_BULLET_SPEED = 500.0f;
constexpr float CANNON_BULLET_DMG = 12.0f;
constexpr float CANNON_BULLET_LIFE = 2.0f;
constexpr float CANNON_BULLET_RAD = 4.0f;
constexpr int   CANNON_PELLETS = 5;
constexpr float CANNON_SPREAD_DEG = 22.0f;

// Object pools
constexpr int   BULLET_POOL_SIZE = 200;
constexpr int   ENEMY_POOL_SIZE = 120;

// Enemy: Stormtrooper
constexpr float TROOPER_SPEED = 75.0f;
constexpr float TROOPER_HEALTH = 40.0f;
constexpr float TROOPER_DAMAGE = 10.0f;
constexpr float TROOPER_RADIUS = 17.0f;

// Enemy: Sith Warrior
constexpr float SITH_SPEED = 135.0f;
constexpr float SITH_HEALTH = 90.0f;
constexpr float SITH_DAMAGE = 22.0f;
constexpr float SITH_RADIUS = 21.0f;

// Spawning
constexpr float SPAWN_INTERVAL_START = 2.4f;
constexpr float SPAWN_INTERVAL_MIN = 0.55f;
constexpr int   SPAWN_COUNT_START = 2;
constexpr int   SPAWN_COUNT_MAX = 8;
constexpr float SPAWN_RADIUS = 700.0f;

// Lightsaber
constexpr float SABER_SPIN_SPEED = 3.5f;    // radians per second
constexpr float SABER_RADIUS = 65.0f;   // orbit distance from player
constexpr float SABER_BLADE_LEN = 130.0f;  // length of blade
constexpr float SABER_BLADE_RADIUS = 12.0f;   // hit radius at blade tip
constexpr float SABER_DAMAGE = 9999.0f; // one-shot kill
constexpr float SABER_HIT_RATE = 0.15f;   // seconds between damage ticks