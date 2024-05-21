#include "../include/garraylist.h"
#include <stdio.h>
#include <string.h>

void _allocate_s(gdata_t *node, gdata_t data) {
    *node = (gdata_t)strdup(data);
}

int main() {
    alist_t *list = create_alist(sizeof(char *), _allocate_s);
    printf("%ld - %ld \n", alist_size(list), alist_capacity(list));
    alist_reserve(list, 5);
    alist_set_at(list, 0, "hello");
    alist_set_at(list, 1, "world");
    alist_set_at(list, 2, "!");
    gdata_t temp = NULL;
    for (int i = 0; i < 5; i++) {
        temp = alist_at(list, i);
        if (temp != NULL)
            printf("%s\n", (char *)temp);
    }
    alist_rm_at(list, 2);
    printf("%ld - %ld \n", alist_size(list), alist_capacity(list));

    destroy_alist(&list);
    return 0;
}
