/*
 *File: huffman.c
 *
 *Description: Project 5 written by Max Abrams (Atkinson Tuesday Lab). This lossless data compression algorithm utilizes Huffamn coding to build a min priority que via a binary heap.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "tree.h"
#include "pack.h"

//Global variable for number of nodes in tree (used for indexing)  for functions like delete and main.
int ind = 0;

//Prints out the bitstring of a given root recursivley by starting at the root, travering to the top of the tree, then printing each left branch as a zero bit and each right branch as a one bit.
void printInBits(struct tree *root){
    
    struct tree *parent=getParent(root);
    
    //Keep going up the tree until you reach the top, then print out as you go down
    if(parent!=NULL){
        //Not at top so go up another level before printing
        printInBits(parent);
        if(getLeft(parent)==root){
            //I must be the left node
            printf("0");
        }else{
            //If I am not the left node, I must be the right node
            printf("1");
        }
    }
}

//Delete and return theminimum value from the heap, and reorder the heap if needed
struct tree *delete(struct tree *heap1[]){
    
    struct tree *min = heap1[0];
    int loc = 0;
    struct tree *temp;
    int minloc;
    
    //First replace root with the right most leaf, and set the right most leaf's old position to NULL since it was moved
    heap1[0]=heap1[ind-1];
    heap1[ind-1]=NULL;
    
    //Ensure that the heap order was not damaged. If a node is in the wrong position (meaning the node has a higher value than its children), the heap must be reorded.
     while(heap1[(2*loc)+1]!=NULL||heap1[(2*loc)+2]!=NULL){
        
         //Calculate the position of the right child. Since at least one of the children exists, if there is no right child then there must be a left chiild.
         if(heap1[(2*loc)+2]==NULL){
             //I must be the left node
             if(getData(heap1[loc])>getData(heap1[(2*loc)+1])){
                 //My child is smaller than me, so we should switch
                 temp=heap1[(2*loc)+1];
                 heap1[(2*loc)+1]=heap1[loc];
                 heap1[loc]=temp;
             }
             //Now that we have made the swap, make sure we didn't break any other order so check the level my child was on
             loc=2*loc+1;
        }else{
             //Must be a right node, so we want to calcuate the min of the two children and see if we are going to swap with it.
             if(getData(heap1[(2*loc)+1])>getData(heap1[(2*loc)+2])){
                 //Right node must be min
                 minloc=2*loc+2;
             }else{
                 //Left node must be min
                 minloc=2*loc+1;
             }
                //Make sure I am smaller than the min of my children. If not, we should swap
            if(getData(heap1[loc])>getData(heap1[minloc])){
                //Min is less than our current parent so need to swap
                temp=heap1[minloc];
                heap1[minloc]=heap1[loc];
                heap1[loc]=temp;
            }
             //This level is in order
            loc=minloc;
        }
     }
    
    ind--;
    return min;
}

//Insert a new node into the given heap, and put it in its correct position in the higharchy to maintain order
void insert(struct tree *heap1[], struct tree *newTree){
   
    //Add the new tree to the lowest level
    heap1[ind]=newTree;
    
    int loc=ind;
    struct tree *temp;
    
    //Loop through and while I am still a lower number then my parent, swap with my parent
    while(getData(newTree)<getData(heap1[(loc-1)/2])){
        
        temp=heap1[(loc-1)/2];
        heap1[(loc-1)/2]=heap1[loc];
        heap1[loc]=temp;
        loc = (loc-1)/2;
    }
    
    //I am in my correct position, and added another node so need to increase my total count.
    ind++;
}

int main( int argc, char *argv[]){

	//Make sure the arguments are correct
	if(argc!=3){
		printf("Invalid arguments \n");
		return -1;
	}

	//Open the file we want
	FILE *fp;
	fp=fopen(argv[1], "r");

	if(fp==NULL){
		printf("Could not open file \n");
		return -1;
	}

	//Create and assign variables
	int c;
    struct tree *low1;
	struct tree *low2;
    //The tree for each char that occured more than once
	struct tree *leaves[257];
    //An array to record the number of times each char occured based on ASCII value
    int count[257];
    //the tree for the leaves organized as a binary heap
    struct tree *heap[257];

	//Create an array, and initially set all slots to zero, to store the number of times the each char occurs based on ASCII values.
	for(int i =0; i<257; i++){
		count[i]=0;
        leaves[i]=NULL;
        heap[i]=NULL;
	}

	//Read the file char by char and record how many of each char there are
	while((c=getc(fp))!=EOF){
		count[c]++;
	}
	
    
	//Create a tree with count as the data for every count that has a value greater than 0 (occured at least once) and insert it into the binary heap
	for(int i=0; i<257; i++){
		if(count[i]>0){	
		//Char must have occured at least once, so add to binary heap
			leaves[i]=createTree(count[i],NULL,NULL);
            insert(heap,leaves[i]);
		}
	}
    
    //Account for EOF in tree
    leaves[256] = createTree(0,NULL,NULL);
    insert(heap,leaves[256]);
    
	//As long as the priority que has more than one tree in it, remove the two lowest prioriteis trees and combine them to create a new tree with these trees as subtrees
    while(ind>1){
        //Find, record, and remove the two lowest values in the heap
        low1 = delete(heap);
        low2 = delete(heap);
        
        //Make a new tree from the two lowest values
        struct tree *new = createTree((getData(low1)+getData(low2)),low1,low2);
        
        //Insert the new tree into the heap
        insert(heap,new);
    }
    
	//Print the tree
    for(int i=0; i<257;i++){
    
        //Only print if the char occured at least once
        if(count[i]>0){
            //Format the output accordingly
            if(isprint(i)==0){
                //Printing a 3 digit octal value of non-printible chars
                printf("%03o: ",i);
            }
            else{
                //Printing a char
                printf("'%c': ",i);
            }
            
            //Print the number of times the value occured
            printf("%d ",count[i]);
            //Print the bit value assosiated with each leaf
            printInBits(leaves[i]);
            printf("\n");
        }
    }
    //Print EOF
    printf("%03o: ",256);
    printf("%d ", count[256]);
    printInBits(leaves[256]);
    printf("\n");
    
    //close file
    fclose(fp);
  
    //Pack the file using the given pack function
    pack(argv[1],argv[2],leaves);
	return 1;
}
