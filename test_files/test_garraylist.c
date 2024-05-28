#include "../include/garraylist.h"
#include <stdio.h>
#include <string.h>

int main() {
    alist_t *list    = create_alist(sizeof(char *));
    alist_t *intlist = create_alist(sizeof(int));
    printf("%ld - %ld \n", alist_size(list), alist_capacity(list));
    alist_reserve(list, 5);
    alist_set_at(list, 0, "hello");
    alist_set_at(list, 1, "world");
    alist_set_at(list, 2, "!");
    {
        int temp = 0;
        alist_push(intlist, &temp);
        temp = 2;
        alist_push(intlist, &temp);
        temp = 3;
        alist_push(intlist, &temp);
    }
    gdata_t temp = NULL;
    for (int i = 0; i < 5; i++) {
        temp = alist_at(list, i);
        if (temp != NULL)
            printf("%s\n", (char *)temp);
    }
    for (int i = 0; i < 5; i++) {
        temp = alist_at(intlist, i);
        if (temp != NULL)
            printf("%d\n", *(int *)temp);
    }
    alist_rm_at(list, 2);
    printf("%ld - %ld \n", alist_size(list), alist_capacity(list));

    destroy_alist(&list);
    return 0;
}
