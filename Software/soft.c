#include "soft.h"
#include "../Solver/solver.h"
#include "../NeuralNetwork/network.h"
#include "build_result.h"

GtkBuilder *builder;
GtkWidget *window;
GtkImage *image = NULL;
SDL_Surface *surface = NULL;
SDL_Surface *res = NULL;

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

GdkPixbuf *load_pix(SDL_Surface *surface)
{   
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w
                *1.5,surface->h*1.5);
    Uint32* pixels = surface->pixels;
    Uint8 r,g,b;    

    for(size_t j = 0; j < (size_t) surface->h; j++)
    {
        for(size_t i = 0; i < (size_t) surface->w; i++)
        {
            SDL_GetRGB(pixels[j*surface->w+i],surface->format,&r,&g,&b);
            put_pixel(pixbuf,i+surface->w/4,j+surface->h/4,r,g,b,255);
        }
    }
    return pixbuf;
}

void load_img(SDL_Surface *surface)
{
    GdkPixbuf *pixbuf = load_pix(surface);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf,400,400,GDK_INTERP_BILINEAR);

    image = GTK_IMAGE(gtk_builder_get_object (builder, "img"));
    gtk_image_set_from_pixbuf(image,pixbuf);
    gtk_widget_set_visible(GTK_WIDGET(image),TRUE);
    g_object_unref(pixbuf);
}

void weight_choosed(GtkFileChooser *button, gpointer data)
{
    gchar *path;

    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    char* ext = get_ext(path);
    struct Network *net = data;
    
    if(strcmp(ext, "weights") == 0)
    {
        load_weights(path, net->networkHidden, net->networkOut);
        net->isInit = 1;
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "The selected file isn't in the good format");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    g_free(path);
}


void file_choosed(GtkFileChooser *button)
{
    gchar *path;

    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    char* ext = get_ext(path);
    
    if(strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || 
        strcmp(ext, "jpeg") == 0 ||strcmp(ext, "bmp") == 0)
    {
        SDL_Surface* temp = IMG_Load(path);
        surface = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGB888,0);
        SDL_FreeSurface(temp);
        image = GTK_IMAGE(gtk_builder_get_object (builder, "img"));

        load_img(surface);
    }
    else
    {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "The selected file isn't a image");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    g_free(path);
}


void rotate(double degree)
{
    float sinAngle,cosAngle;
    int x,y,wHalf,hHalf,xt,yt;
    //int w = sqrt(surface->h*surface->h + surface->w*surface->w);
    Uint8 r,g,b;    
    Uint32* pixels = surface->pixels;

    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8,surface->w*1.5
            ,surface->h*1.5);
    wHalf = surface->w / 2;
    hHalf = surface->h / 2;
    degree = degree * (G_PI/180.0);
    sinAngle = sin(degree);  
    cosAngle = cos(degree);

    for(size_t j = 0; j < (size_t) surface->h; j++)
    {
        for(size_t i = 0; i < (size_t) surface->w; i++)
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

void init_rotate(GtkSpinButton *self)
{
    rotate(gtk_spin_button_get_value(self));
}

void free_grid(char **grid)
{
    for(size_t i = 0; i < 9; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

void process(struct Network *net)
{
    SDL_Surface **training = load_training();
    char input[9][SIZE];
    createInput(input,training);
    char x;
    printf("Network :\n");
    for(size_t i = 0; i < 9; i++)
    {
        //printChar(input[i]);
        x = network(input[i], net->networkHidden, net->networkOut, net->isInit);
        if(!net->isInit)
        {
            net->isInit = 1;
        }
        printf("Resultat : In: %li, Out : %i\n",i+1,x);
    }

    free_numbers(training);
}

void start_process(GtkButton *self,gpointer data)
{
    if(strcmp(gtk_button_get_label(self),"Save"))
    {
        char **grid = get_grid("../Solver/grid_00");
        solve(grid);
        res = build_result(grid);
        load_img(res);
        free_grid(grid);
        gtk_button_set_label(self, "Save");
        struct Network *net = data;
        process(net);
    }
    else
    {
        IMG_SavePNG(res, "grid_res.png");
    }
}

void save_process(GtkButton *self, gpointer data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(self),FALSE);
    struct Network *net = data;
    save_weights(net->networkHidden, net->networkOut);
    net->isInit = 1;
}

void init_soft(int argc, char *argv[])
{
    struct Neuron networkHidden[9];
    struct Neuron networkOut[9];

    struct Network net = {networkHidden, networkOut,0};

    GtkFileChooserButton *file;
    GtkFileChooserButton *weight;
    GtkSpinButton *buttonSpin;
    GtkButton *buttonStart;
    GtkButton *buttonSave;

    SDL_Init(SDL_INIT_VIDEO);
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, UI_FILE, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "Window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object (builder, "file"));
    g_signal_connect(file, "file-set", G_CALLBACK(file_choosed), NULL); 

    weight = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object (builder, 
                "LoadNet"));
    g_signal_connect(weight, "file-set", G_CALLBACK(weight_choosed), &net); 
    
    
    buttonSpin = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"spin"));
    g_signal_connect(buttonSpin, "value-changed", G_CALLBACK(init_rotate),
            NULL);

    buttonStart = GTK_BUTTON(gtk_builder_get_object(builder,"start"));
    g_signal_connect(buttonStart, "clicked", G_CALLBACK(start_process),
            &net);

    buttonSave = GTK_BUTTON(gtk_builder_get_object(builder,"SaveNet"));
    g_signal_connect(buttonSave, "clicked", G_CALLBACK(save_process),
            &net);

    gtk_widget_show (window);

    gtk_main ();

    g_object_unref(builder);

    if(net.isInit)
    {
        free_neuron(net.networkHidden);
        free_neuron(net.networkOut);
    }

    if(surface != NULL)
    {
        SDL_FreeSurface(surface);
    }
    if(res != NULL)
    {
        SDL_FreeSurface(res);
    }
}
