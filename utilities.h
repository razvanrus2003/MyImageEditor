// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#pragma once

#include <stdlib.h>
#include "dependency.h"

void print_pixel_map(pixel_map wrk_image, FILE * out);
void write_pixel_map(pixel_map wrk_image, FILE *out);
void free_mat(int **mat, int height);
void free_map(pixel_map wrk_image);
int *all_vector(int width);
int **all_mat(int width, int height);
int ***all_pixel_map(int depht, int width, int height);
void my_swap(int *a, int *b);
int clamp(int intensity, double a);
void mat_cop(int **mat, int **cop, int height, int width);
int read_num_ascii(FILE *in);
int read_num_binari(FILE *in);
int my_max(int a, int b);
