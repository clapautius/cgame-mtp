#ifndef HYPERSONIC_PARAMS_H
#define HYPERSONIC_PARAMS_H

#include <functional>

// uncomment to have more debug info (useful for unit tests)
#define HYPER_DEBUG

static const int k_bonus_for_close_cell = 50;
static const int k_close_distance_for_bonus = 3;
static const int k_goodie_not_important = 24;
static const int k_goodie_bomb_important = 101;
static const int k_goodie_range_important = 76;

//static const int k_min_vital_space = 10;
//static const int k_min_vital_space = 8;
static const int k_min_vital_space = 6;

static const int k_almost_max_score = 1000;

static const int k_enemy_near_score = 75;

static const int k_extra_bombs_threshold = 3;
static const int k_extra_range_threshold = 4;

static const int k_min_score_for_bomb = 20;

#endif
