#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //For local time function
#define maxdatabase 10
#define maxpass 3

GtkWidget *Main_Menu, *Admin_Portal, *User_Portal;
GtkWidget *Username_Main, *Password_Main;
GtkBuilder *builder;
const char *buffer_ptr;
int init_admin, init_user;

int new_database, database_index;
extern size_t atm_balance;
struct database
{
	char username[20];
	char password[20];
	size_t balance;
	int first;
};
struct database user[maxdatabase];
struct passbook
{
	char time[maxpass][30];
  char transaction[maxpass][30];
	size_t amount[maxpass];
	size_t balance[maxpass];
};
struct passbook user_pass[maxdatabase];

void init_database(void);

void admin_portal(void);
void display_add_account(void);
void add_account(void);
void display(void);
void display_search(void);
void search(void);
void sort(void);

void user_portal(void);
void display_withdraw(void);
void withdraw(void);
void display_deposit(void);
void deposit(void);
void balance(void);
void display_pin_change(void);
void pin_change(void);
void passbook_display(void);
void passbook_entry(void);

void login(void);
void logout(GtkWidget *, gpointer);
void hide_widget(GtkWidget *, gpointer);
void end_program(void);
