#ifndef CATALOG_H
#define CATALOG_H

//βιβλιοθήκη που χρειάζονται
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

//σταθερές μεταβλητές
#define proioda 20
#define parag 10
#define pelates 5

typedef struct //δομή που αποτελεί κάθε πληροφορία προϊόντος στον κατάλογο 
{
    int kodikos; //κωδικός προιόντος
    char onoma[50]; //δήλωση πίνακα όνομα προϊόντος 
    float timi; // τιμή προϊόντος
    int temaxia; //ποσότητα προϊόντος 
} catalog;

// Δηλώσεις συναρτήσεων
void createCatalog(catalog a[]);//Δημιουργία καταλόγου προϊόντων
void printSummary(catalog a[], int zitisi[], int apotixia[][pelates], int epitixia, int apotix, float tziro);//συνάρτηση εκτύπωσης περίληψης πωλήσεων

#endif //τέλος αρχειου header 