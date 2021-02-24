#ifndef REGISTRATION_H_INCLUDED
#define REGISTRATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include "qrcodegen.h"
#include <gtk/gtk.h>
#include "qr.h"
#include "registration_verif.h"

typedef struct {
    const char *server;
    const char *dataBase;
    const char *user;
    const char *password;
    int port;
    const char *site;
}infosDB;

typedef struct {
    gpointer lastName;
    gpointer firstName;
    gpointer email;
    gpointer phoneNumber;
    gpointer city;
    gpointer address;
}Inputs;

typedef struct {
    const char *lastName;
    const char *firstName;
    const char *email;
    const char *phoneNumber;
    const char *city;
    const char *address;
    int userID;
    char *password;
    char *PATH;
}values;

void recoveryEntry(GtkButton *button1, Inputs *In);
void registerVerif(values *value, infosDB *infoDB);
void registerProvider(values *value, infosDB *infoDB);
int return_last_id(const char *table, const char *tableID, infosDB *infoDB);
void recoveryInfoDB(infosDB *infoDB);

#endif // REGISTRATION_H_INCLUDED
