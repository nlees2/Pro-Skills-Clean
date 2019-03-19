#pragma once

// Constants for model arrays
const int kWallQuantity = 4;	  // number of walls, used in the inner area to enclose the player
const int kCrateQuantity = 25;	  // number of tall crates
const int kNumBulletTracers = 10; // number of bullet tracers
const int kNumRobberTargets = 12; // number of targets
const int kNumAmmoClips = 5;	  // number of empty ammo clips that can fall to the floor when reloading

enum targetState { ready, waiting, hit };
