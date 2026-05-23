#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A_PASSWORD 1234    // admin password
#define A_USERNAME "admin" // admin username. flaw to note: i hardcoded these
#define A_PATIENTID "001"  //saved in file
#define A_PASSWORD_PATIENT "1234" //for checking purpose only
#define B_USERNAME "billing" // patient billing portal login username
#define B_PASSWORD 1111      // patient billing portal login password

// main categories
void admin_login(void);
void staff_portal(void);
void patient_portal(void);
void billing_portal(void);

// admin portal options
void view_record(void);
void add_staff(void);
void search_staff(void);
void edit_staff(void);
void delete_staff(void);


// patient portal options
void register_patient(void);
int login_patient(char currentPatientID[]);
void patient_menu(char currentPatientID[]);
void view_patient_profile(char currentPatientID[]);
void view_available_doctors(void);
int is_doctor_available(char doctorID[]);
void book_appointment(char currentPatientID[]);
void view_appointment(char currentPatientID[]);
void search_appointment(char currentPatientID[]);
void reschedule_appointment(char currentPatientID[]);
void cancel_appointment(char currentPatientID[]);
void copy_temp_to_appointment_file(void);
void view_prescription(char currentPatientID[]);
void view_bed_info(char currentPatientID[]);
void view_bill_patient(char currentPatientID[]);

//staff portal options
void staff_portal(void);
void staff_login(void);
void staff_profile(int id);

//billing portal options
void generate_bill(void);
void view_bill(void);
void process_payment(void);
void view_payment_history(void);

//structure for staff members, for use in staff portal
typedef struct Doctor
{
    int ID;
    char name[20];
    char specialization[30];
    char phone[16];
    char email[40];
}Doctor;

// structure for staff members, for use in admin portal
typedef struct Staff
{
    int id;
    char name[100];
    int age;
    char staff_type[30];
    char availability[20];
    char working_hours[20];
} staff;

// structure for patients, for use in patient and billing portals
typedef struct Patient
{
    int id;
    char name[100];
    int height;
    int weight;
    char condition[100];
} patient;

// user functions
staff get_info(void);
int find_max_id(FILE *file);
void read_csv(FILE *file, staff array[]);
void edit_strings(FILE *duplicate, staff array[], int id, int list, int option);
void edit_age(FILE *duplicate, staff array[], int id, int list);

int main(void)
{
    int choice;

    // infinite loop so that program doesn't terminate after each thing user does, only when user
    // exits by pressing 5 does the program stop
    while (1)
    {
        printf("\n=== pediatrics clinic hospital management system ===\n\n");
        printf("1. Staff Management Portal\n2. Staff Portal\n3. Patient Portal\n4. Billing & "
               "Payment Portal\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
            case 1:
                admin_login();
                break;
            case 2:
                staff_portal();
                break;
            case 3:
                patient_portal();
                break;
            case 4:
                billing_portal();
                break;
            case 5:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Error: Please choose from the options given above\n");
                break;
        }
    }
}

// case 1: staff management portal
void admin_login(void)
{
    int options, password;
    char username[10];

    printf("=== Staff Management Portal ===\n");

    // keep asking the user for correct username and password. improvement to be made if there is
    // time: max 3 chances to input correct details
    do
    {
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%d", &password);
        printf("\n");

        if (password != A_PASSWORD || strcmp(username, A_USERNAME) != 0)
            printf("Error: Wrong user credentials\n");
    }
    while (password != A_PASSWORD || strcmp(username, A_USERNAME) != 0);

    printf("Admin logged in successfully.\n");

    // infinite loop so that user doesn't return to home screen after each change in admin portal.
    // only after user presses 7 does he return
    while (1)
    {
        printf("\n== Staff Management Menu ==\n");
        printf("1. View all records\n2. Add Staff\n3. Search Staff\n4. Edit Staff\n5. Delete "
               "Staff\n6. Exit\nEnter choice: ");
        scanf("%d", &options);
        printf("\n");

        switch (options)
        {
            case 1:
                view_record();
                break;
            case 2:
                add_staff();
                break;
            case 3:
                search_staff();
                break;
            case 4:
                edit_staff();
                break;
            case 5:
                delete_staff();
                break;
            case 6:
                return;
            default:
                printf("Error: Please enter valid integer from choices given\n");
                break;
        }
    }
}

// option 1 for staff management portal
void view_record(void)
{
    staff data[1024];
    char buffer[50];
    int list;

    // checks if the file exits in dictionary
    FILE *input = fopen("records.csv", "r");
    FILE *output = fopen("records.txt", "w+");
    if (input == NULL || output == NULL)
    {
        printf("Error: The file did not open\n");
        return;
    }

    printf("File opened successfully\n");
    fprintf(output, "========================================================================================================\n");
    fprintf(output, "ID      Name                Age     Staff Type              Availability    Working Hours\n");
    fprintf(output, "========================================================================================================\n");

    // rewind function puts the pointer back to the starting of the file
    list = find_max_id(input);
    rewind(input);
    read_csv(input, data);

    // line - 1 bc as i said find_id function returns 1 extra line
    // transfer data from the array to the output.txt file
    for (int i = 0; i < list - 1; i++)
    {
        fprintf(output, "%03d  %-23s%-8d%-24s%-16s%-16s\n", data[i].id, data[i].name, data[i].age, data[i].staff_type, data[i].availability, data[i].working_hours);
    }
    rewind(output);

    // print the contents of txt file to the screen
    while (fgets(buffer, 50, output) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(input);
    fclose(output);
}

// transfer data from csv file to an array
void read_csv(FILE *file, staff array[])
{
    int read = 0, records = 0;
    do
    {
        // %99[^n] means keep reading 99 characters till till/unless you find a comma
        read = fscanf(file, "%d,%99[^,],%d,%29[^,],%19[^,],%19[^\n]", &array[records].id, array[records].name, &array[records].age, array[records].staff_type, array[records].availability, array[records].working_hours);

        // i have 6 coloumns, if read is any less than 6 than it means fscanf has failed to read a colloumn
        if (read == 6)
        {
            records++;
        }
        else if (read != 6 && !feof(file))
        {
            printf("Error: File format incorrect\n");
            return;
        }
        if (ferror(file))
        {
            printf("Error reading file\n");
            return;
        }
    }
    while (!feof(file));
}

// finding max id
int find_max_id(FILE *file)
{
    int ch = 0;
    int lines = 1;

    // The fgetc() function returns the ASCII value of the character at the current position in the file and then moves the position indicator to the next character. An int value representing
    // the ASCII value of the character at the current position in the file, or the constant EOF if the end of the file has been reached or an error occurred.
    // so it basically means till end of file is reached keep the loop going. whenever u find a new line character, increase line count (and by extention my id count).
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
            lines++;
    }
    // returns 1 extra line since all files have 1 extra line
    return lines;
}

// option 2 for staff management portal
void add_staff(void)
{
    staff details;

    // a+ mode is to read and append
    FILE *input = fopen("records.txt", "a+");
    FILE *input_csv = fopen("records.csv", "a+");
    if (input == NULL || input_csv == NULL)
    {
        printf("Error: The file did not open\n");
        return;
    }

    // calling get_info function to get staff details which i store in my details variable which is of type staff (a structure i made)
    details = get_info();
    details.id = find_max_id(input_csv);

    fprintf(input_csv, "%03d,%s,%d,%s,%s,%s", details.id, details.name, details.age, details.staff_type, details.availability, details.working_hours);
    fprintf(input, "%03d  %-23s%-8d%-24s%-16s%-16s", details.id, details.name, details.age, details.staff_type, details.availability, details.working_hours);
    printf("Successfully added!\n");

    fclose(input);
    fclose(input_csv);
}

// function to get staff details
staff get_info()
{
    staff p;

    // this while loop is to remove input buffers. like if i didn't use this loop then scanf/fgets
    // would take "enter or \n" as input before we even give anything
    while ((getchar()) != '\n');
    printf("Enter Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = 0; // this is used to remove input buffer as well. it makes the \n character null
    printf("Enter age: ");
    scanf("%d", &p.age);
    while ((getchar()) != '\n');
    printf("Enter staff type: ");
    fgets(p.staff_type, sizeof(p.staff_type), stdin);
    p.staff_type[strcspn(p.staff_type, "\n")] = 0;
    printf("Enter Availability details: ");
    fgets(p.availability, sizeof(p.availability), stdin);
    p.availability[strcspn(p.availability, "\n")] = 0;
    printf("Enter working hours: ");
    fgets(p.working_hours, sizeof(p.working_hours), stdin);

    return p;
}

// option 3 for staff management portal
void search_staff(void)
{
    // array of my own structure used
    int records;
    staff data[100];
    char name[50];

    // note i am using csv file here and not txt, makes it soo much easier
    FILE *input = fopen("records.csv", "r");

    if (input == NULL)
        printf("Error: The file did not open\n");

    read_csv(input, data);
    rewind(input);
    records = find_max_id(input) - 1;

    // taking away input buffer
    while ((getchar()) != '\n');
    printf("Enter staff name: ");
    fgets(name, 50, stdin);

    // major flaw oh hell no, using loop to go through data base. if it was a very large data base, linerar search is horrible.
    // improvement is using a sorting algorithm
    // another flaw is that you can only search by name and that name has to be perfect and have all spaces and capitalization correct
    // improvement convert all letters to lowercase before comparing and taking note of spaces and what not
    for (int i = 0; i < records; i++)
    {
        if (strstr(name, data[i].name) != NULL)
        {
            printf("Staff found\n");
            printf("========================================================================================================\n");
            printf("ID      Name                Age     Staff Type              Availability    Working Hours\n");
            printf("========================================================================================================\n");
            printf("%03d  %-23s%-8d%-24s%-16s%-16s\n", data[i].id, data[i].name, data[i].age, data[i].staff_type, data[i].availability, data[i].working_hours);
            return;
        }
    }
    printf("\nStaff not found\n");

    fclose(input);
}

// option 4 for staff management portal
void edit_staff(void)
{
    staff data[1024];
    int choice, id, list;

    FILE *input = fopen("records.csv", "r");
    FILE *duplicate = fopen("duplicate.csv", "w");

    if (input == NULL || duplicate == NULL)
    {
        printf("Error: Couldn't open the files\n");
        return;
    }

    read_csv(input, data);
    rewind(input);

    list = find_max_id(input);
    fclose(input);

    printf("Edit:\n1. Name\n2. Age\n3. Staff Type\n4. Availability\n5. Working Hours\n6. Exit\nEnter choice: ");
    scanf("%d", &choice);
    while ((getchar()) != '\n');
    printf("\n");

    switch(choice)
    {
        case 1: case 3: case 4: case 5:
            printf("Enter ID: ");
            scanf("%d", &id);
            while ((getchar()) != '\n');

            edit_strings(duplicate, data, id, list, choice);

            remove("records.csv");
            rename("duplicate.csv", "records.csv");
            break;
        case 2:
            printf("Enter ID: ");
            scanf("%d", &id);
            while ((getchar()) != '\n');

            edit_age(duplicate, data, id, list);

            remove("records.csv");
            rename("duplicate.csv", "records.csv");
            break;
        case 6:
            return;
        default:
            printf("Error: Enter valid input\n");
            break;
        }
}

void edit_strings(FILE *duplicate, staff array[], int id, int list, int option)
{
    char string_container[1024];
    if (option == 1)
    {
        for (int i = 0; i < list - 1; i++)
        {
            if (id == array[i].id)
            {
                printf("Current name: %s\n", array[i].name);
                printf("Enter new name: ");
                fgets(string_container, 50, stdin);
                strcpy(array[i].name, string_container);
                array[i].name[strcspn(array[i].name, "\n")] = 0;
                break;
            }
        }
    }
    else if (option == 3)
    {
        for (int i = 0; i < list - 1; i++)
        {
            if (id == array[i].id)
            {
                printf("Current staff type: %s\n", array[i].staff_type);
                printf("Enter staff type: ");
                fgets(string_container, 50, stdin);
                strcpy(array[i].staff_type, string_container);
                array[i].staff_type[strcspn(array[i].staff_type, "\n")] = 0;
                break;
            }
        }
    }
    else if (option == 4)
    {
        for (int i = 0; i < list - 1; i++)
        {
            if (id == array[i].id)
            {
                printf("Current staff type: %s\n", array[i].availability);
                printf("Enter staff type: ");
                fgets(string_container, 50, stdin);
                strcpy(array[i].availability, string_container);
                array[i].availability[strcspn(array[i].availability, "\n")] = 0;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < list - 1; i++)
        {
            if (id == array[i].id)
            {
                printf("Current staff type: %s\n", array[i].working_hours);
                printf("Enter staff type: ");
                fgets(string_container, 50, stdin);
                strcpy(array[i].working_hours, string_container);
                array[i].working_hours[strcspn(array[i].working_hours, "\n")] = 0;
                break;
            }
        }
    }

    for (int i = 0; i < list - 1; i++)
    fprintf(duplicate, "%03d,%s,%d,%s,%s,%s\n", array[i].id, array[i].name, array[i].age, array[i].staff_type, array[i].availability, array[i].working_hours);
    fclose(duplicate);
}

void edit_age(FILE *duplicate, staff array[], int id, int list)
{
    int age;
    for (int i = 0; i < list - 1; i++)
    {
        if (id == array[i].id)
        {
            printf("Current age: %d\n", array[i].age);
            printf("Enter new age: ");
            scanf("%d", &age);
            array[i].age = age;
            break;
        }
    }
    for (int i = 0; i < list - 1; i++)
    fprintf(duplicate, "%03d,%s,%d,%s,%s,%s\n", array[i].id, array[i].name, array[i].age, array[i].staff_type, array[i].availability, array[i].working_hours);
    fclose(duplicate);
}

// option 5 for staff management portal
void delete_staff(void)
{
    char buffer[1024];
    int delete_ID, current_ID = 1;
    bool keep_reading = true;

    FILE *input = fopen("records.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (input == NULL || temp == NULL)
    {
        printf("Error: can't open the file\n");
        return;
    }

    printf("Enter staff ID: ");
    scanf("%d", &delete_ID);

    do
    {
        // get details from the input file and store it into our buffer array
        fgets(buffer, 1024, input);

        // if end of file is reached, set keep_reading to false so that we exit the loop
        if (feof(input))
        {
            keep_reading = false;
        }
        // as long as the id to be deleted doesn't match keep printing to the temporary file
        else if (current_ID != delete_ID)
        {
            fputs(buffer, temp);
        }
        current_ID++;
    }
    while (keep_reading);

    printf("Staff deleted successfully\n");

    fclose(input);
    fclose(temp);

    // delete the original file and rename the temp file to the original file
    remove("records.csv");
    rename("temp.csv", "records.csv");
}

// mahir
void staff_portal(void)
{
    printf("=== Staff Portal ===\n");
    //login
    int choice;
    printf("1. Login\n");
    printf("2. Back\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);
    if((choice == 1))
    {
        staff_login();
    }
    else if((choice == 2))
    {
        return;
    }
    else
    {
        while((choice != 1) && (choice != 2))
        {
            printf("\nError: Please choose from the options given above\n\n");
            printf("1. Login\n");
            printf("2. Back\n");
            printf("Enter Choice: ");
            scanf("%d", &choice);
        }
        if(choice == 1)
        {
            staff_login();
        }
        else if(choice == 2)
        {
            return;
        }
    }
}

//takes staff input for login
void staff_login(void)
{
    int flag = 1;
    //Open file
    FILE *inp;
    inp = fopen("staff_login.txt", "r");

    //user input variables
    int user_input;
    char user_pass;

    //user input
    printf("\n");
    printf("ID: ");
    scanf("%d", &user_input);

    printf("Password: ");
    scanf(" %c", &user_pass);

    //file input variables
    int input;
    char pass;

    //while loop checks file contents row-wise until EOF
    while(fscanf(inp, "%d %c", &input, &pass) != EOF)
    {
        //if condition checks whether the content of the certain row in the certain iteration is equal to the user input content
        if( user_input == input && user_pass == pass )
        {
            printf("\nSuccessful login.\n\n");
            flag = 1;
            //Staff Profile function
            staff_profile(user_input);
            break;
        }
        else
        {
            flag = 0;
        }

    }
    if(flag == 0)
    {
        printf("\nUnsuccessful Try Again.\n\n");
    }

    fclose(inp);
}

//views staff profiles
void staff_profile(int id)
{
    int logout;
    Doctor d;
    FILE *inp;
    inp = fopen("staff_profile.txt", "r");

    while(fscanf(inp, "%d,%20[^,],%30[^,],%16[^,],%40[^\n]",&d.ID, d.name, d.specialization, d.phone, d.email) != EOF) //[^,] tells fscanf keep reading characters until you hit a comma and new line then stop
    {
        if(id == d.ID)
        {
            break;
        }

    }

    printf("=====================================\n\n");
    printf("ID:             %03d\n\n", d.ID);
    printf("Name:           %s\n\n", d.name);
    printf("Specialization: %s\n\n", d.specialization);
    printf("Phone:          %s\n\n", d.phone);
    printf("Email:          %s", d.email);
    printf("\n\n=====================================\n");

    fclose(inp);

    //logout
    printf("\nTo logout press 0: ");
    scanf("%d", &logout);
    if(logout == 0)
    {
        printf("\n");
        //sends back to portal
        staff_portal();
    }
    else
    {
        while(logout != 0)
        {
            printf("\nError: Please choose from the options given above\n");
            printf("\nTo logout press 0: ");
            scanf("%d", &logout);
        }

        if(logout == 0)
        {
            printf("\n");
            //sends back to portal
            staff_portal();
        }
    }

}

// sawom
// PATIENT PORTAL starts here...
void patient_portal(void)
{
    int choice;
    char currentPatientID[20];

//main loop for patient portal, user can keep doing things in the patient portal until they choose to log out and return to home screen
    while (1)
    {
        //main menu for patient portal
        printf("\n=== Patient Portal ===\n");
        printf("1. Login\n2. Register\n3. Back\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                // if login is successful, currentPatientID will store the logged in patient's ID
                if (login_patient(currentPatientID))
                {
                    patient_menu(currentPatientID);
                }
                break;

            case 2:
                register_patient();
                break;

            case 3:
            printf("Exit Patient Portal\n");
                return;

            default:
                printf("Please choose a valid option\n");
        }
    }
}

// can register new patients
void register_patient(void)
{
    FILE *login_file;
    char id[20], password[30];

    printf("\n=== Patient Registration ===\n");

    printf("Enter Patient ID: ");
    scanf("%s", id);

    printf("Create Password: ");
    scanf("%s", password);

    // append(a+) mode is used so new registrations are added at the end of the file
    login_file = fopen("patient_login_file.txt", "a+");

    if (login_file == NULL)
    {
        printf("File could not open.\n");
        return;
    }

    fprintf(login_file, "%s %s\n", id, password);
    fclose(login_file);

    printf("Registration successful.\n");
}

// can login with existing patient ID and password!
int login_patient(char currentPatientID[])
{
    FILE *login_file;
    char id[20], password[30];
    char saved_id[20], saved_password[30];
    char line[300];

    printf("\n=== Patient Login ===\n");

    printf("Enter Patient ID: ");
    scanf("%s", id);

    printf("Enter Password: ");
    scanf("%s", password);

    login_file = fopen("patient_login_file.txt", "r");

    if (login_file == NULL)
    {
        printf("File could not open.\n");
        return 0;
    }


    while (fgets(line, sizeof(line), login_file) != NULL)
    {
        if (sscanf(line, "%20s %10s", saved_id, saved_password) == 2) //used chatgpt to fix bug
        {
            if (strcmp(id, saved_id) == 0 && strcmp(password, saved_password) == 0)
            {
                strcpy(currentPatientID, id);
                fclose(login_file);

                printf("Login successful.\n");
                return 1;
            }
        }
    }

    fclose(login_file);

    printf("Wrong ID or password.\n");
    return 0;
}

// after logging in, patients can view their profile, view available doctors, book appointment, view appointment, view prescription, view bed info and view bill. they can keep doing things until they choose to log out
void patient_menu(char currentPatientID[])
{
    int choice;

    while (1)
    {
        printf("\n=== Patient Menu ===\n");
        printf("1. View Profile\n");
        printf("2. View Available Doctors\n");
        printf("3. Book Appointment\n");
        printf("4. View Appointment\n");
        printf("5. Search Appointment\n");
        printf("6. Reschedule Appointment\n");
        printf("7. Cancel Appointment\n");
        printf("8. View Prescription\n");
        printf("9. View Bed Info\n");
        printf("10. View Bill\n");
        printf("11. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

// based on user choice, call the corresponding function.
        switch (choice)
        {
            case 1:
                view_patient_profile(currentPatientID);
                break;

            case 2:
                view_available_doctors();
                break;

            case 3:
                book_appointment(currentPatientID);
                break;

            case 4:
                view_appointment(currentPatientID);
                break;

            case 5:
                search_appointment(currentPatientID);
                break;

            case 6:
                reschedule_appointment(currentPatientID);
                break;

            case 7:
                cancel_appointment(currentPatientID);
                break;

            case 8:
                view_prescription(currentPatientID);
                break;

            case 9:
                view_bed_info(currentPatientID);
                break;

            case 10:
                view_bill_patient(currentPatientID);
                break;

            case 11:
                printf("Logged out.\n");
                return;

            default:
                printf("Please choose a valid option\n");
        }

    }
}

// functions for patient portal option1 (view patient details)
void view_patient_profile(char currentPatientID[])
{
    FILE *patient_file;
    char line[300];
    int found = 0;

    patient_file = fopen("patientfile.txt", "r");
    if (patient_file == NULL)
    {
        printf("patientfile.txt could not open.\n");
        return;
    }

    printf("\n=== Patient Profile ===\n");
    printf("ID    Name                Height   Weight   Condition            Blood Group   Doctor Assigned\n");


    while (fgets(line, sizeof(line), patient_file) != NULL)
    {
        if (strncmp(line, currentPatientID, strlen(currentPatientID)) == 0)
        {
            printf("%s", line);
            found = 1;
            break;
        }
    }

    fclose(patient_file);

    if (found == 0)
    {
        printf("Patient details not found.\n");
    }
}

//function for patient portal option 2(view available doctor)
void view_available_doctors(void)
{
    FILE *doctor_file;
    char line[300];
    char doctorID[20], name[100], specialization[50], phone[20], availableTime[30], status[20];
    int found = 0;

    doctor_file = fopen("doctors_file.csv", "r");

    if (doctor_file == NULL)
    {
        printf("doctors_file.csv could not open.\n");
        return;
    }

    fgets(line, sizeof(line), doctor_file);

    printf("\n=== Available Doctors ===\n");
    printf("ID    Name                    Specialization          Time\n");
    printf("---------------------------------------------------------------\n");

    // reads one line at a time from the csv file
    while (fgets(line, sizeof(line), doctor_file) != NULL)
    {
        if (sscanf(line, "%19[^,],%99[^,],%49[^,],%19[^,],%29[^,],%19[^\r\n]",
                   doctorID, name, specialization, phone, availableTime, status) == 6) //new concept;for csv.
        {
            if (strcmp(status, "Available") == 0)
            {
                printf("%-5s %-22s %-22s %s\n", doctorID, name, specialization, availableTime);
                found = 1;
            }
        }
    }

    fclose(doctor_file);

    if (found == 0)
    {
        printf("No available doctor found.\n");
    }
}

// checks if selected doctor ID is available before booking appointment.. important for stoppoing patients from booking appointments with unavailable doctors and time slots that are already taken.
int is_doctor_available(char selectedDoctorID[])
{
    FILE *doctor_file;
    char line[300];
    char doctorID[20], name[100], specialization[50], phone[20], availableTime[30], status[20];

    doctor_file = fopen("doctors_file.csv", "r");

    if (doctor_file == NULL)
    {
        printf("doctors_file.csv could not open.\n");
        return 0;
    }


    fgets(line, sizeof(line), doctor_file);

    while (fgets(line, sizeof(line), doctor_file) != NULL)
    {
        if (sscanf(line, "%19[^,],%99[^,],%49[^,],%19[^,],%29[^,],%19[^\r\n]",
                   doctorID, name, specialization, phone, availableTime, status) == 6) //',' is the separator in csv files, so we use it to read different values into different variables
        {
            if (strcmp(doctorID, selectedDoctorID) == 0 && strcmp(status, "Available") == 0)
            {
                fclose(doctor_file);
                return 1;
            }
        }
    }

    fclose(doctor_file);
    return 0;
}

//function for patient portal option 3 (book an appoinment)
void book_appointment(char currentPatientID[])
{
    FILE *appointment_file;
    char doctorID[20], date[20], time[20];

    printf("\n=== Book Appointment ===\n");

    view_available_doctors();// showing the doctor list first to the patient for choosing a doctor ID

    printf("Enter Doctor ID: ");
    scanf("%s", doctorID);

    if (is_doctor_available(doctorID) == 0)    // appointment can only be booked if doctor is available!
    {
        printf("This doctor is not available. Appointment cannot be booked.\n");
        return;
    }

    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%s", date);

    printf("Enter Time (HH:MM): ");
    scanf("%s", time);

    appointment_file = fopen("appointments_file.txt", "a+"); //for read & write
    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    fprintf(appointment_file, "%s %s %s %s Booked\n", currentPatientID, doctorID, date, time);
    fclose(appointment_file);

    printf("Appointment booked successfully.\n");
}

//function for patient portal option 4 (seeing previous appointment)
void view_appointment(char currentPatientID[])
{
    FILE *appointment_file;
    char line[300];
    int found = 0;

    appointment_file = fopen("appointments_file.txt", "r");
    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    printf("\n=== My Appointments ===\n");
    printf("PatientID   DoctorID   	Date         	Time    	Status\n");

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (strncmp(line, currentPatientID, strlen(currentPatientID)) == 0)
        {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(appointment_file);

    if (found == 0)
    {
        printf("No appointment found.\n");
    }
}

//patient portal option 5 (search appoinment)
void search_appointment(char currentPatientID[])
{
    FILE *doctor_file;
    FILE *appointment_file;

    char line[300];

    char doctorID[20], name[100], specialization[50], phone[20], availableTime[30], status[20];
    char searchName[100];
    char selectedDoctorID[20];

    char patientID[20], appointmentDoctorID[20], date[20], time[20], appointmentStatus[20];

    int doctorFound = 0;
    int appointmentFound = 0;

    printf("\n=== Search Appointment By Doctor Name ===\n");

    // show available doctors first so patient can see the names
    view_available_doctors();

    // clearing input buffer before fgets
    while ((getchar()) != '\n');

    printf("\nEnter part of doctor name: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    doctor_file = fopen("doctors_file.csv", "r");

    if (doctor_file == NULL)
    {
        printf("doctors_file.csv could not open.\n");
        return;
    }


    fgets(line, sizeof(line), doctor_file);

    while (fgets(line, sizeof(line), doctor_file) != NULL)
    {
        if (sscanf(line, "%19[^,],%99[^,],%49[^,],%19[^,],%29[^,],%19[^\r\n]",
                   doctorID, name, specialization, phone, availableTime, status) == 6)
        {
            // partial name search, example: typing Sarah can find Dr. Sarah Rahman
            if (strstr(name, searchName) != NULL && strcmp(status, "Available") == 0)
            {
                strcpy(selectedDoctorID, doctorID);
                doctorFound = 1;
                break;
            }
        }
    }

    fclose(doctor_file);

    if (doctorFound == 0)
    {
        printf("Doctor not found or doctor is not available.\n");
        return;
    }

    appointment_file = fopen("appointments_file.txt", "r");

    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    printf("\n=== Search Result ===\n");

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (sscanf(line, "%19s %19s %19s %19s %19s",
                   patientID, appointmentDoctorID, date, time, appointmentStatus) == 5)
        {
            if (strcmp(patientID, currentPatientID) == 0 &&
                strcmp(appointmentDoctorID, selectedDoctorID) == 0)
            {
                printf("Patient ID : %s\n", patientID);
                printf("Doctor ID  : %s\n", appointmentDoctorID);
                printf("Doctor Name: %s\n", name);
                printf("Date       : %s\n", date);
                printf("Time       : %s\n", time);
                printf("Status     : %s\n", appointmentStatus);

                appointmentFound = 1;
            }
        }
    }

    fclose(appointment_file);

    if (appointmentFound == 0)
    {
        printf("No appointment found with this doctor.\n");
    }
}


// copies updated appointment data from temp.txt back to appointments_file.txt
// this avoids remove() and rename(), which sometimes cause problems in Code::Blocks
void copy_temp_to_appointment_file(void)
{
    FILE *temp_file;
    FILE *appointment_file;
    char line[300];

    temp_file = fopen("temp.txt", "r");
    appointment_file = fopen("appointments_file.txt", "w");

    if (temp_file == NULL || appointment_file == NULL)
    {
        printf("File could not update.\n");

        if (temp_file != NULL)
            fclose(temp_file);

        if (appointment_file != NULL)
            fclose(appointment_file);

        return;
    }

    while (fgets(line, sizeof(line), temp_file) != NULL)
    {
        fprintf(appointment_file, "%s", line);
    }

    fclose(temp_file);
    fclose(appointment_file);
}

//patient portal option 6 (reshedule appoinment)
void reschedule_appointment(char currentPatientID[])
{
    FILE *appointment_file;
    FILE *temp_file;

    char line[300];
    char patientID[20], doctorID[20], date[20], time[20], status[20];

    char searchDoctorID[20];
    char searchDate[20];
    char searchTime[20];

    char newDate[20];
    char newTime[20];

    int found = 0;
    int appointmentShown = 0;

    appointment_file = fopen("appointments_file.txt", "r");

    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    printf("\n=== Your Current Booked Appointments ===\n");

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (sscanf(line, "%19s %19s %19s %19s %19s",
                   patientID, doctorID, date, time, status) == 5)
        {
            if (strcmp(patientID, currentPatientID) == 0 &&
                strcmp(status, "Booked") == 0)
            {
                printf("Doctor ID: %s | Date: %s | Time: %s | Status: %s\n",
                       doctorID, date, time, status);
                appointmentShown = 1;
            }
        }
    }

    fclose(appointment_file);

    if (appointmentShown == 0)
    {
        printf("No booked appointment found to reschedule.\n");
        return;
    }

    printf("\n=== Reschedule Appointment ===\n");

    printf("Enter Doctor ID from the list above: ");
    scanf("%s", searchDoctorID);

    printf("Enter Current Date from the list above: ");
    scanf("%s", searchDate);

    printf("Enter Current Time from the list above: ");
    scanf("%s", searchTime);

    printf("Enter New Date (DD-MM-YYYY): ");
    scanf("%s", newDate);

    printf("Enter New Time (HH:MM): ");
    scanf("%s", newTime);

    appointment_file = fopen("appointments_file.txt", "r");
    temp_file = fopen("temp.txt", "w");

    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    if (temp_file == NULL)
    {
        printf("temp.txt could not open.\n");
        fclose(appointment_file);
        return;
    }

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (sscanf(line, "%19s %19s %19s %19s %19s",
                   patientID, doctorID, date, time, status) == 5)
        {
            if (strcmp(patientID, currentPatientID) == 0 &&
                strcmp(doctorID, searchDoctorID) == 0 &&
                strcmp(date, searchDate) == 0 &&
                strcmp(time, searchTime) == 0 &&
                strcmp(status, "Booked") == 0)
            {
                fprintf(temp_file, "%s %s %s %s Booked\n",
                        patientID, doctorID, newDate, newTime);
                found = 1;
            }
            else
            {
                fprintf(temp_file, "%s %s %s %s %s\n",
                        patientID, doctorID, date, time, status);
            }
        }
        else
        {
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(appointment_file);
    fclose(temp_file);

    copy_temp_to_appointment_file();

    if (found == 1)
    {
        printf("Appointment rescheduled successfully.\n");
    }
    else
    {
        printf("Appointment not found. Please enter exact Doctor ID, Date, and Time from the list.\n");
    }
}
//patient portal option 7 (cancel appoinment)
void cancel_appointment(char currentPatientID[])
{
    FILE *appointment_file;
    FILE *temp_file;

    char line[300];
    char patientID[20], doctorID[20], date[20], time[20], status[20];

    char searchDoctorID[20];
    char searchDate[20];
    char searchTime[20];

    int found = 0;
    int appointmentShown = 0;

    appointment_file = fopen("appointments_file.txt", "r");

    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    printf("\n=== Your Current Booked Appointments ===\n");

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (sscanf(line, "%19s %19s %19s %19s %19s",
                   patientID, doctorID, date, time, status) == 5)
        {
            if (strcmp(patientID, currentPatientID) == 0 &&
                strcmp(status, "Booked") == 0)
            {
                printf("Doctor ID: %s | Date: %s | Time: %s | Status: %s\n",
                       doctorID, date, time, status);
                appointmentShown = 1;
            }
        }
    }

    fclose(appointment_file);

    if (appointmentShown == 0)
    {
        printf("No booked appointment found to cancel.\n");
        return;
    }

    printf("\n=== Cancel Appointment ===\n");

    printf("Enter Doctor ID from the list above: ");
    scanf("%s", searchDoctorID);

    printf("Enter Date from the list above: ");
    scanf("%s", searchDate);

    printf("Enter Time from the list above: ");
    scanf("%s", searchTime);

    appointment_file = fopen("appointments_file.txt", "r");
    temp_file = fopen("temp.txt", "w");

    if (appointment_file == NULL)
    {
        printf("appointments_file.txt could not open.\n");
        return;
    }

    if (temp_file == NULL)
    {
        printf("temp.txt could not open.\n");
        fclose(appointment_file);
        return;
    }

    while (fgets(line, sizeof(line), appointment_file) != NULL)
    {
        if (sscanf(line, "%19s %19s %19s %19s %19s",
                   patientID, doctorID, date, time, status) == 5)
        {
            if (strcmp(patientID, currentPatientID) == 0 &&
                strcmp(doctorID, searchDoctorID) == 0 &&
                strcmp(date, searchDate) == 0 &&
                strcmp(time, searchTime) == 0 &&
                strcmp(status, "Booked") == 0)
            {
                fprintf(temp_file, "%s %s %s %s Cancelled\n",
                        patientID, doctorID, date, time);
                found = 1;
            }
            else
            {
                fprintf(temp_file, "%s %s %s %s %s\n",
                        patientID, doctorID, date, time, status);
            }
        }
        else
        {
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(appointment_file);
    fclose(temp_file);

    copy_temp_to_appointment_file();

    if (found == 1)
    {
        printf("Appointment cancelled successfully.\n");
    }
    else
    {
        printf("Appointment not found. Please enter exact Doctor ID, Date, and Time from the list.\n");
    }
}
//function for patient portal option 8 (for prescription)
void view_prescription(char currentPatientID[])
{
    FILE *prescription_file;
    char line[300];
    int found = 0;

    prescription_file = fopen("prescriptions_file.txt", "r");
    if (prescription_file == NULL)
    {
        printf("prescriptions_file.txt could not open.\n");
        return;
    }

    printf("\n=== My Prescription ===\n");
    printf("PatientID   	DoctorID   	Diagnosis      	Medicine        Dosage        	Advice\n");

    while (fgets(line, sizeof(line), prescription_file) != NULL)
    {
        if (strncmp(line, currentPatientID, strlen(currentPatientID)) == 0)
        {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(prescription_file);

    if (found == 0)
    {
        printf("No prescription found.\n");
    }
}

//function for patient portal option 9 (bed booked by admin, patient can't book beds themselves)
void view_bed_info(char currentPatientID[])
{
    FILE *bed_file;
    char line[300];
    int found = 0;

    bed_file = fopen("beds_file.txt", "r");
    if (bed_file == NULL)
    {
        printf("beds_file.txt could not open.\n");
        return;
    }

    printf("\n=== My Bed Info ===\n");
    printf("Patient ID   	Bed No   	Ward         		RoomNo   	Admission Date   	Discharge Date     	Bed Charge   	Status\n");

    while (fgets(line, sizeof(line), bed_file) != NULL)
    {
        if (strncmp(line, currentPatientID, strlen(currentPatientID)) == 0)
        {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(bed_file);

    if (found == 0)
    {
        printf("No bed assigned.\n");
    }
}

//function for patient portal option 10 (view the bill)
void view_bill_patient(char currentPatientID[])
{
    FILE *bill_file;
    char line[300];
    int found = 0;

    bill_file = fopen("bills_file.txt", "r");
    if (bill_file == NULL)
    {
        printf("bills_file.txt could not open.\n");
        return;
    }

    printf("\n=== My Bill ===\n");
    printf("PatientID   	Consultation Fee   	Bed Charge   	Medicine Cost   Total Bill   	Status\n");

    while (fgets(line, sizeof(line), bill_file) != NULL)
    {
        if (strncmp(line, currentPatientID, strlen(currentPatientID)) == 0)
        {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(bill_file);

    if (found == 0)
    {
        printf("No bill found.\n");
    }
}

// zahra
void billing_portal(void)
{
    char username[20];
    int password;

    printf("\n=== Billing & Payment Portal Login ===\n");

    do
    {
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%d", &password);

        if(password != B_PASSWORD || strcmp(username, B_USERNAME)!= 0)
        {
            printf("Wrong credentials. Please try again.\n");
        }
    }
     while(password != B_PASSWORD || strcmp(username, B_USERNAME)!=0);
        printf("\nLogin Sucessful.\n");

        //main menu of billing & payment portal
        int choice;

        while(1)
        {
            printf("\n=== Billing & Payment menu ===\n");
            printf("1. Generate Bill\n");
            printf("2. View Bill Details\n");
            printf("3. Process Payment\n");
            printf("4. View Payment History\n");
            printf("5. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);

            switch(choice)
            {
                case 1:
                    generate_bill();
                    break;
                case 2:
                    view_bill();
                    break;
                case 3:
                    process_payment();
                    break;
                case 4:
                    view_payment_history();
                    break;
                case 5:
                    return;
                default:
                    printf("Error: Please enter valid integer from choices given.\n");
            }
        }
}
//option 1: to generate bill from all costs
void generate_bill(void)
{
   int search_id;
   int found_bill=0, found_bed=0, found_presc = 0;

   //bill types
   int b_id, consult, bed_charge, med_cost, total;
   char b_status[20];

   //bed types
   int bd_id, room;
   char bed_no[20], ward[20], admit[20], discharge[20], bd_status[20];
   int bd_charge;

   //prescription types
   int p_id, doc_id;
   char diagnosis[50], medicine[50], dosage[50], advice[50];

   printf("\n=== Generate Bill ===\n");
   printf("Enter Patient ID: ");
   scanf("%d", &search_id);

   //opening bills, beds, and prescriptions files
   FILE *bills = fopen("bills_file.txt", "r");
   FILE *beds = fopen("beds_file.txt", "r");
   FILE *presc = fopen("prescriptions_file.txt","r");

   if(bills==NULL || beds==NULL || presc==NULL)
   {
       printf("Error: Could not open one or more data file\n");
       return;
   }
    //searching bills file
    //faced issue in opening files due to dashed lines
    //so improvised and skipped all header lines until hit dashed seperator '-'
    char skip[200];
    while(fgets(skip, 200, bills)&&skip[0]!='-');

    while(fscanf(bills, "%d %d %d %d %d %s",
           &b_id, &consult, &bed_charge, &med_cost, &total, b_status) == 6)
    {
        if (b_id == search_id)
        {
            found_bill = 1;
            break;
        }
    }
    fclose(bills);

    //search beds file
    while(fgets(skip, 200, beds) && skip[0] != '-');

    while(fscanf(beds, "%d %s %s %d %s %s %d %s", &bd_id, bed_no, ward, &room, admit, discharge, &bd_charge, bd_status) == 8)
    {
        if(bd_id == search_id)
        {
            found_bed = 1;
            break;
        }
    }
    fclose(beds);

    //search prescription file
    while(fgets(skip, 200, presc) && skip[0] != '-');

    while(fscanf(presc, "%d %d %s %s %s %s",
           &p_id, &doc_id, diagnosis, medicine, dosage, advice) == 6)
    {
        if (p_id == search_id)
        {
            found_presc = 1;
            break;
        }
    }
    fclose(presc);

    if(!found_bill)
    {
        printf("Error: Patient ID %03d not found in billing records.\n", search_id);
        return;
    }

    // calculating total bill and printing generated bill
    int new_total = consult + bed_charge + med_cost;

    printf("\n===============================================================\n");
    printf("\n                  GENERATED BILL - PATIENT %03d                \n", search_id);
    printf("\n===============================================================\n");

    printf("Consultation Fee  : %d BDT\n", consult);

    if(found_bed)
    {
        printf("Ward              : %s\n", ward);
        printf("Bed No            : %s\n", bed_no);
        printf("Room No           : %d\n", room);
        printf("Admission Date    : %s\n", admit);
        printf("Discharge Date    : %s\n", discharge);
        printf("Bed Charge        : %d BDT\n", bed_charge);
    }
    else
    {
        printf("Bed Charge        : 0 BDT (Outpatient)\n");
    }

    if(found_presc)
    {
        printf("Diagnosis         : %s\n", diagnosis);
        printf("Medicine          : %s\n", medicine);
        printf("Dosage            : %s\n", dosage);
    }

    printf("Medicine Cost     : %d BDT\n", med_cost);
    printf("---------------------------------------------------------------\n");
    printf("TOTAL BILL        : %d BDT\n", new_total);
    printf("Status            : %s\n", b_status);
    printf("===============================================================\n");

}
//option 2: to view the generated bill by searching up patient ID
void view_bill(void)
{
    int search_id;
    int b_id, consult, bed_charge, med_cost, total;
    char b_status[20];
    int found = 0;
    char skip[200];

    printf("\n=== View Bill Details ===\n");
    printf("Enter Patient ID: ");
    scanf("%d", &search_id);

    FILE *bills = fopen("bills_file.txt", "r");
    if(bills == NULL)
    {
        printf("Error: Could not open bills_file.txt\n");
        return;
    }

    //skip header lines like before to avoid error
    while(fgets(skip, 200, bills) && skip[0] != '-');

    while(fscanf(bills, "%d %d %d %d %d %s",
           &b_id, &consult, &bed_charge, &med_cost, &total, b_status) == 6)
    {
        if(b_id == search_id)
        {
            found = 1;
            printf("\n===============================================================\n");
            printf("\n               BILL DETAILS - PATIENT %03d                     \n", b_id);
            printf("\n===============================================================\n");
            printf("%-25s: %d BDT\n", "Consultation Fee",  consult);
            printf("%-25s: %d BDT\n", "Bed Charge",        bed_charge);
            printf("%-25s: %d BDT\n", "Medicine Cost",     med_cost);
            printf("---------------------------------------------------------------\n");
            printf("%-25s: %d BDT\n", "Total Bill",        total);
            printf("%-25s: %s\n",     "Payment Status",    b_status);
            printf("===============================================================\n");
            break;
        }
    }

    fclose(bills);

    if(!found)
        printf("Error: Patient ID %03d not found.\n", search_id);
}

//option 3: Processes payment by up choosing payment method and then updates payment status
void process_payment(void)
{
    int search_id, choice;
    int b_id, consult, bed_charge, med_cost, total;
    char b_status[20];
    char payment_method[20];
    int found = 0;
    char skip[200];

    printf("\n=== Process Payment ===\n");

    // payment method selection
    printf("\nSelect Payment Method:\n");
    printf("1. Cash\n");
    printf("2. Credit Card\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            strcpy(payment_method, "Cash");
            printf("Payment method: Cash\n");
            break;
        case 2:
            strcpy(payment_method, "Credit Card");
            printf("Payment method: Credit Card\n");
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("\nEnter Patient ID to be marked as Paid: ");
    scanf("%d", &search_id);

    FILE *bills = fopen("bills_file.txt", "r");
    FILE *temp  = fopen("bills_temp.txt", "w");

    if(bills == NULL || temp == NULL)
    {
        printf("Error: Could not open files.\n");
        return;
    }

    while(fgets(skip, 200, bills) && skip[0] != '-')
        fputs(skip, temp);
    fputs(skip, temp);

    while(fscanf(bills, "%d %d %d %d %d %s",
           &b_id, &consult, &bed_charge, &med_cost, &total, b_status) == 6)
    {
        if(b_id == search_id)
        {
            found = 1;

            if(strcmp(b_status, "Paid") == 0)
            {
                printf("Patient %03d's bill is already marked as Paid.\n", search_id);
                fclose(bills);
                fclose(temp);
                remove("bills_temp.txt");
                return;
            }

            strcpy(b_status, "Paid");
            printf("\n==============================\n");
            printf("       PAYMENT RECEIPT        \n");
            printf("==============================\n");
            printf("Patient ID     : %03d\n", b_id);
            printf("Total Bill     : %d BDT\n", total);
            printf("Payment Method : %s\n", payment_method);
            printf("Status         : Paid\n");
            printf("==============================\n");
        }

        fprintf(temp, "%03d %d %d %d %d %s\n",
                b_id, consult, bed_charge, med_cost, total, b_status);
    }
    fclose(bills);
    fclose(temp);

    if(!found)
    {
        printf("Error: Patient ID %03d not found.\n", search_id);
        remove("bills_temp.txt");
        return;
    }

    remove("bills_file.txt");
    rename("bills_temp.txt", "bills_file.txt");
    printf("Payment status updated successfully.\n");
}
//option 4: shows payment history of all patients who paid
void view_payment_history(void)
{
    int b_id, consult, bed_charge, med_cost, total;
    char b_status[20];
    int count = 0;
    char skip[200];

    printf("\n=== Payment History (Paid Bills) ===\n");

    FILE *bills = fopen("bills_file.txt", "r");
    if(bills == NULL)
    {
        printf("Error: Could not open bills_file.txt\n");
        return;
    }

    // skip header lines
    while (fgets(skip, 200, bills) && skip[0] != '-');

    printf("\n===============================================================\n");
    printf("\n PatientID   Consult   BedCharge   Medicine   Total   Status  \n");
    printf("\n===============================================================\n");

    while(fscanf(bills, "%d %d %d %d %d %s",
           &b_id, &consult, &bed_charge, &med_cost, &total, b_status) == 6)
    {
        if(strcmp(b_status, "Paid") == 0)
        {
            printf("  %03d         %-9d %-11d %-10d %-7d %s\n",
                   b_id, consult, bed_charge, med_cost, total, b_status);
            count++;
        }
    }

    printf("===============================================================\n");

    if(count == 0)
        printf("No paid bills found.\n");
    else
        printf("Total paid records: %d\n", count);

    fclose(bills);
}
