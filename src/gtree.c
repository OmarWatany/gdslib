#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tnode Tnode;

struct Tnode {
    void  *value;
    Tnode *left, *right;
};

typedef struct {
    Tnode *root;
    void (*allocate_data)(void *data, Tnode **node);
    void (*display_data)(void *data, Tnode **node);
} Tree;

void tnode_in_traverse(Tnode **root, void (*do_something)(Tnode **root));
void tnode_post_traverse(Tnode **root, void (*do_something)(Tnode **root));
void tnode_pre_traverse(Tnode **root, void (*do_something)(Tnode **root));
void tnode_bst_add(Tnode **root, void *data, void (*allocate_data)(void *data, Tnode **root));
void tnode_in_add(Tnode **root, void *data, void (*allocate_data)(void *data, Tnode **root));
void tnode_post_add(Tnode **root, void *data, void (*allocate_data)(void *data, Tnode **root));
void tnode_pre_add(Tnode **root, void *data, void (*allocate_data)(void *data, Tnode **root));

Tree *tr_create_tree(void (*allocate_data)(void *data, Tnode **node));
void  tr_set_display_fun(Tree *tree, void (*display_data)(Tree **root));
void  tr_set_allocator(Tree *tree, void (*allocate_data)(Tree **root));
void  tr_in_traverse(Tree *tree);
void  tr_post_traverse(Tree *tree);
void  tr_pre_traverse(Tree *tree);
void  tr_bst_add(Tree *tree, void *data);
void  tr_in_add(Tree *tree, void *data);
void  tr_post_add(Tree *tree, void *data);
void  tr_pre_add(Tree *tree, void *data);
void  tr_destroy_tree(Tree **tree);

void tr_display_str_node(Tnode **root) {
    printf("%s - ", (char *)(*root)->value);
}

void tr_display_int_node(Tnode **root) {
    printf("%d - ", *(int *)(*root)->value);
}

void allocate_str_data(void *data, Tnode **node) {
    Tnode *temp = malloc(sizeof(Tnode));
    temp->value = (char *)data;
    *node       = temp;
}

int main() {
    int   arr[] = {8, 5, 7, 10, 9, 3, 10, 5, 2, 11, 4};
    int  *data  = arr;
    Tree *tree  = tr_create_tree(allocate_str_data);
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        tr_bst_add(tree, data);
    // tnode_bst_add(&tree->root, data);

    tnode_pre_traverse(&tree->root, tr_display_str_node);
    printf("pre \n");
    tnode_post_traverse(&tree->root, tr_display_str_node);
    printf("post \n");
    tnode_in_traverse(&tree->root, tr_display_str_node);
    printf("in \n");

    tr_destroy_tree(&tree);
    return 0;
}

Tree *tr_create_tree(void (*allocate_data)(void *data, Tnode **node)) {
    Tree *temp = malloc(sizeof(Tree));
    memset(temp, 0, sizeof(Tree));
    temp->allocate_data = allocate_data;
    return temp;
}

void tnode_bst_add(Tnode **root, int value) {
    if (*root == NULL) {
        Tnode *temp = malloc(sizeof(Tnode));
        memset(temp, 0, sizeof(Tnode));
        temp->value = value;
        *root       = temp;
    }
    if (value < (*root)->value) {
        tnode_bst_add(&(*root)->left, value);
    } else if (value > (*root)->value)
        tnode_bst_add(&(*root)->right, value);
}

void tnode_in_traverse(Tnode **root, void (*do_something)(Tnode **root)) {
    if (*root == NULL)
        return;
    tnode_in_traverse(&(*root)->left, do_something);
    do_something(root);
    tnode_in_traverse(&(*root)->right, do_something);
}

void tnode_post_traverse(Tnode **root, void (*do_something)(Tnode **root)) {
    if (*root == NULL)
        return;
    tnode_post_traverse(&(*root)->left, do_something);
    tnode_post_traverse(&(*root)->right, do_something);
    do_something(root);
}

void tnode_pre_traverse(Tnode **root, void (*do_something)(Tnode **root)) {
    if (*root == NULL)
        return;
    do_something(root);
    tnode_pre_traverse(&(*root)->left, do_something);
    tnode_pre_traverse(&(*root)->right, do_something);
}

void tnode_in_add(Tnode **root, int value) {
    /*
        if (*root == NULL) {
            tnode *temp = malloc(sizeof(tnode));
            memset(temp, 0, sizeof(tnode));
            temp->value = value;
            *root       = temp;
        }
        if (value < (*root)->value) {
            tnode_bst_add(&(*root)->left, value);
        } else if (value > (*root)->value)
            tnode_bst_add(&(*root)->right, value);
    */
}

void tnode_post_add(Tnode **root, int value) {
}

void tnode_pre_add(Tnode **root, int value) {
}

void tnode_free_node(Tnode **root) {
    free(*root);
}
