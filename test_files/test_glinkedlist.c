#include "../include/glinkedlist.h"
#include <stdio.h>
#include <string.h>

void _print_str(void *data);
void _print_int(void *data);

static inline void push_front_i(llist_t *x, int y) {
    int i = y;
    push_front(x, &i);
}

static inline void push_front_s(llist_t *x, const void *s) {
    push_front(x, (char *)s);
}

static inline void push_back_s(llist_t *x, const void *s) {
    push_back(x, (char *)s);
}

static inline void dump_str_list(llist_t *x) {
    dump_list(x, _print_str);
    printf("\n");
}

static inline void dump_int_list(llist_t *x) {
    dump_list(x, _print_int);
    printf("\n");
}

int main() {

    llist_t *lst      = create_list(sizeof(char *));
    llist_t *new_list = create_list(sizeof(int));

    push_front_i(new_list, 3);
    push_front_i(new_list, 2);
    push_front_i(new_list, 7);
    dump_int_list(new_list);
    destroy_list(&new_list);

    for (int i = 0; i < 7; i++) {
        push_front_s(lst, "world");
        push_front_s(lst, "hello");
        push_front_s(lst, "print");
        push_back_s(lst, "!");
    }

    dump_str_list(lst);

    list_iterator_t *fast_it = create_list_iterator(lst);
    list_iterator_t *slow_it = create_list_iterator(lst);

    node_t *fast = itr_begin(fast_it);
    node_t *slow = itr_begin(slow_it);

    {
        int length = 1, h_index = 0;
        while (fast != itr_end(fast_it)) {
            for (int i = 0; i < 2 && fast != itr_end(fast_it); i++) {
                length++;
                fast = next(fast_it);
            }
            slow = next(slow_it);
            h_index++;
            _print_str(node_data(slow));
        }
        printf("\n");
        printf("length = %d \n", length);
        printf("length / 2 = %d \n", h_index);
    }

    free(fast_it);
    free(slow_it);

    destroy_list(&lst);

    return 0;
}

void _print_str(void *data) {
    if (data == NULL)
        return;
    printf("%s -> ", (char *)data);
}

void _print_int(void *data) {
    if (data == NULL)
        return;
    printf("%d", *(int *)data);
}
