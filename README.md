Copyright Razvan-Constantin Rus 312CAa 2022-2023

## Description
This program is a CLI image editor for PGM and PPM files. It can rotate, crop,
equalize, apply filters, print a histogram and save the results in
either ascii or binary format.

## Usage

1. `LOAD <filename>` - deletes the previously loaded picture and loads a
new one from `<filename>`.

2. `SELECT <x1> <x2> <y1> <y2>` - selects a portion of the picture loaded.

3. `SELECT ALL` - selects the whole picture loaded.

4. `EQUALIZE` - if the loaded picture is a grayscale one, it equalizes the picture.

5. `CROP` - crops the picture, keeping the selected area

6. `APPLY <FILTER>` - applies one of the 4 filters: EDGE, SHARPEN, BLUR,
GAUSSIAN BLUR.

7. `HISTOGRAM <x> <y>` - prints a histogram of the picture loaded, provided
that it is a grayscale one. The histogram is made of `y` bins, while `x` is
the max value of a bin.

8. `ROTATE <angle>` - Only works if the angle is a multiple of 90.

9. `SAVE <filename> [ascii]` - saves the loaded picture into <filename>. The
`ascii` parameter is optional, saving the picture in ASCII format. Otherwise,
the picture is saved in binary format.


