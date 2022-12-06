#pragma once

#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>

#define UI_FILE "UI.glade"

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, 
                guchar blue, guchar alpha);
GdkPixbuf *load_pix(SDL_Surface *surface);
char* get_ext(const char *s);
void rotate(double degree);
void file_choosed(GtkFileChooser* button, gpointer data);
void rotate(double degree);
void init_rotate(GtkSpinButton *self, gpointer user_data);
void init_soft(int argc, char *argv[]);
