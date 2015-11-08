/*
 * File:	treesort.c
 *
 * Description:	Read a sequence of integers from the standard input,
 *		inserting each into a binary search tree, and print them
 *		out in sorted order using an inorder traversal.
 */

# include <stdio.h>
# include <stdlib.h>
# include "tree.h"


/*
 * Function:	inorder
 *
 * Description:	Perform an inorder traversal of the subtree pointed to by
 *		ROOT, writing the data of each node to the standard output.
 */

static void inorder(struct tree *root)
{
    if (root != NULL) {
	inorder(getLeft(root));
	printf("%d\n", getData(root));
	inorder(getRight(root));
    }
}


/*
 * Function:	insert
 *
 * Description:	Insert X into the binary search tree rooted at ROOT by
 *		creating a new leaf node to hold and item.  We return the
 *		possibly new root of the tree.
 */

static struct tree *insert(struct tree *root, int x)
{
    if (root == NULL)
	root = createTree(x, NULL, NULL);
    else if (x < getData(root))
	setLeft(root, insert(getLeft(root), x));
    else
	setRight(root, insert(getRight(root), x));

    return root;
}


/*
 * Function:	main
 *
 * Description:	Driver function for the treesort application.
 */

int main(void)
{
    int x;
    struct tree *root = NULL;


    while (scanf("%d", &x) == 1)
	root = insert(root, x);

    inorder(root);
    destroyTree(root);
    exit(EXIT_SUCCESS);
}
