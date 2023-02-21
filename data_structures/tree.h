typedef struct treenode
{
    char rep;
    struct treenode *child;
    struct treenode *nextSibling;
} treenode;

treenode *initTree();