# Copyright Razvan-Constantin Rus 312CAa 2022-2023

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c functions.c utilities.c
	$(CC) $(CFLAGS) *.c -o image_editor

pack:
	zip -FSr 312CA_RusRazvan-Constantin_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS) *.o

.PHONY: pack clean
