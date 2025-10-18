#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 10  // Define a fixed stack size

// Define the structure for a Train Route
typedef struct TrainRoute
{
    char startStation[50];
    char destination[50];
    int stoppages;
    float duration; // Duration in hours
    struct TrainRoute *next;
} TrainRoute;

// Stack for tracking the last added routes (array-based)
typedef struct RouteStack
{
    TrainRoute *routes[STACK_SIZE];
    int top;
} RouteStack;

// Function to initialize the stack
void initStack(RouteStack *stack)
{
    stack->top = -1;  // Stack is initially empty
}

// Function to check if the stack is empty
int isEmpty(RouteStack *stack)
{
    return stack->top == -1;
}

// Function to check if the stack is full
int isFull(RouteStack *stack)
{
    return stack->top == STACK_SIZE - 1;
}

// Function to push a route onto the stack
void pushStack(RouteStack *stack, TrainRoute *route)
{
    if (isFull(stack))
    {
        printf("Stack is full, cannot undo further.\n");
        return;
    }
    stack->routes[++(stack->top)] = route;
}

// Function to pop a route from the stack (undo last add)
TrainRoute *popStack(RouteStack *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack is empty, nothing to undo.\n");
        return NULL;
    }
    return stack->routes[(stack->top)--];
}

// Function to create a new train route node
TrainRoute *createRoute(char *startStation, char *destination, int stoppages, float duration)
{
    TrainRoute *newRoute = (TrainRoute *)malloc(sizeof(TrainRoute));
    strcpy(newRoute->startStation, startStation);
    strcpy(newRoute->destination, destination);
    newRoute->stoppages = stoppages;
    newRoute->duration = duration;
    newRoute->next = NULL;
    return newRoute;
}

// Function to insert a new train route at the end of the linked list
void insertRoute(TrainRoute **head, RouteStack *stack, char *startStation, char *destination, int stoppages, float duration)
{
    TrainRoute *newRoute = createRoute(startStation, destination, stoppages, duration);
    if (*head == NULL)
    {
        *head = newRoute;
    }
    else
    {
        TrainRoute *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newRoute;
    }
    // Push the newly added route onto the stack
    pushStack(stack, newRoute);
}

// Function to display all train routes
void displayRoutes(TrainRoute *head)
{
    if (head == NULL)
    {
        printf("No routes available.\n");
        return;
    }
    TrainRoute *temp = head;
    while (temp != NULL)
    {
        printf("Start Station: %s, Destination: %s, Stoppages: %d, Duration: %.2f hours\n",
               temp->startStation, temp->destination, temp->stoppages, temp->duration);
        temp = temp->next;
    }
}

// Function to search for a route based on start station and destination
void searchRoute(TrainRoute *head, char *startStation, char *destination)
{
    TrainRoute *temp = head;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->startStation, startStation) == 0 && strcmp(temp->destination, destination) == 0)
        {
            printf("Route Found: Start Station: %s, Destination: %s, Stoppages: %d, Duration: %.2f hours\n",
                   temp->startStation, temp->destination, temp->stoppages, temp->duration);
            found = 1;
            break;
        }
        temp = temp->next;
    }

    if (!found)
    {
        printf("No route found from %s to %s.\n", startStation, destination);
    }
}

// Function to delete a route based on start station and destination
void deleteRoute(TrainRoute **head, char *startStation, char *destination)
{
    TrainRoute *temp = *head;
    TrainRoute *prev = NULL;

    // If head node itself holds the route to be deleted
    if (temp != NULL && strcmp(temp->startStation, startStation) == 0 && strcmp(temp->destination, destination) == 0)
    {
        *head = temp->next; // Changed head
        free(temp);         // Free old head
        printf("Route from %s to %s deleted.\n", startStation, destination);
        return;
    }

    // Search for the route to be deleted
    while (temp != NULL && (strcmp(temp->startStation, startStation) != 0 || strcmp(temp->destination, destination) != 0))
    {
        prev = temp;
        temp = temp->next;
    }

    // If route was not present in the linked list
    if (temp == NULL)
    {
        printf("Route from %s to %s not found.\n", startStation, destination);
        return;
    }

    // Unlink the node from the linked list
    prev->next = temp->next;
    free(temp);
    printf("Route from %s to %s deleted.\n", startStation, destination);
}

// Function to undo the last route added
void undoLastRoute(TrainRoute **head, RouteStack *stack)
{
    TrainRoute *lastRoute = popStack(stack);
    if (lastRoute != NULL)
    {
        // Remove last route from the linked list
        TrainRoute *temp = *head;
        TrainRoute *prev = NULL;

        while (temp != NULL && temp != lastRoute)
        {
            prev = temp;
            temp = temp->next;
        }

        if (temp == lastRoute)
        {
            if (prev == NULL)
            {
                *head = temp->next;  // If head node is the one to be deleted
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            printf("Last route addition undone.\n");
        }
    }
}

// Function to save all routes to a file
void saveRoutesToFile(TrainRoute *head)
{
    FILE *file = fopen("train_routes.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    TrainRoute *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%s %s %d %.2f\n", temp->startStation, temp->destination, temp->stoppages, temp->duration);
        temp = temp->next;
    }

    fclose(file);
}

// Function to load routes from a file
void loadRoutesFromFile(TrainRoute **head)
{
    FILE *file = fopen("train_routes.txt", "r");
    if (file == NULL)
    {
        printf("No existing routes found. Creating initial database.\n");
        // Create an initial database with sample data
        insertRoute(head, NULL, "Sealdah", "Bongaon", 13, 1.75);
        insertRoute(head, NULL, "Sealdah", "Kolkata", 5, 0.45);
        insertRoute(head, NULL, "Sealdah", "Howrah", 8, 0.65);
        insertRoute(head, NULL, "Sealdah", "Naihati", 10, 1.10);
        insertRoute(head, NULL, "Sealdah", "Baranagar", 6, 0.50);
        insertRoute(head, NULL, "Sealdah", "Dum Dum", 4, 0.30);
        insertRoute(head, NULL, "Sealdah", "Bidhan Sarani", 3, 0.20);
        insertRoute(head, NULL, "Sealdah", "North Dumdum", 7, 0.55);
        insertRoute(head, NULL, "Sealdah", "Chandannagar", 15, 2.0);
        insertRoute(head, NULL, "Sealdah", "Kamarhati", 9, 0.75);
        insertRoute(head, NULL, "Sealdah", "Garia", 14, 1.85);
        insertRoute(head, NULL, "Sealdah", "Sodepur", 12, 1.65);
        saveRoutesToFile(*head);
        return;
    }

    char startStation[50], destination[50];
    int stoppages;
    float duration;
    while (fscanf(file, "%s %s %d %f", startStation, destination, &stoppages, &duration) != EOF)
    {
        insertRoute(head, NULL, startStation, destination, stoppages, duration);
    }

    fclose(file);
}

int main()
{
    TrainRoute *head = NULL;
    RouteStack routeStack;
    initStack(&routeStack);

    loadRoutesFromFile(&head);

    int choice;
    char startStation[50], destination[50];
    int stoppages;
    float duration;

    while (1)
    {
        printf("\n1. Insert Route\n2. Delete Route\n3. Search Route\n4. Display All Routes\n5. Undo Last Route Addition\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter start station: ");
            scanf("%s", startStation);
            printf("Enter destination: ");
            scanf("%s", destination);
            printf("Enter number of stoppages: ");
            scanf("%d", &stoppages);
            printf("Enter duration (hours): ");
            scanf("%f", &duration);
            insertRoute(&head, &routeStack, startStation, destination, stoppages, duration);
            saveRoutesToFile(head);
            break;
        case 2:
            printf("Enter start station to delete: ");
            scanf("%s", startStation);
            printf("Enter destination to delete: ");
            scanf("%s", destination);
            deleteRoute(&head, startStation, destination);
            saveRoutesToFile(head);
            break;
        case 3:
            printf("Enter start station to search: ");
            scanf("%s", startStation);
            printf("Enter destination to search: ");
            scanf("%s", destination);
            searchRoute(head, startStation, destination);
            break;
        case 4:
            displayRoutes(head);
            break;
        case 5:
            undoLastRoute(&head, &routeStack);
            saveRoutesToFile(head);
            break;
        case 6:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
