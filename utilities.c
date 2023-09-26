// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dependency.h"
#include "utilities.h"
#include "functions.h"

//scrie o imagine intr-un fisier sub forma binara
void write_pixel_map(pixel_map wrk_image, FILE *out)
{
	unsigned char aux;
	if (wrk_image.map) {
		int i, j, k;

		for (i = 0; i < wrk_image.height; i++) {
			for (j = 0; j < wrk_image.width; j++) {
				for (k = 0; k < wrk_image.depht; k++) {
					aux = (unsigned char)wrk_image.map[k][i][j];
					fwrite(&aux, sizeof(unsigned char), 1, out);
				}
			}
		}
	}
}

//scrie o imagine intr-un fisier folosind caractere
void print_pixel_map(pixel_map wrk_image, FILE *out)
{
	if (wrk_image.map) {
		int i, j, k;

		for (i = 0; i < wrk_image.height; i++) {
			for (j = 0; j < wrk_image.width; j++) {
				for (k = 0; k < wrk_image.depht; k++) {
					fprintf(out, "%d", wrk_image.map[k][i][j]);
					if (j + 1 < wrk_image.width || k + 1 < wrk_image.depht)
						fprintf(out, " ");
					else
						fprintf(out, "\n");
				}
			}
		}
	}
}

//elibereaza spatiul alocat unei matrice
void free_mat(int **mat, int height)
{
	if (mat) {
		int i;
		for (i = 0; i < height; i++)
			free(mat[i]);
		free(mat);
	}
}

//elibereaza spatiul alocat unei imagini
void free_map(pixel_map wrk_image)
{
	if (wrk_image.map) {
		int i, j;
		for (i = 0; i < wrk_image.depht; i++) {
			for (j = 0; j < wrk_image.height; j++)
				free(wrk_image.map[i][j]);
			free(wrk_image.map[i]);
		}
		free(wrk_image.map);
	}
}

//aloca un vector dinamic
int *all_vector(int width)
{
	int *v = (int *)malloc(sizeof(int) * width);
	if (!v)
		return NULL;
	return v;
}

//aloca o matice dinamic
int **all_mat(int width, int height)
{
	int **mat = (int **)malloc(sizeof(int *) * height);
	if (!mat)
		return NULL;
	int i;

	for (i = 0; i < height; i++) {
		mat[i] = all_vector(width);
		if (!mat[i]) {
			free_mat(mat, i);
			return NULL;
		}
	}
	return mat;
}

// aloca o imagine dinamic
int ***all_pixel_map(int depht, int width, int height)
{
	int i;
	int ***map = (int ***)malloc(sizeof(int **) * depht);
	if (!map)
		return NULL;
	for (i = 0; i < depht; i++) {
		map[i] = all_mat(width, height);
		if (!map[i])
			return NULL;
	}
	return map;
}

void my_swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int clamp(int intensity, double a)
{
	if (a < 0)
		return 0;
	if (a > intensity)
		return intensity;
	return (int)a;
}

//copiaza continutul unei matrice in alta
void mat_cop(int **mat, int **cop, int height, int width)
{
	int i, j;
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			cop[i][j] = mat[i][j];
}

//cu funtiile de read_num...
//citesc din fisiere fie cu fwrite sau cu fscanf
int read_num_ascii(FILE *in)
{
	int a;
	fscanf(in, "%d", &a);
	return a;
}

int read_num_binari(FILE *in)
{
	unsigned char a;
	fread(&a, sizeof(unsigned char), 1, in);
	return (int)a;
}

int my_max(int a, int b)
{
	if (b > a)
		return b;
	return a;
}
