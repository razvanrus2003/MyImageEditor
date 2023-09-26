// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#pragma once

#include <stdlib.h>
#include "dependency.h"

char *valid_command(char *command);
char *scan_word(void);
void ignore_comments(FILE *in);
int ***load_map(pixel_map wrk_image, FILE *in);
void save(pixel_map wrk_image, char *wrk_image_name);
int valid_selection(pixel_map *wrk_image, char *parameters);
void select_wrk_area(pixel_map *wrk_image, char *parameters);
void print_histogram(histogram hg);
long *histogram_formation(pixel_map wrk_image, histogram *hg);
void histogram_function(pixel_map wrk_image, char *param);
void equalize_image(pixel_map wrk_image);
pixel_map crop_image(pixel_map wrk_image);
int effect_apply(int *kernel, int *vec, double divider, int intensity);
int *extract_neighbors(int **mat, int x, int y);
void effect_apply_map(pixel_map wrk_image, int *kernel, double divider);
void apply_kernel(pixel_map wrk_image, char *effect);
int verify_selection_shape(pixel_map wrk_image);
int acutal_agle(int angle);
void rotate_90(point a, point b, int **m);
void crop_rotated_image(pixel_map *wrk_image, int true_angle, int max);
void rotate_selection(pixel_map *wrk_image, char *parameters);
void load_function(pixel_map *wrk_image, char *parameters);
