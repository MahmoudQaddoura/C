#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <pthread.h>

// Define entity types
#define ENTITY_BRANCH 1
#define ENTITY_ROOM 2
#define ENTITY_CUSTOMER 3
#define ENTITY_CLEANING_CREW 4
#define ENTITY_BOOKING 5
#define ENTITY_EMPLOYEE 6
#define ENTITY_PHONE 7

// Define operation types
#define OPERATION_CREATE 1
#define OPERATION_READ 2
#define OPERATION_UPDATE 3
#define OPERATION_DELETE 4
#define OPERATION_EXIT 5

typedef struct {
    int branchID;
    char name[50];
    char phone[20];
    char address[100];
    int numberOfRooms;
}
Branch;

typedef struct {
    int roomID;
    int number;
    int floor;
    int cleaningCrewID;
    int branchID;
}
Room;

typedef struct {
    int customerID;
    char SSN[11];
    char name[50];
    char dateOfBirth[10];
    char email[50];
    int bookingID;
    char phone[20];
}
Customer;

typedef struct {
    int cleaningCrewID;
    char employees[50];
    char status[20];
    int roomID;
}
CleaningCrew;

typedef struct {
    int bookingID;
    char date[10];
    char time[10];
    char customerSSN[11];
    int registrarID;
    int roomNumber;
}
Booking;

typedef struct {
    int employeeID;
    char name[50];
    char password[20];
    char email[50];
    double salary;
    int branchID;
    char position[20];
}
Employee;

typedef struct {
    int phoneID;
    char number[20];
    char customerSSN[11];
}
Phone;

// Mutex for thread-safe CRUD operations on shared data
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logMutex;

//non_CRUD functions
void logData(const char * logMessage);
void trimNewline(char * string);
void displayEntityMenu();
void displayCRUDMenu();
void handleOperation(int entityChoice, int operationChoice);
void initializeApplication();
void cleanupApplication();
void flushInput();

// Branch Operations
void createBranch(Branch * branch);
void readBranch(int branchID);
void updateBranch(int branchID, Branch * updatedBranch);
void deleteBranch(int branchID);

// Room Operations
void createRoom(Room * room);
void readRoom(int roomID);
void updateRoom(int roomID, Room * updatedRoom);
void deleteRoom(int roomID);

// Customer Operations
void createCustomer(Customer * customer);
void readCustomer(int customerID);
void updateCustomer(int customerID, Customer * updatedCustomer);
void deleteCustomer(int customerID);

// Cleaning Crew Operations
void createCleaningCrew(CleaningCrew * crew);
void readCleaningCrew(int cleaningCrewID);
void updateCleaningCrew(int cleaningCrewID, CleaningCrew * updatedCrew);
void deleteCleaningCrew(int cleaningCrewID);

// Booking Operations
void createBooking(Booking * booking);
void readBooking(int bookingID);
void updateBooking(int bookingID, Booking * updatedBooking);
void deleteBooking(int bookingID);

// Employee Operations
void createEmployee(Employee * employee);
void readEmployee(int employeeID);
void updateEmployee(int employeeID, Employee * updatedEmployee);
void deleteEmployee(int employeeID);

// Phone Operations
void createPhone(Phone * phone);
void readPhone(int phoneID);
void updatePhone(int phoneID, Phone * updatedPhone);
void deletePhone(int phoneID);

//type definition and node head initialization for entities
typedef struct BranchNode {
    Branch data;
    struct BranchNode * next;
}
BranchNode;
BranchNode * branchHead = NULL;

typedef struct RoomNode {
    Room data;
    struct RoomNode * next;
}
RoomNode;
RoomNode * roomHead = NULL;

typedef struct CustomerNode {
    Customer data;
    struct CustomerNode * next;
}
CustomerNode;
CustomerNode * customerHead = NULL;

typedef struct CleaningCrewNode {
    CleaningCrew data;
    struct CleaningCrewNode * next;
}
CleaningCrewNode;
CleaningCrewNode * cleaningCrewHead = NULL;

typedef struct BookingNode {
    Booking data;
    struct BookingNode * next;
}
BookingNode;
BookingNode * bookingHead = NULL;

typedef struct EmployeeNode {
    Employee data;
    struct EmployeeNode * next;
}
EmployeeNode;
EmployeeNode * employeeHead = NULL;

typedef struct PhoneNode {
    Phone data;
    struct PhoneNode * next;
}
PhoneNode;
PhoneNode * phoneHead = NULL;

//logging function
const char * logFileName = "./application_log.txt";

void logData(const char * logMessage) {
    pthread_mutex_lock( & logMutex);
    FILE * logFile = fopen(logFileName, "a"); // Open in append mode
    if (logFile != NULL) {
        fprintf(logFile, "%s\n", logMessage);
        fclose(logFile); // Close after writing to ensure data is saved
    } else {
        printf("Failed to open log file.\n");
    }
    pthread_mutex_unlock( & logMutex);
}

//Branch
void createBranch(Branch * branch) {
    pthread_mutex_lock( & dataMutex);

    BranchNode * newNode = (BranchNode * ) malloc(sizeof(BranchNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Branch.");
        return;
    }
    newNode -> data = * branch;
    newNode -> next = branchHead;
    branchHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Branch created: ID=%d, Name=%s", branch -> branchID, branch -> name);
    logData(logMessage);
}

void readBranch(int branchID) {
    pthread_mutex_lock( & dataMutex);

    BranchNode * current = branchHead;
    while (current != NULL) {
        if (current -> data.branchID == branchID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Branch read: ID=%d, Name=%s", current -> data.branchID, current -> data.name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Branch not found for read operation.");
}

void updateBranch(int branchID, Branch * updatedBranch) {
    pthread_mutex_lock( & dataMutex);

    BranchNode * current = branchHead;
    while (current != NULL) {
        if (current -> data.branchID == branchID) {
            current -> data = * updatedBranch;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Branch updated: ID=%d, Name=%s", updatedBranch -> branchID, updatedBranch -> name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Branch not found for update operation.");
}

void deleteBranch(int branchID) {
    pthread_mutex_lock( & dataMutex);

    BranchNode ** current = & branchHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.branchID == branchID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Branch deleted: ID=%d", branchID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Branch not found for delete operation.");
}

//Room
void createRoom(Room * room) {
    pthread_mutex_lock( & dataMutex);

    RoomNode * newNode = (RoomNode * ) malloc(sizeof(RoomNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Room.");
        return;
    }
    newNode -> data = * room;
    newNode -> next = roomHead;
    roomHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Room created: ID=%d, Number=%d, Floor=%d, Branch ID=%d", room -> roomID, room -> number, room -> floor, room -> branchID);
    logData(logMessage);
}

void readRoom(int roomID) {
    pthread_mutex_lock( & dataMutex);

    RoomNode * current = roomHead;
    while (current != NULL) {
        if (current -> data.roomID == roomID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Room read: ID=%d, Number=%d, Floor=%d, Branch ID=%d", current -> data.roomID, current -> data.number, current -> data.floor, current -> data.branchID);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Room not found for read operation.");
}

void updateRoom(int roomID, Room * updatedRoom) {
    pthread_mutex_lock( & dataMutex);

    RoomNode * current = roomHead;
    while (current != NULL) {
        if (current -> data.roomID == roomID) {
            current -> data = * updatedRoom;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Room updated: ID=%d, Number=%d, Floor=%d, Branch ID=%d", updatedRoom -> roomID, updatedRoom -> number, updatedRoom -> floor, updatedRoom -> branchID);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Room not found for update operation.");
}

void deleteRoom(int roomID) {
    pthread_mutex_lock( & dataMutex);

    RoomNode ** current = & roomHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.roomID == roomID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Room deleted: ID=%d", roomID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Room not found for delete operation.");
}

//Customer
void createCustomer(Customer * customer) {
    pthread_mutex_lock( & dataMutex);

    CustomerNode * newNode = (CustomerNode * ) malloc(sizeof(CustomerNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Customer.");
        return;
    }
    newNode -> data = * customer;
    newNode -> next = customerHead;
    customerHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Customer created: ID=%d, Name=%s", customer -> customerID, customer -> name);
    logData(logMessage);
}

void readCustomer(int customerID) {
    pthread_mutex_lock( & dataMutex);

    CustomerNode * current = customerHead;
    while (current != NULL) {
        if (current -> data.customerID == customerID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Customer read: ID=%d, Name=%s", current -> data.customerID, current -> data.name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Customer not found for read operation.");
}

void updateCustomer(int customerID, Customer * updatedCustomer) {
    pthread_mutex_lock( & dataMutex);

    CustomerNode * current = customerHead;
    while (current != NULL) {
        if (current -> data.customerID == customerID) {
            current -> data = * updatedCustomer;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Customer updated: ID=%d, Name=%s", updatedCustomer -> customerID, updatedCustomer -> name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Customer not found for update operation.");
}

void deleteCustomer(int customerID) {
    pthread_mutex_lock( & dataMutex);

    CustomerNode ** current = & customerHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.customerID == customerID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Customer deleted: ID=%d", customerID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Customer not found for delete operation.");
}

//Cleaning crew
void createCleaningCrew(CleaningCrew * crew) {
    pthread_mutex_lock( & dataMutex);

    CleaningCrewNode * newNode = (CleaningCrewNode * ) malloc(sizeof(CleaningCrewNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Cleaning Crew.");
        return;
    }
    newNode -> data = * crew;
    newNode -> next = cleaningCrewHead;
    cleaningCrewHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Cleaning Crew created: ID=%d, Employees=%s", crew -> cleaningCrewID, crew -> employees);
    logData(logMessage);
}

void readCleaningCrew(int cleaningCrewID) {
    pthread_mutex_lock( & dataMutex);

    CleaningCrewNode * current = cleaningCrewHead;
    while (current != NULL) {
        if (current -> data.cleaningCrewID == cleaningCrewID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Cleaning Crew read: ID=%d, Employees=%s", current -> data.cleaningCrewID, current -> data.employees);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Cleaning Crew not found for read operation.");
}

void updateCleaningCrew(int cleaningCrewID, CleaningCrew * updatedCrew) {
    pthread_mutex_lock( & dataMutex);

    CleaningCrewNode * current = cleaningCrewHead;
    while (current != NULL) {
        if (current -> data.cleaningCrewID == cleaningCrewID) {
            current -> data = * updatedCrew;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Cleaning Crew updated: ID=%d, Employees=%s", updatedCrew -> cleaningCrewID, updatedCrew -> employees);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Cleaning Crew not found for update operation.");
}

void deleteCleaningCrew(int cleaningCrewID) {
    pthread_mutex_lock( & dataMutex);

    CleaningCrewNode ** current = & cleaningCrewHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.cleaningCrewID == cleaningCrewID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Cleaning Crew deleted: ID=%d", cleaningCrewID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Cleaning Crew not found for delete operation.");
}

//Booking
void createBooking(Booking * booking) {
    pthread_mutex_lock( & dataMutex);

    BookingNode * newNode = (BookingNode * ) malloc(sizeof(BookingNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Booking.");
        return;
    }
    newNode -> data = * booking;
    newNode -> next = bookingHead;
    bookingHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Booking created: ID=%d, Date=%s", booking -> bookingID, booking -> date);
    logData(logMessage);
}

void readBooking(int bookingID) {
    pthread_mutex_lock( & dataMutex);

    BookingNode * current = bookingHead;
    while (current != NULL) {
        if (current -> data.bookingID == bookingID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Booking read: ID=%d, Date=%s", current -> data.bookingID, current -> data.date);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Booking not found for read operation.");
}

void updateBooking(int bookingID, Booking * updatedBooking) {
    pthread_mutex_lock( & dataMutex);

    BookingNode * current = bookingHead;
    while (current != NULL) {
        if (current -> data.bookingID == bookingID) {
            current -> data = * updatedBooking;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Booking updated: ID=%d, Date=%s", updatedBooking -> bookingID, updatedBooking -> date);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Booking not found for update operation.");
}

void deleteBooking(int bookingID) {
    pthread_mutex_lock( & dataMutex);

    BookingNode ** current = & bookingHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.bookingID == bookingID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Booking deleted: ID=%d", bookingID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Booking not found for delete operation.");
}

//Employee
void createEmployee(Employee * employee) {
    pthread_mutex_lock( & dataMutex);

    EmployeeNode * newNode = (EmployeeNode * ) malloc(sizeof(EmployeeNode));
    if (newNode == NULL) {
        pthread_mutex_unlock( & dataMutex);
        logData("Error: Failed to allocate memory for new Employee.");
        return;
    }
    newNode -> data = * employee;
    newNode -> next = employeeHead;
    employeeHead = newNode;

    pthread_mutex_unlock( & dataMutex);

    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Employee created: ID=%d, Name=%s", employee -> employeeID, employee -> name);
    logData(logMessage);
}

void readEmployee(int employeeID) {
    pthread_mutex_lock( & dataMutex);

    EmployeeNode * current = employeeHead;
    while (current != NULL) {
        if (current -> data.employeeID == employeeID) {
            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Employee read: ID=%d, Name=%s", current -> data.employeeID, current -> data.name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Employee not found for read operation.");
}

void updateEmployee(int employeeID, Employee * updatedEmployee) {
    pthread_mutex_lock( & dataMutex);

    EmployeeNode * current = employeeHead;
    while (current != NULL) {
        if (current -> data.employeeID == employeeID) {
            current -> data = * updatedEmployee;

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Employee updated: ID=%d, Name=%s", updatedEmployee -> employeeID, updatedEmployee -> name);
            logData(logMessage);
            return;
        }
        current = current -> next;
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Employee not found for update operation.");
}

void deleteEmployee(int employeeID) {
    pthread_mutex_lock( & dataMutex);

    EmployeeNode ** current = & employeeHead, * temp;
    while ( * current != NULL) {
        if (( * current) -> data.employeeID == employeeID) {
            temp = * current;
            * current = ( * current) -> next;
            free(temp);

            pthread_mutex_unlock( & dataMutex);

            char logMessage[256];
            snprintf(logMessage, sizeof(logMessage), "Employee deleted: ID=%d", employeeID);
            logData(logMessage);
            return;
        }
        current = & (( * current) -> next);
    }

    pthread_mutex_unlock( & dataMutex);
    logData("Employee not found for delete operation.");
}

//Phone
void createPhone(Phone * phone) {
    pthread_mutex_lock( & dataMutex);

    PhoneNode * newNode = (PhoneNode * ) malloc(sizeof(PhoneNode));
    if (newNode == NULL) {
        printf("Error creating a new phone.\n");
    } else {
        newNode -> data = * phone;
        newNode -> next = phoneHead;
        phoneHead = newNode;
    }

    pthread_mutex_lock( & dataMutex);
}

void readPhone(int phoneID) {
    pthread_mutex_lock( & dataMutex);

    PhoneNode * current = phoneHead;
    while (current != NULL) {
        if (current -> data.phoneID == phoneID) {
            printf("ID: %d, Number: %s, Customer SSN: %s\n",
                current -> data.phoneID, current -> data.number, current -> data.customerSSN);
            pthread_mutex_lock( & dataMutex);
            return;
        }
        current = current -> next;
    }
    printf("Phone not found.\n");

    pthread_mutex_lock( & dataMutex);
}

void updatePhone(int phoneID, Phone * updatedPhone) {
    pthread_mutex_lock( & dataMutex);

    PhoneNode * current = phoneHead;
    while (current != NULL) {
        if (current -> data.phoneID == phoneID) {
            current -> data = * updatedPhone;
            pthread_mutex_lock( & dataMutex);
            return;
        }
        current = current -> next;
    }
    printf("Phone not found.\n");

    pthread_mutex_lock( & dataMutex);
}

void deletePhone(int phoneID) {
    pthread_mutex_lock( & dataMutex);

    PhoneNode ** current = & phoneHead;
    while ( * current != NULL) {
        PhoneNode * entry = * current;
        if (entry -> data.phoneID == phoneID) {
            * current = entry -> next;
            free(entry);
            pthread_mutex_lock( & dataMutex);
            return;
        }
        current = & ( * current) -> next;
    }
    printf("Phone not found.\n");

    pthread_mutex_lock( & dataMutex);
}

void trimNewline(char* string) {
    int length = strlen(string);
    if(length > 0 && string[length-1] == '\n') {
        string[length-1] = '\0';
    }
}


void handleOperation(int entityChoice, int operationChoice) {
  char inputBuffer[256];

  switch (entityChoice) {
    case ENTITY_BRANCH:
    if (operationChoice == OPERATION_CREATE) {
        Branch branch = {0};

        printf("Enter Branch ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        branch.branchID = atoi(inputBuffer);

        printf("Enter Branch Name: ");
        fgets(branch.name, sizeof(branch.name), stdin);
        trimNewline(branch.name);

        printf("Enter Branch Phone: ");
        fgets(branch.phone, sizeof(branch.phone), stdin);
        trimNewline(branch.phone);

        printf("Enter Branch Address: ");
        fgets(branch.address, sizeof(branch.address), stdin);
        trimNewline(branch.address);

        printf("Enter Number of Rooms: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        branch.numberOfRooms = atoi(inputBuffer);

        createBranch(&branch);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Branch ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readBranch(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Branch updatedBranch = {0};
        int id;

        printf("Enter Branch ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Branch Name: ");
        fgets(updatedBranch.name, sizeof(updatedBranch.name), stdin);
        trimNewline(updatedBranch.name);

        printf("Enter new Branch Phone: ");
        fgets(updatedBranch.phone, sizeof(updatedBranch.phone), stdin);
        trimNewline(updatedBranch.phone);

        printf("Enter new Branch Address: ");
        fgets(updatedBranch.address, sizeof(updatedBranch.address), stdin);
        trimNewline(updatedBranch.address);

        printf("Enter new Number of Rooms: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedBranch.numberOfRooms = atoi(inputBuffer);

        updateBranch(id, &updatedBranch);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Branch ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteBranch(id);
    }
    break;
    
    case ENTITY_ROOM:
    if (operationChoice == OPERATION_CREATE) {
        Room room = {0};

        printf("Enter Room ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        room.roomID = atoi(inputBuffer);

        printf("Enter Room Number: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        room.number = atoi(inputBuffer);

        printf("Enter Room Floor: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        room.floor = atoi(inputBuffer);

        printf("Enter Cleaning Crew ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        room.cleaningCrewID = atoi(inputBuffer);

        printf("Enter Branch ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        room.branchID = atoi(inputBuffer);

        createRoom(&room);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Room ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readRoom(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Room updatedRoom = {0};
        int id;

        printf("Enter Room ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Room Number: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedRoom.number = atoi(inputBuffer);

        printf("Enter new Room Floor: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedRoom.floor = atoi(inputBuffer);

        printf("Enter new Cleaning Crew ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedRoom.cleaningCrewID = atoi(inputBuffer);

        printf("Enter new Branch ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedRoom.branchID = atoi(inputBuffer);

        updateRoom(id, &updatedRoom);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Room ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteRoom(id);
    }
    break;

    case ENTITY_CUSTOMER:
    if (operationChoice == OPERATION_CREATE) {
        Customer customer = {0};

        printf("Enter Customer ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        customer.customerID = atoi(inputBuffer);

        printf("Enter Customer SSN: ");
        fgets(customer.SSN, sizeof(customer.SSN), stdin);
        trimNewline(customer.SSN);

        printf("Enter Customer Name: ");
        fgets(customer.name, sizeof(customer.name), stdin);
        trimNewline(customer.name);

        printf("Enter Customer Date of Birth: ");
        fgets(customer.dateOfBirth, sizeof(customer.dateOfBirth), stdin);
        trimNewline(customer.dateOfBirth);

        printf("Enter Customer Email: ");
        fgets(customer.email, sizeof(customer.email), stdin);
        trimNewline(customer.email);

        printf("Enter Customer Booking ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        customer.bookingID = atoi(inputBuffer);

        printf("Enter Customer Phone Number: ");
        fgets(customer.phone, sizeof(customer.phone), stdin);
        trimNewline(customer.phone);

        createCustomer(&customer);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Customer ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readCustomer(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Customer updatedCustomer = {0};
        int id;

        printf("Enter Customer ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Customer SSN: ");
        fgets(updatedCustomer.SSN, sizeof(updatedCustomer.SSN), stdin);
        trimNewline(updatedCustomer.SSN);

        printf("Enter new Customer Name: ");
        fgets(updatedCustomer.name, sizeof(updatedCustomer.name), stdin);
        trimNewline(updatedCustomer.name);

        printf("Enter new Customer Date of Birth: ");
        fgets(updatedCustomer.dateOfBirth, sizeof(updatedCustomer.dateOfBirth), stdin);
        trimNewline(updatedCustomer.dateOfBirth);

        printf("Enter new Customer Email: ");
        fgets(updatedCustomer.email, sizeof(updatedCustomer.email), stdin);
        trimNewline(updatedCustomer.email);

        printf("Enter new Booking ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedCustomer.bookingID = atoi(inputBuffer);

        printf("Enter new Customer Phone Number: ");
        fgets(updatedCustomer.phone, sizeof(updatedCustomer.phone), stdin);
        trimNewline(updatedCustomer.phone);

        updateCustomer(id, &updatedCustomer);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Customer ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteCustomer(id);
    }
    break;
    
    case ENTITY_CLEANING_CREW:
    if (operationChoice == OPERATION_CREATE) {
        CleaningCrew crew = {0};

        printf("Enter Cleaning Crew ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        crew.cleaningCrewID = atoi(inputBuffer);

        printf("Enter Cleaning Crew Employees: ");
        fgets(crew.employees, sizeof(crew.employees), stdin);
        trimNewline(crew.employees);

        printf("Enter Cleaning Crew Status: ");
        fgets(crew.status, sizeof(crew.status), stdin);
        trimNewline(crew.status);

        printf("Enter Room ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        crew.roomID = atoi(inputBuffer);

        createCleaningCrew(&crew);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Cleaning Crew ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readCleaningCrew(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        CleaningCrew updatedCrew = {0};
        int id;

        printf("Enter Cleaning Crew ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Cleaning Crew Employees: ");
        fgets(updatedCrew.employees, sizeof(updatedCrew.employees), stdin);
        trimNewline(updatedCrew.employees);

        printf("Enter new Cleaning Crew Status: ");
        fgets(updatedCrew.status, sizeof(updatedCrew.status), stdin);
        trimNewline(updatedCrew.status);

        printf("Enter new Room ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedCrew.roomID = atoi(inputBuffer);

        updateCleaningCrew(id, &updatedCrew);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Cleaning Crew ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteCleaningCrew(id);
    }
    break;

    case ENTITY_BOOKING:
    if (operationChoice == OPERATION_CREATE) {
        Booking booking = {0};

        printf("Enter Booking ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        booking.bookingID = atoi(inputBuffer);

        printf("Enter Booking Date: ");
        fgets(booking.date, sizeof(booking.date), stdin);
        trimNewline(booking.date);

        printf("Enter Booking Time: ");
        fgets(booking.time, sizeof(booking.time), stdin);
        trimNewline(booking.time);

        printf("Enter Customer SSN: ");
        fgets(booking.customerSSN, sizeof(booking.customerSSN), stdin);
        trimNewline(booking.customerSSN);

        printf("Enter Registrar ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        booking.registrarID = atoi(inputBuffer);

        printf("Enter Room Number: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        booking.roomNumber = atoi(inputBuffer);

        createBooking(&booking);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Booking ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readBooking(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Booking updatedBooking = {0};
        int id;

        printf("Enter Booking ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Booking Date: ");
        fgets(updatedBooking.date, sizeof(updatedBooking.date), stdin);
        trimNewline(updatedBooking.date);

        printf("Enter new Booking Time: ");
        fgets(updatedBooking.time, sizeof(updatedBooking.time), stdin);
        trimNewline(updatedBooking.time);

        printf("Enter new Customer SSN: ");
        fgets(updatedBooking.customerSSN, sizeof(updatedBooking.customerSSN), stdin);
        trimNewline(updatedBooking.customerSSN);

        printf("Enter new Registrar ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedBooking.registrarID = atoi(inputBuffer);

        printf("Enter new Room Number: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedBooking.roomNumber = atoi(inputBuffer);

        updateBooking(id, &updatedBooking);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Booking ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteBooking(id);
    }
    break;
    
    case ENTITY_EMPLOYEE:
    if (operationChoice == OPERATION_CREATE) {
        Employee employee = {0};

        printf("Enter Employee ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        employee.employeeID = atoi(inputBuffer);

        printf("Enter Employee Name: ");
        fgets(employee.name, sizeof(employee.name), stdin);
        trimNewline(employee.name);

        printf("Enter Employee Password: ");
        fgets(employee.password, sizeof(employee.password), stdin);
        trimNewline(employee.password);

        printf("Enter Employee Email: ");
        fgets(employee.email, sizeof(employee.email), stdin);
        trimNewline(employee.email);

        printf("Enter Employee Salary: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        employee.salary = atof(inputBuffer);

        printf("Enter Employee Branch ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        employee.branchID = atoi(inputBuffer);

        printf("Enter Employee Position: ");
        fgets(employee.position, sizeof(employee.position), stdin);
        trimNewline(employee.position);

        createEmployee(&employee);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Employee ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readEmployee(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Employee updatedEmployee = {0};
        int id;

        printf("Enter Employee ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Employee Name: ");
        fgets(updatedEmployee.name, sizeof(updatedEmployee.name), stdin);
        trimNewline(updatedEmployee.name);

        printf("Enter new Employee Password: ");
        fgets(updatedEmployee.password, sizeof(updatedEmployee.password), stdin);
        trimNewline(updatedEmployee.password);

        printf("Enter new Employee Email: ");
        fgets(updatedEmployee.email, sizeof(updatedEmployee.email), stdin);
        trimNewline(updatedEmployee.email);

        printf("Enter new Employee Salary: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedEmployee.salary = atof(inputBuffer);

        printf("Enter new Employee Branch ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        updatedEmployee.branchID = atoi(inputBuffer);

        printf("Enter new Employee Position: ");
        fgets(updatedEmployee.position, sizeof(updatedEmployee.position), stdin);
        trimNewline(updatedEmployee.position);

        updateEmployee(id, &updatedEmployee);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Employee ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deleteEmployee(id);
    }
    break;
    
    case ENTITY_PHONE:
    if (operationChoice == OPERATION_CREATE) {
        Phone phone = {0};

        printf("Enter Phone ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        phone.phoneID = atoi(inputBuffer);

        printf("Enter Phone Number: ");
        fgets(phone.number, sizeof(phone.number), stdin);
        trimNewline(phone.number);

        printf("Enter Customer SSN: ");
        fgets(phone.customerSSN, sizeof(phone.customerSSN), stdin);
        trimNewline(phone.customerSSN);

        createPhone(&phone);
    } else if (operationChoice == OPERATION_READ) {
        int id;
        printf("Enter Phone ID to read: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        readPhone(id);
    } else if (operationChoice == OPERATION_UPDATE) {
        Phone updatedPhone = {0};
        int id;

        printf("Enter Phone ID to update: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        printf("Enter new Phone Number: ");
        fgets(updatedPhone.number, sizeof(updatedPhone.number), stdin);
        trimNewline(updatedPhone.number);

        printf("Enter new Customer SSN: ");
        fgets(updatedPhone.customerSSN, sizeof(updatedPhone.customerSSN), stdin);
        trimNewline(updatedPhone.customerSSN);

        updatePhone(id, &updatedPhone);
    } else if (operationChoice == OPERATION_DELETE) {
        int id;
        printf("Enter Phone ID to delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        id = atoi(inputBuffer);

        deletePhone(id);
    }
    break;
    
    default:
    printf("Invalid entity choice.\n");
    break;
  }

}

void displayEntityMenu() {
    printf("\nSelect an entity:\n");
    printf("1. Branch\n");
    printf("2. Room\n");
    printf("3. Customer\n");
    printf("4. Cleaning Crew\n");
    printf("5. Booking\n");
    printf("6. Employee\n");
    printf("7. Phone\n");
    printf("8. Exit\n");
}

void displayCRUDMenu() {
    printf("\nSelect an operation:\n");
    printf("1. Create\n");
    printf("2. Read\n");
    printf("3. Update\n");
    printf("4. Delete\n");
    printf("5. Back\n");
}

void initializeApplication() {
    pthread_mutex_init( & dataMutex, NULL);
    pthread_mutex_init( & logMutex, NULL);
    printf("Application initialized successfully.\n");
}

void cleanupResources() {
    pthread_mutex_destroy( & dataMutex);
    pthread_mutex_destroy( & logMutex);

    BranchNode * branchCurrent;
    while (branchHead != NULL) {
        branchCurrent = branchHead;
        branchHead = branchHead -> next;
        free(branchCurrent);
    }

    RoomNode * roomCurrent;
    while (roomHead != NULL) {
        roomCurrent = roomHead;
        roomHead = roomHead -> next;
        free(roomCurrent);
    }

    CustomerNode * customerCurrent;
    while (customerHead != NULL) {
        customerCurrent = customerHead;
        customerHead = customerHead -> next;
        free(customerCurrent);
    }

    CleaningCrewNode * cleaningCrewCurrent;
    while (cleaningCrewHead != NULL) {
        cleaningCrewCurrent = cleaningCrewHead;
        cleaningCrewHead = cleaningCrewHead -> next;
        free(cleaningCrewCurrent);
    }

    BookingNode * bookingCurrent;
    while (bookingHead != NULL) {
        bookingCurrent = bookingHead;
        bookingHead = bookingHead -> next;
        free(bookingCurrent);
    }

    EmployeeNode * employeeCurrent;
    while (employeeHead != NULL) {
        employeeCurrent = employeeHead;
        employeeHead = employeeHead -> next;
        free(employeeCurrent);
    }

    PhoneNode * phoneCurrent;
    while (phoneHead != NULL) {
        phoneCurrent = phoneHead;
        phoneHead = phoneHead -> next;
        free(phoneCurrent);
    }
}

//this function prevents input skips
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main() {
    initializeApplication();

    char inputBuffer[100]; // Buffer to hold input strings
    int entityChoice, operationChoice;

    do {
        displayEntityMenu();
        printf("Enter your choice: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        entityChoice = atoi(inputBuffer); // Convert input to integer

        if (entityChoice == 8) break; // Exit condition

        displayCRUDMenu();
        printf("Enter your operation choice: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        operationChoice = atoi(inputBuffer); // Convert input to integer

        if (operationChoice == 5) continue; // Go back to the main menu

        handleOperation(entityChoice, operationChoice);

    } while (entityChoice != 8);

    cleanupResources();
    return 0;
}
