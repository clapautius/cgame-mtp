#ifndef PARAMS_H
#define PARAMS_H

static const int k_standard_step = 1000;

static const int k_recommended_distance_more = 5500;
static const int k_recommended_distance_many_more = 6500; // prev + 1000
static const int k_recommended_distance_one = 5500;
static const int k_recommended_distance_strong = 3800;
static const double k_recommended_factor = 1.4;
static const int k_recommended_dist_radius_around = 6500;

static const int k_min_distance = 2100;
static const int k_safe_distance = 2600;
static const int k_enemy_distance = 3500;

static const int k_runaway_step = 1000;
static const int k_runaway_step_diag = k_runaway_step / 1.4142;
static const int k_runaway_safe_dist_to_point = 800;
static const int k_runaway_small_angle = 15;

/// just some big value, bigger than any possible distance
static const int k_max_dist = 100000;

static const int k_safe_angle = 70;
static const int k_safe_dist_me_from_point = 1500;
static const int k_safe_dist_enemy_from_point = 1500;
static const int k_safe_dist_me_from_enemy_away = 1500;

static const int k_max_enemies_to_proc = 30;
static const int k_max_points_to_proc = 60;

#endif
