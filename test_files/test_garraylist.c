#include "../include/garraylist.h"
#include <stdio.h>

int main() {
    {
        alist_t *strings = create_alist(30);
        printf("%ld - %ld \n", alist_size(strings), alist_capacity(strings));
        alist_reserve(strings, 5);
        alist_push(strings, "hello");
        alist_push(strings, "world");
        for (size_t i = 0; i < alist_size(strings); i++) {
            printf("%s\n", (char *)alist_at(strings, i));
        }
        alist_rm_at(strings, 2);
        printf("%ld - %ld \n", alist_size(strings), alist_capacity(strings));
        destroy_alist(&strings);
    }
    {
        alist_t *intlist = create_alist(sizeof(int));
        {
            int temp = 0;
            alist_push(intlist, &temp);
            temp = 2;
            alist_push(intlist, &temp);
            temp = 3;
            alist_push(intlist, &temp);
        }
        for (int i = 0; i < 5; i++) {
            gdata_t temp = alist_at(intlist, i);
            if (temp != NULL)
                printf("%d\n", *(int *)temp);
        }
        destroy_alist(&intlist);
    }

    return 0;
}
