#include "../include/garraylist.h"
#include <stdio.h>
#include <string.h>
#define NEW_LINE printf("\n");

typedef struct {
    size_t x, y;
} point;

point *alist_pt_at(alist_t *list, size_t pos) {
    return alist_at(list, pos);
}

point create_point(int x, int y) {
    point t = {x, y};
    return t;
}

int16_t alist_push_i(alist_t *list, int i) {
    return alist_push(list, &i);
}

#define fnw point (**)(int, int)

void functoins_p_list() {
    point (**a)(int, int) = malloc(sizeof(point(*)(int, int)));
    *a = create_point;

    alist_t *funws = alist_create(sizeof(fnw));

    alist_push(funws, a);
    alist_push(funws, a);

    printf("__ funcs list __\n");
    printf("size: %ld - capacity: %ld item_size: %ld \n", alist_size(funws), alist_capacity(funws),
           funws->item_size);

    // convert to fpwrapper pointer then derefrence it then use it
    point p = (*(fnw)alist_at(funws, 0))(3, 4);
    printf("%ld - %ld\n", p.x, p.y);
    p = (*(fnw)alist_at(funws, 1))(5, 3);
    printf("%ld - %ld\n", p.x, p.y);

    alist_destroy(funws);
    free(a);
    free(funws);
}

void integers_list() {
    alist_t *integers = alist_create(sizeof(int));
    alist_push_i(integers, 0x39939800);
    alist_push_i(integers, 4);
    alist_push_i(integers, 5);

    for (size_t i = 0; i < alist_size(integers); i++)
        printf("%d - ", *(int *)alist_at(integers, i));
    printf("\n");

    alist_destroy(integers);
    free(integers);
}

void points_list() {
    NEW_LINE;
    printf("__ points list __\n");

    alist_t *points = alist_create(sizeof(point));
    alist_t *p_refs = alist_create(sizeof(point *));

    point  p = create_point(8, 4);
    point *a = malloc(sizeof(point));
    a->x = 9;
    a->y = 5;
    point *b = NULL;

    // save point 'p'
    alist_push(points, &p);
    // save the point refrenced by a
    alist_push(points, a);
    // save the 'a' pointer
    alist_push(p_refs, &a);

    p = *alist_pt_at(points, 0);
    printf("%ld - %ld\n", p.x, p.y);

    p = *alist_pt_at(points, 1);
    printf("%ld - %ld\n", p.x, p.y);

    b = alist_pt_at(points, 0);

    // a = *(point **)alist_at(p_refs, 0);
    printf("%ld - %ld\n", b->x, b->y);
    // printf("%ld \n", (size_t)a);
    free(a);

    alist_destroy(points);
    free(points);
    alist_destroy(p_refs);
    free(p_refs);
}

void dyn_strings() {
    NEW_LINE;
    printf("__ dynamic size strings list __\n");
    alist_t *dyn_strings_sizes = alist_create(4);
    alist_set_allocator(dyn_strings_sizes, str_allocator);

    printf("size: %ld - capacity: %ld \n", alist_size(dyn_strings_sizes),
           alist_capacity(dyn_strings_sizes));
    alist_reserve(dyn_strings_sizes, 5);
    printf("size: %ld - capacity: %ld \n", alist_size(dyn_strings_sizes),
           alist_capacity(dyn_strings_sizes));
    alist_push(dyn_strings_sizes, "hello");
    alist_push(dyn_strings_sizes, "world");
    alist_push(dyn_strings_sizes, "hello");
    alist_push(dyn_strings_sizes, "world");

    char *fs = "f string";
    char *ds = strdup("d string");
    alist_push(dyn_strings_sizes, fs);
    alist_push(dyn_strings_sizes, ds);

    for (size_t i = 0; i < alist_size(dyn_strings_sizes); i++) {
        char *s = alist_at(dyn_strings_sizes, i);
        if (s) printf("%ld - %s\n", i, s);
    }
    alist_rm_at(dyn_strings_sizes, 1);

    printf("__ after remove __\n");
    alist_set_at(dyn_strings_sizes, 1, "new hello");
    for (size_t i = 0; i < alist_size(dyn_strings_sizes); i++) {
        char *s = alist_at(dyn_strings_sizes, i);
        if (s) printf("%ld - %s\n", i, s);
    }

    printf("size: %ld - capacity: %ld \n", alist_size(dyn_strings_sizes),
           alist_capacity(dyn_strings_sizes));
    alist_destroy(dyn_strings_sizes);
    free(dyn_strings_sizes);
    free(ds);
}

void fixed_strings() {
    NEW_LINE;
    printf("__ fixed size strings list __\n");
    // each list element has 30 bytes
    alist_t *strings = alist_create(30);
    printf("size %ld - capacity %ld \n", alist_size(strings), alist_capacity(strings));
    char *ds = strdup("dynamic string");
    char *fs = "fixed string";

    alist_reserve(strings, 5);
    alist_push_safe(strings, strlen("hello"), "hello");
    alist_push_safe(strings, strlen("world"), "world");
    alist_push_safe(strings, strlen(fs), fs);
    alist_push_safe(strings, strlen(ds), ds);

    for (size_t i = 0; i < alist_size(strings); i++) {
        printf("%s\n", (char *)alist_at(strings, i));
    }

    alist_rm_at(strings, 1);
    for (size_t i = 0; i < alist_size(strings); i++) {
        printf("%s\n", (char *)alist_at(strings, i));
    }

    printf("size %ld - capacity %ld \n", alist_size(strings), alist_capacity(strings));
    alist_destroy(strings);
    free(strings);
    free(ds);
}

int main() {

    functoins_p_list();
    points_list();
    fixed_strings();
    dyn_strings();
    integers_list();

    return 0;
}
