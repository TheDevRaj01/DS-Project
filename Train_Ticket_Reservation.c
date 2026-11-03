#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 3 // Maximum confirmed seats for demonstration

// Structure to store Passenger Details
typedef struct Passenger {
    int pnr;
    char name[50];
    int age;
    char train_name[30];
    struct Passenger* next;
} Passenger;

// Structure for Waiting List Queue
typedef struct QueueNode {
    int pnr;
    char name[50];
    int age;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *front, *rear;
} WaitingQueue;

// Global variables
Passenger* confirmedHead = NULL;
WaitingQueue waitingQueue = {NULL, NULL};
int pnrCounter = 1001;
int confirmedCount = 0;

// Function prototypes
void bookTicket();
void cancelTicket();
void displayPassengers();
void searchTicket();

// Queue Operations
void enqueue(int pnr, char* name, int age) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->pnr = pnr;
    strcpy(newNode->name, name);
    newNode->age = age;
    newNode->next = NULL;
    
    if (waitingQueue.rear == NULL) {
        waitingQueue.front = waitingQueue.rear = newNode;
    } else {
        waitingQueue.rear->next = newNode;
        waitingQueue.rear = newNode;
    }
    printf("-> Seat full! Added to Waiting List with PNR: %d\n", pnr);
}

void dequeue(int* pnr, char* name, int* age) {
    if (waitingQueue.front == NULL) return;
    
    QueueNode* temp = waitingQueue.front;
    *pnr = temp->pnr;
    strcpy(name, temp->name);
    *age = temp->age;
    
    waitingQueue.front = waitingQueue.front->next;
    if (waitingQueue.front == NULL) {
        waitingQueue.rear = NULL;
    }
    free(temp);
}

// 1. Book Ticket
void bookTicket() {
    char name[50];
    int age;
    char train[30];

    printf("\nEnter Passenger Name: ");
    scanf(" %[^\n]s", name);
    printf("Enter Age: ");
    scanf("%d", &age);
    printf("Enter Train Name: ");
    scanf(" %[^\n]s", train);

    int currentPNR = pnrCounter++;

    if (confirmedCount < MAX_SEATS) {
        // Add to Confirmed Linked List
        Passenger* newPassenger = (Passenger*)malloc(sizeof(Passenger));
        newPassenger->pnr = currentPNR;
        strcpy(newPassenger->name, name);
        newPassenger->age = age;
        strcpy(newPassenger->train_name, train);
        newPassenger->next = confirmedHead;
        confirmedHead = newPassenger;
        
        confirmedCount++;
        printf("Success! Ticket Confirmed. PNR: %d\n", currentPNR);
    } else {
        // Add to Waiting List Queue
        enqueue(currentPNR, name, age);
    }
}

// 2. Cancel Ticket
void cancelTicket() {
    int targetPNR;
    printf("\nEnter PNR to cancel: ");
    scanf("%d", &targetPNR);

    Passenger *current = confirmedHead, *prev = NULL;

    // Search in Confirmed List
    while (current != NULL && current->pnr != targetPNR) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        // Found in confirmed list
        if (prev == NULL) {
            confirmedHead = current->next;
        } else {
            prev->next = current->next;
        }
        printf("Ticket with PNR %d cancelled successfully.\n", current->pnr);
        free(current);
        confirmedCount--;

        // If waiting list has passengers, promote the front one
        if (waitingQueue.front != NULL) {
            int w_pnr, w_age;
            char w_name[50];
            dequeue(&w_pnr, w_name, &w_age);

            Passenger* promoted = (Passenger*)malloc(sizeof(Passenger));
            promoted->pnr = w_pnr;
            strcpy(promoted->name, w_name);
            promoted->age = w_age;
            strcpy(promoted->train_name, "Express 101"); // Default train for waiting list promotion
            promoted->next = confirmedHead;
            confirmedHead = promoted;
            confirmedCount++;

            printf("--> Waiting list passenger promoted! PNR: %d (%s)\n", w_pnr, w_name);
        }
    } else {
        printf("PNR %d not found in confirmed tickets.\n", targetPNR);
    }
}

// 3. Display Passengers
void displayPassengers() {
    printf("\n=================== CONFIRMED TICKETS ===================\n");
    if (confirmedHead == NULL) {
        printf("No confirmed passengers.\n");
    } else {
        printf("%-10s | %-20s | %-5s | %-15s\n", "PNR", "Name", "Age", "Train");
        printf("---------------------------------------------------------\n");
        Passenger* temp = confirmedHead;
        while (temp != NULL) {
            printf("%-10d | %-20s | %-5d | %-15s\n", temp->pnr, temp->name, temp->age, temp->train_name);
            temp = temp->next;
        }
    }

    printf("\n==================== WAITING LIST =======================\n");
    if (waitingQueue.front == NULL) {
        printf("Waiting list is empty.\n");
    } else {
        printf("%-10s | %-20s | %-5s\n", "PNR", "Name", "Age");
        printf("------------------------------------\n");
        QueueNode* temp = waitingQueue.front;
        while (temp != NULL) {
            printf("%-10d | %-20s | %-5d\n", temp->pnr, temp->name, temp->age);
            temp = temp->next;
        }
    }
    printf("=========================================================\n");
}

// 4. Search Ticket by PNR
void searchTicket() {
    int targetPNR;
    printf("\nEnter PNR to search: ");
    scanf("%d", &targetPNR);

    Passenger* temp = confirmedHead;
    while (temp != NULL) {
        if (temp->pnr == targetPNR) {
            printf("\n[Found in Confirmed List]\n");
            printf("PNR: %d | Name: %s | Age: %d | Train: %s\n", temp->pnr, temp->name, temp->age, temp->train_name);
            return;
        }
        temp = temp->next;
    }

    QueueNode* qTemp = waitingQueue.front;
    while (qTemp != NULL) {
        if (qTemp->pnr == targetPNR) {
            printf("\n[Found in Waiting List]\n");
            printf("PNR: %d | Name: %s | Age: %d\n", qTemp->pnr, qTemp->name, qTemp->age);
            return;
        }
        qTemp = qTemp->next;
    }

    printf("PNR %d not found.\n", targetPNR);
}

// Main Menu
int main() {
    int choice;
    do {
        printf("\n--- Train Ticket Reservation System ---\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Passengers (Confirmed & Waiting)\n");
        printf("4. Search Ticket by PNR\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: displayPassengers(); break;
            case 4: searchTicket(); break;
            case 5: printf("Exiting program. Have a safe journey!\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
