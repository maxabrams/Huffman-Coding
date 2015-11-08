/* File: tree.c
 *
 * Description: Project 5 written by Max Abrams (Atkinson Tuesday Lab). This tree is an implementation of a binary tree abstract data type. The binary tree incorperates parent points parent pointers as well as pointers to the two (left and right) children.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include "tree.h"

//Creates the structure of what the tree contains
//O(1)
struct tree{

	int data;
	struct tree *left, *right, *parent;

};

//Detatches the the given from the parent tree
//O(1)
void detatch(struct tree *child){

	//Check to see if there is a parent to set the respective right and left to null
	if(child->parent!=NULL){
		
		//Check if left node
		if(child->parent->left==child){
			child->parent->left=NULL;
		}else{
			//Must be right node
			child->parent->right=NULL;
		}
	}
}

//Create a tree with the given data, left subtree, and right subtree then return the tree.
//O(1)
struct tree *createTree(int data, struct tree *left, struct tree *right){

	//create the root
	struct tree *root;
	root=malloc(sizeof(struct tree));

	//Assign that data and pointers of the new tree
	root->data=data;
	root->right=right;
	root->left=left;
	root->parent=NULL;

	//If the right and left parents are not NULL, the left and right nodes came from another tree, and need to be detatched before adding to our tree
	if(right!=NULL){
		detatch(right);
		root->right->parent=root;
	}
   
	if(left!=NULL){
		detatch(left);
		root->left->parent=root;
	}

	return root;
}

//Deallocates memory for the given root and all its subtrees
//O(h)
void destroyTree(struct tree *root){

    //Since this is a tree, we can use post order recursion to recusrivley delete all the nodes in the left and right subtree then the root
	if(root!=NULL){
		destroyTree(root->left);
		destroyTree(root->right);
		free(root);
	}
}

//Returns the data assosiated with the given root
//O(1)
int getData(struct tree *root){

	assert(root!=NULL);
	
	return root->data;
}

//Returns the left subtree assosiated with the given root
//O(1)
struct tree *getLeft(struct tree *root){

	assert(root!=NULL);

	return root->left;
}

//Returns the right subtree assosiated with the given root
//O(1)
struct tree *getRight(struct tree *root){
	
	assert(root!=NULL);
	
	return root->right;
}

//Returns the parent tree node assosiated with the given root
//O(1)
struct tree *getParent(struct tree *root){
	
	assert(root!=NULL);
	
	return root->parent;
}

//Assings the left subtree of the given node to the given left subtree
//O(1)
void setLeft(struct tree *root, struct tree *left){
	
	assert(root!=NULL);
	
    //If the root's left is not null, we need to set it's parent pointer to null so it can be reassigned
	if(root->left!=NULL)
		root->left->parent=NULL;
	
    //Now we can set the root's left subtree to the given left, and make sure it is not attatched to some other tree
	root->left=left;
	detatch(left);
}

//Assings the right subtree of the given node to the given right subtree
//O(1)
void setRight(struct tree *root, struct tree  *right){

	assert(root!=NULL);
	
    //If the root's right is not null, we need to set it's parent pointer to null so it can be reassigned
	if(root->right!=NULL)
		root->right->parent=NULL;	
	
    //Now we can set the root's left subtree to the given left, and make sure it is not attatched to some other tree
	root->right=right;
	detatch(right);
}
