#include <gtk/gtk.h>

#define UI_FILE "UI.glade"

/* Callback for the buttons */
void on_button_clicked (GtkButton* button,
                        gpointer user_data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkBuilder *builder;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, UI_FILE, NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "Window"));
    gtk_builder_connect_signals (builder, NULL);
    
    gtk_widget_show (window);

    gtk_main ();
    return 0;
}
