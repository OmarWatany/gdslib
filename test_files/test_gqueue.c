#include "../include/gqueue.h"
#include <stdio.h>
#include <string.h>

void __allocate_int(node_t *node, void *data);
void __print_int(void *data);
void __allocate_s(node_t *node, void *data);
void __print_s(void *data);

bool queue_search_int(node_t *node, void *data) {
    return *(int *)node_data(node) == *(int *)data;
}

bool q_search_s(node_t *node, void *data) {
    return strcmp(data, node_data(node));
}

int main() {
    queue_t *cards_deck      = create_queue(sizeof(int));
    queue_t *discarded_cards = create_queue(sizeof(int));
    queue_t *names           = create_queue(sizeof(char *));
    enqueue(names, "hello");
    enqueue(names, "world");
    enqueue(names, "!");
    if (in_queue(names, "!", q_search_s))
        dump_queue(names, __print_s);
    dequeue(names);
    dump_queue(names, __print_s);

    int deck_size = 0;
    printf("deck size : ");
    scanf("%d", &deck_size);
    for (int i = 1; i <= deck_size; i++)
        enqueue(cards_deck, &i);

    printf("card deck: ");
    dump_queue(cards_deck, __print_int);

    {
        int s = 8;
        int r = in_queue(cards_deck, &s, queue_search_int);
        printf("is %d there ? : ", s);
        r ? printf("yes\n") : printf("no\n");
    }

    while (queue_length(cards_deck) > 1) {
        int i = *(int *)queue_front(cards_deck);
        enqueue(discarded_cards, &i);
        dequeue(cards_deck);
        i = *(int *)queue_front(cards_deck);
        enqueue(cards_deck, &i);
        dequeue(cards_deck);
    }

    printf("discarded cards : ");
    dump_queue(discarded_cards, __print_int);

    printf("remaining card : ");
    dump_queue(cards_deck, __print_int);

    destroy_queue(&cards_deck);
    destroy_queue(&discarded_cards);
    destroy_queue(&names);
    return 0;
}

void __allocate_int(node_t *node, void *data) {
    int *temp = (int *)malloc(sizeof(int));
    *temp     = *((int *)data);
    node_set_data(node, temp);
}

void __print_int(void *data) {
    printf("%d ,", *(int *)data);
}

void __allocate_s(node_t *node, void *data) {
    void *temp = strdup(data);
    node_set_data(node, temp);
}

void __print_s(void *data) {
    printf("%s ,", (char *)data);
}
