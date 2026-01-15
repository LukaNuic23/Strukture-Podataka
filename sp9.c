#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Node {
    int value;
    struct _Node* left;
    struct _Node* right;
} Node;

static Node* createNode(int x);
static Node* insert(Node* root, int x);

static int replace(Node* root);

static int inorderToFile(Node* root, FILE* f);
static int freeTree(Node* root);


int main(void) {
    Node* root = NULL;
    FILE* f;

    srand((unsigned)time(NULL));

    int n = 10;
    int x;

    /*generiranje slucajnih brojeva */
    for (int i = 0; i < n; i++) {
        x = 10 + rand() % 81;   
        root = insert(root, x);
    }

    f = fopen("inorder_out.txt", "w");
    if (!f) return 1;

    fprintf(f, "Inorder nakon insert:\n");
    inorderToFile(root, f);
    fprintf(f, "\n");

    replace(root);

    fprintf(f, "Inorder nakon replace:\n");
    inorderToFile(root, f);
    fprintf(f, "\n");

    fclose(f);
    freeTree(root);

    return 0;
}

static Node* createNode(int x) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->value = x;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/*insert */
static Node* insert(Node* root, int x) {
    if (!root) return createNode(x);

    if (x >= root->value)
        root->left = insert(root->left, x);
    else
        root->right = insert(root->right, x);

    return root;
}

/*replace */
static int replace(Node* root) {
    if (!root) return 0;

    int left = replace(root->left);
    int right = replace(root->right);

    int orig = root->value;
    root->value = left + right;

    return orig + left + right;
}

/* inorder u datoteku */
static int inorderToFile(Node* root, FILE* f) {
    if (!root) return 0;

    inorderToFile(root->left, f);
    fprintf(f, "%d ", root->value);
    inorderToFile(root->right, f);

    return 0;
}

/* brisanje stabla */
static int freeTree(Node* root) {
    if (!root) return 0;

    freeTree(root->left);
    freeTree(root->right);
    free(root);

    return 0;
}

