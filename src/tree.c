#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tnode tnode;

struct tnode {
    int    value;
    tnode *left, *right;
};

typedef struct {
    tnode *root;
} iTree;

iTree *tr_create_tree() {
    iTree *temp = malloc(sizeof(iTree));
    memset(temp, 0, sizeof(iTree));
    return temp;
}

void tr_bst_add(tnode **root, int value);
void tr_in_traverse(tnode **root, void (*do_something)(tnode **root));
void tr_post_traverse(tnode **root, void (*do_something)(tnode **root));
void tr_pre_traverse(tnode **root, void (*do_something)(tnode **root));
void tr_in_add(tnode **root, int value);
void tr_post_add(tnode **root, int value);
void tr_pre_add(tnode **root, int value);
void tr_destroy_tree(iTree **tree);

void tr_display_node(tnode **root) {
    printf("%d - ", (*root)->value);
}

int main() {
    int    arr[] = {8, 5, 7, 10, 9, 3, 10, 5, 2, 11, 4};
    iTree *tree = tr_create_tree();
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        tr_bst_add(&tree->root, arr[i]);

    tr_pre_traverse(&tree->root, tr_display_node);
    printf("pre \n");
    tr_post_traverse(&tree->root, tr_display_node);
    printf("post \n");
    tr_in_traverse(&tree->root, tr_display_node);
    printf("in \n");

    tr_destroy_tree(&tree);
    return 0;
}

void tr_bst_add(tnode **root, int value) {
    if (*root == NULL) {
        tnode *temp = malloc(sizeof(tnode));
        memset(temp, 0, sizeof(tnode));
        temp->value = value;
        *root = temp;
    }
    if (value < (*root)->value) {
        tr_bst_add(&(*root)->left, value);
    } else if (value > (*root)->value)
        tr_bst_add(&(*root)->right, value);
}

void tr_in_traverse(tnode **root, void (*do_something)(tnode **root)) {
    if (*root == NULL) return;
    tr_in_traverse(&(*root)->left, do_something);
    do_something(root);
    tr_in_traverse(&(*root)->right, do_something);
}

void tr_post_traverse(tnode **root, void (*do_something)(tnode **root)) {
    if (*root == NULL) return;
    tr_post_traverse(&(*root)->left, do_something);
    tr_post_traverse(&(*root)->right, do_something);
    do_something(root);
}

void tr_pre_traverse(tnode **root, void (*do_something)(tnode **root)) {
    if (*root == NULL) return;
    do_something(root);
    tr_pre_traverse(&(*root)->left, do_something);
    tr_pre_traverse(&(*root)->right, do_something);
}

void tr_in_add(tnode **root, int value) {
    /*
        if (*root == NULL) {
            tnode *temp = malloc(sizeof(tnode));
            memset(temp, 0, sizeof(tnode));
            temp->value = value;
            *root       = temp;
        }
        if (value < (*root)->value) {
            tr_bst_add(&(*root)->left, value);
        } else if (value > (*root)->value)
            tr_bst_add(&(*root)->right, value);
    */
}

void tr_post_add(tnode **root, int value) {
}

void tr_pre_add(tnode **root, int value) {
}

void tr_free_node(tnode **root) {
    free(*root);
}

void tr_destroy_tree(iTree **tree) {
    tr_post_traverse(&(*tree)->root, tr_free_node);
    free(*tree);
    *tree = NULL;
}
