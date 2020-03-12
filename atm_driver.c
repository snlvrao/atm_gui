#include "atm.h"

void init_database(void)
{
	FILE *fptr;
	int len;
	char buffer[10];

	fptr = fopen("database.txt", "r");

	if (fptr == NULL)
    	{
        	printf("File does not exist\n\n") ;
		exit(0);
    	}
	else
	{
		while(fgets(user[new_database].username, 20, fptr) != NULL)
		{
			len = strlen(user[new_database].username);
			if(user[new_database].username[len - 1] == '\n')
   				user[new_database].username[len - 1] = '\0';

			fgets(user[new_database].password, 20, fptr);
			len = strlen(user[new_database].password);
			if(user[new_database].password[len - 1] == '\n')
   				user[new_database].password[len - 1] = '\0';

			fgets(buffer, 10, fptr);
			len = strlen(buffer);
			if(buffer[len - 1] == '\n')
   				buffer[len - 1] = '\0';
			user[new_database].balance = strtoul(buffer, NULL, 0); //Convert string to size_t
			new_database++;
		}
	}
	sort();

	fclose(fptr);
}

int main(int argc,char *argv[])
{
  gtk_init (&argc, &argv);
  init_database();
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "atm_layout.glade", NULL);

  Main_Menu = (GtkWidget *) gtk_builder_get_object (builder, "Main_Menu");

  Username_Main = (GtkWidget *) gtk_builder_get_object (builder, "Username_Main");
  Password_Main = (GtkWidget *) gtk_builder_get_object (builder, "Password_Main");

  GtkWidget *Login_Main_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Login_Main_Btn");

  g_signal_connect (Main_Menu, "delete_event", G_CALLBACK (end_program), NULL);
  g_signal_connect (Login_Main_Btn, "clicked", G_CALLBACK(login), NULL);

  Admin_Portal = (GtkWidget *) gtk_builder_get_object (builder, "Admin_Portal");
  g_signal_connect (Admin_Portal, "delete_event", G_CALLBACK (end_program), NULL);

  User_Portal = (GtkWidget *) gtk_builder_get_object (builder, "User_Portal");
  g_signal_connect (User_Portal, "delete_event", G_CALLBACK (end_program), NULL);

  gtk_widget_show_all (Main_Menu);
  gtk_main ();
}
void hide_widget(GtkWidget *wid, gpointer ptr)
{
  gtk_widget_hide (ptr);
}
void end_program(void)
{
  gtk_main_quit();
}
