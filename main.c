#include <stdio.h>
#include <stdlib.h>

const int MAX_INTERACTIONS = 1000000;
int MAX_BUGS = 2000;
int addedVertices[1000001];

char *FILE_NAME = "sample.txt";
int previousBugType = 0;

typedef struct vertex {
    int isVisited;
    struct linkedList *adjacentVertices;
    int bugType;
} vertex;

typedef struct linkedList {
    int quantity;
    struct listItem *beginning;
} linkedList;

typedef struct listItem {
    int value;
    struct listItem *next;
} listItem;


void processFile(char *fileName);

int dfs(vertex *vertices, int vertexIndex);

int main() {
    processFile(FILE_NAME);
    return 0;
}

linkedList *newLinkedList() {
    return (linkedList *) calloc(1, sizeof(linkedList));
}

listItem *newListItem() {
    return (listItem *) calloc(1, sizeof(listItem));
}

vertex *newVertices() { return (vertex *) calloc(MAX_INTERACTIONS, sizeof(vertex)); }

void pushVertexToList(vertex *vertexInstance, int vertexValue) {
    if (vertexInstance->adjacentVertices == NULL)
        vertexInstance->adjacentVertices = newLinkedList();
    linkedList *adjacentVertices = vertexInstance->adjacentVertices;
    if (adjacentVertices == NULL)
        return;

    listItem *newItem, *current = NULL, *previous = NULL;
    newItem = newListItem();
    newItem->value = vertexValue;

    if (adjacentVertices->beginning == NULL) {
        adjacentVertices->beginning = newItem;
    } else {
        int isAdded = 0;
        current = adjacentVertices->beginning;
        while (current != NULL && !isAdded) {
            if (current->value == newItem->value) {
                return;
            }

            if (current->value < newItem->value) {
                previous = current;
                current = current->next;
            } else {
                if (previous == NULL)
                    adjacentVertices->beginning = newItem;
                else
                    previous->next = newItem;

                newItem->next = current;
                isAdded = 1;
            }
        }
        if (!isAdded) {
            previous->next = newItem;
            isAdded = 1;
        }
    }
    adjacentVertices->quantity++;
}

void processFile(char *fileName) {
    printf("\nFILENAME: %s", fileName);
    FILE *file = fopen(fileName, "r");
    printf("\nFile opened");

    printf("\nStarting allocating vertices");
    for (int i = 0; i < MAX_INTERACTIONS + 1; i++) {
        addedVertices[i] = 0;
    }
    printf("\nFinished allocating vertices");

    if (file == NULL) {
        printf("\nFile not found");
        return;
    }

    printf("\nStarting file processing");
    int scenarios;
    fscanf(file, "%d", &scenarios);
    printf("\nscenarios: %d \n\n", scenarios);

    int shouldContinueDueToOverflow = 0;
    for (int i = 0; i < scenarios; i++) {
        if (shouldContinueDueToOverflow == 1) {
            continue;
        }
        int numberOfBugs, numberOfInteractions, quantity = 0;
        fscanf(file, "%d %d", &numberOfBugs, &numberOfInteractions);

        printf("\nScenario #%d", i);
        vertex *vertices = newVertices();
        if (numberOfInteractions > MAX_INTERACTIONS) {
            printf("\n----> Max number of interactions reached");
            shouldContinueDueToOverflow = 1;
            numberOfInteractions = MAX_INTERACTIONS;
        }

        for (int j = 0; j < numberOfInteractions; j++) {
            if (quantity >= MAX_BUGS) {
                printf("\n----> Max number of vertices reached");
                continue;
            }

            int vertexA, vertexB;
            fscanf(file, "%d %d", &vertexA, &vertexB);

            if (addedVertices[vertexA] == 0) {
                quantity++;
            }
            addedVertices[vertexA] = 1;

            if (addedVertices[vertexB] == 0) {
                quantity++;
            }
            addedVertices[vertexB] = 1;

            pushVertexToList(&vertices[vertexA], vertexB);
            pushVertexToList(&vertices[vertexB], vertexA);
        }
        int result = 1;
        for (int k = 0; k < quantity; k++) {
            result = dfs(vertices, k);
            if (result == 0) {
                printf("\nSuspicious bugs found!");
                break;
            }
        }
        if (result == 1) {
            printf("\nNo suspicious bugs found!");
        }
    }
}


// Returns 0 only if was visited and has the same bugType as the previously executed vertex (I.E is sus)
int dfs(vertex *vertices, int vertexIndex) {

    if (vertices[vertexIndex].isVisited == 1 && vertices[vertexIndex].bugType == previousBugType) {
        return 0;
    }

    if (vertices[vertexIndex].isVisited == 1) {
        return 1;
    }

    previousBugType = vertices[vertexIndex].bugType;
    vertices[vertexIndex].isVisited = 1;
    if (vertices[vertexIndex].adjacentVertices == NULL) {
        return 1;
    }

    listItem *adjacentVertices = vertices[vertexIndex].adjacentVertices->beginning;
    while (adjacentVertices != NULL) {
        int childVertexIndex = adjacentVertices->value;
        if (vertices[childVertexIndex].isVisited == 0) {
            dfs(vertices, childVertexIndex);
        }
        adjacentVertices = adjacentVertices->next;
    }
    return 1;
}