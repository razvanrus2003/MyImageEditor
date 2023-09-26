// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dependency.h"
#include "utilities.h"
#include "functions.h"

//verifica prima parte unei commenzi
char *valid_command(char *command)
{
	int ok = 1;
	char *token = strtok(command, "\n ");
	token = strtok(NULL, "\n");
	if (!strcmp(command, "EXIT"))
		ok = 0;
	if (!strcmp(command, "EQUALIZE"))
		ok = 0;
	if (!strcmp(command, "APPLY"))
		ok = 0;
	if (!strcmp(command, "CROP"))
		ok = 0;
	if (!strcmp(command, "HISTOGRAM"))
		ok = 0;
	if (!strcmp(command, "LOAD"))
		ok = 0;
	if (!strcmp(command, "SELECT"))
		ok = 0;
	if (!strcmp(command, "SAVE"))
		ok = 0;
	if (!strcmp(command, "ROTATE"))
		ok = 0;
	if (!strcmp(command, "APPLY"))
		ok = 0;

	if (ok)
		printf("Invalid command\n");
	return token;
}

//citeste cate o comanda de la stdin
char *scan_word(void)
{
	char *tmp;
	tmp = (char *)malloc(sizeof(char) * 100);
	if (!tmp) {
		printf("Dinamic allocation failed\n");
		return NULL;
	}
	fgets(tmp, 100, stdin);
	return tmp;
}

//functia folosita pt ignorarea comentariilor
void ignore_comments(FILE *in)
{
	fpos_t pos;

	char c = '#';
	while (c == '#') {
		fgetpos(in, &pos);
		fread(&c, sizeof(char), 1, in);
		if (c == '#')
			while (c != '\n')
				fread(&c, sizeof(char), 1, in);
		else
			fsetpos(in, &pos);
	}
}

//functie folosita la comanda LOAD
int ***load_map(pixel_map wrk_image, FILE *in)
{
	int i, j, k, ***tmp;
	char trash;
	tmp = all_pixel_map(wrk_image.depht, wrk_image.width, wrk_image.height);
	if (!tmp)
		return NULL;
	if (wrk_image.type == 'b')
		fread(&trash, sizeof(char), 1, in);
	for (i = 0; i < wrk_image.height; i++) {
		for (j = 0; j < wrk_image.width; j++) {
			for (k = 0; k < wrk_image.depht; k++) {
				if (wrk_image.type == 'a')
					tmp[k][i][j] = read_num_ascii(in);
				if (wrk_image.type == 'b')
					tmp[k][i][j] = read_num_binari(in);
			}
		}
	}
	return tmp;
}

//functie folosita la comanda SAVE
void save(pixel_map wrk_image, char *wrk_image_name)
{
	int width = wrk_image.width;
	int height = wrk_image.height;
	int intensity = wrk_image.intensity;
	int ok = 1;
	if (!wrk_image_name) {
		printf("Invalid command\n");
		return;
	}
	char *token = strtok(wrk_image_name, " ");
	token = strtok(NULL, " ");

	if (token && strcmp(token, "ascii")) {
		printf("Invalid commad\n");
		return;
	}
	if (!token)
		ok = 0;

	FILE *out = fopen(wrk_image_name, "w");
	if (!out) {
		printf("Dinamic allocation failed\n");
		return;
	}
	if (wrk_image.depht == 1 && ok)
		fprintf(out, "P2\n%d %d\n%d\n", width, height, intensity);
	if (wrk_image.depht == 1 && !ok)
		fprintf(out, "P5\n%d %d\n%d\n", width, height, intensity);
	if (wrk_image.depht == 3 && ok)
		fprintf(out, "P3\n%d %d\n%d\n", width, height, intensity);
	if (wrk_image.depht == 3 && !ok)
		fprintf(out, "P6\n%d %d\n%d\n", width, height, intensity);

	if (!ok) {
		fclose(out);
		out = fopen(wrk_image_name, "ab");
		write_pixel_map(wrk_image, out);
	} else {
		print_pixel_map(wrk_image, out);
	}
	printf("Saved %s\n", wrk_image_name);
	fclose(out);
}

//functie valideaza o selectie
int valid_selection(pixel_map *wrk_image, char *parameters)
{   int aux, m, n, r, q, val;
	val = sscanf(parameters, "%d %d %d %d", &m, &n, &r, &q);
	if (val < 4) {
		printf("Invalid command\n");
		return 1;
	}

	if (m > r) {
		aux = m;
		m = r;
		r = aux;
	}
	if (n > q) {
		aux = n;
		n = q;
		q = aux;
	}
	if (m == r && n == q)
		return 0;
	if (m < 0 || m >= wrk_image->width)
		return 0;
	if (n < 0 || n >= wrk_image->height)
		return 0;
	if (q < -1 || q > wrk_image->height)
		return 0;
	if (r < -1 || r > wrk_image->width)
		return 0;
	wrk_image->a.x = m;
	wrk_image->a.y = n;
	wrk_image->b.x = r - 1;
	wrk_image->b.y = q - 1;

	printf("Selected %d %d %d %d\n", m, n, r, q);
	return 1;
}

//functie folosita la comenzile SELECT <x1> <y1>
//<x2> <y2> si SELECT ALL
void select_wrk_area(pixel_map *wrk_image, char *parameters)
{
	if (parameters[0] == 'A')
		strtok(parameters, " ");
	if (!strcmp(parameters, "ALL")) {
		wrk_image->a.x = 0;
		wrk_image->a.y = 0;
		wrk_image->b.x = wrk_image->width - 1;
		wrk_image->b.y = wrk_image->height - 1;
		printf("Selected ALL\n");
	} else {
		if (!valid_selection(wrk_image, parameters))
			printf("Invalid set of coordinates\n");
	}
}

//primeste structura unei histogramei si printeaza graficul
//conform cerintei
void print_histogram(histogram hg)
{
	int i, j, nr_stars;
	for (i = 0; i < hg.size; i++) {
		nr_stars = ((double)hg.graph[i] / hg.max_val) * hg.ratio;
		printf("%d\t|\t", nr_stars);
		for (j = 0; j < nr_stars; j++)
			printf("*");
		printf("\n");
	}
}

//primeste un numar de binuri si formeaza o historama
long *histogram_formation(pixel_map wrk_image, histogram *hg)
{
	if (wrk_image.depht != 1) {
		printf("Black and white image needed\n");
		return NULL;
	}
	int i, j, aux;

	aux = (wrk_image.intensity + 1) / hg->size;
	long *tmp = (long *)calloc(sizeof(long), hg->size);
	if (!tmp) {
		printf("Dinamic allocation failed\n");
		return NULL;
	}
	hg->max_val = 0;

	for (i = 0; i < wrk_image.height; i++) {
		for (j = 0; j < wrk_image.width; j++) {
			tmp[wrk_image.map[0][i][j] / aux] += 1;
			if (hg->max_val < tmp[wrk_image.map[0][i][j] / aux])
				hg->max_val = tmp[wrk_image.map[0][i][j] / aux];
		}
	}

	return tmp;
}

//functie folosita la comanda HISTOGRAM pt citirea parametrilor
void histogram_function(pixel_map wrk_image, char *param)
{
	histogram hg;
	int extr_prm, ssc_val;
	if (!wrk_image.map) {
		printf("No image loaded\n");
	} else {
		if (!param) {
			printf("Invalid command\n");
		} else {
			ssc_val = sscanf(param, "%d%d%d", &hg.ratio, &hg.size, &extr_prm);
			if (ssc_val != 2) {
				printf("Invalid command\n");
			} else {
				hg.graph = histogram_formation(wrk_image, &hg);
				if (wrk_image.depht == 1 && hg.graph) {
					print_histogram(hg);
					free(hg.graph);
				}
			}
		}
	}
}

//functie folosita la comanda EQUALIZE
void equalize_image(pixel_map wrk_image)
{
	int i, j, k;
	double tmp, area;
	histogram hg;

	if (!wrk_image.map) {
		printf("No image loaded\n");
	} else {
		hg.size = wrk_image.intensity + 1;
		hg.graph = histogram_formation(wrk_image, &hg);
	if (hg.graph) {
		for (i = 0; i < wrk_image.height; i++) {
			for (j = 0; j < wrk_image.width; j++) {
				tmp = 0.0;
				for (k = 0; k <= wrk_image.map[0][i][j]; k++)
					tmp = tmp + hg.graph[k];
				tmp = tmp * wrk_image.intensity;
				area = wrk_image.height * wrk_image.width;
				tmp = tmp / area;
				wrk_image.map[0][i][j] = clamp(wrk_image.intensity, tmp);
			}
		}
		free(hg.graph);
		printf("Equalize done\n");
		}
	}
}

//functie folosita la comanda CROP
pixel_map crop_image(pixel_map wrk_image)
{
	pixel_map cop;
	cop.height = wrk_image.b.y + 1 - wrk_image.a.y;
	cop.width = wrk_image.b.x + 1 - wrk_image.a.x;
	cop.depht = wrk_image.depht;
	cop.intensity = wrk_image.intensity;
	cop.type = wrk_image.type;
	int i, j, k;
	int start_x, start_y;
	cop.a.x = 0;
	cop.a.y = 0;
	cop.b.x = cop.width - 1;
	cop.b.y = cop.height - 1;

	i = cop.height;
	j = cop.width;
	cop.map = all_pixel_map(cop.depht, j, i);
	if (!cop.map) {
		printf("Dinamic allocation failed\n");
		return cop;
	}

	start_x = wrk_image.a.x;
	start_y = wrk_image.a.y;
	for (i = 0; i < wrk_image.b.y - wrk_image.a.y + 1; i++)
		for (j = 0; j < wrk_image.b.x - wrk_image.a.x + 1; j++)
			for (k = 0; k < wrk_image.depht; k++)
				cop.map[k][i][j] = wrk_image.map[k][i + start_y][j + start_x];

	free_map(wrk_image);
	return cop;
}

//primeste 2 matrice desfasurate si returneaza noua valoare a pixel-ului
int effect_apply(int *kernel, int *vec, double divider, int intensity)
{
	int i;
	double tmp = 0.0;
	for (i = 0; i < 9; i++)
		tmp = tmp + (kernel[i] * vec[i]);
	tmp = tmp / divider;
	return clamp(intensity, tmp);
}

//returneaza vecinii unui numar sub forma unei matrice desfasurate
int *extract_neighbors(int **mat, int x, int y)
{
	int i, j = 8, *v;
	v = (int *)malloc(sizeof(int) * 9);
	if (!v)
		return NULL;
	for (i = -1; i < 2; i++)
		v[j--] = mat[x - 1][y + i];
	for (i = -1; i < 2; i++)
		v[j--] = mat[x][y + i];
	for (i = -1; i < 2; i++)
		v[j--] = mat[x + 1][y + i];
	return v;
}

//aplica functia effect_apply pe fiecare element al matricei
void effect_apply_map(pixel_map wrk_image, int *kernel, double divider)
{
	int **tmp, *vec, i, j, k;
	int height = wrk_image.height;
	int width = wrk_image.width;
	int intensity = wrk_image.intensity;

	for (k = 0; k < wrk_image.depht; k++) {
		tmp = all_mat(wrk_image.width, wrk_image.height);
		if (!tmp) {
			printf("Dinamic allocation failed\n");
			return;
		}
		mat_cop(wrk_image.map[k], tmp, wrk_image.height, wrk_image.width);
		for (i = wrk_image.a.y; i <= wrk_image.b.y; i++)
			for (j = wrk_image.a.x; j <= wrk_image.b.x; j++)
				if (i > 0 && j > 0 && i < height - 1 && j < width - 1) {
					vec = extract_neighbors(wrk_image.map[k], i, j);
					if (!vec) {
						printf("Dinamic allocation failed\n");
						free_mat(tmp, wrk_image.height);
						return;
					}
					tmp[i][j] = effect_apply(kernel, vec, divider, intensity);
					free(vec);
				}
		free_mat(wrk_image.map[k], wrk_image.height);
		wrk_image.map[k] = tmp;
	}
}

//functie folosita la comanda APPLY
void apply_kernel(pixel_map wrk_image, char *effect)
{
	int edge_det[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	int sharpen[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	int blur[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	int gaussian_blur[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	double divider = 1.0;
	int *kernel;
	int ok = 0;

	if (!strcmp(effect, "EDGE")) {
		kernel = edge_det;
		ok = 1;
	}
	if (!strcmp(effect, "SHARPEN")) {
		kernel = sharpen;
		ok = 1;
	}
	if (!strcmp(effect, "BLUR")) {
		kernel = blur;
		divider = 9.0;
		ok = 1;
	}
	if (!strcmp(effect, "GAUSSIAN_BLUR")) {
		kernel = gaussian_blur;
		divider = 16.0;
		ok = 1;
	}
	if (ok) {
		effect_apply_map(wrk_image, kernel, divider);
		printf("APPLY %s done\n", effect);
	} else {
		printf("APPLY parameter invalid\n");
	}
}

//verifica daca forma selectata este un patrat sau toata imaginea
int verify_selection_shape(pixel_map wrk_image)
{
	if (wrk_image.b.y - wrk_image.a.y ==
	wrk_image.b.x - wrk_image.a.x)
		return 1;
	if (wrk_image.a.y == 0 && wrk_image.a.x == 0)
		if (wrk_image.b.x == wrk_image.width - 1)
			if (wrk_image.b.y == wrk_image.height - 1)
				return 2;
	return 0;
}

//simplifica periodicitatea rotirii
int acutal_agle(int angle)
{
	if (angle < 0)
		angle += 360;
	if (angle == 360)
		angle = 0;
	return angle;
}

//roteste aria dintre puntele a si b la 90 de grade
void rotate_90(point a, point b, int **m)
{
	int aux;
	int i, j;
	for (i = a.y; i < b.y - i + a.y; i++) {
		for (j = a.x + i - a.y; j < b.x - i + a.y ; j++) {
			aux = m[i][j];
			m[i][j] = m[b.y - j + a.x][a.x + i - a.y];
			m[b.y - j + a.x][a.x + i - a.y] = m[b.y - i + a.y][b.x - j + a.x];
			m[b.y - i + a.y][b.x - j + a.x] = m[a.y + j - a.x][b.x - i + a.y];
			m[a.y + j - a.x][b.x - i + a.y] = aux;
		}
	}
}

//in cazul rotirii unei imagini dreptunghiulare aplica crop
//suprafata adaugata in plus
void crop_rotated_image(pixel_map *wrk_image, int true_angle, int max)
{
	if (wrk_image->height > wrk_image->width) {
		if (true_angle == 90) {
			wrk_image->b.x = wrk_image->height - 1;
			wrk_image->b.y = wrk_image->width - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
		if (true_angle == 180) {
			wrk_image->a.x = max - wrk_image->width;
			wrk_image->b.x = max - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
		if (true_angle == 270) {
			wrk_image->a.y = max - wrk_image->width;
			wrk_image->b.x = wrk_image->height - 1;
			wrk_image->b.y = max - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
	} else {
		if (true_angle == 90) {
			wrk_image->a.x = max - wrk_image->height;
			wrk_image->b.x = max - 1;
			wrk_image->b.y = max - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
		if (true_angle == 180) {
			wrk_image->a.y = max - wrk_image->height;
			wrk_image->b.x = max - 1;
			wrk_image->b.y = max - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
		if (true_angle == 270) {
			wrk_image->b.x = wrk_image->height - 1;
			wrk_image->b.y = wrk_image->width - 1;
			wrk_image->height = max;
			wrk_image->width = max;
			*wrk_image = crop_image(*wrk_image);
		}
	}
}

//functie folosita la comanda ROTATEs
void rotate_selection(pixel_map *wrk_image, char *parameters)
{
	int angle, true_angle, i, j, **tmp;
	int max = my_max(wrk_image->height, wrk_image->width);
	int selection_shape = verify_selection_shape(*wrk_image);
	point a, b;

	int height = wrk_image->height;
	int width = wrk_image->width;

	int val = sscanf(parameters, "%d", &angle);
	if (val != 1) {
		printf("Invalid command\n");
		return;
	}
	if ((angle % 90) || angle > 360 || angle < -360) {
		printf("Unsupported rotation angle\n");
		return;
	}

	true_angle = acutal_agle(angle);
	if (selection_shape == 1 || !true_angle) {
		for (i = 0; i < true_angle / 90; i++)
			for (j = 0; j < wrk_image->depht; j++)
				rotate_90(wrk_image->a, wrk_image->b, wrk_image->map[j]);
	} else {
		a.x = 0;
		a.y = 0;
		b.x = max - 1;
		b.y = max - 1;
		for (j = 0; j < wrk_image->depht; j++) {
			tmp = all_mat(max, max);
			if (!tmp) {
				printf("Dinamic allocation failed\n");
				return;
			}
			mat_cop(wrk_image->map[j], tmp, height, width);
			for (i = 0; i < true_angle / 90; i++)
				rotate_90(a, b, tmp);
			free_mat(wrk_image->map[j], wrk_image->height);
			wrk_image->map[j] = tmp;
		}
		crop_rotated_image(wrk_image, true_angle, max);
	}
	printf("Rotated %d\n", angle);
}

void load_function(pixel_map *wrk_image, char *parameters)
{
	FILE *input_image;
	char tmp[4];

	free_map(*wrk_image);
	wrk_image->map = NULL;
	if (!parameters)
		printf("Invalid command\n");
	input_image = fopen(parameters, "rb");

	if (!input_image) {
		printf("Failed to load %s\n", parameters);
	} else {
		ignore_comments(input_image);

		fread(tmp, sizeof(char), 3, input_image);

		if (tmp[1] == '3' || tmp[1] == '6')
			wrk_image->depht = 3;
		else
			wrk_image->depht = 1;

		if (tmp[1] == '5' || tmp[1] == '6')
			wrk_image->type = 'b';
		else
			wrk_image->type = 'a';

		ignore_comments(input_image);

		wrk_image->width = read_num_ascii(input_image);
		ignore_comments(input_image);

		wrk_image->height = read_num_ascii(input_image);
		ignore_comments(input_image);

		wrk_image->intensity = read_num_ascii(input_image);
		wrk_image->a.x = 0;
		wrk_image->a.y = 0;
		wrk_image->b.x = wrk_image->width - 1;
		wrk_image->b.y = wrk_image->height - 1;
		wrk_image->map = load_map(*wrk_image, input_image);
		if (!wrk_image->map)
			printf("Failed to load %s\n", parameters);
		else
			printf("Loaded %s\n", parameters);
		fclose(input_image);
	}
}
