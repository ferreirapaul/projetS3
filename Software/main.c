#include <stdio.h>
#include <gtk/gtk.h>

#define UI_FILE "UI.glade"

void file_choosed(GtkFileChooser* button, gpointer data)
{
    GtkBuilder *build;
    GtkWidget *window;
    GtkImage *img;

    build = GTK_BUILDER(data);
    window = GTK_WIDGET (gtk_builder_get_object (build, "Window"));
    img = GTK_IMAGE(gtk_builder_get_object (build, "img"));

    gchar *path;
    path = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(button));
    gtk_image_set_from_file(img, path);

    gtk_widget_show(window);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkBuilder *builder;
    GtkImage *picture;
    GtkFileChooserButton *file;

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
