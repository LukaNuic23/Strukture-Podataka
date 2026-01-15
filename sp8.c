#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int value;
    struct _Node* left;
    struct _Node* right;
} Node;

typedef struct _QNode {
    Node* treeNode;
    struct _QNode* next;
} QNode;

typedef struct _Queue {
    QNode* front;
    QNode* back;
} Queue;

static Node* createNode(int x);
static Node* insertNode(Node* root, int x);
static Node* findNode(Node* root, int x);
static Node* findMin(Node* root);

static Node* deleteNode(Node* root, int x);

static int inorder(Node* root);
static int preorder(Node* root);
static int postorder(Node* root);
static int levelOrder(Node* root);

static int freeTree(Node* root);


static int queueInit(Queue* q);
static int enqueue(Queue* q, Node* n);
static int dequeue(Queue* q, Node** out);
static int queueEmpty(Queue* q);
static int queueFree(Queue* q);


static int runZad8(void);


int main(void) {
    return runZad8();
}

static Node* createNode(int x) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->value = x;
    n->left = NULL;
    n->right = NULL;
    return n;
}

static Node* insertNode(Node* root, int x) {
    if (!root) return createNode(x);
    if (x < root->value) root->left = insertNode(root->left, x);
    else root->right = insertNode(root->right, x); /* duplikat ide desno */
    return root;
}

static Node* findNode(Node* root, int x) {
    if (!root) return NULL;
    if (x == root->value) return root;
    if (x < root->value) return findNode(root->left, x);
    return findNode(root->right, x);
}

static Node* findMin(Node* root) {
    if (!root) return NULL;
    while (root->left) root = root->left;
    return root;
}

static Node* deleteNode(Node* root, int x) {
    if (!root) return NULL;

    if (x < root->value) root->left = deleteNode(root->left, x);
    else if (x > root->value) root->right = deleteNode(root->right, x);
    else {
        /* nasli smo */
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        } else if (!root->left) {
            Node* tmp = root->right;
            free(root);
            return tmp;
        } else if (!root->right) {
            Node* tmp = root->left;
            free(root);
            return tmp;
        } else {
            Node* minRight = findMin(root->right);
            root->value = minRight->value;
            root->right = deleteNode(root->right, minRight->value);
        }
    }
    return root;
}

static int inorder(Node* root) {
    if (!root) return 0;
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
    return 0;
}

static int preorder(Node* root) {
    if (!root) return 0;
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
    return 0;
}

static int postorder(Node* root) {
    if (!root) return 0;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
    return 0;
}

/* Queue pomocno (bez void) */
static int queueInit(Queue* q) {
    if (!q) return -1;
    q->front = NULL;
    q->back = NULL;
    return 0;
}

static int enqueue(Queue* q, Node* n) {
    QNode* nn;
    if (!q) return -1;
    nn = (QNode*)malloc(sizeof(QNode));
    if (!nn) return -2;
    nn->treeNode = n;
    nn->next = NULL;
    if (!q->back) {
        q->front = nn;
        q->back = nn;
    } else {
        q->back->next = nn;
        q->back = nn;
    }
    return 0;
}

static int dequeue(Queue* q, Node** out) {
    QNode* tmp;
    if (!q || !out) return -1;
    if (!q->front) return -2;
    tmp = q->front;
    *out = tmp->treeNode;
    q->front = tmp->next;
    if (!q->front) q->back = NULL;
    free(tmp);
    return 0;
}

static int queueEmpty(Queue* q) {
    if (!q) return 1;
    return (q->front == NULL);
}

static int queueFree(Queue* q) {
    Node* dummy = NULL;
    if (!q) return -1;
    while (!queueEmpty(q)) dequeue(q, &dummy);
    return 0;
}

static int levelOrder(Node* root) {
    Queue q;
    Node* cur = NULL;

    if (!root) return 0;
    queueInit(&q);
    if (enqueue(&q, root) < 0) return -1;

    while (!queueEmpty(&q)) {
        if (dequeue(&q, &cur) < 0) break;
        printf("%d ", cur->value);
        if (cur->left) enqueue(&q, cur->left);
        if (cur->right) enqueue(&q, cur->right);
    }
    queueFree(&q);
    return 0;
}

static int freeTree(Node* root) {
    if (!root) return 0;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 0;
}

static int runZad8(void) {
    Node* root = NULL;
    int ch = 0, x = 0;
    Node* f = NULL;

    while (1) {
        printf("\n1-insert 2-find 3-delete 4-in 5-pre 6-post 7-level 8-exit\n> ");
        if (scanf("%d", &ch) != 1) return -1;

        if (ch == 8) break;

        if (ch == 1) {
            printf("Broj: ");
            if (scanf("%d", &x) != 1) return -2;
            root = insertNode(root, x);
            if (!root) return -3;
        } else if (ch == 2) {
            printf("Trazi: ");
            if (scanf("%d", &x) != 1) return -2;
            f = findNode(root, x);
            if (f) printf("Naden: %d\n", f->value);
            else printf("Nema ga\n");
        } else if (ch == 3) {
            printf("Brisi: ");
            if (scanf("%d", &x) != 1) return -2;
            root = deleteNode(root, x);
        } else if (ch == 4) {
            inorder(root); printf("\n");
        } else if (ch == 5) {
            preorder(root); printf("\n");
        } else if (ch == 6) {
            postorder(root); printf("\n");
        } else if (ch == 7) {
            levelOrder(root); printf("\n");
        }
    }

    freeTree(root);
    return 0;
}


