#include "atm.h"

void login(void)
{
  static int admin_count = 0, user_count[maxdatabase] = {0};
  int i, flag = 0;
  char pwd[20], usr[20];

  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Username_Main));
  strcpy(usr, buffer_ptr);

  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Password_Main));
  strcpy(pwd, buffer_ptr);

  gtk_entry_set_text(GTK_ENTRY(Password_Main), "");

  if(strcmp(usr, "admin") == 0)
    {
      if(admin_count < 2)
        {
          if(strcmp(pwd, "admin") == 0)
            {
              admin_count = 0;
              admin_portal();
            }
          else
            {
              admin_count++;
              gtk_entry_set_text(GTK_ENTRY(Password_Main), "");
            }
        }
      else
        {
          GtkWidget *dialog1 = (GtkWidget *) gtk_builder_get_object (builder, "dialog1");
          GtkWidget *close1 = (GtkWidget *) gtk_builder_get_object (builder, "close1");
          g_signal_connect (close1, "clicked", G_CALLBACK (hide_widget), dialog1);
          gtk_widget_show_all (dialog1);
        }
    }
  else
    {
      for(i = 0; i < maxdatabase; i++)
	    {
		    if(strcmp(usr, user[i].username) == 0)
		    {
			    database_index = i;
			    flag = 1;
			    break;
		    }
	    }
	    if(flag == 0)
	    {
		    GtkWidget *dialog5 = (GtkWidget *) gtk_builder_get_object (builder, "dialog5");
        GtkWidget *close5 = (GtkWidget *) gtk_builder_get_object (builder, "close5");
        g_signal_connect (close5, "clicked", G_CALLBACK (hide_widget), dialog5);
        gtk_widget_show_all (dialog5);
	    }
      else
      {
        if(user_count[database_index] < 2)
          {
            if(strcmp(pwd, user[database_index].password) == 0)
              {
                user_count[database_index] = 0;
                user_portal();
              }
            else
              {
                user_count[database_index]++;
                gtk_entry_set_text(GTK_ENTRY(Password_Main), "");
              }
          }
        else
          {
            GtkWidget *dialog1 = (GtkWidget *) gtk_builder_get_object (builder, "dialog1");
            GtkWidget *close1 = (GtkWidget *) gtk_builder_get_object (builder, "close1");
            g_signal_connect (close1, "clicked", G_CALLBACK (hide_widget), dialog1);
            gtk_widget_show_all (dialog1);
          }
      }
    }
}
void logout(GtkWidget *wid, gpointer ptr)
{
  gtk_widget_hide (ptr);
  gtk_widget_show_all (Main_Menu);
}
