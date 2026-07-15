#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX 500

typedef struct {
    int id;
    char name[100];
    char phone[15];
    char email[100];
    char address[200];
    char history[500];
    char blood[5];
    char password[50];
    char regDate[20];
    int age;
    char gender;
} Patient;

typedef struct {
    int id;
    char name[100];
    char spec[100];
    char phone[15];
    char email[100];
    int exp;
    float fee;
} Doctor;

typedef struct {
    int id;
    int patientID;
    int doctorID;
    char date[20];
    char time[10];
    char status[20];
    char reason[200];
} Appointment;

typedef struct {
    int id;
    int occupiedBy;
    char type[50];
    char status[20];
    char admitDate[20];
    float charge;
} Bed;

typedef struct {
    int id;
    int qty;
    char name[100];
    char expiry[20];
    char mfr[100];
    float price;
} Medicine;

typedef struct {
    int id;
    int patientID;
    float consult;
    float medicine;
    float bed;
    float lab;
    float total;
    float paid;
    float outstanding;
    char status[20];
    char date[20];
} Bill;

Patient patients[MAX];
Doctor doctors[MAX];
Appointment appts[MAX];
Bed beds[MAX];
Medicine meds[MAX];
Bill bills[MAX];

int pCnt = 0;
int dCnt = 0;
int aCnt = 0;
int bCnt = 0;
int mCnt = 0;
int blCnt = 0;

void getDate(char *d) {
    time_t t = time(NULL);
    strftime(d, 20, "%d-%m-%Y", localtime(&t));
}

void clrBuf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause() {
    printf("\nPress Enter to continue...");
    clrBuf();
    getchar();
}

int isAlphaOnly(char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isalpha(s[i]) && s[i] != ' ') {
            return 0;
        }
    }
    return strlen(s) > 0;
}

int isNumericOnly(char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return strlen(s) > 0;
}

int isValidPhone(char *s) {
    int len = strlen(s);
    if (len == 0 || len > 12) {
        return 0;
    }
    return isNumericOnly(s);
}

int isValidEmail(char *s) {
    int len = strlen(s);
    if (len < 11) {
        return 0;
    }
    return strcmp(s + len - 10, "@gmail.com") == 0;
}

int isValidDate(char *s) {
    if (strlen(s) != 10) {
        return 0;
    }
    if (s[2] != '-' || s[5] != '-') {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidTime(char *s) {
    if (strlen(s) != 5) {
        return 0;
    }
    if (s[2] != ':') {
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            continue;
        }
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidBlood(char *s) {
    char *valid[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(s, valid[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isValidHistory(char *s) {
    if (strcasecmp(s, "clear") == 0 || strcasecmp(s, "not") == 0) {
        return 1;
    }
    return 0;
}

int isValidBedType(char *s) {
    if (strcasecmp(s, "Standard") == 0 || strcasecmp(s, "Deluxe") == 0 || strcasecmp(s, "ICU") == 0) {
        return 1;
    }
    return 0;
}

int isValidSpecialization(char *s) {
    char *valid[] = {
        "Cardiology", "Neurology", "Orthopedics", "Pediatrics", "Dermatology",
        "ENT", "Ophthalmology", "Gynecology", "Psychiatry", "General",
        "Dentistry", "Oncology", "Urology", "Gastroenterology", "Pulmonology"
    };
    for (int i = 0; i < 15; i++) {
        if (strcasecmp(s, valid[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int findPatient(int id) {
    for (int i = 0; i < pCnt; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findDoctor(int id) {
    for (int i = 0; i < dCnt; i++) {
        if (doctors[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findAppointment(int id) {
    for (int i = 0; i < aCnt; i++) {
        if (appts[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findBill(int id) {
    for (int i = 0; i < blCnt; i++) {
        if (bills[i].id == id) {
            return i;
        }
    }
    return -1;
}

float getValidFloat(char *prompt) {
    char buf[50];
    float val;
    while (1) {
        printf("%s", prompt);
        scanf("%s", buf);
        int valid = 1;
        for (int i = 0; buf[i]; i++) {
            if (!isdigit(buf[i]) && buf[i] != '.') {
                valid = 0;
            }
        }
        if (valid) {
            val = atof(buf);
            return val;
        }
        printf("Invalid! Enter numeric value only.\n");
    }
}

int getValidInt(char *prompt) {
    char buf[50];
    while (1) {
        printf("%s", prompt);
        scanf("%s", buf);
        if (isNumericOnly(buf)) {
            return atoi(buf);
        }
        printf("Invalid! Enter numeric value only.\n");
    }
}

void savePatients() {
    FILE *f = fopen("patients.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", pCnt);
    for (int i = 0; i < pCnt; i++) {
        fprintf(f, "%d|%s|%d|%c|%s|%s|%s|%s|%s|%s|%s\n",
            patients[i].id,
            patients[i].name,
            patients[i].age,
            patients[i].gender,
            patients[i].phone,
            patients[i].email,
            patients[i].address,
            patients[i].history,
            patients[i].blood,
            patients[i].password,
            patients[i].regDate);
    }
    fclose(f);
}

void loadPatients() {
    FILE *f = fopen("patients.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &pCnt);
    for (int i = 0; i < pCnt; i++) {
        fscanf(f, "%d|%[^|]|%d|%c|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
            &patients[i].id,
            patients[i].name,
            &patients[i].age,
            &patients[i].gender,
            patients[i].phone,
            patients[i].email,
            patients[i].address,
            patients[i].history,
            patients[i].blood,
            patients[i].password,
            patients[i].regDate);
    }
    fclose(f);
}

void saveDoctors() {
    FILE *f = fopen("doctors.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", dCnt);
    for (int i = 0; i < dCnt; i++) {
        fprintf(f, "%d|%s|%s|%d|%.2f|%s|%s\n",
            doctors[i].id,
            doctors[i].name,
            doctors[i].spec,
            doctors[i].exp,
            doctors[i].fee,
            doctors[i].phone,
            doctors[i].email);
    }
    fclose(f);
}

void loadDoctors() {
    FILE *f = fopen("doctors.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &dCnt);
    for (int i = 0; i < dCnt; i++) {
        fscanf(f, "%d|%[^|]|%[^|]|%d|%f|%[^|]|%[^\n]\n",
            &doctors[i].id,
            doctors[i].name,
            doctors[i].spec,
            &doctors[i].exp,
            &doctors[i].fee,
            doctors[i].phone,
            doctors[i].email);
    }
    fclose(f);
}

void saveAppointments() {
    FILE *f = fopen("appointments.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", aCnt);
    for (int i = 0; i < aCnt; i++) {
        fprintf(f, "%d|%d|%d|%s|%s|%s|%s\n",
            appts[i].id,
            appts[i].patientID,
            appts[i].doctorID,
            appts[i].date,
            appts[i].time,
            appts[i].status,
            appts[i].reason);
    }
    fclose(f);
}

void loadAppointments() {
    FILE *f = fopen("appointments.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &aCnt);
    for (int i = 0; i < aCnt; i++) {
        fscanf(f, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%[^\n]\n",
            &appts[i].id,
            &appts[i].patientID,
            &appts[i].doctorID,
            appts[i].date,
            appts[i].time,
            appts[i].status,
            appts[i].reason);
    }
    fclose(f);
}

void saveBeds() {
    FILE *f = fopen("beds.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", bCnt);
    for (int i = 0; i < bCnt; i++) {
        fprintf(f, "%d|%s|%s|%d|%.2f|%s\n",
            beds[i].id,
            beds[i].type,
            beds[i].status,
            beds[i].occupiedBy,
            beds[i].charge,
            beds[i].admitDate);
    }
    fclose(f);
}

void loadBeds() {
    FILE *f = fopen("beds.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &bCnt);
    for (int i = 0; i < bCnt; i++) {
        fscanf(f, "%d|%[^|]|%[^|]|%d|%f|%[^\n]\n",
            &beds[i].id,
            beds[i].type,
            beds[i].status,
            &beds[i].occupiedBy,
            &beds[i].charge,
            beds[i].admitDate);
    }
    fclose(f);
}

void saveMedicines() {
    FILE *f = fopen("medicines.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", mCnt);
    for (int i = 0; i < mCnt; i++) {
        fprintf(f, "%d|%s|%.2f|%d|%s|%s\n",
            meds[i].id,
            meds[i].name,
            meds[i].price,
            meds[i].qty,
            meds[i].expiry,
            meds[i].mfr);
    }
    fclose(f);
}

void loadMedicines() {
    FILE *f = fopen("medicines.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &mCnt);
    for (int i = 0; i < mCnt; i++) {
        fscanf(f, "%d|%[^|]|%f|%d|%[^|]|%[^\n]\n",
            &meds[i].id,
            meds[i].name,
            &meds[i].price,
            &meds[i].qty,
            meds[i].expiry,
            meds[i].mfr);
    }
    fclose(f);
}

void saveBills() {
    FILE *f = fopen("bills.txt", "w");
    if (!f) return;
    fprintf(f, "%d\n", blCnt);
    for (int i = 0; i < blCnt; i++) {
        fprintf(f, "%d|%d|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%s|%s\n",
            bills[i].id,
            bills[i].patientID,
            bills[i].consult,
            bills[i].medicine,
            bills[i].bed,
            bills[i].lab,
            bills[i].total,
            bills[i].paid,
            bills[i].outstanding,
            bills[i].status,
            bills[i].date);
    }
    fclose(f);
}

void loadBills() {
    FILE *f = fopen("bills.txt", "r");
    if (!f) return;
    fscanf(f, "%d\n", &blCnt);
    for (int i = 0; i < blCnt; i++) {
        fscanf(f, "%d|%d|%f|%f|%f|%f|%f|%f|%f|%[^|]|%[^\n]\n",
            &bills[i].id,
            &bills[i].patientID,
            &bills[i].consult,
            &bills[i].medicine,
            &bills[i].bed,
            &bills[i].lab,
            &bills[i].total,
            &bills[i].paid,
            &bills[i].outstanding,
            bills[i].status,
            bills[i].date);
    }
    fclose(f);
}

void saveAll() {
    savePatients();
    saveDoctors();
    saveAppointments();
    saveBeds();
    saveMedicines();
    saveBills();
}

void loadAll() {
    loadPatients();
    loadDoctors();
    loadAppointments();
    loadBeds();
    loadMedicines();
    loadBills();
}

void addPatient() {
    if (pCnt >= MAX) {
        printf("Max patients reached!\n");
        return;
    }
    Patient *p = &patients[pCnt];
    p->id = 1001 + pCnt;
    printf("\n=== ADD PATIENT ===\n");

    do {
        printf("Name (alphabets only): ");
        clrBuf();
        fgets(p->name, 100, stdin);
        p->name[strcspn(p->name, "\n")] = 0;
        if (!isAlphaOnly(p->name)) {
            printf("Invalid! Name should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(p->name));

    char ageStr[20];
    do {
        printf("Age (numeric only): ");
        scanf("%s", ageStr);
        if (!isNumericOnly(ageStr)) {
            printf("Invalid! Age should be numeric only.\n");
        }
    } while (!isNumericOnly(ageStr));
    p->age = atoi(ageStr);

    char gen[10];
    do {
        printf("Gender (Male/Female): ");
        scanf("%s", gen);
        if (strcasecmp(gen, "male") != 0 && strcasecmp(gen, "female") != 0) {
            printf("Invalid! Enter Male or Female only.\n");
        }
    } while (strcasecmp(gen, "male") != 0 && strcasecmp(gen, "female") != 0);
    p->gender = (tolower(gen[0]) == 'm') ? 'M' : 'F';

    do {
        printf("Phone (numeric, max 12 digits): ");
        scanf("%s", p->phone);
        if (!isValidPhone(p->phone)) {
            printf("Invalid! Phone should be numeric and max 12 digits.\n");
        }
    } while (!isValidPhone(p->phone));

    do {
        printf("Email (must end with @gmail.com): ");
        scanf("%s", p->email);
        if (!isValidEmail(p->email)) {
            printf("Invalid! Email must end with @gmail.com.\n");
        }
    } while (!isValidEmail(p->email));

    printf("Address: ");
    clrBuf();
    fgets(p->address, 200, stdin);
    p->address[strcspn(p->address, "\n")] = 0;

    do {
        printf("Medical History (clear/not): ");
        scanf("%s", p->history);
        if (!isValidHistory(p->history)) {
            printf("Invalid! Enter 'clear' or 'not' only.\n");
        }
    } while (!isValidHistory(p->history));

    do {
        printf("Blood Group (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
        scanf("%s", p->blood);
        if (!isValidBlood(p->blood)) {
            printf("Invalid! Enter a valid blood group.\n");
        }
    } while (!isValidBlood(p->blood));

    getDate(p->regDate);
    strcpy(p->password, "patient123");
    pCnt++;
    printf("Patient added! ID: %d\n", p->id);
    savePatients();
}

void viewPatients() {
    printf("\n=== ALL PATIENTS ===\n");
    if (!pCnt) {
        printf("No patients found!\n");
        return;
    }
    for (int i = 0; i < pCnt; i++) {
        printf("ID:%d | %s | Age:%d | %c | Blood:%s | Phone:%s\n",
            patients[i].id,
            patients[i].name,
            patients[i].age,
            patients[i].gender,
            patients[i].blood,
            patients[i].phone);
    }
}

void searchPatient() {
    int id;
    printf("\n=== SEARCH PATIENT ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < pCnt; i++) {
        if (patients[i].id == id) {
            printf("Found: %s | Age:%d | Phone:%s | Email:%s\nAddress:%s | Blood:%s | History:%s\n",
                patients[i].name,
                patients[i].age,
                patients[i].phone,
                patients[i].email,
                patients[i].address,
                patients[i].blood,
                patients[i].history);
            return;
        }
    }
    printf("Not found!\n");
}

void deletePatient() {
    int id;
    printf("\n=== DELETE PATIENT ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < pCnt; i++) {
        if (patients[i].id == id) {
            for (int j = i; j < pCnt - 1; j++) {
                patients[j] = patients[j + 1];
            }
            pCnt--;
            printf("Deleted!\n");
            savePatients();
            return;
        }
    }
    printf("Not found!\n");
}

void addDoctor() {
    if (dCnt >= MAX) {
        printf("Max doctors reached!\n");
        return;
    }
    Doctor *d = &doctors[dCnt];
    d->id = 101 + dCnt;
    printf("\n=== ADD DOCTOR ===\n");

    do {
        printf("Name (alphabets only): ");
        clrBuf();
        fgets(d->name, 100, stdin);
        d->name[strcspn(d->name, "\n")] = 0;
        if (!isAlphaOnly(d->name)) {
            printf("Invalid! Name should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(d->name));

    printf("Valid specializations: Cardiology, Neurology, Orthopedics, Pediatrics, Dermatology, ENT, Ophthalmology, Gynecology, Psychiatry, General, Dentistry, Oncology, Urology, Gastroenterology, Pulmonology\n");
    do {
        printf("Specialization: ");
        clrBuf();
        fgets(d->spec, 100, stdin);
        d->spec[strcspn(d->spec, "\n")] = 0;
        if (!isValidSpecialization(d->spec)) {
            printf("Invalid! Enter a valid specialization from the list above.\n");
        }
    } while (!isValidSpecialization(d->spec));

    d->exp = getValidInt("Experience (years, numeric only): ");

    d->fee = getValidFloat("Consultation Fee (numeric only): ");

    do {
        printf("Phone (numeric, max 12 digits): ");
        scanf("%s", d->phone);
        if (!isValidPhone(d->phone)) {
            printf("Invalid! Phone should be numeric and max 12 digits.\n");
        }
    } while (!isValidPhone(d->phone));

    do {
        printf("Email (must end with @gmail.com): ");
        scanf("%s", d->email);
        if (!isValidEmail(d->email)) {
            printf("Invalid! Email must end with @gmail.com.\n");
        }
    } while (!isValidEmail(d->email));

    dCnt++;
    printf("Doctor added! ID: %d\n", d->id);
    saveDoctors();
}

void viewDoctors() {
    printf("\n=== ALL DOCTORS ===\n");
    if (!dCnt) {
        printf("No doctors found!\n");
        return;
    }
    for (int i = 0; i < dCnt; i++) {
        printf("ID:%d | %s | %s | Exp:%d yrs | Fee:%.2f\n",
            doctors[i].id,
            doctors[i].name,
            doctors[i].spec,
            doctors[i].exp,
            doctors[i].fee);
    }
}

void searchDoctor() {
    char spec[100];
    printf("\n=== SEARCH DOCTOR ===\nSpecialization: ");
    clrBuf();
    fgets(spec, 100, stdin);
    spec[strcspn(spec, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < dCnt; i++) {
        if (strstr(doctors[i].spec, spec)) {
            printf("ID:%d | %s | %s | Fee:%.2f\n",
                doctors[i].id,
                doctors[i].name,
                doctors[i].spec,
                doctors[i].fee);
            found = 1;
        }
    }
    if (!found) {
        printf("Not found!\n");
    }
}

void deleteDoctor() {
    int id;
    printf("\n=== DELETE DOCTOR ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < dCnt; i++) {
        if (doctors[i].id == id) {
            for (int j = i; j < dCnt - 1; j++) {
                doctors[j] = doctors[j + 1];
            }
            dCnt--;
            printf("Deleted!\n");
            saveDoctors();
            return;
        }
    }
    printf("Not found!\n");
}

void scheduleAppt() {
    if (aCnt >= MAX) {
        printf("Max appointments reached!\n");
        return;
    }
    Appointment *a = &appts[aCnt];
    a->id = 601 + aCnt;
    printf("\n=== SCHEDULE APPOINTMENT ===\n");

    int pid;
    do {
        pid = getValidInt("Patient ID: ");
        if (findPatient(pid) == -1) {
            printf("Patient ID not found! Try again or first register/add patient.\n");
        }
    } while (findPatient(pid) == -1);
    a->patientID = pid;

    int did;
    do {
        did = getValidInt("Doctor ID: ");
        if (findDoctor(did) == -1) {
            printf("Doctor ID not found! Try again or first add doctor.\n");
        }
    } while (findDoctor(did) == -1);
    a->doctorID = did;

    do {
        printf("Date (DD-MM-YYYY): ");
        scanf("%s", a->date);
        if (!isValidDate(a->date)) {
            printf("Invalid! Enter date in DD-MM-YYYY format with numeric values only.\n");
        }
    } while (!isValidDate(a->date));

    do {
        printf("Time (HH:MM): ");
        scanf("%s", a->time);
        if (!isValidTime(a->time)) {
            printf("Invalid! Enter time in HH:MM format with numeric values only.\n");
        }
    } while (!isValidTime(a->time));

    do {
        printf("Reason (alphabets only): ");
        clrBuf();
        fgets(a->reason, 200, stdin);
        a->reason[strcspn(a->reason, "\n")] = 0;
        if (!isAlphaOnly(a->reason)) {
            printf("Invalid! Reason should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(a->reason));

    strcpy(a->status, "Scheduled");
    aCnt++;
    printf("Appointment scheduled! ID: %d\n", a->id);
    saveAppointments();
}

void viewAppts() {
    printf("\n=== ALL APPOINTMENTS ===\n");
    if (!aCnt) {
        printf("No appointments found!\n");
        return;
    }
    for (int i = 0; i < aCnt; i++) {
        printf("ID:%d | Patient:%d | Doctor:%d | %s %s | Status:%s\n",
            appts[i].id,
            appts[i].patientID,
            appts[i].doctorID,
            appts[i].date,
            appts[i].time,
            appts[i].status);
    }
}

void updateApptStatus() {
    int id;
    printf("\n=== UPDATE APPOINTMENT STATUS ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < aCnt; i++) {
        if (appts[i].id == id) {
            printf("Current: %s\nNew Status (Scheduled/Completed/Cancelled): ", appts[i].status);
            scanf("%s", appts[i].status);
            printf("Updated!\n");
            saveAppointments();
            return;
        }
    }
    printf("Not found!\n");
}

void addBed() {
    if (bCnt >= MAX) {
        printf("Max beds reached!\n");
        return;
    }
    Bed *b = &beds[bCnt];
    b->id = 301 + bCnt;
    printf("\n=== ADD BED ===\n");

    do {
        printf("Type (Standard/Deluxe/ICU): ");
        clrBuf();
        fgets(b->type, 50, stdin);
        b->type[strcspn(b->type, "\n")] = 0;
        if (!isValidBedType(b->type)) {
            printf("Invalid! Enter Standard, Deluxe, or ICU only.\n");
        }
    } while (!isValidBedType(b->type));

    b->charge = getValidFloat("Daily Charge (numeric only): ");

    strcpy(b->status, "Available");
    b->occupiedBy = 0;
    strcpy(b->admitDate, "N/A");
    bCnt++;
    printf("Bed added! ID: %d\n", b->id);
    saveBeds();
}

void viewBeds() {
    printf("\n=== ALL BEDS ===\n");
    if (!bCnt) {
        printf("No beds found!\n");
        return;
    }
    int avail = 0, occ = 0;
    for (int i = 0; i < bCnt; i++) {
        printf("ID:%d | %s | %s | Charge:%.2f",
            beds[i].id,
            beds[i].type,
            beds[i].status,
            beds[i].charge);
        if (strcmp(beds[i].status, "Available") == 0) {
            avail++;
        } else {
            occ++;
            printf(" | Patient:%d", beds[i].occupiedBy);
        }
        printf("\n");
    }
    printf("Total:%d | Available:%d | Occupied:%d\n", bCnt, avail, occ);
}

void admitToBed() {
    int pid, bid;
    printf("\n=== ADMIT PATIENT TO BED ===\nPatient ID: ");
    scanf("%d", &pid);
    printf("Bed ID: ");
    scanf("%d", &bid);
    for (int i = 0; i < bCnt; i++) {
        if (beds[i].id == bid) {
            if (strcmp(beds[i].status, "Available") == 0) {
                beds[i].occupiedBy = pid;
                strcpy(beds[i].status, "Occupied");
                getDate(beds[i].admitDate);
                printf("Admitted!\n");
                saveBeds();
                return;
            } else {
                printf("Bed not available!\n");
                return;
            }
        }
    }
    printf("Bed not found!\n");
}

void dischargeBed() {
    int bid;
    printf("\n=== DISCHARGE PATIENT ===\nBed ID: ");
    scanf("%d", &bid);
    for (int i = 0; i < bCnt; i++) {
        if (beds[i].id == bid) {
            if (strcmp(beds[i].status, "Occupied") == 0) {
                strcpy(beds[i].status, "Available");
                beds[i].occupiedBy = 0;
                strcpy(beds[i].admitDate, "N/A");
                printf("Discharged!\n");
                saveBeds();
                return;
            } else {
                printf("Bed already available!\n");
                return;
            }
        }
    }
    printf("Bed not found!\n");
}

void addMedicine() {
    if (mCnt >= MAX) {
        printf("Max medicines reached!\n");
        return;
    }
    Medicine *m = &meds[mCnt];
    m->id = 1 + mCnt;
    printf("\n=== ADD MEDICINE ===\n");

    do {
        printf("Name (alphabets only): ");
        clrBuf();
        fgets(m->name, 100, stdin);
        m->name[strcspn(m->name, "\n")] = 0;
        if (!isAlphaOnly(m->name)) {
            printf("Invalid! Medicine name should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(m->name));

    m->price = getValidFloat("Price (numeric only): ");

    m->qty = getValidInt("Quantity (numeric only): ");

    do {
        printf("Expiry (DD-MM-YYYY): ");
        scanf("%s", m->expiry);
        if (!isValidDate(m->expiry)) {
            printf("Invalid! Enter date in DD-MM-YYYY format with numeric values only.\n");
        }
    } while (!isValidDate(m->expiry));

    do {
        printf("Manufacturer (alphabets only): ");
        clrBuf();
        fgets(m->mfr, 100, stdin);
        m->mfr[strcspn(m->mfr, "\n")] = 0;
        if (!isAlphaOnly(m->mfr)) {
            printf("Invalid! Manufacturer should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(m->mfr));

    mCnt++;
    printf("Medicine added! ID: %d\n", m->id);
    saveMedicines();
}

void viewMedicines() {
    printf("\n=== ALL MEDICINES ===\n");
    if (!mCnt) {
        printf("No medicines found!\n");
        return;
    }
    for (int i = 0; i < mCnt; i++) {
        printf("ID:%d | %s | Price:%.2f | Qty:%d | Exp:%s\n",
            meds[i].id,
            meds[i].name,
            meds[i].price,
            meds[i].qty,
            meds[i].expiry);
        if (meds[i].qty < 10) {
            printf("** LOW STOCK **\n");
        }
    }
}

void searchMedicine() {
    char name[100];
    printf("\n=== SEARCH MEDICINE ===\nName: ");
    clrBuf();
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < mCnt; i++) {
        if (strstr(meds[i].name, name)) {
            printf("ID:%d | %s | Price:%.2f | Qty:%d\n",
                meds[i].id,
                meds[i].name,
                meds[i].price,
                meds[i].qty);
            found = 1;
        }
    }
    if (!found) {
        printf("Not found!\n");
    }
}

void editMedicine() {
    int id;
    printf("\n=== EDIT MEDICINE ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < mCnt; i++) {
        if (meds[i].id == id) {
            printf("Current: %s | Price:%.2f | Qty:%d\n",
                meds[i].name,
                meds[i].price,
                meds[i].qty);
            meds[i].qty = getValidInt("New Qty: ");
            meds[i].price = getValidFloat("New Price: ");
            printf("Updated!\n");
            saveMedicines();
            return;
        }
    }
    printf("Not found!\n");
}

void deleteMedicine() {
    int id;
    printf("\n=== DELETE MEDICINE ===\nEnter ID: ");
    scanf("%d", &id);
    for (int i = 0; i < mCnt; i++) {
        if (meds[i].id == id) {
            for (int j = i; j < mCnt - 1; j++) {
                meds[j] = meds[j + 1];
            }
            mCnt--;
            printf("Deleted!\n");
            saveMedicines();
            return;
        }
    }
    printf("Not found!\n");
}

void generateBill() {
    if (blCnt >= MAX) {
        printf("Max bills reached!\n");
        return;
    }
    Bill *b = &bills[blCnt];
    b->id = 701 + blCnt;
    printf("\n=== GENERATE BILL ===\n");

    int pid;
    do {
        pid = getValidInt("Patient ID: ");
        if (findPatient(pid) == -1) {
            printf("Patient ID not found! Try again or first register/add patient.\n");
        }
    } while (findPatient(pid) == -1);
    b->patientID = pid;

    b->consult = getValidFloat("Consultation Fee (numeric only): ");
    b->medicine = getValidFloat("Medicine Charges (numeric only): ");
    b->bed = getValidFloat("Bed Charges (numeric only): ");
    b->lab = 0;

    b->total = b->consult + b->medicine + b->bed + b->lab;
    b->paid = 0;
    b->outstanding = b->total;
    strcpy(b->status, "Pending");
    getDate(b->date);
    blCnt++;
    printf("Bill generated! ID: %d | Total: %.2f\n", b->id, b->total);
    saveBills();
}

void viewBills() {
    printf("\n=== ALL BILLS ===\n");
    if (!blCnt) {
        printf("No bills found!\n");
        return;
    }
    for (int i = 0; i < blCnt; i++) {
        printf("ID:%d | Patient:%d | Total:%.2f | Paid:%.2f | Due:%.2f | %s\n",
            bills[i].id,
            bills[i].patientID,
            bills[i].total,
            bills[i].paid,
            bills[i].outstanding,
            bills[i].status);
    }
}

void processBillPayment() {
    int id;
    printf("\n=== PROCESS PAYMENT ===\nBill ID: ");
    scanf("%d", &id);
    for (int i = 0; i < blCnt; i++) {
        if (bills[i].id == id) {
            printf("Total:%.2f | Paid:%.2f | Due:%.2f\n",
                bills[i].total,
                bills[i].paid,
                bills[i].outstanding);

            float pay = getValidFloat("Payment Amount (numeric only): ");

            if (pay < bills[i].outstanding) {
                printf("Amount is less than due! Please enter the full amount to be paid.\n");
                continue;
            }

            if (pay > bills[i].outstanding) {
                float excess = pay - bills[i].outstanding;
                printf("Remaining amount of %.2f is returned.\n", excess);
                pay = bills[i].outstanding;
            }

            bills[i].paid += pay;
            bills[i].outstanding = bills[i].total - bills[i].paid;
            strcpy(bills[i].status, bills[i].outstanding == 0 ? "Paid" : "Partial");
            printf("Payment processed! Outstanding: %.2f\n", bills[i].outstanding);
            saveBills();
            return;
        }
    }
    printf("Not found!\n");
}

void patientMenu() {
    int c;
    do {
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1.Add 2.View 3.Search 4.Delete 5.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: deletePatient(); break;
        }
    } while (c != 5);
}

void doctorMenu() {
    int c;
    do {
        printf("\n=== DOCTOR MANAGEMENT ===\n");
        printf("1.Add 2.View 3.Search 4.Delete 5.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: addDoctor(); break;
            case 2: viewDoctors(); break;
            case 3: searchDoctor(); break;
            case 4: deleteDoctor(); break;
        }
    } while (c != 5);
}

void apptMenu() {
    int c;
    do {
        printf("\n=== APPOINTMENT MANAGEMENT ===\n");
        printf("1.Schedule 2.View All 3.Update Status 4.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: scheduleAppt(); break;
            case 2: viewAppts(); break;
            case 3: updateApptStatus(); break;
        }
    } while (c != 4);
}

void bedMenu() {
    int c;
    do {
        printf("\n=== BED MANAGEMENT ===\n");
        printf("1.Add 2.View 3.Admit 4.Discharge 5.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: addBed(); break;
            case 2: viewBeds(); break;
            case 3: admitToBed(); break;
            case 4: dischargeBed(); break;
        }
    } while (c != 5);
}

void medMenu() {
    int c;
    do {
        printf("\n=== MEDICINE INVENTORY ===\n");
        printf("1.Add 2.View 3.Search 4.Edit 5.Delete 6.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: addMedicine(); break;
            case 2: viewMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: editMedicine(); break;
            case 5: deleteMedicine(); break;
        }
    } while (c != 6);
}

void billMenu() {
    int c;
    do {
        printf("\n=== BILLING SYSTEM ===\n");
        printf("1.Generate 2.View All 3.Process Payment 4.Back\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: generateBill(); break;
            case 2: viewBills(); break;
            case 3: processBillPayment(); break;
        }
    } while (c != 4);
}

void managerDashboard() {
    int c;
    do {
        printf("\n========================================\n");
        printf("         MANAGER DASHBOARD\n");
        printf("========================================\n");
        printf("1.Patient Management\n");
        printf("2.Doctor Management\n");
        printf("3.Appointment Management\n");
        printf("4.Bed Management\n");
        printf("5.Medicine Inventory\n");
        printf("6.Billing System\n");
        printf("7.Logout\n");
        printf("Choice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: patientMenu(); break;
            case 2: doctorMenu(); break;
            case 3: apptMenu(); break;
            case 4: bedMenu(); break;
            case 5: medMenu(); break;
            case 6: billMenu(); break;
            case 7:
                printf("Logging out...\n");
                saveAll();
                break;
            default:
                printf("Invalid!\n");
        }
    } while (c != 7);
}

void managerLogin() {
    char user[50], pass[50];
    int tries = 3;
    printf("\n=== MANAGER LOGIN ===\n");
    while (tries > 0) {
        printf("Username: ");
        scanf("%s", user);
        printf("Password: ");
        scanf("%s", pass);
        if ((strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) ||
            (strcmp(user, "manager") == 0 && strcmp(pass, "manager123") == 0)) {
            printf("Login Successful!\n");
            managerDashboard();
            return;
        }
        tries--;
        if (tries > 0) {
            printf("Invalid credentials! Attempts remaining: %d\n", tries);
        }
    }
    printf("Too many failed attempts! Returning to main menu.\n");
}

void patientRegister() {
    if (pCnt >= MAX) {
        printf("Max patients reached!\n");
        return;
    }
    Patient *p = &patients[pCnt];
    p->id = 1001 + pCnt;
    printf("\n=== PATIENT REGISTRATION ===\n");

    do {
        printf("Name (alphabets only): ");
        clrBuf();
        fgets(p->name, 100, stdin);
        p->name[strcspn(p->name, "\n")] = 0;
        if (!isAlphaOnly(p->name)) {
            printf("Invalid! Name should contain alphabets only.\n");
        }
    } while (!isAlphaOnly(p->name));

    char ageStr[20];
    do {
        printf("Age (numeric only): ");
        scanf("%s", ageStr);
        if (!isNumericOnly(ageStr)) {
            printf("Invalid! Age should be numeric only.\n");
        }
    } while (!isNumericOnly(ageStr));
    p->age = atoi(ageStr);

    char gen[10];
    do {
        printf("Gender (Male/Female): ");
        scanf("%s", gen);
        if (strcasecmp(gen, "male") != 0 && strcasecmp(gen, "female") != 0) {
            printf("Invalid! Enter Male or Female only.\n");
        }
    } while (strcasecmp(gen, "male") != 0 && strcasecmp(gen, "female") != 0);
    p->gender = (tolower(gen[0]) == 'm') ? 'M' : 'F';

    do {
        printf("Phone (numeric, max 12 digits): ");
        scanf("%s", p->phone);
        if (!isValidPhone(p->phone)) {
            printf("Invalid! Phone should be numeric and max 12 digits.\n");
        }
    } while (!isValidPhone(p->phone));

    do {
        printf("Email (must end with @gmail.com): ");
        scanf("%s", p->email);
        if (!isValidEmail(p->email)) {
            printf("Invalid! Email must end with @gmail.com.\n");
        }
    } while (!isValidEmail(p->email));

    printf("Address: ");
    clrBuf();
    fgets(p->address, 200, stdin);
    p->address[strcspn(p->address, "\n")] = 0;

    do {
        printf("Medical History (clear/not): ");
        scanf("%s", p->history);
        if (!isValidHistory(p->history)) {
            printf("Invalid! Enter 'clear' or 'not' only.\n");
        }
    } while (!isValidHistory(p->history));

    do {
        printf("Blood Group (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
        scanf("%s", p->blood);
        if (!isValidBlood(p->blood)) {
            printf("Invalid! Enter a valid blood group.\n");
        }
    } while (!isValidBlood(p->blood));

    printf("Password: ");
    scanf("%s", p->password);
    getDate(p->regDate);
    pCnt++;
    printf("\nRegistration successful! Your ID: %d\nSave your ID and password for login.\n", p->id);
    savePatients();
}

int patientLogin() {
    int id;
    char pass[50];
    printf("\n=== PATIENT LOGIN ===\n");
    printf("Patient ID: ");
    scanf("%d", &id);
    printf("Password: ");
    scanf("%s", pass);
    for (int i = 0; i < pCnt; i++) {
        if (patients[i].id == id && strcmp(patients[i].password, pass) == 0) {
            printf("Welcome, %s!\n", patients[i].name);
            return i;
        }
    }
    printf("Invalid credentials!\n");
    return -1;
}

void patientDashboard(int idx) {
    int c;
    do {
        printf("\n========================================\n");
        printf("         PATIENT DASHBOARD\n");
        printf("========================================\n");
        printf("1.View Profile\n");
        printf("2.Schedule Appointment\n");
        printf("3.View Appointments\n");
        printf("4.Cancel Appointment\n");
        printf("5.View Bills\n");
        printf("6.Pay Bill\n");
        printf("7.Check Bed Availability\n");
        printf("8.Doctor Directory\n");
        printf("9.Logout\n");
        printf("Choice: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                printf("\n=== MY PROFILE ===\n");
                printf("ID:%d | %s | Age:%d | %c\n",
                    patients[idx].id,
                    patients[idx].name,
                    patients[idx].age,
                    patients[idx].gender);
                printf("Phone:%s | Email:%s\n",
                    patients[idx].phone,
                    patients[idx].email);
                printf("Address:%s\n", patients[idx].address);
                printf("Blood:%s | History:%s\n",
                    patients[idx].blood,
                    patients[idx].history);
                break;

            case 2: {
                if (aCnt >= MAX) {
                    printf("Max appointments!\n");
                    break;
                }
                printf("\n=== SCHEDULE APPOINTMENT ===\n");
                printf("Available Doctors:\n");
                if (dCnt == 0) {
                    printf("No doctors available! Ask manager to add doctors first.\n");
                    break;
                }
                for (int i = 0; i < dCnt; i++) {
                    printf("ID:%d | %s | %s | Fee:%.2f\n",
                        doctors[i].id,
                        doctors[i].name,
                        doctors[i].spec,
                        doctors[i].fee);
                }
                Appointment *a = &appts[aCnt];
                a->id = 601 + aCnt;
                a->patientID = patients[idx].id;

                int did;
                do {
                    did = getValidInt("Doctor ID: ");
                    if (findDoctor(did) == -1) {
                        printf("Doctor ID not found! Try again or ask manager to add doctor.\n");
                    }
                } while (findDoctor(did) == -1);
                a->doctorID = did;

                do {
                    printf("Date (DD-MM-YYYY): ");
                    scanf("%s", a->date);
                    if (!isValidDate(a->date)) {
                        printf("Invalid! Enter date in DD-MM-YYYY format with numeric values only.\n");
                    }
                } while (!isValidDate(a->date));

                do {
                    printf("Time (HH:MM): ");
                    scanf("%s", a->time);
                    if (!isValidTime(a->time)) {
                        printf("Invalid! Enter time in HH:MM format with numeric values only.\n");
                    }
                } while (!isValidTime(a->time));

                do {
                    printf("Reason (alphabets only): ");
                    clrBuf();
                    fgets(a->reason, 200, stdin);
                    a->reason[strcspn(a->reason, "\n")] = 0;
                    if (!isAlphaOnly(a->reason)) {
                        printf("Invalid! Reason should contain alphabets only.\n");
                    }
                } while (!isAlphaOnly(a->reason));

                strcpy(a->status, "Scheduled");
                aCnt++;
                printf("Scheduled! ID: %d\n", a->id);
                saveAppointments();
            } break;

            case 3: {
                printf("\n=== MY APPOINTMENTS ===\n");
                int found = 0;
                for (int i = 0; i < aCnt; i++) {
                    if (appts[i].patientID == patients[idx].id) {
                        printf("ID:%d | Doctor:%d | %s %s | %s | %s\n",
                            appts[i].id,
                            appts[i].doctorID,
                            appts[i].date,
                            appts[i].time,
                            appts[i].status,
                            appts[i].reason);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No appointments!\n");
                }
            } break;

            case 4: {
                printf("\n=== CANCEL APPOINTMENT ===\n");
                int aid, found = 0;
                do {
                    aid = getValidInt("Appointment ID: ");
                    for (int i = 0; i < aCnt; i++) {
                        if (appts[i].id == aid && appts[i].patientID == patients[idx].id) {
                            strcpy(appts[i].status, "Cancelled");
                            printf("Appointment cancelled!\n");
                            saveAppointments();
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Appointment not found! Enter correct ID or first schedule an appointment.\n");
                    }
                } while (!found);
            } break;

            case 5: {
                printf("\n=== MY BILLS ===\n");
                int found = 0;
                for (int i = 0; i < blCnt; i++) {
                    if (bills[i].patientID == patients[idx].id) {
                        printf("ID:%d | Total:%.2f | Paid:%.2f | Due:%.2f | %s\n",
                            bills[i].id,
                            bills[i].total,
                            bills[i].paid,
                            bills[i].outstanding,
                            bills[i].status);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No bills!\n");
                }
            } break;

            case 6: {
                int bid;
                printf("\n=== PAY BILL ===\nBill ID: ");
                scanf("%d", &bid);
                for (int i = 0; i < blCnt; i++) {
                    if (bills[i].id == bid && bills[i].patientID == patients[idx].id) {
                        printf("Total:%.2f | Paid:%.2f | Due:%.2f\n",
                            bills[i].total,
                            bills[i].paid,
                            bills[i].outstanding);
                        float pay = getValidFloat("Amount (numeric only): ");

                        if (pay < bills[i].outstanding) {
                            printf("Amount is less than due! Please pay the full amount.\n");
                            break;
                        }
                        if (pay > bills[i].outstanding) {
                            float excess = pay - bills[i].outstanding;
                            printf("Remaining amount of %.2f is returned.\n", excess);
                            pay = bills[i].outstanding;
                        }

                        bills[i].paid += pay;
                        bills[i].outstanding = bills[i].total - bills[i].paid;
                        strcpy(bills[i].status, bills[i].outstanding == 0 ? "Paid" : "Partial");
                        printf("Paid! Outstanding: %.2f\n", bills[i].outstanding);
                        saveBills();
                        break;
                    }
                }
            } break;

            case 7:
                viewBeds();
                break;

            case 8:
                viewDoctors();
                break;

            case 9:
                printf("Logging out...\n");
                saveAll();
                break;

            default:
                printf("Invalid!\n");
        }
    } while (c != 9);
}

int main() {
    int c;
    loadAll();
    do {
        printf("\n========================================\n");
        printf("     HOSPITAL MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("1.Manager Login\n");
        printf("2.Patient Login\n");
        printf("3.Patient Registration\n");
        printf("4.Exit\n");
        printf("Choice: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                managerLogin();
                break;
            case 2: {
                int idx = patientLogin();
                if (idx >= 0) {
                    patientDashboard(idx);
                }
            } break;
            case 3:
                patientRegister();
                break;
            case 4:
                printf("Thank you! Goodbye.\n");
                saveAll();
                break;
            default:
                printf("Invalid!\n");
        }
    } while (c != 4);
    return 0;
}