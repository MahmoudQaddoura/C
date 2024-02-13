#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ENTITY_COUNT 7

//entity structs
typedef struct {
    int branchID;
    char name[50];
    char phone[20];
    char address[100];
    int numberOfRooms;
} Branch;

typedef struct {
    int roomID;
    int number;
    int floor;
    int cleaningCrewID;  
    int branchID;       
} Room;

typedef struct {
    int customerID;
    char SSN[11];
    char name[50];
    char dateOfBirth[10];
    char email[50];
    int bookingID;  
    char phone[20];
} Customer;

typedef struct {
    int cleaningCrewID;
    char employees[50];  
    char status[20];
    int roomID;    
} CleaningCrew;

typedef struct {
    int bookingID;
    char date[10];
    char time[10];
    char customerSSN[11]; 
    int registrarID;     
    int roomNumber;     
} Booking;

typedef struct {
    int employeeID;
    char name[50];
    char password[20];
    char email[50];
    double salary;
    int branchID;
    char position[20];
} Employee;

typedef struct {
    int phoneID;
    char number[20];
    char customerSSN[11];
} Phone;

// Linked list node structures
typedef struct BranchNode {
   Branch data;
   struct BranchNode *next;
} BranchNode;

typedef struct RoomNode {
   Room data;
   struct RoomNode *next;
} RoomNode;

typedef struct CustomerNode {
   Customer data;
   struct CustomerNode *next;
} CustomerNode;

typedef struct CleaningCrewNode {
   CleaningCrew data;
   struct CleaningCrewNode *next;
} CleaningCrewNode;

typedef struct BookingNode {
   Booking data;
   struct BookingNode *next;
} BookingNode;

typedef struct EmployeeNode {
   Employee data;
   struct EmployeeNode *next;
} EmployeeNode;

typedef struct PhoneNode {
   Phone data;
   struct PhoneNode *next;
} PhoneNode;

BranchNode *branchHead = NULL;
RoomNode *roomHead = NULL;
CustomerNode *customerHead = NULL;
CleaningCrewNode *cleaningCrewHead = NULL;
BookingNode *bookingHead = NULL;
EmployeeNode *employeeHead = NULL;
PhoneNode *phoneHead = NULL;

//generic CRUD operations and thread function prototypes
void createRecord(void **head, int entityType);
void readRecord(void* head, int entityType);
void updateRecord(void **head, int entityType, int searchID);
void deleteRecord(void **head, int entityType, int searchID);
void* threadFunction(void* args);

//enums that point to entity type to be used in generic CRUD functions
typedef enum {
    ENTITY_BRANCH,
    ENTITY_ROOM,
    ENTITY_CUSTOMER,
    ENTITY_CLEANING_CREW,
    ENTITY_BOOKING,
    ENTITY_EMPLOYEE,
    ENTITY_PHONE
} EntityType;

//globaly declared array of mutexes
pthread_mutex_t entityMutexes[ENTITY_COUNT];

//mutex for db log file
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

// Path to the log file
const char* logFilePath = "operation_log.txt";

//CRUD functions operations implementation with mutex Lock and Unlock to handle concurruncy
//create
void createRecord(void **head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
    int c; // Variable for clearing input buffer
    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *newNode = (BranchNode *)malloc(sizeof(BranchNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new BranchNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(BranchNode));

            // Numerical input, no buffer clearing needed immediately after
            printf("Enter new branch ID: ");
            scanf("%d", &newNode->data.branchID);

            // Clear buffer before reading strings
            while ((c = getchar()) != '\n' && c != EOF) { }

            printf("Enter new branch name: ");
            scanf("%49[^\n]", newNode->data.name); // scanf with %[^\n] to read until newline

            printf("Enter new branch phone: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf("%19s", newNode->data.phone);

            printf("Enter new branch address: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf(" %99[^\n]", newNode->data.address); // Leading space in format string to skip whitespace

            // Numerical input, buffer clearing before is not strictly necessary
            printf("Enter number of rooms: ");
            scanf("%d", &newNode->data.numberOfRooms);

            newNode->next = *(BranchNode **)head;
            *(BranchNode **)head = newNode;
            break;
        }
                case ENTITY_ROOM: {
            RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new RoomNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(RoomNode));

            printf("Enter new room ID: ");
            scanf("%d", &newNode->data.roomID);

            printf("Enter room number: ");
            scanf("%d", &newNode->data.number);

            printf("Enter floor: ");
            scanf("%d", &newNode->data.floor);

            printf("Enter cleaning crew ID: ");
            scanf("%d", &newNode->data.cleaningCrewID);

            printf("Enter branch ID: ");
            scanf("%d", &newNode->data.branchID);

            newNode->next = *(RoomNode **)head;
            *(RoomNode **)head = newNode;
            break;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode *newNode = (CustomerNode *)malloc(sizeof(CustomerNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new CustomerNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(CustomerNode));

            printf("Enter new customer ID: ");
            scanf("%d", &newNode->data.customerID);

            // Clear buffer before reading strings
            while ((c = getchar()) != '\n' && c != EOF) { }

            printf("Enter customer SSN: ");
            scanf("%10s", newNode->data.SSN);

            printf("Enter customer name: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf("%49[^\n]", newNode->data.name);

            printf("Enter date of birth (YYYY-MM-DD): ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf("%10s", newNode->data.dateOfBirth);

            printf("Enter email: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf("%49s", newNode->data.email);

            printf("Enter booking ID: ");
            scanf("%d", &newNode->data.bookingID);

            printf("Enter phone: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf("%19s", newNode->data.phone);

            newNode->next = *(CustomerNode **)head;
            *(CustomerNode **)head = newNode;
            break;
        }
                case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *newNode = (CleaningCrewNode *)malloc(sizeof(CleaningCrewNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new CleaningCrewNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(CleaningCrewNode));

            printf("Enter cleaning crew ID: ");
            scanf("%d", &newNode->data.cleaningCrewID);

            // Clear buffer before reading strings
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }

            printf("Enter employees names: ");
            scanf(" %49[^\n]", newNode->data.employees);

            printf("Enter status: ");
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer
            scanf(" %19[^\n]", newNode->data.status);

            printf("Enter room ID: ");
            scanf("%d", &newNode->data.roomID);

            newNode->next = *(CleaningCrewNode **)head;
            *(CleaningCrewNode **)head = newNode;
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode *newNode = (BookingNode *)malloc(sizeof(BookingNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new BookingNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(BookingNode));

            printf("Enter booking ID: ");
            scanf("%d", &newNode->data.bookingID);

            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer

            printf("Enter date (YYYY-MM-DD): ");
            scanf("%10s", newNode->data.date);

            printf("Enter time (HH:MM): ");
            scanf("%5s", newNode->data.time);

            printf("Enter customer SSN: ");
            scanf("%11s", newNode->data.customerSSN);

            printf("Enter registrar ID: ");
            scanf("%d", &newNode->data.registrarID);

            printf("Enter room number: ");
            scanf("%d", &newNode->data.roomNumber);

            newNode->next = *(BookingNode **)head;
            *(BookingNode **)head = newNode;
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *newNode = (EmployeeNode *)malloc(sizeof(EmployeeNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new EmployeeNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(EmployeeNode));

            printf("Enter employee ID: ");
            scanf("%d", &newNode->data.employeeID);

            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer

            printf("Enter name: ");
            scanf(" %49[^\n]", newNode->data.name);

            printf("Enter password: ");
            scanf(" %19s", newNode->data.password);

            printf("Enter email: ");
            scanf(" %49s", newNode->data.email);

            printf("Enter salary: ");
            scanf("%lf", &newNode->data.salary);

            printf("Enter branch ID: ");
            scanf("%d", &newNode->data.branchID);

            printf("Enter position: ");
            scanf(" %19[^\n]", newNode->data.position);

            newNode->next = *(EmployeeNode **)head;
            *(EmployeeNode **)head = newNode;
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode *newNode = (PhoneNode *)malloc(sizeof(PhoneNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new PhoneNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(PhoneNode));

            printf("Enter phone ID: ");
            scanf("%d", &newNode->data.phoneID);

            while ((c = getchar()) != '\n' && c != EOF) { } // Clear buffer

            printf("Enter number: ");
            scanf("%19s", newNode->data.number);

            printf("Enter customer SSN: ");
            scanf("%11s", newNode->data.customerSSN);

            newNode->next = *(PhoneNode **)head;
            *(PhoneNode **)head = newNode;
            break;
        }
    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}

//Display functions to show readRecord results to the user

void displayBranch(const Branch* branch) {
    printf("ID: %d, Name: %s, Phone: %s, Address: %s, Number of Rooms: %d\n",
           branch->branchID, branch->name, branch->phone, branch->address, branch->numberOfRooms);
}

void displayRoom(const Room* room) {
    printf("ID: %d, Number: %d, Floor: %d, Cleaning Crew ID: %d, Branch ID: %d\n",
           room->roomID, room->number, room->floor, room->cleaningCrewID, room->branchID);
}

void displayCustomer(const Customer* customer) {
    printf("ID: %d, SSN: %s, Name: %s, Date of Birth: %s, Email: %s, Booking ID: %d, Phone: %s\n",
           customer->customerID, customer->SSN, customer->name, customer->dateOfBirth, customer->email, customer->bookingID, customer->phone);
}

void displayCleaningCrew(const CleaningCrew* crew) {
    printf("ID: %d, Employees: %s, Status: %s, Room ID: %d\n",
           crew->cleaningCrewID, crew->employees, crew->status, crew->roomID);
}

void displayBooking(const Booking* booking) {
    printf("ID: %d, Date: %s, Time: %s, Customer SSN: %s, Registrar ID: %d, Room Number: %d\n",
           booking->bookingID, booking->date, booking->time, booking->customerSSN, booking->registrarID, booking->roomNumber);
}

void displayEmployee(const Employee* employee) {
    printf("ID: %d, Name: %s, Password: [PROTECTED], Email: %s, Salary: %.2f, Branch ID: %d, Position: %s\n",
           employee->employeeID, employee->name, employee->email, employee->salary, employee->branchID, employee->position);
}

void displayPhone(const Phone* phone) {
    printf("ID: %d, Number: %s, Customer SSN: %s\n",
           phone->phoneID, phone->number, phone->customerSSN);
}

//read
void readRecord(void *head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]); 

    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *current = (BranchNode *)head;
            while (current != NULL) {
                displayBranch(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_ROOM: {
            RoomNode *current = (RoomNode *)head;
            while (current != NULL) {
                displayRoom(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode *current = (CustomerNode *)head;
            while (current != NULL) {
                displayCustomer(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *current = (CleaningCrewNode *)head;
            while (current != NULL) {
                displayCleaningCrew(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode *current = (BookingNode *)head;
            while (current != NULL) {
                displayBooking(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *current = (EmployeeNode *)head;
            while (current != NULL) {
                displayEmployee(&current->data); 
                current = current->next;
            }
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode *current = (PhoneNode *)head;
            while (current != NULL) {
                displayPhone(&current->data); 
                current = current->next;
            }
            break;
        }
        default:
            printf("Invalid entity type.\n");
            break;
    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}

//update
void updateRecord(void **head, int entityType, int searchID) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
    int c; // Variable to clear the input buffer

    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *current = *(BranchNode **)head;
            while (current) {
                if (current->data.branchID == searchID) {
                    printf("Updating Branch ID %d\n", searchID);
                    printf("Enter new branch name: ");
                    scanf("%49s", current->data.name);
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    printf("Enter new branch phone: ");
                    scanf("%19s", current->data.phone);
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    printf("Enter new branch address: ");
                    scanf(" %99[^\n]", current->data.address);
                    printf("Enter number of rooms: ");
                    scanf("%d", &current->data.numberOfRooms);
                    break;
                }
                current = current->next;
            }
            break;
        }
        case ENTITY_ROOM: {
            RoomNode *current = *(RoomNode **)head;
            while (current) {
                if (current->data.roomID == searchID) {
                    printf("Updating Room ID %d\n", searchID);
                    printf("Enter room number: ");
                    scanf("%d", &current->data.number);
                    printf("Enter floor: ");
                    scanf("%d", &current->data.floor);
                    printf("Enter cleaning crew ID: ");
                    scanf("%d", &current->data.cleaningCrewID);
                    printf("Enter branch ID: ");
                    scanf("%d", &current->data.branchID);
                    break;
                }
                current = current->next;
            }
            break;
        }        case ENTITY_CUSTOMER: {
            CustomerNode *current = *(CustomerNode **)head;
            while (current) {
                if (current->data.customerID == searchID) {
                    printf("Updating Customer ID %d\n", searchID);
                    printf("Enter customer SSN: ");
                    scanf("%10s", current->data.SSN);
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    printf("Enter customer name: ");
                    scanf(" %49[^\n]", current->data.name);
                    printf("Enter date of birth (YYYY-MM-DD): ");
                    scanf("%10s", current->data.dateOfBirth);
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    printf("Enter email: ");
                    scanf(" %49[^\n]", current->data.email);
                    printf("Enter booking ID: ");
                    scanf("%d", &current->data.bookingID);
                    printf("Enter phone: ");
                    scanf("%19s", current->data.phone);
                    break;
                }
                current = current->next;
            }
            break;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *current = *(CleaningCrewNode **)head;
            while (current) {
                if (current->data.cleaningCrewID == searchID) {
                    printf("Updating Cleaning Crew ID %d\n", searchID);
                    printf("Enter employees names: ");
                    scanf(" %49[^\n]", current->data.employees);
                    printf("Enter status: ");
                    scanf(" %19[^\n]", current->data.status);
                    printf("Enter room ID: ");
                    scanf("%d", &current->data.roomID);
                    break;
                }
                current = current->next;
            }
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode *current = *(BookingNode **)head;
            while (current) {
                if (current->data.bookingID == searchID) {
                    printf("Updating Booking ID %d\n", searchID);
                    printf("Enter date (YYYY-MM-DD): ");
                    scanf("%10s", current->data.date);
                    printf("Enter time (HH:MM): ");
                    scanf("%5s", current->data.time);
                    printf("Enter customer SSN: ");
                    scanf("%11s", current->data.customerSSN);
                    printf("Enter registrar ID: ");
                    scanf("%d", &current->data.registrarID);
                    printf("Enter room number: ");
                    scanf("%d", &current->data.roomNumber);
                    break;
                }
                current = current->next;
            }
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *current = *(EmployeeNode **)head;
            while (current) {
                if (current->data.employeeID == searchID) {
                    printf("Updating Employee ID %d\n", searchID);
                    printf("Enter name: ");
                    scanf(" %49[^\n]", current->data.name);
                    printf("Enter password: ");
                    scanf("%19s", current->data.password);
                    printf("Enter email: ");
                    scanf(" %49[^\n]", current->data.email);
                    printf("Enter salary: ");
                    scanf("%lf", &current->data.salary);
                    printf("Enter branch ID: ");
                    scanf("%d", &current->data.branchID);
                    printf("Enter position: ");
                    scanf(" %19[^\n]", current->data.position);
                    break;
                }
                current = current->next;
            }
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode *current = *(PhoneNode **)head;
            while (current) {
                if (current->data.phoneID == searchID) {
                    printf("Updating Phone ID %d\n", searchID);
                    printf("Enter number: ");
                    scanf("%20s", current->data.number);
                    printf("Enter customer SSN: ");
                    scanf("%11s", current->data.customerSSN);
                    break;
                }
                current = current->next;
            }
            break;
        }

    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}

//delete
void deleteRecord(void **head, int entityType, int searchID) {
    if (!head || !*head) return; // Check if the list is empty or the head is NULL.

    pthread_mutex_lock(&entityMutexes[entityType - 1]);

    // Use temporary pointers for traversal and deletion.
    void **curr = head;
    while (*curr) {
        int idMatch = 0; // Flag to indicate if the ID matches.
        // Cast the current node based on the entity type and compare IDs.
        switch (entityType) {
            case ENTITY_BRANCH: {
                BranchNode *node = (BranchNode *)*curr;
                idMatch = (node->data.branchID == searchID);
                break;
            }
            case ENTITY_ROOM: {
                RoomNode *node = (RoomNode *)*curr;
                idMatch = (node->data.roomID == searchID);
                break;
            }
            // Repeat for other entity types...
            case ENTITY_CUSTOMER: {
                CustomerNode *node = (CustomerNode *)*curr;
                idMatch = (node->data.customerID == searchID);
                break;
            }
            case ENTITY_CLEANING_CREW: {
                CleaningCrewNode *node = (CleaningCrewNode *)*curr;
                idMatch = (node->data.cleaningCrewID == searchID);
                break;
            }
            case ENTITY_BOOKING: {
                BookingNode *node = (BookingNode *)*curr;
                idMatch = (node->data.bookingID == searchID);
                break;
            }
            case ENTITY_EMPLOYEE: {
                EmployeeNode *node = (EmployeeNode *)*curr;
                idMatch = (node->data.employeeID == searchID);
                break;
            }
            case ENTITY_PHONE: {
                PhoneNode *node = (PhoneNode *)*curr;
                idMatch = (node->data.phoneID == searchID);
                break;
            }
            default:
                printf("Invalid entity type.\n");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
        }

        if (idMatch) {
            void *temp = *curr; // Store the node to be deleted.
            *curr = *((void **)*curr); // Update the link to bypass the deleted node.
            free(temp); // Free the memory of the deleted node.
            break; // Exit the loop after deletion.
        } else {
            curr = (void **)*curr; // Move to the next node.
        }
    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}


//2 level menus for user input
void displayEntityMenu() {
    printf("Select an entity:\n");
    printf("1. Branch\n");
    printf("2. Room\n");
    printf("3. Customer\n");
    printf("4. Cleaning Crew\n");
    printf("5. Booking\n");
    printf("6. Employee\n");
    printf("7. Phone\n");
    printf("8. Back\n");
}

void displayCRUDMenu() {
    printf("Select an operation:\n");
    printf("1. Create\n");
    printf("2. Read\n");
    printf("3. Update\n");
    printf("4. Delete\n");
    printf("5. Back\n");
}

typedef struct ThreadArgs {
    pthread_mutex_t *mutexes;
    int entityType;
    int operationType;
    void **heads;
} ThreadArgs;

//thread function that handles concurrent access to the program using mutexes
void *threadFunction(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int operationChoice, entityChoice, searchID, c;

    while (1) {
        printf("\nChoose operation: 1. Create 2. Read 3. Update 4. Delete 5. Exit\n");
        scanf("%d", &operationChoice);
        while ((c = getchar()) != '\n' && c != EOF) { } // Clear input buffer

        if (operationChoice == 5) {
            break;
        }

        printf("\nChoose entity type: 1. Branch 2. Room 3. Customer 4. Cleaning Crew 5. Booking 6. Employee 7. Phone\n");
        scanf("%d", &entityChoice);
        while ((c = getchar()) != '\n' && c != EOF) { } // Clear input buffer

        entityChoice--;
        
        if (operationChoice == 3 || operationChoice == 4) { // For Update or Delete, require ID
            printf("Enter ID for the operation: ");
            scanf("%d", &searchID);
            while ((c = getchar()) != '\n' && c != EOF) { } // Clear input buffer
        }

        // Execute the selected operation
        switch (operationChoice) {
            case 1: // Create
                createRecord(&(threadArgs->heads[entityChoice]), entityChoice);
                break;
            case 2: // Read
                readRecord(threadArgs->heads[entityChoice], entityChoice);
                break;
            case 3: // Update
                updateRecord(&(threadArgs->heads[entityChoice]), entityChoice, searchID);
                break;
            case 4: // Delete
                deleteRecord(&(threadArgs->heads[entityChoice]), entityChoice, searchID);
                break;
            default:
                printf("Invalid operation choice.\n");
                break;
        }
    }

    return NULL;
}



//clean up functions for each entity for exit
void freeBranchLinkedList(BranchNode *head) {
    BranchNode *current = head;
    while (current != NULL) {
        BranchNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeRoomLinkedList(RoomNode *head) {
    RoomNode *current = head;
    while (current != NULL) {
        RoomNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeCustomerLinkedList(CustomerNode *head) {
    CustomerNode *current = head;
    while (current != NULL) {
        CustomerNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeCleaningCrewLinkedList(CleaningCrewNode *head) {
    CleaningCrewNode *current = head;
    while (current != NULL) {
        CleaningCrewNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeBookingLinkedList(BookingNode *head) {
    BookingNode *current = head;
    while (current != NULL) {
        BookingNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeEmployeeLinkedList(EmployeeNode *head) {
    EmployeeNode *current = head;
    while (current != NULL) {
        EmployeeNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freePhoneLinkedList(PhoneNode *head) {
    PhoneNode *current = head;
    while (current != NULL) {
        PhoneNode *temp = current;
        current = current->next;
        free(temp);
    }
}

//function to log operations to a file
void logOperation(const char* operation) {
    pthread_mutex_lock(&fileMutex);
    FILE *logFile = fopen(logFilePath, "a");
    if (logFile) {
        fprintf(logFile, "%s\n", operation);
        fclose(logFile);
    } else {
        perror("Failed to open log file");
    }
    pthread_mutex_unlock(&fileMutex);
}

//this function handels user interaction with the inner most functionalities of the proram
void* userInteractionThread(void* arg) {
    ThreadArgs *args = (ThreadArgs*) arg;
    int operationChoice, entityChoice, idToActOn;
    EntityType selectedEntityType;
    void *selectedHead = NULL;
    int c; // For clearing the input buffer

    // Loop until the user decides to exit
    while (1) {
        displayCRUDMenu();
        scanf("%d", &operationChoice);

        // Clear the input buffer
        while ((c = getchar()) != '\n' && c != EOF) {}

        // Exit condition
        if (operationChoice == 5) break;

        displayEntityMenu();
        scanf("%d", &entityChoice);

        // Clear the input buffer
        while ((c = getchar()) != '\n' && c != EOF) {}

        selectedEntityType = entityChoice - 1; // Adjusting for 0-based indexing

        // Ensure the selection is valid
        if (selectedEntityType < 0 || selectedEntityType >= ENTITY_COUNT) {
            printf("Invalid entity type selected.\n");
            continue;
        }

        selectedHead = args->heads[selectedEntityType];

        // Perform the selected operation
        switch (operationChoice) {
            case 1: // Create
                createRecord(&selectedHead, selectedEntityType);
                break;
            case 2: // Read
                printf("Enter ID to read: ");
                scanf("%d", &idToActOn);
                // Clear the input buffer
                while ((c = getchar()) != '\n' && c != EOF) {}
                readRecord(selectedHead, selectedEntityType);
                break;
            case 3: // Update
                printf("Enter ID to update: ");
                scanf("%d", &idToActOn);
                // Clear the input buffer
                while ((c = getchar()) != '\n' && c != EOF) {}
                updateRecord(&selectedHead, selectedEntityType, idToActOn);
                break;
            case 4: // Delete
                printf("Enter ID to delete: ");
                scanf("%d", &idToActOn);
                // Clear the input buffer
                while ((c = getchar()) != '\n' && c != EOF) {}
                deleteRecord(&selectedHead, selectedEntityType, idToActOn);
                break;
            default:
                printf("Invalid operation selected.\n");
                break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_mutex_t mutexes[ENTITY_COUNT];
    for (int i = 0; i < ENTITY_COUNT; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    // Initialize pointers to head of each entity list
    void *heads[ENTITY_COUNT] = {
        &branchHead, &roomHead, &customerHead,
        &cleaningCrewHead, &bookingHead, &employeeHead, &phoneHead
    };

    // Main program loop
    while (1) {
        int entityChoice = 0;
        int operationChoice = 0;
        int c; // Variable for clearing input buffer

        displayEntityMenu();
        printf("Enter your choice (or 8 to exit): ");
        scanf("%d", &entityChoice);

        // Clear the input buffer after reading choice
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (entityChoice == 8) break; // Exit program

        displayCRUDMenu();
        printf("Enter your choice (or 5 to go back): ");
        scanf("%d", &operationChoice);

        // Clear the input buffer after reading choice
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (operationChoice == 5) continue; // Go back to the entity selection

        ThreadArgs threadArgs = {
            .mutexes = mutexes,
            .entityType = entityChoice - 1, // Adjust for 0-based indexing in heads array
            .heads = heads
        };

        pthread_t tid;
        pthread_create(&tid, NULL, userInteractionThread, (void*)&threadArgs);
        pthread_join(tid, NULL); // Wait for the thread to complete its operation
    }

    // Cleanup
    for (int i = 0; i < ENTITY_COUNT; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    freeBranchLinkedList(branchHead);
    freeRoomLinkedList(roomHead);
    freeCustomerLinkedList(customerHead);
    freeCleaningCrewLinkedList(cleaningCrewHead);
    freeBookingLinkedList(bookingHead);
    freeEmployeeLinkedList(employeeHead);
    freePhoneLinkedList(phoneHead);

    return 0;
}
