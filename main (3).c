#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define MAXCHAR 21

FILE* readW;

FILE* write;

FILE* writeIn;

typedef struct tree_node{
  char word[MAXCHAR];
  int frequency;
  int depth;
}tree_node;

typedef struct BSTnode{
  struct BSTnode* left;
  struct BSTnode* right;
  tree_node* nodeWord;
}BSTnode;

BSTnode* create_node(int freq, char* newWord, int wordLenght, int depth)  // This function creates a Binary Tree node and the node with the frequency and the word we want to insert, and allocates memory for the Binary Tree node and the node which we want to insert.
{

  wordLenght = strlen(newWord);
  // Allocate space for the node, set the fields.
  BSTnode* temp;
  temp = malloc(sizeof(BSTnode));
  temp->nodeWord = malloc(sizeof(tree_node));
  temp->nodeWord->frequency = freq;
  strcpy(temp->nodeWord->word, newWord);
  
  temp->nodeWord->depth = depth;

  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}

BSTnode* insert(BSTnode* root, char* newWord, int depth) // This function inserts the node in the Binary Tree by checking character by character the word in the tree.
{
  int lenght = strlen(newWord);


  if(root == NULL)
  {
    //printf("%d", depth);
    BSTnode* element = create_node(1, newWord, lenght, depth);
    //printf("%s", newWord);
    return element;
  }

  else
  {
    
    if(strcmp(newWord, root -> nodeWord -> word ) > 0 )
    {

      if(root->right != NULL)
      {
        root->right = insert(root->right, newWord, depth + 1);
        
      }
      else
      {
        BSTnode* element;
        element = create_node(1, newWord, lenght, depth + 1);
        root->right = element; 
      }     

    }
    else if(strcmp(newWord, root -> nodeWord-> word) < 0)
    {
      if(root->left != NULL)
        root->left = insert(root->left, newWord, depth + 1);

      else
      {
        BSTnode* element = create_node(1, newWord, lenght, depth + 1);
        root->left = element;
      }

      
    }

    else if(newWord == '\0')
    {
      return NULL;
    }

    else
      root->nodeWord->frequency++;

    return root;


  }


}

BSTnode* find(BSTnode* current, char* word) // This function finds the node we query and prints its frequency and depth, and returns this node.
{
  if(current != NULL)
  {
    if(strcmp(word, current->nodeWord->word) == 0)
    {
     printf("%d %d\n", current->nodeWord->frequency, current->nodeWord->depth);
     fprintf(write, "%d %d\n", current->nodeWord->frequency,current->nodeWord->depth);
      return current;
    }

    if(strcmp(word, current->nodeWord->word) < 0)
      return find(current->left, word);

    if(strcmp(word, current->nodeWord->word) > 0)
      return find(current->right, word);
  
  }

  else
  {
    printf("%d %d\n", -1, -1);
    fprintf(write, "%d %d\n", -1, -1 );
    return NULL;
  }

  return NULL;
}

void inorder(BSTnode* current)
{
  if(current != NULL)
  {
    inorder(current->left);
    printf("%s \n", current->nodeWord->word);
    inorder(current->right);
  }
}

int compareTo(tree_node* firstNode, tree_node* scdNode)
{
  if(strcmp(firstNode->word, scdNode->word) < 0)
    return 1;

  else
    return -1;
}

void merge(tree_node **a, int left, int mid, int right)
{
  int i,j,k;

  int leftsize = mid - left +1;
  int r1 = right - mid;

  tree_node** L = malloc(sizeof(tree_node*)* leftsize);
  tree_node** R = malloc(sizeof(tree_node*)*r1);

  for(i = 0; i<leftsize; i++){

    L[i] = a[left+i];
  }

  for(i = 0; i<r1;i++)
  {

    R[i] = a[mid+1+i];
  }

  i = 0;
  j = 0;
  k = left;


  while(i< leftsize && j< r1)
  {
    if(L[i]->frequency > R[j]->frequency)
    {
      //int wordPos = compareTo(L[i], R[j]);

      //if(L[i]->frequency == R[j]->frequency && wordPos == 1)
     // {
      a[k] = L[i];
      i++;
      k++;

      //}
    }
    else if(L[i]->frequency == R[j]->frequency && compareTo(L[i], R[j]) == 1) // This part checks if the frequencies are equal and then it checks the words character by character and if the letter of the first word in the first node is less than letter of the word in the second node.
    {
      //int wordPos = compareTo(L[i], L[i+1]);
      //if(compareTo > 0)
      {
      a[k] = L[i];
      i++;
      k++;
      }

    }
    else {
      a[k] = R[j];
      j++;
      k++;
    }
  }



  while( i < leftsize)
  {
    a[k] = L[i];
      i++;
      k++;
  }

  while(j < r1)
  {
    a[k] = R[j];
      j++;
      k++;
  }

  free(L);
  free(R);
  
}

void MergeSort(tree_node** a, int l, int r)
{
  if (l<r)
  {
    int mid = (l+r)/ 2;

    MergeSort(a, l, mid);

    MergeSort(a, mid+1, r);

    merge(a, l, mid, r);
  }
}


int CountNodes(BSTnode* current_ptr) // This function counts the number of the nodes in the Binary Tree.
{
  if(current_ptr != NULL)
  {
    return 1 + CountNodes(current_ptr->left) + CountNodes(current_ptr->right);
  }

  else
    return 0;
}

tree_node** Array(BSTnode*createdtree) // Thus function creates array of node pointers of the Binary Tree.
{
  int NumNodes = CountNodes(createdtree);

  tree_node** arr = malloc(NumNodes * sizeof(tree_node*));

  return arr;

}

void preorder(tree_node** current, BSTnode* root, int* i) // This function traverses over the Binary Tree and puts all the nodes into the array.
{

  if(root != NULL)
  {
    //printf("i = %d\n", *i);
    current[*i] = root->nodeWord;
    (*i)++;
    preorder(current, root->left, i);
    //printf("%s \n", (*current)->nodeWord->word);
    preorder(current, root->right, i);
  }
}
void postorderFreeMemory(BSTnode* current)
{
  if(current != NULL)
  {
    postorderFreeMemory(current->left);
    postorderFreeMemory(current->right);
    free(current->nodeWord);
    free(current);
  }
}
void freeMemory(tree_node** arr)
{
  free(arr);
}

void Sort(tree_node** current, BSTnode* root)
{
 
}



int main(void) {

  atexit(report_mem_leak);

  //printf("Hello");
  readW = fopen("in.txt", "r");
  //writeIn = fopen("in2.txt", "w");
  write = fopen("out.txt", "w");
  int numWords;

  BSTnode* root = NULL;

  //printf("Hello");
  fscanf(readW, "%d", &numWords);

  //printf("Hello");
  for(int i = 0; i < numWords; i++)
  {
    int action;
    //char letters[26] = {"abcdefghijklmnopqrstuvwxyz"};
    char newWord[MAXCHAR];

    //int lenght = 10;
    //char newWordIn[lenght];
    //for(int l = 0; l < 10; l++)
    //{
     // int j2 = rand() % 26;

      //newWordIn[l] = letters[j2];
    //}

    //fprintf(writeIn, "%d %s\n", action, )

      fscanf(readW, "%d %s", &action, newWord);

  
      if(action == 1)
      {
        root = insert(root, newWord, 0);
      }
      else
      {
        find(root, newWord);
      }

      //inorder(root);
      //printf("\n");


  }
  int NumNodes1 = CountNodes(root);
  //printf("%d\n", NumNodes1);
  // inorder(root);

  tree_node** createArray;

  int i = 0;

  createArray = Array(root);

  preorder(createArray, root, &i);

  //int j1=0;

  //int NumNodes1 = CountNodes(root);

 // printf("%d", NumNodes1);

  //while(createArray != NULL && j1 < NumNodes1)
  //{
//printf("%s", createArray[j1]->word);
  //  j1++;
//}
  int NumNodes = CountNodes(root);

  MergeSort(createArray, 0, NumNodes - 1);

  int j = 0;

  while(createArray != NULL && j < NumNodes)
  {

    printf("%s %d\n", createArray[j]->word, createArray[j]->frequency );
    fprintf(write, "%s %d\n", createArray[j]->word, createArray[j]->frequency);
    j++;
  }

  freeMemory(createArray);

  postorderFreeMemory(root);

  fclose(readW);

  fclose(write);



  return 0;
}