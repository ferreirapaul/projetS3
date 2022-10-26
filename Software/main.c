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
SDL_Surface *surface = NULL;

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, 
                guchar blue, guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;
    n_channels = gdk_pixbuf_get_n_channels (pixbuf);
    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    if (x<0||x>=height||y < 0 || y >= width )
    {
        return;
    }

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

void rotate(double degree);


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
        surface = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
        SDL_FreeSurface(temp);
        GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w*1.5,surface->h*1.5);
        Uint32* pixels = surface->pixels;
        Uint8 r,g,b;    

        for(size_t j = 0; j < surface->h; j++)
        {
            for(size_t i = 0; i < surface->w; i++)
            {
                SDL_GetRGB(pixels[j*surface->w+i],surface->format,&r,&g,&b);
                put_pixel(pixbuf,i+surface->w/4,j+surface->h/4,r,g,b,255);
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
    g_free(path);
}

void rotate(double degree)
{
    float sinAngle,cosAngle;
    int x,y,wHalf,hHalf,xt,yt;
    int w = sqrt(surface->h*surface->h + surface->w*surface->w);
    Uint8 r,g,b;    
    Uint32* pixels = surface->pixels;

    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w*1.5,surface->h*1.5);
    wHalf = surface->w / 2;
    hHalf = surface->h / 2;
    degree = degree * (G_PI/180.0);
    sinAngle = sin(degree);  
    cosAngle = cos(degree);

    for(size_t j = 0; j < surface->h; j++)
    {
        for(size_t i = 0; i < surface->w; i++)
        {
            SDL_GetRGB(pixels[j*surface->w+i],surface->format,&r,&g,&b);
            xt = i - wHalf;
            yt = j - hHalf;
            x = (cosAngle * xt - sinAngle * yt)+wHalf*1.5;
            y = (sinAngle * xt + cosAngle * yt)+hHalf*1.5;
            put_pixel(pixbuf,x,y,r,g,b,255);
        }
    }

    pixbuf = gdk_pixbuf_scale_simple(pixbuf,400,400,GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image),pixbuf);
}

void init_rotate(GtkSpinButton *self, gpointer user_data)
{
    rotate(gtk_spin_button_get_value(self));
}


int main(int argc, char *argv[])
{
    GtkFileChooserButton *file;
    GtkWidget *buttonSpin;

    SDL_Init(SDL_INIT_VIDEO);
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, UI_FILE, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "Window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object (builder, "file"));
    g_signal_connect(file, "file-set", G_CALLBACK(file_choosed), NULL); 
    
    buttonSpin = gtk_builder_get_object(builder,"spin");
    g_signal_connect(buttonSpin, "value-changed", G_CALLBACK(init_rotate),NULL);

    gtk_widget_show (window);

    gtk_main ();

    g_object_unref(builder);
    g_object_unref(window);
    if(surface != NULL)
    {
        SDL_FreeSurface(surface);
    }
    return 0;
}
