#include "atm.h"
size_t atm_balance = 100000;

void user_portal(void)
{
  gtk_widget_hide (Main_Menu);
  gtk_widget_show_all (User_Portal);

  GtkStack *User_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Stack");
  GtkWidget *Change_Default_Password = (GtkWidget *) gtk_builder_get_object (builder, "Change_Default_Password");
  GtkWidget *User_Menu = (GtkWidget *) gtk_builder_get_object (builder, "User_Menu");

  GtkWidget *Save_Password_Default_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Save_Password_Default_Btn");
  GtkWidget *Withdraw_Cash_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Withdraw_Cash_Btn");
  GtkWidget *Deposit_Cash_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Deposit_Cash_Btn");
  GtkWidget *Passbook_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Passbook_Btn");
  GtkWidget *Change_Pin_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Change_Pin_Btn");
  GtkWidget *User_Logout_Btn = (GtkWidget *) gtk_builder_get_object (builder, "User_Logout_Btn");
  GtkWidget *Confirm_Withdraw_Cash_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Withdraw_Cash_Btn");
  GtkWidget *Confirm_Deposit_Cash_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Deposit_Cash_Btn");
  GtkWidget *Save_Password_Btn = (GtkWidget *) gtk_builder_get_object (builder, "Save_Password_Btn");

  if(init_user == 0)
    {
      g_signal_connect(Save_Password_Default_Btn, "clicked", G_CALLBACK (pin_change), NULL);
      g_signal_connect(Withdraw_Cash_Btn, "clicked", G_CALLBACK (display_withdraw), NULL);
      g_signal_connect(Deposit_Cash_Btn, "clicked", G_CALLBACK (display_deposit), NULL);
      g_signal_connect(Passbook_Btn, "clicked", G_CALLBACK (passbook_display), &database_index);
      g_signal_connect(Change_Pin_Btn, "clicked", G_CALLBACK (display_pin_change), NULL);
      g_signal_connect(User_Logout_Btn, "clicked", G_CALLBACK (logout), User_Portal);
      g_signal_connect(Confirm_Withdraw_Cash_Btn, "clicked", G_CALLBACK (withdraw), NULL);
      g_signal_connect(Confirm_Deposit_Cash_Btn, "clicked", G_CALLBACK (deposit), NULL);
      g_signal_connect(Save_Password_Btn, "clicked", G_CALLBACK (pin_change), NULL);
      init_user = 1;
    }
  if(user[database_index].first == 0)
    {
      GtkWidget *New_Password_Default = (GtkWidget *) gtk_builder_get_object (builder, "New_Password_Default");
      GtkWidget *Confirm_Password_Default = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Password_Default");

      gtk_entry_set_text(GTK_ENTRY(New_Password_Default), "");
      gtk_entry_set_text(GTK_ENTRY(Confirm_Password_Default), "");

      gtk_stack_set_visible_child (User_Stack, Change_Default_Password);
    }
  else
    {
      gtk_stack_set_visible_child (User_Stack, User_Menu);
      display_withdraw();
    }
}
void display_withdraw(void)
{
  GtkStack *User_Menu_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Menu_Stack");
  GtkWidget *Withdraw_Menu = (GtkWidget *) gtk_builder_get_object (builder, "Withdraw_Menu");

  GtkSpinButton *Withdraw_Input = (GtkSpinButton *) gtk_builder_get_object (builder, "Withdraw_Input");
  GtkWidget *Withdraw_Cash_Display = (GtkWidget *) gtk_builder_get_object (builder, "Withdraw_Cash_Display");

  gtk_spin_button_set_value (Withdraw_Input, 100);
  gtk_spin_button_set_range (Withdraw_Input, 100, 50000);
  gtk_spin_button_set_increments (Withdraw_Input, 100, 100);

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Withdraw_Cash_Display));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  gtk_stack_set_visible_child (User_Menu_Stack, Withdraw_Menu);
}
void withdraw(void)
{
  size_t amount = 0;
  time_t t = time(NULL);
  char temp[100];

  GtkSpinButton *Withdraw_Input = (GtkSpinButton *) gtk_builder_get_object (builder, "Withdraw_Input");
  GtkWidget *Withdraw_Cash_Display = (GtkWidget *) gtk_builder_get_object (builder, "Withdraw_Cash_Display");

  amount = (size_t) gtk_spin_button_get_value (Withdraw_Input);

  if(amount > user[database_index].balance)
    {
      GtkWidget *dialog7 = (GtkWidget *) gtk_builder_get_object (builder, "dialog7");
      GtkWidget *close7 = (GtkWidget *) gtk_builder_get_object (builder, "close7");
      g_signal_connect (close7, "clicked", G_CALLBACK (hide_widget), dialog7);
      gtk_widget_show_all (dialog7);
    }
	else if(amount > atm_balance)
    {
      GtkWidget *dialog8 = (GtkWidget *) gtk_builder_get_object (builder, "dialog8");
      GtkWidget *close8 = (GtkWidget *) gtk_builder_get_object (builder, "close8");
      g_signal_connect (close8, "clicked", G_CALLBACK (hide_widget), dialog8);
      gtk_widget_show_all (dialog8);
    }
	else
	{
		user[database_index].balance -= amount;
		atm_balance -= amount;

		passbook_entry();
		strcpy(user_pass[database_index].transaction[0], "Withdrawal");
		user_pass[database_index].amount[0] = amount;
		user_pass[database_index].balance[0] = user[database_index].balance;
		strcpy(user_pass[database_index].time[0], asctime(localtime(&t)));

    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Withdraw_Cash_Display));
    GtkTextIter start, end;

    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);

    sprintf(temp, "Cash Withdrawn : %lu\n", amount);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
    sprintf(temp, "Account Balance : %lu\n", user[database_index].balance);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
    sprintf(temp, "Transaction Time : %s\n\n", asctime(localtime(&t)));
    gtk_text_buffer_insert (buffer, &end, temp, -1);
	}
}
void display_deposit(void)
{
  GtkStack *User_Menu_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Menu_Stack");
  GtkWidget *Deposit_Menu = (GtkWidget *) gtk_builder_get_object (builder, "Deposit_Menu");

  GtkSpinButton *Deposit_Input = (GtkSpinButton *) gtk_builder_get_object (builder, "Deposit_Input");
  GtkWidget *Deposit_Cash_Display = (GtkWidget *) gtk_builder_get_object (builder, "Deposit_Cash_Display");

  gtk_spin_button_set_value (Deposit_Input, 100);
  gtk_spin_button_set_range (Deposit_Input, 100, 50000);
  gtk_spin_button_set_increments (Deposit_Input, 100, 100);

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Deposit_Cash_Display));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  gtk_stack_set_visible_child (User_Menu_Stack, Deposit_Menu);
}
void deposit(void)
{
  size_t amount = 0;
  time_t t = time(NULL);
  char temp[100];

  GtkSpinButton *Deposit_Input = (GtkSpinButton *) gtk_builder_get_object (builder, "Deposit_Input");
  GtkWidget *Deposit_Cash_Display = (GtkWidget *) gtk_builder_get_object (builder, "Deposit_Cash_Display");

  amount = (size_t) gtk_spin_button_get_value (Deposit_Input);

	user[database_index].balance += amount;
	atm_balance += amount;

	passbook_entry();
	strcpy(user_pass[database_index].transaction[0], "Deposit");
	user_pass[database_index].amount[0] = amount;
	user_pass[database_index].balance[0] = user[database_index].balance;
	strcpy(user_pass[database_index].time[0], asctime(localtime(&t)));

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Deposit_Cash_Display));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  sprintf(temp, "Cash Deposited : %lu\n", amount);
  gtk_text_buffer_insert (buffer, &end, temp, -1);
  sprintf(temp, "Account Balance : %lu\n", user[database_index].balance);
  gtk_text_buffer_insert (buffer, &end, temp, -1);
  sprintf(temp, "Transaction Time : %s\n", asctime(localtime(&t)));
  gtk_text_buffer_insert (buffer, &end, temp, -1);
}
void passbook_entry(void)
{
	int i;

	for(i = maxpass - 1; i > 0; i--)
	{
		strcpy(user_pass[database_index].transaction[i], user_pass[database_index].transaction[i - 1]);
		user_pass[database_index].amount[i] = user_pass[database_index].amount[i - 1];
		user_pass[database_index].balance[i] = user_pass[database_index].balance[i - 1];
		strcpy(user_pass[database_index].time[i], user_pass[database_index].time[i - 1]);
	}
}
void passbook_display(void)
{
  int i;

  GtkStack *User_Menu_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Menu_Stack");
  GtkWidget *Passbook_Menu = (GtkWidget *) gtk_builder_get_object (builder, "Passbook_Menu");

  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Passbook_Menu));
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  gtk_text_buffer_delete (buffer, &start, &end);

  for(i = 0; i < maxpass; i++)
	{
    char temp[100];
	  sprintf(temp, "Transaction Type : %s\n", user_pass[database_index].transaction[i]);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
    sprintf(temp, "Amount : %lu\n", user_pass[database_index].amount[i]);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
    sprintf(temp, "Account Balance : %lu\n", user_pass[database_index].balance[i]);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
    sprintf(temp, "Transaction Time : %s\n\n", user_pass[database_index].time[i]);
    gtk_text_buffer_insert (buffer, &end, temp, -1);
	}
  gtk_stack_set_visible_child (User_Menu_Stack, Passbook_Menu);
}
void display_pin_change(void)
{
  GtkStack *User_Menu_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Menu_Stack");
  GtkWidget *Change_Pin_Menu = (GtkWidget *) gtk_builder_get_object (builder, "Change_Pin_Menu");

  GtkWidget *User_Menu = (GtkWidget *) gtk_builder_get_object (builder, "User_Menu");
  GtkWidget *New_Password = (GtkWidget *) gtk_builder_get_object (builder, "New_Password");
  GtkWidget *Confirm_Password = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Password");

  gtk_entry_set_text(GTK_ENTRY(New_Password), "");
  gtk_entry_set_text(GTK_ENTRY(Confirm_Password), "");

  gtk_stack_set_visible_child (User_Menu_Stack, Change_Pin_Menu);
}
void pin_change(void)
{
	char pwd[20], confirm[20];

  GtkStack *User_Stack = (GtkStack *) gtk_builder_get_object (builder, "User_Stack");
  GtkWidget *Change_Default_Password = (GtkWidget *) gtk_builder_get_object (builder, "Change_Default_Password");
  GtkWidget *User_Menu = (GtkWidget *) gtk_builder_get_object (builder, "User_Menu");

  GtkWidget *New_Password_Default = (GtkWidget *) gtk_builder_get_object (builder, "New_Password_Default");
  GtkWidget *Confirm_Password_Default = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Password_Default");
  GtkWidget *New_Password = (GtkWidget *) gtk_builder_get_object (builder, "New_Password");
  GtkWidget *Confirm_Password = (GtkWidget *) gtk_builder_get_object (builder, "Confirm_Password");

  if(user[database_index].first == 0)
    {
      buffer_ptr = gtk_entry_get_text (GTK_ENTRY (New_Password_Default));
      strcpy(pwd, buffer_ptr);
      buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Confirm_Password_Default));
      strcpy(confirm, buffer_ptr);
    }
  else
    {
      buffer_ptr = gtk_entry_get_text (GTK_ENTRY (New_Password));
      strcpy(pwd, buffer_ptr);
      buffer_ptr = gtk_entry_get_text (GTK_ENTRY (Confirm_Password));
      strcpy(confirm, buffer_ptr);
    }

	if(strcmp(confirm, pwd) == 0)
	{
		strcpy(user[database_index].password, pwd);

    GtkWidget *dialog6 = (GtkWidget *) gtk_builder_get_object (builder, "dialog6");
    GtkWidget *close6 = (GtkWidget *) gtk_builder_get_object (builder, "close6");
    g_signal_connect (close6, "clicked", G_CALLBACK (hide_widget), dialog6);
    gtk_widget_show_all (dialog6);

    gtk_stack_set_visible_child (User_Stack, User_Menu);
    if(user[database_index].first == 0)
      {
        display_withdraw();
      }
    user[database_index].first = 1;
	}
	else
  {
		GtkWidget *dialog2 = (GtkWidget *) gtk_builder_get_object (builder, "dialog2");
    GtkWidget *close2 = (GtkWidget *) gtk_builder_get_object (builder, "close2");
    g_signal_connect (close2, "clicked", G_CALLBACK (hide_widget), dialog2);
    gtk_widget_show_all (dialog2);
	}
}
