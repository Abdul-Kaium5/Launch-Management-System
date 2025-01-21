#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define RESET "\033[0m"
#define LIGHT_CYAN "\033[1;36m"
#define LIGHT_YELLOW "\033[1;93m"
#define LIGHT_GREEN "\033[92m"
#define LIGHT_RED "\033[91m"
#define ROWS 15
#define COLS 4
#define SEAT_PRICE 500.00f
#define MAX_SCHEDULES 7
#define CABIN_PRICE_AC 2500.00f
#define CABIN_PRICE_NON_AC 1500.00f
#define CABIN_PRICE_VIP 5000.00f
#define NUM_CABINS_AC 10
#define NUM_CABINS_NON_AC 15
#define NUM_CABINS_VIP 5
#define MAX_CABINS_PER_ORDER 15
#define MAX_LEN 100

int currentScheduleId = -1;
int acCabins[NUM_CABINS_AC] = {0};
int nonAcCabins[NUM_CABINS_NON_AC] = {0};
int vipCabins[NUM_CABINS_VIP] = {0};
int chairSeats[MAX_SCHEDULES][ROWS * COLS];

typedef struct ChairOrder
{
    int orderId;
    char userName[50];
    char userPhone[15];
    char seatsBooked[ROWS * COLS][4];
    int numSeatsBooked;
    char orderTime[20];
    float chairPrice;
} ChairOrder;


typedef struct Schedule
{
    char route[17];
    char day[11];
    char departureTime[9];
    char arrivalTime[9];
    int id;
} Schedule;


typedef struct CabinOrder
{
    int orderId;
    char userName[50];
    char userPhone[15];
    int cabinNumbers[MAX_CABINS_PER_ORDER];
    float cabinPrice;
    int numCabinsBooked;
    char orderTime[20];
    char cabinType[10];
    int scheduleId;
} CabinOrder;

void createSecretFile();
int adminLogin();
void displayHeader();
void clearInputBuffer();
void printOption();
void passengerPortal();
void adminDashboard();
void viewSchedules();
void bookATicket();
void addSchedule();
void updateSchedule();
void bubbleSortSchedules(Schedule schedules[], int count);
void deleteSchedule();
int idExists(int targetId);
void chair();
void viewAvailableSeats(int currentScheduleId);
void bookChairSeats(int currentScheduleId);
void saveChairSeatsToFile(int currentScheduledId);
void loadChairSeatsFromFile();
void saveChairBookingToFile(ChairOrder *order);
void loadOrdersFromFiles();
float calculateTotalPrice(int numSeats);
int generateUniqueOrderID();
void clearAllSeats(int currentScheduleId);
void bookCabin(int cabinType);
void showCabinInvoice(CabinOrder *order);
void viewAvailableCabinsByType(int cabinType);
void saveCabinStatusesToFile();
void loadCabinStatusesFromFile();
void resetCabinStatusesByType();
void loadCabinStatusesForSchedule(int scheduleId);
void saveCabinStatusesForSchedule(int scheduleId);
void initializeCabinStatusesForSchedule(int scheduleId);
void initializeCabinStatusFile(int scheduleId);

void createSecretFile()
{
    FILE *file = fopen("secret.txt", "r");

    if (file == NULL)
    {
        file = fopen("secret.txt", "w");
        if (file == NULL)
        {
            printf("\n\t%sError: Unable to create 'secret.txt'.%s\n", LIGHT_RED, RESET);
            return;
        }

        const char *username = "admin";
        const char *password = "password123";

        fprintf(file, "%s\n%s\n", username, password);
        fclose(file);
    }
    else
    {
        fclose(file);
        printf("'secret.txt' already exists.\n");
    }
}


int adminLogin()
{
    char storedUsername[MAX_LEN], storedPassword[MAX_LEN];
    char inputUsername[MAX_LEN], inputPassword[MAX_LEN];

    FILE *file = fopen("secret.txt", "r");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to open secret file.%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return 0;
    }

    fscanf(file, "%s", storedUsername);
    fscanf(file, "%s", storedPassword);
    fclose(file);

    printf("\n\t%sCheck 'secret.txt' file for 'username' & 'password'!\n%s", LIGHT_RED, RESET);
    printf("\n\t%sEnter Admin Username: %s", LIGHT_CYAN, RESET);
    fgets(inputUsername, sizeof(inputUsername), stdin);
    inputUsername[strcspn(inputUsername, "\n")] = '\0';

    printf("\n\t%sEnter Admin Password: %s", LIGHT_CYAN, RESET);
    fgets(inputPassword, sizeof(inputPassword), stdin);
    inputPassword[strcspn(inputPassword, "\n")] = '\0';

    if (strcmp(inputUsername, storedUsername) == 0 && strcmp(inputPassword, storedPassword) == 0)
    {
        printf("\n\t%sLogin successful! Welcome to the Admin Dashboard.%s\n", LIGHT_GREEN, RESET);
        return 1;
    }
    else
    {
        printf("\n\t%sInvalid username or password.%s\n", LIGHT_RED, RESET);
        return 0;
    }
}

void displayHeader()
{
    printf("\n\n\t%s         ========================         \n", LIGHT_CYAN);
    printf("\t\t %sLaunch Management System\t%s   \n", LIGHT_YELLOW, LIGHT_CYAN);
    printf("\t         ========================         %s\n\n", RESET);
}

void clearInputBuffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void printOption()
{
    printf("\t%sWelcome to the Launch Management System!\n\n", LIGHT_CYAN);
    printf("\t%s====================\n", LIGHT_YELLOW);
    printf("\t1. Passenger Portal\n");
    printf("\t2. Admin Dashboard\n");
    printf("\t3. Exit\n");
    printf("\t====================\n%s", RESET);
    printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);
}

void passengerPortal()
{
    int option;
    while (1)
    {

        system("cls");
        printf("\n\t%s=========================\n", LIGHT_YELLOW);
        printf("\t     Passenger Portal\n");
        printf("\t=========================\n");
        printf("\t1. View Schedules\n");
        printf("\t2. Book a Ticket\n");
        printf("\t3. Home\n");
        printf("\t4. Exit\n");
        printf("\t=========================\n%s", RESET);
        printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);

        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        clearInputBuffer();

        switch (option)
        {
        case 1:
            viewSchedules();
            break;
        case 2:
            bookATicket();
            break;
        case 3:
            return;
        case 4:
            system("cls");
            printf("%s\n\n\tThank you for choosing our service.\n\n%s", LIGHT_GREEN, RESET);
            exit(0);
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
}

void adminDashboard()
{
    int option;
    while (1)
    {

        system("cls");
        printf("\n\t%s========================\n", LIGHT_YELLOW);
        printf("\t     Admin Dashboard\n");
        printf("\t========================\n");
        printf("\t1. Add Schedule\n");
        printf("\t2. Update Schedule\n");
        printf("\t3. Delete Schedule\n");
        printf("\t4. Reset Chair Seat\n");
        printf("\t5. Reset Cabin status\n");
        printf("\t6. Order History\n");
        printf("\t7. Home\n");
        printf("\t8. Exit\n");
        printf("\t========================\n%s", RESET);
        printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);

        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        clearInputBuffer();

        switch (option)
        {
        case 1:
            addSchedule();
            break;
        case 2:
            updateSchedule();
            break;
        case 3:
            deleteSchedule();
            break;
        case 4:
            system("cls");
            printf("\n\t%sEnter schedule ID (1 to %d) to reset seats: %s", LIGHT_CYAN, MAX_SCHEDULES, RESET);
            scanf("%d", &currentScheduleId);
            clearAllSeats(currentScheduleId);
            break;
        case 5:
            system("cls");
            resetCabinStatusesByType();
            printf("\n\t%sPress any key to return to the admin dashboard...%s", LIGHT_CYAN, RESET);
            getchar();
            break;
        case 6:
            system("cls");
            loadOrdersFromFiles();
            printf("\n\t%sPress any key to return to the admin dashboard...%s", LIGHT_CYAN, RESET);
            getchar();
            break;
        case 7:
            return;
        case 8:
            system("cls");
            printf("%s\n\n\tThank you for choosing our service.\n\n%s", LIGHT_GREEN, RESET);
            exit(0);
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
}

void bubbleSortSchedules(Schedule schedules[], int count)
{
    Schedule temp;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (schedules[i].id > schedules[j].id)
            {
                temp = schedules[j];
                schedules[j] = schedules[i];
                schedules[i] = temp;
            }
        }
    }
}

void viewSchedules()
{
    system("cls");
    Schedule schedules[100];
    int count = 0;
    char line[100];
    FILE *file = fopen("schedules.txt", "r");
    if (file == NULL)
    {
        printf("\n\t%sError opening file.%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }
    printf("\n\t\t%s=========================\n", LIGHT_YELLOW);
    printf("\t\t     View Schedules\n");
    printf("\t\t=========================\n");
    printf("\n  %-5s    %-14s %-8s %-10s %-10s\n", "Id", "Route", "Day", "Departure", "Arrival");
    printf("--------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedules[count].id, schedules[count].route, schedules[count].day, schedules[count].departureTime, schedules[count].arrivalTime) == 5)
        {
            count++;
        }
    }
    fclose(file);

    bubbleSortSchedules(schedules, count);
    for (int i = 0; i < count; i++)
    {
        printf("  %-3d %-17s %-10s %-10s %-10s\n", schedules[i].id, schedules[i].route, schedules[i].day, schedules[i].departureTime, schedules[i].arrivalTime);
    }
    printf("\nPress any key to return to the menu...%s", RESET);
    getchar();
}

void bookATicket()
{
    system("cls");
    int option;
    printf("\n\t%s=========================\n", LIGHT_YELLOW);
    printf("\t      Book a Ticket\n");
    printf("\t=========================\n");
    printf("\t1. Chair\n");
    printf("\t2. AC Cabin\n");
    printf("\t3. Non-AC Cabin\n");
    printf("\t4. VIP Cabin\n");
    printf("\t5. Home\n");
    printf("\t6. Exit\n");
    printf("\t========================\n%s", RESET);
    printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);

    if (scanf("%d", &option) != 1)
    {
        clearInputBuffer();
        printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }

    clearInputBuffer();

    switch (option)
    {
    case 1:
        chair();
        break;
    case 2:
        bookCabin(1);
        break;
    case 3:
        bookCabin(2);
        break;
    case 4:
        bookCabin(3);
        break;
    case 5:
        return;
    case 6:
        system("cls");
        printf("%s\n\n\tThank you for choosing our service.\n\n%s", LIGHT_GREEN, RESET);
        exit(0);
    default:
        printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
        Sleep(1500);
        break;
    }
}

void orderFood()
{
}

void addSchedule()
{
    int option, newId;
    Schedule schedule;
    FILE *file;

    while (1)
    {
        system("cls");
        printf("\n\t%s==================================\n", LIGHT_YELLOW);
        printf("\t          Add Schedule\n");
        printf("\t==================================%S\n", RESET);

        file = fopen("schedules.txt", "a");

        if (file == NULL)
        {
            printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);
            Sleep(1500);
            return;
        }

        printf("\n\t%sEnter the ID number (e.g, '1'): %s", LIGHT_CYAN, RESET);
        scanf("%d", &newId);
        clearInputBuffer();

        while (idExists(newId))
        {
            printf("\n\t%sError: ID %d already exists. Please choose a different ID.%s\n", LIGHT_RED, newId, RESET);
            printf("\n\t%sEnter a new ID: %s", LIGHT_CYAN, RESET);
            scanf("%d", &newId);
            clearInputBuffer();
        }

        schedule.id = newId;

        printf("\t%sEnter the route (e.g, 'Dhaka to Barisal'): %s", LIGHT_CYAN, RESET);
        scanf("%30[^\n]", schedule.route);
        clearInputBuffer();

        printf("\t%sEnter the day (e.g, 'Friday'): %s", LIGHT_CYAN, RESET);
        scanf("%11[^\n]", schedule.day);
        clearInputBuffer();

        printf("\t%sEnter the departure time (e.g., '9:00 PM'): %s", LIGHT_CYAN, RESET);
        scanf("%9[^\n]", schedule.departureTime);
        clearInputBuffer();

        printf("\t%sEnter the arrival time (e.g., '5:00 AM'): %s", LIGHT_CYAN, RESET);
        scanf("%9[^\n]", schedule.arrivalTime);
        clearInputBuffer();

        fprintf(file, "%d,%s,%s,%s,%s\n", schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime);

        fclose(file);
        printf("\n\t%sSchedule added successfully.%s\n", LIGHT_GREEN, RESET);
        Sleep(1500);

        printf("\n\t%sDo you want to add another schedule?\n", LIGHT_CYAN);
        printf("\t1. Yes\n");
        printf("\t2. No\n");
        printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);

        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        switch (option)
        {
        case 1:
            continue;
            break;
        case 2:
            return;
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
}

void updateSchedule()
{
    int targetId, found = 0, option;
    char line[100];
    Schedule schedule;
    FILE *file, *tempFile;

    while (1)
    {
        system("cls");
        printf("\n\t%s==================================\n", LIGHT_YELLOW);
        printf("\t          Update Schedule\n");
        printf("\t==================================%s\n", RESET);
        printf("\n\t%sEnter the ID of the schedule to update: %s", LIGHT_CYAN, RESET);
        scanf("%d", &targetId);
        clearInputBuffer();

        file = fopen("schedules.txt", "r");
        tempFile = fopen("temp.txt", "w");
        if (file == NULL || tempFile == NULL)
        {
            printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);
            Sleep(1500);
            return;
        }

        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
            {
                if (schedule.id == targetId)
                {
                    found = 1;
                    printf("\t%sEnter the new route (current: %s): %s", LIGHT_CYAN, schedule.route, RESET);
                    scanf("%30[^\n]", schedule.route);
                    clearInputBuffer();

                    printf("\t%sEnter the new day (current: %s): %s", LIGHT_CYAN, schedule.day, RESET);
                    scanf("%11[^\n]", schedule.day);
                    clearInputBuffer();

                    printf("\t%sEnter the new departure time (current: %s): %s", LIGHT_CYAN, schedule.departureTime, RESET);
                    scanf("%9[^\n]", schedule.departureTime);
                    clearInputBuffer();

                    printf("\t%sEnter the new arrival time (current: %s): %s", LIGHT_CYAN, schedule.arrivalTime, RESET);
                    scanf("%9[^\n]", schedule.arrivalTime);
                    clearInputBuffer();
                }
                fprintf(tempFile, "%d,%s,%s,%s,%s\n", schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime);
            }
        }

        fclose(file);
        fclose(tempFile);

        if (found)
        {
            remove("schedules.txt");
            rename("temp.txt", "schedules.txt");
            printf("\n\t%sSchedule updated successfully.%s\n", LIGHT_GREEN, RESET);
            Sleep(1500);
        }
        else
        {
            printf("\n\t%sNo schedule found with ID %d. Please try again.%s\n", LIGHT_RED, targetId, RESET);
            remove("temp.txt");
            Sleep(1500);
            continue;
        }

        printf("\n\t%sDo you want to update another schedule?\n", LIGHT_CYAN);
        printf("\t1. Yes\n");
        printf("\t2. No\n");
        printf("\n\t%sPlease select an option: %s", LIGHT_CYAN, RESET);

        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.%s\n", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        clearInputBuffer();

        switch (option)
        {
        case 1:
            continue;
            break;
        case 2:
            return;
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
}

void deleteSchedule()
{
    int targetId, found = 0;
    char line[100];
    Schedule schedule;

    FILE *file = fopen("schedules.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }
    system("cls");
    printf("\n\t%s==================================\n", LIGHT_YELLOW);
    printf("\t          Delete Schedule\n");
    printf("\t==================================%s\n", RESET);
    printf("\n\t%sEnter the ID of the schedule to delete: %s", LIGHT_CYAN, RESET);
    scanf("%d", &targetId);
    clearInputBuffer();

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
        {
            if (schedule.id == targetId)
            {
                found = 1;
                continue;
            }
            fprintf(tempFile, "%d,%s,%s,%s,%s\n", schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("schedules.txt");
        rename("temp.txt", "schedules.txt");
        printf("\n\t%sSchedule with ID %d has been deleted sucessfully.%s\n", LIGHT_GREEN, targetId, RESET);
        Sleep(1500);
    }
    else
    {
        printf("\n\t%sNo schedule found with ID %d.%s\n", LIGHT_RED, targetId, RESET);
        remove("temp.txt");
        Sleep(1500);
    }
}

int idExists(int targetId)
{
    FILE *file = fopen("schedules.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    Schedule schedule;
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
        {
            if (schedule.id == targetId)
            {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

void chair()
{
    system("cls");
    int option, targetId, found = 0;
    Schedule schedule;
    FILE *file = fopen("Schedules.txt", "r");
    char line[100];

    if (file == NULL)
    {
        printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }

    printf("\n\t%s==================================\n", LIGHT_YELLOW);
    printf("\t         Book Chair Ticket\n");
    printf("\t==================================\n");
    printf("\n  %-5s    %-14s %-8s %-10s %-10s\n", "Id", "Route", "Day", "Departure", "Arrival");
    printf("--------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
        {
            printf("  %-3d %-17s %-10s %-10s %-10s\n", schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime);
        }
    }
    fclose(file);
    printf("\n%sEnter the ID of the schedule to view seats or book a Chair ticket: %s", LIGHT_CYAN, RESET);
    scanf("%d", &targetId);
    clearInputBuffer();
    file = fopen("schedules.txt", "r");

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
        {
            if (schedule.id == targetId)
            {
                found = 1;
                break;
            }
        }
    }
    fclose(file);
    if (found)
    {
        currentScheduleId = targetId;
        loadChairSeatsFromFile(currentScheduleId);
        system("cls");
        printf("\n%s========================\n", LIGHT_YELLOW);
        printf("1. View Available Seats\n");
        printf("2. Book Chair Ticket\n");
        printf("3. Home\n");
        printf("4. Exit\n");
        printf("========================\n%s", RESET);
        printf("\n%sPlease select an option: %s", LIGHT_CYAN, RESET);
        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
            Sleep(1500);
            return;
        }
        clearInputBuffer();

        switch (option)
        {
        case 1:
            viewAvailableSeats(currentScheduleId);
            break;
        case 2:
            bookChairSeats(currentScheduleId);
            break;
        case 3:
            return;
        case 4:
            system("cls");
            printf("%s\n\n\tThank you for choosing our service.\n\n%s", LIGHT_GREEN, RESET);
            exit(0);
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
    else
    {
        printf("\n\t%sNo schedule found with ID %d.%s\n", LIGHT_RED, targetId, RESET);
    }

    printf("\nPress any key to return to the menu...");
    getchar();
}


void viewAvailableSeats(int currentScheduleId)
{
    if (currentScheduleId == -1)
    {
        printf("\n\t%sNo schedule selected. Please select a schedule first. %s\n", LIGHT_RED, RESET);
        return;
    }
    loadChairSeatsFromFile(currentScheduleId);
    system("cls");
    printf("\n%sAvailable Seats for Chair (Schedule ID: %d)%s\n", LIGHT_YELLOW, currentScheduleId, RESET);
    printf("\n------------------------------------------\n");

    for (int row = 0; row < ROWS; row++)
    {
        printf("\t");
        for (int col = 0; col < COLS; col++)
        {
            int seatIndex = row * COLS + col;
            char seatLabel[4];
            snprintf(seatLabel, sizeof(seatLabel), "%c%d", 'A' + row, col + 1);

            if (chairSeats[currentScheduleId][seatIndex] == 0)
            {
                printf("%s[%s]%s  ", LIGHT_GREEN, seatLabel, RESET);
            }
            else
            {
                printf("%s[%s%sX%s%s ]%s  ", LIGHT_GREEN, RESET, LIGHT_RED, RESET, LIGHT_GREEN, RESET);
            }
        }
        printf("\n");
    }
    printf("\n------------------------------------------\n");
}

void bookChairSeats(int currentScheduleId)
{
    loadChairSeatsFromFile(currentScheduleId);

    char seatLabel[4];
    char userName[50], userPhone[15], Time[20];
    int row, col, seatIndex, numTickets, bookedCount = 0, availableSeats = 0;
    char confirm;
    float totalChairPrice = 0.0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(Time, sizeof(Time), "%I:%M %p %d-%m-%Y", &tm);

    ChairOrder newOrder;

    FILE *file;

    for (int i = 0; i < ROWS * COLS; i++)
    {
        if (chairSeats[currentScheduleId][i] == 0)
        {
            availableSeats++;
        }
    }

    if (availableSeats == 0)
    {
        printf("\n\t%sNo seats are available for booking.%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }

    system("cls");
    printf("\n\t%sUser Information%s\n", LIGHT_YELLOW, RESET);
    printf("\n\t%sEnter your name: %s", LIGHT_CYAN, RESET);
    fgets(userName, sizeof(userName), stdin);

    userName[strcspn(userName, "\n")] = '\0';

    printf("\n\t%sEnter your phone number: %s", LIGHT_CYAN, RESET);
    fgets(userPhone, sizeof(userPhone), stdin);

    userPhone[strcspn(userPhone, "\n")] = '\0';

    newOrder.orderId = generateUniqueOrderID();
    strcpy(newOrder.userName, userName);
    strcpy(newOrder.userPhone, userPhone);
    strcpy(newOrder.orderTime, Time);
    newOrder.numSeatsBooked = 0;

    while (1)
    {
        system("cls");
        printf("\n\t%sBook a Chair Seat (Schedule ID: %d)%s\n", LIGHT_YELLOW, currentScheduleId, RESET);
        viewAvailableSeats(currentScheduleId);

        availableSeats = 0;

        for (int i = 0; i < ROWS * COLS; i++)
        {
            if (chairSeats[currentScheduleId][i] == 0)
            {
                availableSeats++;
            }
        }

        printf("\n\t%sAvailable Seats: %d%s\n", LIGHT_GREEN, availableSeats, RESET);
        printf("\n\t%sPrice per seat: %.2f%s\n", LIGHT_GREEN, SEAT_PRICE, RESET);

        printf("\n\t%sHow many tickets would you like to book? %s", LIGHT_CYAN, RESET);

        if (scanf("%d", &numTickets) != 1 || numTickets <= 0)
        {
            clearInputBuffer();
            printf("\n\t%sInvalid input. Please enter a valid number.%s\n", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        clearInputBuffer();

        if (numTickets > availableSeats)
        {
            printf("\n\t%sYou requested %d tickets, but only %d seats are available.%s\n", LIGHT_RED, numTickets, availableSeats, RESET);
            Sleep(1500);
            continue;
        }

        bookedCount = 0;

        while (bookedCount < numTickets)
        {
            printf("\n\t%sEnter the seat label for ticket %d (e.g., A1): %s", LIGHT_CYAN, bookedCount + 1, RESET);
            scanf("%3s", seatLabel);
            clearInputBuffer();

            seatLabel[0] = toupper(seatLabel[0]);

            if (strlen(seatLabel) < 2 || strlen(seatLabel) > 3 || seatLabel[0] < 'A' || seatLabel[0] >= 'A' + 15 || !isdigit(seatLabel[1]) || (seatLabel[2] && !isdigit(seatLabel[2])))
            {
                printf("\n\t%sInvalid seat label. Please try again.%s\n", LIGHT_RED, RESET);
                Sleep(1500);
                continue;
            }

            row = seatLabel[0] - 'A';
            col = atoi(&seatLabel[1]) - 1;

            if (col < 0 || col >= COLS)
            {
                printf("\n\t%sInvalid seat label. Please try again.%s\n", LIGHT_RED, RESET);
                Sleep(1500);
                continue;
            }

            seatIndex = row * COLS + col;

            if (chairSeats[currentScheduleId][seatIndex] == 1)
            {
                printf("\n\t%sSeat %s is already booked. Please choose another seat.%s\n", LIGHT_RED, seatLabel, RESET);
                Sleep(1500);
                continue;
            }

            printf("\n\t%sAre you sure you want to book seat %s? (Y/n): %s", LIGHT_CYAN, seatLabel, RESET);
            confirm = getchar();
            clearInputBuffer();
            if (confirm == 'n' || confirm == 'N')
            {
                printf("\n\t%sBooking for seat %s canceled.%s\n", LIGHT_RED, seatLabel, RESET);
                Sleep(1500);
                continue;
            }
            else if (confirm == 'y' || confirm == 'Y')
            {
                chairSeats[currentScheduleId][seatIndex] = 1;
                strcpy(newOrder.seatsBooked[bookedCount], seatLabel);

                newOrder.numSeatsBooked++;

                bookedCount++;
                printf("\n\t%sSeat %s booked successfully.%s\n", LIGHT_GREEN, seatLabel, RESET);
                sleep(1);

            }
            else
            {
                printf("\n\t%sInvalid input. Please press 'Y' to confirm or 'n' to cancel.%s\n", LIGHT_RED, RESET);

                Sleep(1500);
                continue;
            }
        }

        totalChairPrice = calculateTotalPrice(newOrder.numSeatsBooked);
        newOrder.chairPrice = totalChairPrice;

        saveChairSeatsToFile(currentScheduleId);
        saveChairBookingToFile(&newOrder);

        system("cls");
        printf("\n\t%s===================================\n", LIGHT_YELLOW);
        printf("\t       Chair Booking Invoice\n");
        printf("\t===================================\n%s", RESET);
        printf("\t%sUser Details%s\n", LIGHT_CYAN, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sOrder ID: %d\n%s", LIGHT_CYAN, newOrder.orderId, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sName: %s\n%s", LIGHT_CYAN, newOrder.userName, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sPhone: %s\n%s", LIGHT_CYAN, newOrder.userPhone, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sDate: %02d-%02d-%04d\n%s", LIGHT_CYAN, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sTime: %s\n%s", LIGHT_CYAN, Time, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sBooking Details%s\n", LIGHT_YELLOW, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sSchedule ID: %d\n%s", LIGHT_CYAN, currentScheduleId, RESET);
        printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sSeats Booked: ", LIGHT_CYAN);

        for (int i = 0; i < newOrder.numSeatsBooked; i++)
        {
            printf("[%s]  ", newOrder.seatsBooked[i]);
        }
        printf("%s", RESET);
        printf("\n\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
        printf("\t%sTotal Price: %.2f\n%s", LIGHT_GREEN, totalChairPrice, RESET);
        printf("\t%s===================================\n%s", LIGHT_YELLOW, RESET);
        printf("\t%s   Thank you for booking with us!\n%s", LIGHT_GREEN, RESET);
        printf("\t%s===================================\n%s", LIGHT_YELLOW, RESET);
        Sleep(1500);
        printf("\n\t%sDo you want to book more seats? (Y/n): ", LIGHT_CYAN, RESET);
        confirm = getchar();
        clearInputBuffer();
        if (confirm == 'n' || confirm == 'N')
        {
            break;
        }
        else if (confirm == 'y' || confirm == 'Y')
        {
            continue;
        }
        else
        {
            printf("\n\t%sInvalid input. Please press 'Y' to confirm or 'n' to cancel.%s\n", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }
    }
}

void showCabinInvoice(CabinOrder *order)
{
    system("cls");
    printf("\n\t%s===================================\n", LIGHT_YELLOW);
    printf("\t    Invoice for %s Cabin Booking\n", order->cabinType);
    printf("\t===================================\n%s", RESET);
    printf("\t%sSchedule ID: %d%s\n", LIGHT_CYAN, order->scheduleId, RESET);
    printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
    printf("\t%sName: %s%s\n", LIGHT_CYAN, order->userName, RESET);
    printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
    printf("\t%sPhone: %s%s\n", LIGHT_CYAN, order->userPhone, RESET);
    printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
    printf("\t%sCabins Booked: ", LIGHT_CYAN);
    for (int i = 0; i < order->numCabinsBooked; i++)
    {
        printf("%s-%d ", order->cabinType, order->cabinNumbers[i]);
    }
    printf("\n\t%s%s-----------------------------------\n%s", RESET, LIGHT_YELLOW, RESET);
    printf("\t%sBooking Time: %s%s\n", LIGHT_CYAN, order->orderTime, RESET);
    printf("\t%s-----------------------------------\n%s", LIGHT_YELLOW, RESET);
    printf("\t%sTotal Price: %.2f%s\n", LIGHT_CYAN, order->cabinPrice * order->numCabinsBooked, RESET);
    printf("\t%s=====================================\n%s", LIGHT_YELLOW, RESET);
    printf("\t%s   Thank you for booking with us!\n%s", LIGHT_GREEN, RESET);
    printf("\t%s=====================================\n%s", LIGHT_YELLOW, RESET);
}

void saveChairSeatsToFile(int currentScheduledId)
{
    if (currentScheduleId == -1)
    {
        printf("\n\t%sNo schedule selected. Cannot save seats. %s\n", LIGHT_RED, RESET);
        return;
    }
    char filename[50];
    snprintf(filename, sizeof(filename), "schedule_%d.txt", currentScheduleId);

    FILE *file;
    file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);

        Sleep(1500);
        return;
    }
    for (int i = 0; i < ROWS * COLS; i++)
    {
        fprintf(file, "%d\n", chairSeats[currentScheduleId][i]);
    }

    fclose(file);
}

void loadChairSeatsFromFile(int currentScheduleId)
{
    if (currentScheduleId == -1)
    {
        printf("\n\t%sNo schedule selected. Please select a schedule first. %s\n", LIGHT_RED, RESET);
        return;
    }

    char filename[50];
    snprintf(filename, sizeof(filename), "schedule_%d.txt", currentScheduleId);

    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        for (int i = 0; i < ROWS * COLS; i++)
        {
            chairSeats[currentScheduleId][i] = 0;
        }
        return;
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fscanf(file, "%d", &chairSeats[currentScheduleId][i * COLS + j]);
        }
    }

    fclose(file);
}

void saveChairBookingToFile(ChairOrder *order)
{
    FILE *file = fopen("chair_orders.txt", "a");
    if (file == NULL)
    {
        printf("\n\t%sError opening file!%s\n", LIGHT_RED, RESET);
        return;
    }

    fprintf(file, "Order ID: %d\n", order->orderId);
    fprintf(file, "Name: %s\n", order->userName);
    fprintf(file, "Phone: %s\n", order->userPhone);
    fprintf(file, "Seats Booked: ");
    for (int i = 0; i < order->numSeatsBooked; i++)
    {
        fprintf(file, "%s ", order->seatsBooked[i]);
    }
    fprintf(file, "\nOrder Time: %s\n", order->orderTime);
    fprintf(file, "Chair Price: %.2f\n", order->chairPrice);
    fprintf(file, "---------------------------\n");
    fprintf(file, "\n\n");

    fclose(file);
}

void saveCabinBookingToFile(CabinOrder *order)
{
    FILE *file = fopen("cabin_orders.txt", "a");

    if (file == NULL)
    {
        printf("\n\t%sError: Unable to open cabin orders file.%s\n", LIGHT_RED, RESET);
        return;
    }

    fprintf(file, "Schedule ID: %d\n", order->scheduleId);
    fprintf(file, "Order ID: %d\n", order->orderId);
    fprintf(file, "Name: %s\n", order->userName);
    fprintf(file, "Phone: %s\n", order->userPhone);
    fprintf(file, "Cabin Type: %s\n", order->cabinType);

    fprintf(file, "Cabin Numbers: ");
    for (int i = 0; i < order->numCabinsBooked; i++)
    {
        fprintf(file, "%d ", order->cabinNumbers[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "Cabin Price: %.2f\n", order->cabinPrice);
    fprintf(file, "Order Time: %s\n", order->orderTime);
    fprintf(file, "---------------------------\n\n");

    fclose(file);
}

void loadOrdersFromFiles()
{
    FILE *file1 = fopen("chair_orders.txt", "r");
    FILE *file2 = fopen("cabin_orders.txt", "r");

    char line[256];

    if (file1 == NULL || file2 == NULL)
    {
        printf("\n\t%sError opening one or both files!%s\n", LIGHT_RED, RESET);
        return;
    }

    printf("\n\t%sOrder History:%s\n", LIGHT_YELLOW, RESET);

    printf("\n\t%sChair Orders:%s\n", LIGHT_CYAN, RESET);
    while (fgets(line, sizeof(line), file1))
    {
        printf("%s", line);
    }

    printf("\n\t%sCabin Orders:%s\n", LIGHT_CYAN, RESET);
    while (fgets(line, sizeof(line), file2))
    {
        if (strstr(line, "Order ID:"))
        {
            printf("\n%s", line);
        }
        else if (strstr(line, "Cabin Numbers:"))
        {
            printf("%s", line);
        }
        else
        {
            printf("%s", line);
        }
    }
    fclose(file1);
    fclose(file2);
}


int generateUniqueOrderID()
{
    FILE *file = fopen("last_order_id.txt", "r+");
    int orderId = 1000;

    if (file == NULL)
    {
        file = fopen("last_order_id.txt", "w");
        if (file == NULL)
        {
            printf("Error: Unable to create order ID file.\n");
            return -1;
        }
        fprintf(file, "%d", orderId);
        fclose(file);
        return orderId;
    }

    fscanf(file, "%d", &orderId);
    fclose(file);

    file = fopen("last_order_id.txt", "w");
    if (file == NULL)
    {
        printf("Error: Unable to update order ID file.\n");
        return -1;
    }
    fprintf(file, "%d", orderId + 1);
    fclose(file);

    return orderId;
}

float calculateTotalPrice(int numSeats)
{
    return numSeats * SEAT_PRICE;
}

void clearAllSeats(int currentScheduleId)
{
    if (currentScheduleId == -1)
    {
        printf("\n\t%sNo schedule selected. Please select a schedule first. %s\n", LIGHT_RED, RESET);
        return;
    }

    for (int i = 0; i < ROWS * COLS; i++)
    {
        chairSeats[currentScheduleId][i] = 0;
    }

    saveChairSeatsToFile(currentScheduleId);

    printf("\n\t%sAll seats have been cleared and set to available.%s\n", LIGHT_GREEN, RESET);
    Sleep(1500);

}

void bookCabin(int cabinType)
{
    int totalCabins = (cabinType == 1) ? NUM_CABINS_AC : (cabinType == 2) ? NUM_CABINS_NON_AC
                      : NUM_CABINS_VIP;
    int *cabins = (cabinType == 1) ? acCabins : (cabinType == 2) ? nonAcCabins
                  : vipCabins;
    float cabinPrice = (cabinType == 1) ? CABIN_PRICE_AC : (cabinType == 2) ? CABIN_PRICE_NON_AC
                       : CABIN_PRICE_VIP;

    CabinOrder newOrder;
    newOrder.numCabinsBooked = 0;
    strcpy(newOrder.cabinType, (cabinType == 1) ? "AC" : (cabinType == 2) ? "Non-AC"
           : "VIP");

    int scheduleId;
    Schedule schedule;
    FILE *file = fopen("schedules.txt", "r");
    char line[100];
    int found = 0;

    if (file == NULL)
    {
        printf("\n\t%sError opening schedules file!%s\n", LIGHT_RED, RESET);
        Sleep(1500);
        return;
    }

    printf("\n\t%s==================================\n", LIGHT_YELLOW);
    printf("\t       Select Schedule\n");
    printf("\t==================================\n");
    printf("\n  %-5s    %-14s %-8s %-10s %-10s\n", "Id", "Route", "Day", "Departure", "Arrival");
    printf("--------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%30[^,],%10[^,],%8[^,],%8[^\n]\n", &schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime) == 5)
        {
            printf("  %-3d %-17s %-10s %-10s %-10s\n", schedule.id, schedule.route, schedule.day, schedule.departureTime, schedule.arrivalTime);
        }
    }
    fclose(file);

    printf("\n\t%s%sEnter Schedule ID to book cabins: %s", RESET, LIGHT_CYAN, RESET);
    if (scanf("%d", &scheduleId) != 1)
    {
        clearInputBuffer();
        printf("\n\t%sInvalid Schedule ID. Please try again.%s\n", LIGHT_RED, RESET);
        return;
    }

    newOrder.scheduleId = scheduleId;

    loadCabinStatusesForSchedule(scheduleId);

    while (1)
    {
        viewAvailableCabinsByType(cabinType);
        printf("\t%sThe price for %s Cabin: %.2f%s\n", LIGHT_GREEN, newOrder.cabinType, cabinPrice, RESET);

        int numCabinsToBook;
        printf("\n\t%sHow many %s cabins do you want to book? %s", LIGHT_CYAN, newOrder.cabinType, RESET);
        if (scanf("%d", &numCabinsToBook) != 1 || numCabinsToBook < 1)
        {
            clearInputBuffer();
            printf("\n\t%sInvalid input. Please enter a positive number.%s\n", LIGHT_RED, RESET);
            continue;
        }

        clearInputBuffer();

        printf("\n\t%sEnter the %d cabin numbers (space-separated): %s", LIGHT_CYAN, numCabinsToBook, RESET);
        for (int i = 0; i < numCabinsToBook; i++)
        {
            int cabinNumber;
            if (scanf("%d", &cabinNumber) != 1 || cabinNumber < 1 || cabinNumber > totalCabins || cabins[cabinNumber - 1] == 1)
            {
                clearInputBuffer();
                printf("\n\t%sInvalid or already booked cabin: %d. Please try again.%s\n", LIGHT_RED, cabinNumber, RESET);
                i--;
                continue;
            }
            newOrder.cabinNumbers[newOrder.numCabinsBooked++] = cabinNumber;
            cabins[cabinNumber - 1] = 1;
            printf("\n\t%sCabin %d booked successfully!%s\n", LIGHT_GREEN, cabinNumber, RESET);
        }

        newOrder.orderId = generateUniqueOrderID();
        newOrder.cabinPrice = cabinPrice;

        clearInputBuffer();

        printf("\n\tEnter your name: ");
        fgets(newOrder.userName, sizeof(newOrder.userName), stdin);
        newOrder.userName[strcspn(newOrder.userName, "\n")] = '\0';

        printf("\n\tEnter your phone number: ");
        fgets(newOrder.userPhone, sizeof(newOrder.userPhone), stdin);
        newOrder.userPhone[strcspn(newOrder.userPhone, "\n")] = '\0';

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        strftime(newOrder.orderTime, sizeof(newOrder.orderTime), "%I:%M %p %d-%m-%Y", &tm);

        saveCabinBookingToFile(&newOrder);

        saveCabinStatusesForSchedule(scheduleId);

        showCabinInvoice(&newOrder);

        printf("\n\tDo you want to book another %s cabin? (Y/n): ", newOrder.cabinType);
        char confirm = getchar();
        clearInputBuffer();
        if (confirm == 'n' || confirm == 'N')
        {
            printf("\n\t%sThank you for your booking! Exiting.%s\n", LIGHT_GREEN, RESET);
            return;
        }
    }
}

void viewAvailableCabinsByType(int cabinType)
{
    int cabinsPerRow = 5;
    int count = 0;
    if (cabinType == 1)
    {
        printf("\n\t%sAvailable AC Cabins: %s\n\n\t", LIGHT_CYAN, RESET);
        for (int i = 0; i < NUM_CABINS_AC; i++)
        {
            if (acCabins[i] == 0)
            {
                printf("%s[AC-%d] %s", LIGHT_YELLOW, i + 1, RESET);
            }
            else
            {
                printf("%s[AC-%d X] %s", LIGHT_RED, i + 1, RESET);
            }

            count++;
            if (count == cabinsPerRow)
            {
                printf("\n\t");
                count = 0;
            }
        }
        printf("\n");
    }
    else if (cabinType == 2)
    {
        printf("\n\t%sAvailable Non-AC Cabins: %s\n\n\t", LIGHT_CYAN, RESET);
        for (int i = 0; i < NUM_CABINS_NON_AC; i++)
        {
            if (nonAcCabins[i] == 0)
            {
                printf("%s[Non-AC-%d] %s",LIGHT_YELLOW, i + 1, RESET);
            }
            else
            {
                printf("%s[Non-AC-%d X] %s", LIGHT_RED, i + 1, RESET);
            }

            count++;
            if (count == cabinsPerRow)
            {
                printf("\n\t");
                count = 0;
            }
        }
        printf("\n");
    }
    else if (cabinType == 3)
    {
        printf("\n\t%sAvailable VIP Cabins: %s\n\n\t", LIGHT_CYAN, RESET);
        for (int i = 0; i < NUM_CABINS_VIP; i++)
        {
            if (vipCabins[i] == 0)
            {
                printf("%s[VIP-%d] %s", LIGHT_YELLOW, i + 1, RESET);
            }
            else
            {
                printf("%s[VIP-%d X] %s", LIGHT_RED, i + 1, RESET);
            }

            count++;
            if (count == cabinsPerRow)
            {
                printf("\n\t");
                count = 0;
            }
        }
        printf("\n");
    }
}

void saveCabinStatusesToFile()
{
    FILE *file = fopen("cabin_status.txt", "w");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to open cabin status file.%s\n", LIGHT_RED, RESET);
        return;
    }

    fprintf(file, "AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_AC; i++)
    {
        fprintf(file, "%d ", acCabins[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "Non-AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_NON_AC; i++)
    {
        fprintf(file, "%d ", nonAcCabins[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "VIP Cabins: ");
    for (int i = 0; i < NUM_CABINS_VIP; i++)
    {
        fprintf(file, "%d ", vipCabins[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

void loadCabinStatusesFromFile()
{
    FILE *file = fopen("cabin_status.txt", "r");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to open cabin status file.%s\n", LIGHT_RED, RESET);
        return;
    }

    fscanf(file, "AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_AC; i++)
    {
        fscanf(file, "%d", &acCabins[i]);
    }

    fscanf(file, "\nNon-AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_NON_AC; i++)
    {
        fscanf(file, "%d", &nonAcCabins[i]);
    }

    fscanf(file, "\nVIP Cabins: ");
    for (int i = 0; i < NUM_CABINS_VIP; i++)
    {
        fscanf(file, "%d", &vipCabins[i]);
    }

    fclose(file);
}

void resetCabinStatusesByType()
{
    int option, scheduleId;

    printf("\n\t%sEnter Schedule ID to reset cabins: %s", LIGHT_CYAN, RESET);
    if (scanf("%d", &scheduleId) != 1)
    {
        clearInputBuffer();
        printf("\n\t%sInvalid Schedule ID! Please try again.%s\n", LIGHT_RED, RESET);
        return;
    }

    clearInputBuffer();

    printf("\n\t%sSelect Cabin Type to Reset:%s\n", LIGHT_YELLOW, RESET);
    printf("\t1. Reset AC Cabins\n");
    printf("\t2. Reset Non-AC Cabins\n");
    printf("\t3. Reset VIP Cabins\n");
    printf("\t4. Reset All Cabins\n");
    printf("\n\t%sEnter your choice: %s", LIGHT_CYAN, RESET);

    if (scanf("%d", &option) != 1)
    {
        clearInputBuffer();
        printf("\n\t%sInvalid input! Please enter a valid option.%s\n", LIGHT_RED, RESET);
        return;
    }

    clearInputBuffer();

    loadCabinStatusesForSchedule(scheduleId);

    switch (option)
    {
    case 1:
        for (int i = 0; i < NUM_CABINS_AC; i++)
        {
            acCabins[i] = 0;
        }
        printf("\n\t%sAC cabins for schedule %d have been reset successfully.%s\n", LIGHT_GREEN, scheduleId, RESET);
        break;
    case 2:
        for (int i = 0; i < NUM_CABINS_NON_AC; i++)
        {
            nonAcCabins[i] = 0;
        }
        printf("\n\t%sNon-AC cabins for schedule %d have been reset successfully.%s\n", LIGHT_GREEN, scheduleId, RESET);
        break;
    case 3:
        for (int i = 0; i < NUM_CABINS_VIP; i++)
        {
            vipCabins[i] = 0;
        }
        printf("\n\t%sVIP cabins for schedule %d have been reset successfully.%s\n", LIGHT_GREEN, scheduleId, RESET);
        break;
    case 4:
        for (int i = 0; i < NUM_CABINS_AC; i++)
        {
            acCabins[i] = 0;
        }
        for (int i = 0; i < NUM_CABINS_NON_AC; i++)
        {
            nonAcCabins[i] = 0;
        }
        for (int i = 0; i < NUM_CABINS_VIP; i++)
        {
            vipCabins[i] = 0;
        }
        printf("\n\t%sAll cabins for schedule %d have been reset successfully.%s\n", LIGHT_GREEN, scheduleId, RESET);
        break;
    default:
        printf("\n\t%sInvalid choice. No cabins were reset.%s\n", LIGHT_RED, RESET);
        return;
    }

    saveCabinStatusesForSchedule(scheduleId);
}

void loadCabinStatusesForSchedule(int scheduleId)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "cabin_status_%d.txt", scheduleId);

    initializeCabinStatusFile(scheduleId);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to load cabin statuses for schedule %d.%s\n", LIGHT_RED, scheduleId, RESET);
        return;
    }

    fscanf(file, "AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_AC; i++)
    {
        fscanf(file, "%d", &acCabins[i]);
    }

    fscanf(file, "\nNon-AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_NON_AC; i++)
    {
        fscanf(file, "%d", &nonAcCabins[i]);
    }

    fscanf(file, "\nVIP Cabins: ");
    for (int i = 0; i < NUM_CABINS_VIP; i++)
    {
        fscanf(file, "%d", &vipCabins[i]);
    }

    fclose(file);
}

void saveCabinStatusesForSchedule(int scheduleId)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "cabin_status_%d.txt", scheduleId);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to save cabin statuses for schedule %d.%s\n", LIGHT_RED, scheduleId, RESET);
        return;
    }

    fprintf(file, "AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_AC; i++)
    {
        fprintf(file, "%d ", acCabins[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "Non-AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_NON_AC; i++)
    {
        fprintf(file, "%d ", nonAcCabins[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "VIP Cabins: ");
    for (int i = 0; i < NUM_CABINS_VIP; i++)
    {
        fprintf(file, "%d ", vipCabins[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

void initializeCabinStatusesForSchedule(int scheduleId)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "cabin_status_%d.txt", scheduleId);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("\n\t%sError: Unable to initialize cabin statuses for schedule %d.%s\n", LIGHT_RED, scheduleId, RESET);
        return;
    }

    fprintf(file, "AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_AC; i++)
    {
        fprintf(file, "0 ");
    }
    fprintf(file, "\n");

    fprintf(file, "Non-AC Cabins: ");
    for (int i = 0; i < NUM_CABINS_NON_AC; i++)
    {
        fprintf(file, "0 ");
    }
    fprintf(file, "\n");

    fprintf(file, "VIP Cabins: ");
    for (int i = 0; i < NUM_CABINS_VIP; i++)
    {
        fprintf(file, "0 ");
    }
    fprintf(file, "\n");

    fclose(file);
}

void initializeCabinStatusFile(int scheduleId)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "cabin_status_%d.txt", scheduleId);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("\n\tError: Unable to create cabin status file for schedule %d.\n", scheduleId);
            return;
        }

        fprintf(file, "AC Cabins: ");
        for (int i = 0; i < NUM_CABINS_AC; i++)
        {
            fprintf(file, "0 ");
        }
        fprintf(file, "\n");

        fprintf(file, "Non-AC Cabins: ");
        for (int i = 0; i < NUM_CABINS_NON_AC; i++)
        {
            fprintf(file, "0 ");
        }
        fprintf(file, "\n");

        fprintf(file, "VIP Cabins: ");
        for (int i = 0; i < NUM_CABINS_VIP; i++)
        {
            fprintf(file, "0 ");
        }
        fprintf(file, "\n");

        fclose(file);
    }
    else
    {
        fclose(file);
    }
}

int main()
{
    createSecretFile();
    loadCabinStatusesFromFile();
    int option;
    while (1)
    {
        system("cls");
        displayHeader();
        printOption();
        if (scanf("%d", &option) != 1)
        {
            clearInputBuffer();
            printf("%s\n\tInvalid Option. Please try again.\n%s", LIGHT_RED, RESET);
            Sleep(1500);
            continue;
        }

        clearInputBuffer();
        switch (option)
        {
        case 1:
            passengerPortal();
            break;
        case 2:
            if (adminLogin())
            {
                adminDashboard();
            }
            else
            {
                printf("%s\n\tAccess Denied. Please try again.%s", LIGHT_RED, RESET);
                Sleep(1500);
            }
            break;
        case 3:
            system("cls");
            printf("%s\n\n\tThank you for choosing our service.\n\n%s", LIGHT_GREEN, RESET);
            return 0;
        default:
            printf("%s\n\tInvalid Option %d. Please try again.\n%s", LIGHT_RED, option, RESET);
            Sleep(1500);
            break;
        }
    }
    return 0;
}
