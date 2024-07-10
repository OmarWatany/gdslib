#include "../include/gqueue.h"
#include <stdio.h>
#include <string.h>

void __allocate_int(lnode_t *node, void *data);
void __print_int(void *data);
void __allocate_s(lnode_t *node, void *data);
void __print_s(void *data);

bool queue_search_int(lnode_t *node, void *data) {
    return *(int *)lnode_data(node) == *(int *)data;
}

bool q_search_s(lnode_t *node, void *data) {
    return strcmp(data, lnode_data(node));
}

int main() {
    queue_t cards_deck = {0}, discarded_cards = {0}, names = {0};
    queue_init(&cards_deck, sizeof(int));
    queue_init(&discarded_cards, sizeof(int));
    queue_init(&names, 6);

    enqueue(&names, "hello");
    enqueue(&names, "world");
    // enqueue(names, "!");
    if (queue_find(&names, "!", q_search_s)) queue_dump(&names, __print_s);
    dequeue(&names);
    if (queue_find(&names, "!", q_search_s)) queue_dump(&names, __print_s);

    int deck_size = 1000000;
    printf("deck size : %d\n", deck_size);
    // scanf("%d", &deck_size);
    for (int i = 1; i <= deck_size; i++)
        enqueue(&cards_deck, &i);

    {
        int s = 8;
        int r = queue_find(&cards_deck, &s, queue_search_int);
        printf("is %d there ? : ", s);
        r ? printf("yes\n") : printf("no\n");
    }

    while (queue_length(&cards_deck) > 1) {
        int i = *(int *)queue_front(&cards_deck);
        enqueue(&discarded_cards, &i);
        dequeue(&cards_deck);
        i = *(int *)queue_front(&cards_deck);
        enqueue(&cards_deck, &i);
        dequeue(&cards_deck);
    }

    // printf("discarded cards : %ld\n", queue_length(&discarded_cards));
    printf("remaining card : ");
    printf("%d\n", *(int *)queue_front(&cards_deck));
    // dump_queue(&cards_deck, __print_int);

    queue_destroy(&names);
    queue_destroy(&cards_deck);
    queue_destroy(&discarded_cards);
    return 0;
}

void __print_int(void *data) {
    printf("%d ,", *(int *)data);
}

void __print_s(void *data) {
    printf("%s ,", (char *)data);
}
