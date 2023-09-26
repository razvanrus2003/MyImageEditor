// Copyright Razvan-Constantin Rus 312CAa 2022-2023

#pragma once

//structura punct
typedef struct {
	int x;
	int y;
} point;

//structura imagine
typedef struct {
	//pixel map-ul
	int ***map;
	//latimea
	int width;
	//inaltimea
	int height;
	//1 daca imaginea este greyscale sau 4 daca este RGB
	int depht;
	int intensity;
	//ascii sau binari
	char type;
	//colutul din stanga sus respectiv dreapta jos
	point a;
	point b;
} pixel_map;

//structura care retine datele pt histograma
typedef struct {
	//vectorul de frecventa
	long *graph;
	//numarul de binuri
	int size;
	//valoarea maxima a unui element din vector
	long max_val;
	//nr maxim de stelute
	int ratio;
} histogram;
