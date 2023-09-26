// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dependency.h"
#include "utilities.h"
#include "functions.h"

int main(void)
{
	char *command, *parameters;
	pixel_map wrk_image;
	wrk_image.map = NULL;

	command = scan_word();
	parameters = valid_command(command);

	while (strcmp(command, "EXIT")) {
		if (!strcmp(command, "LOAD"))
			load_function(&wrk_image, parameters);

		if (!strcmp(command, "SAVE")) {
			if (!wrk_image.map)
				printf("No image loaded\n");
			else
				save(wrk_image, parameters);
		}

		if (!strcmp(command, "SELECT")) {
			if (!wrk_image.map)
				printf("No image loaded\n");
			else
				select_wrk_area(&wrk_image, parameters);
		}

		if (!strcmp(command, "HISTOGRAM"))
			histogram_function(wrk_image, parameters);

		if (!strcmp(command, "EQUALIZE"))
			equalize_image(wrk_image);

		if (!strcmp(command, "CROP")) {
			if (!wrk_image.map) {
				printf("No image loaded\n");
			} else {
				wrk_image = crop_image(wrk_image);
				printf("Image cropped\n");
			}
		}

		if (!strcmp(command, "APPLY")) {
			if (!wrk_image.map) {
				printf("No image loaded\n");
			} else {
				if (!parameters) {
					printf("Invalid command\n");
				} else {
					if (wrk_image.depht == 1)
						printf("Easy, Charlie Chaplin\n");
					else
						apply_kernel(wrk_image, parameters);
				}
			}
		}

		if (!strcmp(command, "ROTATE")) {
			if (!wrk_image.map) {
				printf("No image loaded\n");
			} else {
				if (!verify_selection_shape(wrk_image))
					printf("The selection must be square\n");
				else
					rotate_selection(&wrk_image, parameters);
			}
		}
		free(command);
		command = scan_word();
		parameters = valid_command(command);
	}

	if (!wrk_image.map)
		printf("No image loaded\n");
	free_map(wrk_image);
	free(command);

	return 0;
}
