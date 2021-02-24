#include "registration_verif.h"

int nameVerif(const char *name) {

    int lenghtName = 0;
    int space = 0;

    lenghtName = strlen(name);
    if(lenghtName <= 2 || lenghtName >= 51) {
        printf("\nVotre nom, votre prénom et votre ville doivent faire entre 3 et 50 caractères");
        return 1;
    }

    for(int i = 0; i < lenghtName; i++) {
        if((name[i] == ' ' || name[i] == '-' ) && (i-space >= 1)) {
            space = i;
            i++;
        }
        if((name[i] < 'a' || name[i] > 'z') && (name[i] < 'A' || name[i] > 'Z') && (name[i] < 'ü' || name[i] > 'Ü')) {
            printf("\nVotre nom, votre prénom et votre ville ne doivent contenir que des lettres, des espaces ou des -");
            return 1;
        }
    }
    return 0;
}

int emailVerif(const char *email) {

    int lenghtEmail = 0;
    int arobaseVerif = 0;

    lenghtEmail = strlen(email);
    if(lenghtEmail <= 9 || lenghtEmail >= 141) {
        printf("\nVotre email doit faire entre 10 et 140 caractères");
        return 1;
    }

    for(int i = 0; i < lenghtEmail; i++) {
        if(arobaseVerif == 0 && email[i] == '@' && (lenghtEmail-i >= 7) && i >= 2) {
            arobaseVerif++;
        }
        if(arobaseVerif == 1 && email[i] == '.' && (lenghtEmail-i >= 3)) {
            arobaseVerif++;
        }
    }

    if(arobaseVerif != 2) {
        printf("\nVotre email n'est pas valide");
        return 1;
    }

    return 0;
}

int addressVerif(const char *address) {

    int lenghtAddress = 0;
    int streetVerif = 0;
    int space = 0;

    lenghtAddress = strlen(address);
    if(lenghtAddress <= 8 || lenghtAddress >= 141) {
        printf("\nVotre address doit faire entre 9 et 140 caractères");
        return 1;
    }

    for(int i = 0; i < lenghtAddress; i++) {
        if(streetVerif == 0 && address[i] == ' ' && (lenghtAddress-i >= 7) && i <= 4 && i > 0) {
            for(int j = 0; j < i; j++) {
                if(address[j] < '0' || address[j] > '9') {
                    printf("\nVotre numéro d'adresse est incorrecte");
                    return 1;
                }
            }
            space = i;
            for(int k = i+1; k < lenghtAddress; k++) {
                if(address[k] == ' ' && (k-space >= 1)) {
                    space = k;
                    k++;
                }
                if((address[k] < 'a' || address[k] > 'z') && (address[k] < 'A' || address[k] > 'Z') && (address[i] < 'ü' || address[i] > 'Ü')) {
                    printf("\nVotre  adresse doit avoir la syntaxe suivant : \"3 rue des potiers\"");
                    return 1;
                }
            }
            streetVerif++;
        }
    }
    if(streetVerif != 1) {
        printf("\nVotre  adresse doit avoir la syntaxe suivant : \"5 rue des potiers\"");
        return 1;
    }

    return 0;
}

int phoneNumberVerif(const char *phoneNumber) {

    int lenghtPhoneNumber = 0;

    lenghtPhoneNumber = strlen(phoneNumber);
    if(lenghtPhoneNumber != 9) {
        printf("\nVotre numéro de téléphone doit faire 9 caractères");
        return 1;
    }

    for(int i = 0; i < lenghtPhoneNumber; i++) {
        if(phoneNumber[i] < '0' || phoneNumber[i] > '9') {
            printf("\nUn numéro de téléphone ne contient que des chiffres");
            return 1;
        }
    }

    return 0;
}
