#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD 20
#define DESCRIPTION 500

#define path_name_database ("word.txt")
struct Node
{
      char word[WORD];
      char description[DESCRIPTION];
      struct Node *leftChild;
      struct Node *rightChild;
      int height;
};

      struct Node *root = NULL;


int height(struct Node *node)
{
      if (node == NULL)
            return 0;
      return node->height;
}

int max(int a, int b)
{
      return (a > b) ? a : b;
}

struct Node *newNode(char word[], char description[])
{
      struct Node *node = (struct Node *)malloc(sizeof(struct Node));

      for (int i = 0; i < WORD; i++)
      {
            node->word[i] = word[i];
      }
      for (int i = 0; i < DESCRIPTION; i++)
            node->description[i] = description[i];

      node->leftChild = NULL;
      node->rightChild = NULL;
      node->height = 1;
      return (node);
}

struct Node *rightRotate(struct Node *y)
{
      struct Node *x = y->leftChild;
      struct Node *T2 = x->rightChild;

      x->rightChild = y;
      y->leftChild = T2;
      y->height = max(height(y->leftChild), height(y->rightChild)) + 1;
      x->height = max(height(x->leftChild), height(x->rightChild)) + 1;

      return x;
}

struct Node *leftRotate(struct Node *x)
{
      struct Node *y = x->rightChild;
      struct Node *T2 = y->leftChild;

      y->leftChild = x;
      x->rightChild = T2;
      x->height = max(height(x->leftChild), height(x->rightChild)) + 1;
      y->height = max(height(y->leftChild), height(y->rightChild)) + 1;

      return y;
}

struct Node *insertHere(struct Node *root, char word[], char description[])
{
      struct Node *newN = newNode(word, description);

      newN->leftChild = root->leftChild;
      newN->rightChild = root->rightChild;
      root->height = max(height(root->leftChild), height(root->rightChild)) + 1;
      newN->height = max(height(newN->leftChild), height(newN->rightChild)) + 1;

      return newN;
}

int getBalance(struct Node *N)
{
      if (N == NULL)
            return 0;
      return height(N->leftChild) - height(N->rightChild);
}

struct Node *insert(struct Node *node, char word[], int i, char description[])
{
      if (node == NULL)
            return (newNode(word, description));

      struct Node *newN;

      if (i == WORD - 1)
      {
            newN = insertHere(node, word, description);
            return newN;
      }
      if (word[i] < node->word[i])
            node->leftChild = insert(node->leftChild, word, 0, description);
      else if (word[i] > node->word[i])
            node->rightChild = insert(node->rightChild, word, 0, description);
      else
            node = insert(node, word, i + 1, description);

      int balance = getBalance(node);

      if (balance > 1 && word[i] < node->leftChild->word[i])
            return rightRotate(node);

      if (balance < -1 && word[i] > node->rightChild->word[i])
            return leftRotate(node);

      if (balance > 1 && word[i] > node->leftChild->word[i])
      {
            node->leftChild = leftRotate(node->leftChild);
            return rightRotate(node);
      }
      if (balance < -1 && word[i] < node->rightChild->word[i])
      {
            node->rightChild = rightRotate(node->rightChild);
            return leftRotate(node);
      }

      return node;
}

void inOrder(struct Node *root) {
	if(root != NULL) {
		inOrder(root->leftChild);
		for (int i=0; i < WORD; i++) {
            printf ("%c", root->word[i]);
		}
		printf ("\n");

		inOrder(root->rightChild);
	}
}

void delete(struct Node *root, FILE *fptr, char delete_word[])
{
      char word[WORD] = {}, description[DESCRIPTION] = {};
      if (root != NULL)
      {
            delete(root->leftChild, fptr, delete_word);
            for (int i = 0; i < WORD; i++)
            {
                  word[i] = root->word[i];
            }



            for (int i = 0; i < DESCRIPTION; i++)
            {
                  description[i] = root->description[i];
            }

            if (strcmp(word, delete_word) != 0)
            {
                  fprintf(fptr, "\n%s", word);

                  fprintf(fptr, "&");
      
                  fprintf(fptr, "%s", description);
            }

            delete(root->rightChild, fptr, delete_word);
      }
}

void search(struct Node *root, char word[], int i)
{
      
      if (root != NULL)
      {
            if (i == WORD - 1)
            {
                  printf("Meaning: ");
                  for (int p = 0; p < DESCRIPTION; p++)
                  {
                        if (root->description[p] == 0)
                              break;
                        printf("%c", root->description[p]);
                  }
                  return;
            }
            else if (word[i] < root->word[i])
                  search(root->leftChild, word, 0);
            else if (word[i] > root->word[i])
                  search(root->rightChild, word, 0);
            else
                  search(root, word, i + 1);
      }
      else
            printf("Word not found!");
}

void load_word(char path_name[])
{
      FILE *fptr;

      fptr = fopen(path_name, "a+");
      if (fptr == NULL)
      {
            printf("Cannot open file %s", path_name);
            exit(0);
      }
      char word[WORD] = {}, description[DESCRIPTION] = {}, c;
      c = fgetc(fptr);
      while (c != EOF)
      {
            for (int i = 0; i < WORD; i++)
            {
                  if (c == '&')
                        break;
                  word[i] = c;
                  c = fgetc(fptr);
            }
            c = fgetc(fptr);
            for (int i = 0; i < DESCRIPTION; i++)
            {
                  if (c == '\n' || c == EOF)
                        break;
                  description[i] = c;
                  c = fgetc(fptr);
            }
            root = insert(root, word, 0, description);
            for (int i = 0; i < WORD; i++)
            {
                  word[i] = 0;
            }
            for (int i = 0; i < DESCRIPTION; i++)
            {
                  description[i] = 0;
            }
            c = fgetc(fptr);
      }

      fclose(fptr);
}


void user_search()
{
            char word[WORD] = {};
            printf ("\n\nEnter word to search: ");
            scanf ("%[^\n]%*c", word);
            printf ("\n\nWord: %s\n", word);
            search (root, word, 0);        
}



void user_insert()
{
      char word[WORD] = {}, description[DESCRIPTION] = {};

      printf("Enter the new word: ");
      scanf("%[^\n]%*c", word);
      word[strlen(word)] = '\0';

      printf("Enter the new description: ");
      scanf("%[^\n]%*c", description);
      description[strlen(description)] = '\0';

      root = insert(root, word, 0, description);

      FILE *fptr;
      fptr = fopen(path_name_database, "a+");
      if (fptr == NULL)
      {
            printf("Cannot open file %s", path_name_database);
            exit(0);
      }
      fprintf(fptr, "\n%s&%s", word, description);
      pclose(fptr);
}


int main()
{

      int option = 0;
      load_word(path_name_database);


      while (1)
      {
            printf("\n\t\t\t\t\tOPTION: \n");
            printf("\t\t1: Display\n\t\t2: Search\n\t\t3: Delete\n\t\t4: Insert\n\t\t5: Exit\n");
            printf("\t\t\t\t\t\t\t\tYour Option: ");
            scanf("%d%*c", &option);

            switch (option)
            {
            case 1:
                  printf("1: Dislay all word in dictionary\n");
                  printf("\n\t\t\t\t\tWORD IN DICTIONARY\n\n");
                  inOrder(root);
                  // custom_database(root);
                  break;
            case 2:
                  printf("\n\t\t\t\t\tSEARCH\n");
                  user_search();
                  break;
            case 3:

                  printf("3\t\t\t\t\tDELETE\n");
                  FILE *fptr;
                  fptr = fopen(path_name_database, "w");

                  printf("Enter word to delete: ");
                  char delete_word[] = {};
                  scanf("%[^\n]%*c", delete_word);

                  delete(root, fptr, delete_word);

                  pclose(fptr);
                  root = NULL;
                  load_word(path_name_database);
                  break;

            case 4:
                  printf("\n\t\t\t\t\tINSERT\n");
                  user_insert();
                  break;
            default:
                  printf("\n\t\t\t\t\tEXIT\n");
                  return 0;
            }
      }



      return 0;
}