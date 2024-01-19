#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

int compulsoryMisses = 0;
int capacityMisses = 0;

char *allItems[9999];
int itemCount = 0;

bool WasCached(const char *item) {
    for (int i = 0; i < itemCount; i++) {
        if (strcmp(allItems[i], item) == 0) {
            return true;
        }
    }
    return false;
}

void AllChar(const char *item) {
    if (itemCount < 9999) {
        allItems[itemCount++] = strdup(item);
    }
}

typedef struct Node {
    char *data;
    struct Node *next;
    struct Node *prev;
    int value;
    int reference;
} Node;

typedef struct DoublyLinkedList {
    Node *head;
    Node *tail;
    Node *current;
    int pos;
    int size;
    int maxSize;
} DoublyLinkedList;

void initList(DoublyLinkedList *list, int maxSize) {
    list->head = NULL;
    list->tail = NULL;
    list->pos = 0;
    list->size = 0;
    list->maxSize = maxSize;
}

void freeList(DoublyLinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int isFull(DoublyLinkedList *list) {
    return list->size == list->maxSize;
}

int isEmpty(DoublyLinkedList *list) {
    return list->size == 0;
}

void addNodeFIFO(DoublyLinkedList *list, const char *data) {
    if (isFull(list)) {
        Node *oldHead = list->head;
        list->head = oldHead->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        }
        free(oldHead->data);
        free(oldHead);
        list->size--;
    }

    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->data = strdup(data);
    newNode->next = NULL;
    newNode->prev = list->tail;
    newNode->value = 0;

    if (isEmpty(list)) {
        list->head = newNode;
    } else {
        list->tail->next = newNode;
    }

    list->tail = newNode;
    list->size++;
}

void removeNode(DoublyLinkedList *list, Node *node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    free(node->data);
    free(node);
    list->size--;
}

void addNodeclock(DoublyLinkedList *list, const char *data) {
    if (isFull(list)) {
        if (list->current == NULL) {
            list->current = list->head;
        }

        while (list->current->reference != 0) {
            list->current->reference = 0;
            list->current = list->current->next ? list->current->next : list->head;
        }

        Node *nodeToEvict = list->current;
        list->current = list->current->next ? list->current->next : list->head;
        removeNode(list, nodeToEvict);
    }
    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->data = strdup(data);
    newNode->next = NULL;
    newNode->prev = list->tail;
    newNode->reference = 0;

    if (isEmpty(list)) {
        list->head = newNode;
        list->current = newNode;
    } else {
        list->tail->next = newNode;
    }
    list->tail = newNode;
    list->size++;
}

void addNodeLRU(DoublyLinkedList *list, const char *data) {
    if (isFull(list)) {
        Node *current = list->head;
        Node *lruNode = current;
        int minVal = current->value;
        while (current != NULL) {
            if (current->value < minVal) {
                minVal = current->value;
                lruNode = current;
            }
            current = current->next;
        }
        removeNode(list, lruNode);
    }

    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->data = strdup(data);
    newNode->next = NULL;
    newNode->prev = list->tail;
    newNode->value = list->pos++;

    if (isEmpty(list)) {
        list->head = newNode;
    } else {
        list->tail->next = newNode;
    }

    list->tail = newNode;
    list->size++;
}

bool findItem(DoublyLinkedList *list, const char *item) {
    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->data, item) == 0) {
            current->value = list->pos++;
            current->reference = 1;
            return true;
        }
        current = current->next;
    }
    return false;
}

void printList(const DoublyLinkedList *list) {
    Node *current = list->head;
    printf("[");

    while (current != NULL) {
        char *data = current->data;
        int reference = current->reference;
        if (data != NULL) {
            size_t len = strlen(data);
            if (len > 0 && data[len - 1] == '\n') {
                data[len - 1] = '\0';
            }
            printf("%s %d", data, reference);
            if (len > 0 && data[len - 1] == '\0') {
                data[len - 1] = '\n';
            }
        }

        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }

    printf("]\n");
}

void FIFO(int size) {
    // printf("inside FIFO\n");
    DoublyLinkedList FIFO;
    initList(&FIFO, size);
    char input[100];

    // printf("Enter items (EOF to stop):\n");
    while (fgets(input, 100, stdin) != NULL) {
        // printf("Received: %s", input);
        if (findItem(&FIFO, input)) {
            printf("HIT\n");
        } else {
            if (isFull(&FIFO)) {
                if (WasCached(input)) {
                    capacityMisses++;
                } else {
                    compulsoryMisses++;
                    AllChar(input);
                }
            } else {
                if (!WasCached(input)) {
                    AllChar(input);
                    compulsoryMisses++;
                }
            }
            addNodeFIFO(&FIFO, input);
            printf("MISS\n");
        }
        // printList(&FIFO);
    }
    freeList(&FIFO);
    // printf("End of input.\n");
}

void LRU(int size) {
    // printf("inside LRU\n");
    DoublyLinkedList LRU;
    initList(&LRU, size);
    char input[100];

    // printf("Enter items (EOF to stop):\n");
    while (fgets(input, 100, stdin) != NULL) {
        // printf("Received: %s", input);
        if (findItem(&LRU, input)) {
            printf("HIT\n");
        } else {
            if (isFull(&LRU)) {
                if (WasCached(input)) {
                    capacityMisses++;
                } else {
                    compulsoryMisses++;
                    AllChar(input);
                }
            } else {
                if (!WasCached(input)) {
                    AllChar(input);
                    compulsoryMisses++;
                }
            }
            addNodeLRU(&LRU, input);
            printf("MISS\n");
        }
        // printList(&LRU);
    }
    freeList(&LRU);
    // printf("End of input.\n");
}

void clock(int size) {
    // printf("inside clock\n");
    DoublyLinkedList clock;
    initList(&clock, size);
    char input[100];

    // printf("Enter items (EOF to stop):\n");
    while (fgets(input, 100, stdin) != NULL) {
        // printf("Received: %s", input);
        if (findItem(&clock, input)) {
            printf("HIT\n");
        } else {
            if (isFull(&clock)) {
                if (WasCached(input)) {
                    capacityMisses++;
                } else {
                    compulsoryMisses++;
                    AllChar(input);
                }
            } else {
                if (!WasCached(input)) {
                    AllChar(input);
                    compulsoryMisses++;
                }
            }
            addNodeclock(&clock, input);
            printf("MISS\n");
        }
        // printList(&clock);
    }
    freeList(&clock);
    // printf("End of input.\n");
}

int main(int argc, char *argv[]) {
    int size = 0;
    char *policy = NULL;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: ./cacher -N size [policy]\n");
        return 1;
    }

    if (strcmp(argv[1], "-N") != 0) {
        fprintf(stderr, "Usage: ./cacher -N size [policy]\n");
        return 1;
    }

    size = atoi(argv[2]);
    if (size <= 0) {
        fprintf(stderr, "Error: Invalid size provided\n");
        return 1;
    }

    if (argc == 4) {
        policy = argv[3];
    }

    // printf("Size: %d\n", size);
    // if (policy != NULL) {
    //     printf("Policy: %s\n", policy);
    // } else {
    //     printf("No policy provided\n");
    // }

    if (policy == NULL || strcmp(policy, "-F") == 0) {
        // printf("starting FIFO\n");
        FIFO(size);
        printf("%d %d\n", compulsoryMisses, capacityMisses);
    } else if (strcmp(policy, "-L") == 0) {
        // printf("starting LRU\n");
        LRU(size);
        printf("%d %d\n", compulsoryMisses, capacityMisses);
    } else if (strcmp(policy, "-C") == 0) {
        // printf("starting C\n");
        clock(size);
        printf("%d %d\n", compulsoryMisses, capacityMisses);
    } else {
        printf("Policy incorrect\n");
        return 1;
    }

    for (int i = 0; i < itemCount; i++) {
        free(allItems[i]);
    }

    return 0;
}
