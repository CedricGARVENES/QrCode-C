#include "registration.h"
#include "registration_verif.h"

void recoveryEntry(GtkButton *button1, Inputs *In) {

    values *entry;
    entry = malloc(sizeof(values));
    if(entry == NULL) {
        exit(0);
    }

    infosDB *infoDB;
    infoDB = malloc(sizeof(infosDB));
    if(infoDB == NULL) {
        printf("Allocation error");
        exit(0);
    }
    recoveryInfoDB(infoDB);

    entry->lastName = gtk_entry_get_text(GTK_ENTRY(In->lastName));
    entry->firstName = gtk_entry_get_text(GTK_ENTRY(In->firstName));
    entry->email = gtk_entry_get_text(GTK_ENTRY(In->email));
    entry->phoneNumber = gtk_entry_get_text(GTK_ENTRY(In->phoneNumber));
    entry->city = gtk_entry_get_text(GTK_ENTRY(In->city));
    entry->address = gtk_entry_get_text(GTK_ENTRY(In->address));
    entry->userID = return_last_id("ServiceProvider","providerID",infoDB);
    entry->PATH = malloc(sizeof(char)*256);
    if(entry->PATH == NULL){
        printf("Allocation error");
        exit(0);
    }
    sprintf(entry->PATH,"qrcode_%s.bmp",entry->email);

    srand (time(NULL));
    entry->password = malloc(sizeof(char)*7);
    entry->password[0] = entry->lastName[0];
    entry->password[1] = entry->firstName[0];
    sprintf(entry->password+2,"%d",rand()%10);
    sprintf(entry->password+3,"%d",rand()%10);
    sprintf(entry->password+4,"%d",rand()%10);
    sprintf(entry->password+5,"%d",rand()%10);
    entry->password[6] = '\0';

    registerVerif(entry,infoDB);

}

void registerVerif(values *value, infosDB *infoDB){

    int testLastName = nameVerif(value->lastName);
    int testFirstName = nameVerif(value->firstName);
    int testEmail = emailVerif(value->email);
    int testPhoneNumber = phoneNumberVerif(value->phoneNumber);
    int testCity = nameVerif(value->city);
    int testAddress = addressVerif(value->address);

    if(testLastName == 0 && testFirstName == 0 && testEmail == 0 && testAddress == 0 && testPhoneNumber == 0 && testCity == 0) {
        registerProvider(value, infoDB);
    }

}

void registerProvider(values *value, infosDB *infoDB){

    char *request = malloc(sizeof(char)*256);
    char *phoneNumberV = malloc(sizeof(char)*11);
    if(request == NULL || phoneNumberV == NULL) {
        printf("Allocation error");
        exit(0);
    }
    strcpy(phoneNumberV,"0");
    strcat(phoneNumberV,value->phoneNumber);

    sprintf(request, "INSERT INTO serviceprovider(providerID,agency,lastName,firstName,email,password,city,address,phoneNumber,qrCode) VALUES ('%d','%s','%s','%s','%s','%s','%s','%s','%s','%s')",value->userID,infoDB->site,value->lastName,value->firstName,value->email,value->password,value->city,value->address,phoneNumberV,value->PATH);
    printf("%s\n",request);

    //Déclaration du pointeur de structure de type MYSQL
    MYSQL mysql;
    //Initialisation de MySQL
    mysql_init(&mysql);
    //Options de connexion
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    //Si la connexion réussie...
    if (mysql_real_connect(&mysql, infoDB->server, infoDB->user, infoDB->password, infoDB->dataBase, infoDB->port, NULL, 0)) {

        if(mysql_query(&mysql, request) != 0) {
            printf("error request\n");
            exit(0);
        }
        doBasicDemo(value->email, value->userID, value->PATH);

    } else {
        printf("Une erreur s'est produite lors de la connexion à la BDD!\n");
    }

    free(request);
    free(phoneNumberV);

}

int return_last_id(const char *table, const char *tableID, infosDB *infoDB) {
    char *request = NULL;
    int id = 0;
    int test_id = 0;
    request = malloc(sizeof(char)*256);
    if(request == NULL) {
        printf("Allocation error");
        exit(0);
    }

    //Déclaration du pointeur de structure de type MYSQL
    MYSQL mysql;
    //Initialisation de MySQL
    mysql_init(&mysql);
    //Options de connexion
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    //Si la connexion réussie...
    if (mysql_real_connect(&mysql, infoDB->server, infoDB->user, infoDB->password, infoDB->dataBase, infoDB->port, NULL, 0)) {

        //Requête qui sélectionne userID dans la table useraccount
        sprintf(request, "SELECT %s FROM %s ORDER BY %s",tableID,table,tableID);
        if(mysql_query(&mysql, request) != 0) {
            printf("Error request\n");
            return 0;
        }

        //Déclaration des objets
        MYSQL_RES *result = NULL;
        MYSQL_ROW row;
        unsigned int i = 0;

        //On met le jeu de résultat dans le pointeur result
        result = mysql_use_result(&mysql);

        //Tant qu'il y a encore un résultat ...
        while((row = mysql_fetch_row(result))) {

            sscanf(row[i],"%d",&id);
            if(id != test_id) {
               return test_id;
            }
            test_id++;
        }

        //Libération du jeu de résultat
        mysql_free_result(result);
        //Fermeture de MySQL
        mysql_close(&mysql);

    } else {
        printf("Une erreur s'est produite lors de la connexion à la BDD!\n");
    }

    free(request);
    return test_id;
}

void recoveryInfoDB(infosDB *infoDB){

    char *server = malloc(sizeof(char)*65);
    char *dataBase = malloc(sizeof(char)*65);
    char *user = malloc(sizeof(char)*65);
    char *password = malloc(sizeof(char)*65);
    char *port = malloc(sizeof(char)*5);
    char *site = malloc(sizeof(char)*65);
    if(server == NULL || dataBase == NULL || user == NULL || password == NULL || port == NULL || site == NULL) {
        printf("Allocation error");
        exit(0);
    }
    FILE *file = fopen("Configuration.txt", "r");
    if(file == NULL) {
        printf("Impossible d'ouvrir le fichier \"Configuration.txt\"");
        exit(0);
    }
    char line[129];
    while(fgets(line, sizeof(line), file) != NULL) {
        if(strstr(line, "Server") != NULL) {
            strncpy(server, line+9, strlen(line+9)-1);
            server[strlen(line+9)-1]='\0';
            infoDB->server = server;
        }
        if(strstr(line, "Data Base") != NULL) {
            strncpy(dataBase, line+12, strlen(line+12)-1);
            dataBase[strlen(line+12)-1]='\0';
            infoDB->dataBase = dataBase;
        }
        if(strstr(line, "User") != NULL) {
            strncpy(user, line+7, strlen(line+7)-1);
            user[strlen(line+7)-1]='\0';
            infoDB->user = user;
        }
        if(strstr(line, "Password") != NULL) {
            strncpy(password, line+11, strlen(line+11)-1);
            password[strlen(line+11)-1]='\0';
            infoDB->password = password;
        }
        if(strstr(line, "Port") != NULL) {
            strncpy(port, line+7, strlen(line+7));
            port[strlen(line+7)]='\0';
            sscanf(port,"%d",&infoDB->port);
        }
        if(strstr(line, "Site") != NULL) {
            strncpy(site, line+7, strlen(line+7));
            site[strlen(line+7)]='\0';
            infoDB->site = site;
        }
    }
    fclose(file);
    free(server);
    free(dataBase);
    free(user);
    free(password);
    free(port);
    free(site);
}
