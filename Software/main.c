#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#define UI_FILE "UI.glade"

GtkBuilder *builder;
GtkWidget *window;

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, 
                guchar blue, guchar alpha)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
  g_assert (n_channels == 4);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
  p[3] = alpha;
}


void file_choosed(GtkFileChooser* button, gpointer data)
{
    GtkImage *img;

    gchar *path;
    path = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(button));
    SDL_Surface *surface = load_image((char) path);
    GdkPixbuf *pixbuf = gtk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w,surface->h);
    Uint32* pixels = surface->pixels;

    Uint8 r,g,b;    
    for(size_t j = 0; j < surface->h; j++)
    {
        for(size_t i = 0; i < surface->w; i++)
        {
            SDL_GetRGB(pixels[j*surface->w+i],surface->format,&r,&g,&b);
            put_pixel(pixbuf,i,j,r,g,b,255);
        }
    }


    gtk_widget_show(window);
}

int main(int argc, char *argv[])
{
    GtkFileChooserButton *file;

    SDL_Init(SDL_INIT_VIDEO);
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, UI_FILE, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "Window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object (builder, "file"));
    g_signal_connect(file, "file-set", G_CALLBACK(file_choosed), builder); 
      

    gtk_widget_show (window);

    gtk_main ();
    return 0;
}
