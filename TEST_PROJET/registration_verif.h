#ifndef REGISTRATION_VERIF_H_INCLUDED
#define REGISTRATION_VERIF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nameVerif(const char *name);
int emailVerif(const char *email);
int addressVerif(const char *address);
int phoneNumberVerif(const char *phoneNumber);

#endif // REGISTRATION_VERIF_H_INCLUDED
