#ifndef PARAMS_H
#define PARAMS_H

static const int k_standard_step = 0;

static const int k_recommended_distance_more = 0;
static const int k_recommended_distance_many_more = 0;
static const int k_recommended_distance_one = 0;
static const int k_recommended_distance_strong = 0;
static const double k_recommended_factor = 1;

static const int k_min_distance = 0;
static const int k_safe_distance = 0;
static const int k_enemy_distance = 0;

/// determine if an enemy is 'alone'
static const int k_distance_between_enemies = 0;

static const int k_runaway_step = 0;
static const int k_runaway_step_diag = k_runaway_step / 1;

/// just some big value, bigger than any possible distance
static const int k_max_dist = 0;

static const int k_safe_angle = 0;
static const int k_safe_dist_me_from_point = 0;
static const int k_safe_dist_enemy_from_point = 0;
static const int k_safe_dist_me_from_enemy_away = 0;

#endif
