#include "../include/garraylist.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    size_t x, y;
} point;

point *alist_pt_at(alist_t *list, size_t pos) {
    return alist_at(list, pos);
}

gdata_t string_allocator(gdata_t str) {
    return strdup((char *)str);
}

point create_point(int x, int y) {
    point t = {x, y};
    return t;
}

void points_list() {
    printf("__ pointers list list __\n");
    alist_t *p_refs = create_alist(sizeof(point *));
    alist_t *points = create_alist(sizeof(point));
    point    p = create_point(8, 4);
    point   *a = malloc(sizeof(point));

    a->x = 9;
    a->y = 5;

    alist_push(points, &p);
    alist_push(p_refs, &a);

    p = *alist_pt_at(points, 0);
    printf("%ld - %ld\n", p.x, p.y);

    p = **(point **)alist_at(p_refs, 0);
    printf("%ld - %ld\n", p.x, p.y);

    free(a);
    destroy_alist(&points);
    destroy_alist(&p_refs);
}

void dyn_strings() {
    printf("__ dynamic size strings list __\n");
    alist_t *dyn_strings_sizes = create_alist(4);
    alist_set_allocator(dyn_strings_sizes, string_allocator);

    printf("%ld - %ld \n", alist_size(dyn_strings_sizes), alist_capacity(dyn_strings_sizes));
    alist_reserve(dyn_strings_sizes, 5);
    alist_push(dyn_strings_sizes, "hello");
    alist_push(dyn_strings_sizes, "world");

    for (size_t i = 0; i < alist_size(dyn_strings_sizes); i++) {
        printf("%s\n", (char *)alist_at(dyn_strings_sizes, i));
    }
    alist_rm_at(dyn_strings_sizes, 1);

    for (size_t i = 0; i < alist_size(dyn_strings_sizes); i++) {
        printf("%s\n", (char *)alist_at(dyn_strings_sizes, i));
    }
    printf("%ld - %ld \n", alist_size(dyn_strings_sizes), alist_capacity(dyn_strings_sizes));
    destroy_alist(&dyn_strings_sizes);
}

void fixed_strings() {
    printf("__ fixed size strings list __\n");
    // each list element has 30 bytes
    alist_t *strings = create_alist(30);
    printf("%ld - %ld \n", alist_size(strings), alist_capacity(strings));

    alist_reserve(strings, 5);
    alist_push(strings, "hello");
    alist_push(strings, "world");

    for (size_t i = 0; i < alist_size(strings); i++) {
        printf("%s\n", (char *)alist_at(strings, i));
    }

    alist_rm_at(strings, 1);
    for (size_t i = 0; i < alist_size(strings); i++) {
        printf("%s\n", (char *)alist_at(strings, i));
    }

    printf("%ld - %ld \n", alist_size(strings), alist_capacity(strings));
    destroy_alist(&strings);
}

void int_list() {
    printf("__ int list __\n");
    alist_t *intlist = create_alist(sizeof(int));
    alist_reserve(intlist, 5);
    {
        int temp = 0;
        alist_push(intlist, &temp);
        temp = 2;
        alist_push(intlist, &temp);
        temp = 3;
        alist_push(intlist, &temp);
    }
    for (size_t i = 0; i < alist_capacity(intlist); i++) {
        gdata_t temp = alist_at(intlist, i);
        if (temp)
            printf("%d\n", *(int *)temp);
    }
    destroy_alist(&intlist);
}

int main() {

    points_list();
    dyn_strings();

    return 0;
}
