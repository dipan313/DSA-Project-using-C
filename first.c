#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a Train Route
typedef struct TrainRoute
{
    char startStation[50];
    char destination[50];
    int stoppages;
    float duration; // Duration in hours
    struct TrainRoute *next;
} TrainRoute;

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
void insertRoute(TrainRoute **head, char *startStation, char *destination, int stoppages, float duration)
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

int main()
{
    TrainRoute *head = NULL;
    int user, choice, stoppages,password;
    float duration;
    char startStation[50], destination[50];

   // while (1)
    //{
ab:
        printf("Who are you\nare you 1.normal\n2.administor\n");
        scanf("%d", &user);

        switch (user)
        {
        case 1:
            printf("\nTrain Scheduler Menu:\n");
            printf("1. Add Route\n");
            printf("2. Display All Routes\n");
            printf("3. Delete a Route\n");
            printf("4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice)
            {
            case 1:
                printf("Enter Start Station: ");
                scanf("%s", startStation);
                printf("Enter Destination: ");
                scanf("%s", destination);
                printf("Enter Number of Stoppages: ");
                scanf("%d", &stoppages);
                printf("Enter Duration (in hours): ");
                scanf("%f", &duration);
                insertRoute(&head, startStation, destination, stoppages, duration);
                break;
                goto ab;

            case 2:
                displayRoutes(head);
                break;

            case 3:
                printf("Enter Start Station of the Route to be Deleted: ");
                scanf("%s", startStation);
                printf("Enter Destination of the Route to be Deleted: ");
                scanf("%s", destination);
                deleteRoute(&head, startStation, destination);
                break;

            case 4:
                printf("Exiting...\n");
                //exit(0);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
            }
        case 2:
            printf("administration\n");
            printf("Enter password");
            scanf("%d",&password);
            switch (password)
                {
                    case 123:
                        printf("go ahead");
                        break;
                    default:
                        printf("wrong password");
                }
        }
    //}

    return 0;
}