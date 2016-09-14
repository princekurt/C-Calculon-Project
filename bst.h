/*
 *
 * bsh.h - public interface to bst.c, the binary search tree module
 *
 */

typedef struct bst {
    struct node *root;
    int size;
    
} bst;

extern bst *newBST(void);
extern value *findVariable(bst *workingBST, char *findNode);
extern void addBST(bst *workingBST, node *newNode);
extern void editBST(bst *workingBST, char * findString, value *newValue);