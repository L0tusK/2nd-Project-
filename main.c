#include "catalog.h"//συμπεριλαμβάνουμε το αρχειο header που εχει τις δηλώσεις συναρτήσεων 

void createCatalog(catalog a[]) //συνάρτηση αρχηκοποίησης καταλόγου 
{
    for (int x = 0; x < proioda; x++) 
    {
        a[x].kodikos = x + 1;//κωδικός προϊόντος
        sprintf(a[x].onoma, "Proion %d", x + 1); //αριθμοποίηση/ονομασια προϊόντων 
        a[x].timi = rand() % 100 + 1;//τυχαία τιμη προϊόντος απο 1-100
        a[x].temaxia = 3;// αρχικοποίηση διαθεσιμότητας καθε προϊόντος σε 3
    }
}
//συνάρτηση για την εμφάνιση της περίληψης των πωλησεων
void printSummary(catalog a[], int klimaka[], int apotixia[][pelates], int epitixia, int apotix, float tziro) 
{
    printf("\n ~Περίληψη Πωλήσεων~ \n");
    printf("\n");
    for (int y = 0; y < proioda; y++) 
    {//εμφάνιση σοιχείων για καθε προϊόν
        printf("%s πουλήθηκε : %d φορές ενω ζητήθηκε : %d φορές \n", a[y].onoma, 3 - a[y].temaxia, klimaka[y]);
        printf("\n");
        //επανάληψη για εμφάνιση καθε προϊόντος αποτυχημένης παραγγελίας απο πελάτη 
        for (int x = 0; x < pelates; x++) 
        {
            if (apotixia[y][x] > 0) 
            {
                printf("    ~Πελάτης %d απέτυχε %d φορές για %s\n", x + 1, apotixia[y][x], a[y].onoma);
            }
        }
        printf("\n");
    }
    printf("Συνολικές επιτυχημένες παραγγελίες: %d\n", epitixia);//εμμφάνιση επιτυχημένων παραγγελιών
    printf("Συνολικές αποτυχημένες παραγγελίες: %d\n", apotix);//εμφάνιση αποτυχημένων παραγγελιών
    printf("Συνολικός τζίρος: %.2f\n", tziro);//εμφάνιση συνόλικων εισόδων 
}

int main() 
{
    catalog a[proioda];//πίνακας με τα προϊόντα του καταστήματος
    int fdpel[2],fdkat[2]; //διαχείριση των pipes γονικής-παιδικής διεργασίας μέσω των fd φιλτρων
    int x, pid;
    int apotix = 0, epitixia = 0;// μεταβλητές πλήθους απ
    float tziro = 0.0;//συνολικά είσοδα
    int apotixia[20][5] = {0};//πινακας μέτρησης αποτυχημένων παραγγελιών προϊόντος-πελάτη
    int zitisi[proioda] = {0}; //πίνακας με 20 θέσης για την μέτρηση ζήτησης κάθε προιόντος
    //επανάληψη για κάθε πελάτη
    for (x = 0; x < pelates; x++) 
    {//δημηουργία των pipes για επικοινωνιά μεταξύ καταστήματος και πελάτη
        if (pipe(fdpel) == -1) 
        {
            printf("Η pipe fdpel απέτυχε :(\n");
            exit(1);
        }
        if (pipe(fdkat) == -1) 
        {
            printf("Η pipe fdkat απέτυχε :(\n");
            exit(1);
        }
        //δημιουργία fork του πελάτη 
        pid = fork();
        if (pid == -1) 
        {
            printf("Η fork απετυχε :(\n");
            exit(1);
        }

        if (pid == 0) 
        {   //αρχή παιδικής διεργασίας
            close(fdpel[0]);
            close(fdkat[1]);
            srand(time(NULL) ^ getpid()); // seed αναπαραγωγής τυχαίων αριθμών για τιν πελάτη
            //επανάληψη για τις παραγγελίες του πελάτη 
            for (int y = 0; y < parag; y++) 
            {
                int proion = (rand() % 20) + 1;// τυχαία κωδικός 1-20
                //αποστολή κωδικού απο παιδι/πελάτη στον γονέα/κατάστημα
                if (write(fdpel[1], &proion, sizeof(int)) == -1) 
                {
                    printf("Η write στο fdpel απέτυχε :(\n");
                    exit(1);
                }

                char apantisi[130];
                //ανάγνωση απάντησης του γονέα/καταστήματος
                if (read(fdkat[0], &apantisi, sizeof(apantisi)) == -1) 
                {
                    printf("Η read απο την fdkat απέτυχε :(\n");
                    exit(1);
                }
                printf("Πελάτης %d: %s\n", x + 1, apantisi); //εμφάνιση της επιλογής του πελάτη
            }
            close(fdpel[1]);
            close(fdkat[0]);
            exit(0);    //τέλος της παιδικής διεργασιάς  
        } 
        else 
        { // Γονική διεργασία δηλαδή το κατάστημα 
            close(fdpel[1]);
            close(fdkat[0]);

            if (x == 0) 
            {
                createCatalog(a);    //δημιουργιά του καταλόγου προϊόντων
            }
            // αρχή επανάληψης για καθε παραγγελία του πελάτη 
            for (int y = 0; y < parag; y++) 
            {
                int proionta;
                char xoros[130];
                //ανάγνωση κωδικού απο πελάτη
                if (read(fdpel[0], &proionta, sizeof(int)) == -1) 
                {
                    printf("Η read απο fdpel απέτυχε\n");
                    exit(1);
                }

                int diakoptis = 0;
                //επανάληψη για το διάβασμα και την διαχείριση των παραγγελιών
                for (int y = 0; y < proioda; y++) 
                {
                    if (a[y].kodikos == proionta) 
                    {
                        zitisi[y]++; //αύξηση ζήτησης προϊντος κατα ενα
                        if (a[y].temaxia > 0) //εαν το προϊον ειναι διαθέσιμο
                        {
                            sprintf(xoros, "Επιτυχής παραγγελία %d για %s", y + 1, a[y].onoma);
                            if (write(fdkat[1], xoros, strlen(xoros) + 1) == -1) 
                            {
                                printf("Η write στο fdkat απέτυχε\n");
                                exit(1);
                            }

                            tziro += a[y].timi; // αύξηση τζίρου μετά απο κάθε αγορά
                            epitixia++; //αύξηση μεταβλητής έπειτα απο κάθε επιτυχής αγορά 
                            a[y].temaxia--; //μείωση διαθεσιμότητας του προϊοντος μετά απο κάθε αγορά
                        } 
                        else 
                        {
                            apotixia[y][x]++; //καταμέτρηση παραγγελίων που δεν ολοκληρώθηκαν 
                            apotix++; //αύξηση της μεταβλητής συνολικών παραγγελιών που δεν ολοκληρώθηκαν 
                            sprintf(xoros, "Αποτυχία παραγγελίας %d για %s", y + 1, a[y].onoma);
                            if (write(fdkat[1], xoros, strlen(xoros) + 1) == -1) 
                            {
                                printf("Η write στο fdkat απέτυχε\n");
                                exit(1);
                            }
                        }
                        diakoptis = 1;
                        break;
                    }
                }
                if (!diakoptis) // έλλεγχος εγκυρότητας κωδικού προϊοντος 
                {
                    sprintf(xoros, "Άκυρος κωδικός προιόντος %d", proionta);
                    if (write(fdkat[1], xoros, strlen(xoros) + 1) == -1) 
                    {
                        printf("Η write για fdkat απέτυχε\n");
                        exit(1);
                    }
                }
            }
            printf("\n");
            close(fdpel[0]);
            close(fdkat[1]);
            wait(NULL);    // περιμένουμε την ολοκλήρωση της παιδικής διεργασίας
        }
    }

    //κάλεσμα συνάρτησης για την εκτύπωση τον τελικών αποτελεσμάτων 
    printSummary(a, zitisi, apotixia, epitixia, apotix, tziro);

    return 0;
}