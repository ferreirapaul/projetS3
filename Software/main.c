#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>

#define UI_FILE "UI.glade"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *image = NULL;

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

char* get_ext(const char *s)
{
    char* l = strrchr(s,'.');
    if(!l || l == s)
    { 
        return "";
    }
    return l + 1;
}


void file_choosed(GtkFileChooser* button, gpointer data)
{
    gchar *path;
    GtkBox *pannel;

    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    char* ext = get_ext(path);
    
    if(strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || 
        strcmp(ext, "jpeg") == 0 ||strcmp(ext, "bmp") == 0)
    {
        SDL_Surface* temp = IMG_Load(path);
        SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
        SDL_FreeSurface(temp);
        GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w,surface->h);
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
        pixbuf = gdk_pixbuf_scale_simple(pixbuf,400,400,GDK_INTERP_BILINEAR);

        image = gtk_builder_get_object (builder, "img");
        gtk_image_set_from_pixbuf(GTK_IMAGE(image),pixbuf);
        gtk_widget_set_visible(image,TRUE);
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "The selected file isn't a image");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
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
