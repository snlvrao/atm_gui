#include "atm.h"

void admin_portal(void)
{
  gtk_widget_hide (Main_Menu);
  gtk_widget_show_all (Admin_Portal);

  GtkWidget *Add_Account_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Add_Account_Btn");
  GtkWidget *Display_Accounts_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Display_Accounts_Btn");
  GtkWidget *Search_Account_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Search_Account_Btn");
  GtkWidget *Admin_Logout_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Admin_Logout_Btn");
  GtkWidget *Confirm_Add_Account_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Add_Account_Btn");
  GtkWidget *Find_Account_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Find_Account_Btn");

  if(init_admin == 0)
    {
      g_signal_connect (Add_Account_Btn, "clicked", G_CALLBACK(display_add_account), NULL);
      g_signal_connect (Display_Accounts_Btn, "clicked", G_CALLBACK(display), NULL);
      g_signal_connect (Search_Account_Btn, "clicked", G_CALLBACK(display_search), NULL);
      g_signal_connect (Admin_Logout_Btn, "clicked", G_CALLBACK(logout), Admin_Portal);
      g_signal_connect (Confirm_Add_Account_Btn, "clicked", G_CALLBACK(add_account), NULL);
      g_signal_connect (Find_Account_Btn, "clicked", G_CALLBACK(search), NULL);

      init_admin = 1;
    }
  display_add_account();
}
void display_add_account(void)
{
  GtkStack *Admin_Stack = (GtkStack *) gtk_builder_get_object (builder, "Admin_Stack");
  GtkWidget *Add_Account = (GtkWidget *) gtk_builder_get_object (builder, "Add_Account");

  GtkWidget *Username_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Username_Admin");
  GtkWidget *Password_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Password_Admin");
  GtkWidget *Confirm_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Admin");
  GtkWidget *Balance_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Balance_Admin");

  gtk_entry_set_text(GTK_ENTRY(Username_Admin), "");
  gtk_entry_set_text(GTK_ENTRY(Password_Admin), "");
  gtk_entry_set_text(GTK_ENTRY(Confirm_Admin), "");
  gtk_entry_set_text(GTK_ENTRY(Balance_Admin), "");

  gtk_stack_set_visible_child (Admin_Stack, Add_Account);
}
void add_account(void)
{
  char username[20], password[20], confirm[20];
  size_t amount;
  int flag = 0, i;

  GtkWidget *Username_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Username_Admin");
  GtkWidget *Password_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Password_Admin");
  GtkWidget *Confirm_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Admin");
  GtkWidget *Balance_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Balance_Admin");

  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Username_Admin));
  strcpy(username, buffer_ptr);
  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Password_Admin));
  strcpy(password, buffer_ptr);
  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Confirm_Admin));
  strcpy(confirm, buffer_ptr);
  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Balance_Admin));
  amount = strtoul(buffer_ptr, NULL, 0);

	for(i = 0; i < new_database; i++)
	{
		if(strcmp(username, user[i].username) == 0)
		{
      GtkWidget *dialog4 = (GtkWidget *) gtk_builder_get_object (builder, "dialog4");
      GtkWidget *close4 = (GtkWidget *) gtk_builder_get_object (builder, "close4");
      g_signal_connect (close4, "clicked", G_CALLBACK (hide_widget), dialog4);
      gtk_widget_show_all (dialog4);
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
    if(strcmp(password, confirm) != 0)
    {
      GtkWidget *dialog2 = (GtkWidget *) gtk_builder_get_object (builder, "dialog2");
      GtkWidget *close2 = (GtkWidget *) gtk_builder_get_object (builder, "close2");
      g_signal_connect (close2, "clicked", G_CALLBACK (hide_widget), dialog2);
      gtk_widget_show_all (dialog2);
    }
    else
      {
        strcpy(user[new_database].username, username);
        strcpy(user[new_database].password, password);
        user[new_database].balance = amount;
        new_database++;
        flag = 0;

        GtkWidget *dialog3 = (GtkWidget *) gtk_builder_get_object (builder, "dialog3");
        GtkWidget *close3 = (GtkWidget *) gtk_builder_get_object (builder, "close3");
        g_signal_connect (close3, "clicked", G_CALLBACK (hide_widget), dialog3);
        gtk_widget_show_all (dialog3);
      }
	}
}
void display(void)
{
  int i;

  GtkStack *Admin_Stack = (GtkStack *) gtk_builder_get_object (builder, "Admin_Stack");
  GtkWidget *Display_Accounts = (GtkWidget *) gtk_builder_get_object (builder, "Display_Accounts");

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Display_Accounts));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  for(i = 0; i < new_database; i++)
		{
      char temp[100];
      sprintf(temp, "Username : %s\n", user[i].username);
      gtk_text_buffer_insert (buffer, &end, temp, -1);
      sprintf(temp, "Account Balance : %lu\n\n", user[i].balance);
      gtk_text_buffer_insert (buffer, &end, temp, -1);
		}

  gtk_stack_set_visible_child (Admin_Stack, Display_Accounts);
}
void display_search(void)
{
  GtkStack *Admin_Stack = (GtkStack *) gtk_builder_get_object (builder, "Admin_Stack");
  GtkWidget *Display_Search_Result = (GtkWidget *) gtk_builder_get_object (builder, "Display_Search_Result");
  GtkWidget *Search_Account = (GtkWidget *) gtk_builder_get_object (builder, "Search_Account");

  GtkWidget *Search_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Search_Admin");
  gtk_entry_set_text(GTK_ENTRY(Search_Admin), "");

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Display_Search_Result));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  gtk_stack_set_visible_child (Admin_Stack, Search_Account);
}
void search(void)
{
  int i, flag = 0;
  char *check;
  char keyword[20];

  GtkWidget *Display_Search_Result = (GtkWidget *) gtk_builder_get_object (builder, "Display_Search_Result");
  GtkWidget *Search_Admin = (GtkWidget *) gtk_builder_get_object (builder, "Search_Admin");

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Display_Search_Result));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Search_Admin));
  strcpy(keyword, buffer_ptr);

  for(i = 0; i < new_database; i++)
	{
		check = strcasestr(user[i].username, keyword);
		if (check != NULL) /* Expected character is found */
		{
      char temp[100];
      sprintf(temp, "Username : %s\n", user[i].username);
      gtk_text_buffer_insert (buffer, &end, temp, -1);
      sprintf(temp, "Account Balance : %lu\n\n", user[i].balance);
      gtk_text_buffer_insert (buffer, &end, temp, -1);
			flag = 1;
		}
	}
	if(flag == 0)
    {
      gtk_text_buffer_insert (buffer, &end, "No Match Found\n\n", -1);
    }
}
void sort(void)
{
	int i, j;
	size_t temp_balance;
	char temp_name[20];

	for(i = 0; i < new_database - 1; i++)
		for(j = 0; j < new_database - i - 1; j++)
		{
			if(strcmp(user[j].username, user[j + 1].username) > 0)
			{
				strcpy(temp_name, user[j].username);
				strcpy(user[j].username, user[j + 1].username);
				strcpy(user[j + 1].username, temp_name);
				temp_balance = user[j].balance;
				user[j].balance = user[j + 1].balance;
				user[j + 1].balance = temp_balance;
			}
		}
}
