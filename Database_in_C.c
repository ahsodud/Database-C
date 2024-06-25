//---------------------------------------------------------------------------------------------------------------------
// a3.c
//
// My implementation of SQL-like database using C structs.
//
//
// Group: Group 3
//
// Author: 12134500
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // yeah yeah I know, minus styling points :(
#define MAX_BST 50

//---------------------------------------------------------------------------------------------------------------------
///
///KeyValue struct. Hold key, value of it and a pointer to next key. If pointer == null, then its the last key-value pair
typedef struct KeyValue 
{
  char *key;
  char *value;
  struct KeyValue *next;
} KeyValue;

//---------------------------------------------------------------------------------------------------------------------
///
///BSTNode struct. Single node of BST. Has a ID, data and a pointers to the left and right nodes
typedef struct BSTNode 
{
  int id;
  KeyValue *data;
  struct BSTNode *left;
  struct BSTNode *right;
} BSTNode;

//---------------------------------------------------------------------------------------------------------------------
///
///BSTMap struct. Kinda data base itself. Holds root bst, name of it, sort key and total nodes in this BST
typedef struct BSTMap 
{
  BSTNode *bst;
  char name[50];
  char sortKey[50];
  int totalNodes;
} BSTMap;


//structs handling part
KeyValue* createKeyValue(char *key, char *value);
BSTNode* createBSTNode(int id, KeyValue *data);
void freeKeyValue(KeyValue *kv);
void freeBST(BSTNode *root);
int compareKeyValue(KeyValue *a, KeyValue *b, const char *key);
KeyValue* searchKeyValue(KeyValue *kv, const char *key);

//ADD
BSTNode* readFileAndCreateBST(const char *filename, const char *sortKey, int *total_nodes);

//PRINT
void preorderTraversal(BSTNode *root);

//ADD and INSERT
BSTNode* insertIntoBST(BSTNode *root, BSTNode *node, const char *key);

//SELECT
int searchBST(BSTNode *root, const char *key, const char *value, int sortKey, int *found);

//INSERT
void insertKeyValuePairs(BSTMap *map, char *values[], int numValues);

//DELETE
BSTNode* findMinValueNode(BSTNode* node);
BSTNode* deleteNode(BSTNode* root, char* key, char* value, const char* sortKey);



//---------------------------------------------------------------------------------------------------------------------
///
/// Main function. Depending on the type of command performs actions. There are 6 commands in total : QUIT, ADD,
/// PRINT, SELECT, INSERT and DELETE. Their work is similar to SQL analogs. For more detail look implementation following
///
//
int main(void) 
{
  //initialization
  char input[512];
  BSTMap bstMaps[MAX_BST]; // Array of BSTMaps to hold multiple BSTs
  int numBSTs = 0;
  BSTNode *root = NULL;
 
  while (1) 
  {
    printf("Enter command\n > ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    char *command = strtok(input, " ");
    if (command && strcmp(command, "QUIT") == 0) 
    {
      break;
    }
    if (command && strcmp(command, "ADD") == 0) 
    {
      char *filename = strtok(NULL, " ");
      char *bstName = strtok(NULL, " ");
      char *sortKey = strtok(NULL, " ");
      int total_nodes = 0;
      root = readFileAndCreateBST(filename, sortKey, &total_nodes);
      strcpy(bstMaps[numBSTs].name, bstName);
      strcpy(bstMaps[numBSTs].sortKey, sortKey);
      bstMaps[numBSTs].bst = root;
      bstMaps[numBSTs].totalNodes = total_nodes;
      numBSTs++;
      printf("\nNew BST was added! Name=%s Sorting-Key=%s\n\n",bstName , sortKey);
      preorderTraversal(root);
      printf("\n");
    }
    if (command && strcmp(command, "PRINT") == 0) 
    {
      if(numBSTs > 1)
      {
        printf("Available databases:\n");
        for(int count = 0; count < numBSTs; count++)
        {
          printf("%s\n",bstMaps[count].name);
        }
        printf("Which database do you want to be printed?\n > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        for(int count = 0; count < numBSTs; count++)
        {
          if(strcmp(input, bstMaps[count].name) == 0)
          {
            preorderTraversal(bstMaps[count].bst);
          }
        }
      }
      else
      {
        preorderTraversal(root);
      }
      printf("\n");
    }
    if (command && strcmp(command, "SELECT") == 0) 
    {
      char *key = strtok(NULL, " ");
      strtok(NULL, " "); // Skip "FROM"
      char *bstName = strtok(NULL, " ");
      strtok(NULL, " "); // Skip "WHERE"
      strtok(NULL, " "); // Skip "="
      char *value = strtok(NULL, " ");
      int found = 0;
      for (int count = 0; count < numBSTs; count++) 
      {
        if (strcmp(bstName, bstMaps[count].name) == 0) 
        {
          if (strcmp(key, bstMaps[count].sortKey) != 0) 
          {
            printf("key is not sorting-key, depth-first-search is used!\n");
            searchBST(bstMaps[count].bst, key, value, 0,&found);
          } 
          else 
          {
            searchBST(bstMaps[count].bst, key, value, 1,&found);
          }
          found = 1;
          break;
        }
      }
    }
    if(command && strcmp(command, "INSERT") == 0)
    {
      strtok(NULL, " ");
      char *bstName = strtok(NULL, " ");
      char *values[MAX_LINE_LENGTH / 2];
      int numValues = 0;
      char *token = strtok(NULL, " ");
      while (token) 
      {
        values[numValues++] = token;
        token = strtok(NULL, " ");
      }
      char *lastToken = values[numValues - 1];
      size_t len = strlen(lastToken);
      if (lastToken[len - 1] != '\n') 
      {
        lastToken[len] = '\n';
        lastToken[len + 1] = '\0';
      }
      for (int count = 0; count < numBSTs; count++) 
      {
        if (strcmp(bstName, bstMaps[count].name) == 0) 
        {
          insertKeyValuePairs(&bstMaps[count], values, numValues);
          break;
        }
      }
      printf("\n");
    }
    if (strcmp(command, "DELETE") == 0) 
    {
      strtok(NULL, " ");
      char *bstName = strtok(NULL, " ");
      strtok(NULL, " ");
      char *key = strtok(NULL, " ");
      strtok(NULL, " ");
      char *value = strtok(NULL, " ");
      for (int count = 0; count < numBSTs; count++) 
      {
        if (strcmp(bstName, bstMaps[count].name) == 0) 
        {
          bstMaps[count].bst = deleteNode(bstMaps[count].bst, key, value, bstMaps[count].sortKey);
          break;
        }
      }
      printf("\n");
    }
  }

  for (int count = 0; count < numBSTs; count++) 
  {
    freeBST(bstMaps[count].bst);
  }
  return 0;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// Key-Value chain construction
///
/// @param key key
/// @param value value
///
/// @return linked key-value
//
KeyValue* createKeyValue(char *key, char *value) 
{
  KeyValue *newKeyValue = (KeyValue*)malloc(2*sizeof(KeyValue));
  newKeyValue->key = strdup(key);
  newKeyValue->value = strdup(value);
  newKeyValue->next = NULL;
  return newKeyValue;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// BSTNode constructor
///
/// @param id id
/// @param data key-value chain
///
/// @return Single node of BST
//
BSTNode* createBSTNode(int id, KeyValue *data) 
{
  BSTNode *newNode = (BSTNode*)malloc(2*sizeof(BSTNode));
  newNode->id = id;
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Destructor for key-value
///
/// @param kv key-value
///
//
void freeKeyValue(KeyValue *kv) 
{
  if (kv) 
  {
    free(kv->key);
    free(kv->value);
    free(kv);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Destructor for BSTnode
///
/// @param root root of BST
///
//
void freeBST(BSTNode *root) 
{
  if (root) 
  {
    freeBST(root->left);
    freeBST(root->right);
    KeyValue *current = root->data;
    while (current) 
    {
      KeyValue *next = current->next;
      freeKeyValue(current);
      current = next;
    }
    free(root);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function to compare key values
///
/// @param a key value 1
/// @param b key value 2
/// @param key a key to compare with
///
/// @return 1, if values are same, 0 if not
//
int compareKeyValue(KeyValue *a, KeyValue *b, const char *key) 
{
  KeyValue *aVal = a, *bVal = b;
  while (aVal && strcmp(aVal->key, key) != 0) aVal = aVal->next;
  while (bVal && strcmp(bVal->key, key) != 0) bVal = bVal->next;
  return strcmp(aVal->value, bVal->value);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function to insert a BSTnode into the BST
///
/// @param root root node
/// @param node node to insert
/// @param key a key to compare with
///
/// @return new BST with inserted node
//
BSTNode* insertIntoBST(BSTNode *root, BSTNode *node, const char *key) 
{
  if (root == NULL) 
  {
    printf("Insert here: id=%d", node->id + 1);
    KeyValue *kv = node->data;
    while (kv) 
    {
      printf(" %s=%s", kv->key, kv->value);
      kv = kv->next;
    }
    return node;
  }
  printf("id: %d\n", root->id + 1);
  if (compareKeyValue(node->data, root->data, key) < 0) 
  {
    root->left = insertIntoBST(root->left, node, key);
  } 
  else 
  {
    root->right = insertIntoBST(root->right, node, key);
  }
  return root;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function to print BST
///
/// @param root root of the printed BST
///
//
void preorderTraversal(BSTNode *root) 
{
  if (root) 
  {
    KeyValue *kv = root->data;
    printf("id=%d", root->id + 1);
    while (kv) {
        printf(" %s=%s", kv->key, kv->value);
        kv = kv->next;
    }
    //printf("\n");
    preorderTraversal(root->left);
    preorderTraversal(root->right);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function for BST initialization
///
/// @param filename name of the executable file
/// @param sortkey key for sorting
/// @param total_nodes total number of nodes
///
/// @return BSTNode
//
BSTNode* readFileAndCreateBST(const char *filename, const char *sortKey, int *total_nodes) 
{
  FILE *file = fopen(filename, "r");

  char line[MAX_LINE_LENGTH];
  if (!fgets(line, sizeof(line), file)) 
  {
    fclose(file);
    return NULL;
  }

  // Read and tokenize keys
  char *keys[MAX_LINE_LENGTH / 2];
  int numKeys = 0;
  char *token = strtok(line, ";");
  while (token) 
  {
    // Check if token is the last one and remove newline character if present
    if (token[strlen(token) - 1] == '\n') 
    {
    token[strlen(token) - 1] = '\0'; // Remove newline character
    }
    keys[numKeys++] = strdup(token);
    token = strtok(NULL, ";");
  }

  BSTNode *root = NULL;
  int id = 0;
  while (fgets(line, sizeof(line), file)) 
  {
    // Check if the last line doesn't end with a newline character
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] != '\n') 
    {
      line[len] = '\n';
      line[len + 1] = '\0';
    }
    KeyValue *head = NULL, *tail = NULL;
    token = strtok(line, ";");
    for (int i = 0; i < numKeys && token; i++) 
    {
      // Check if token is NULL
      if (token) 
      {
        KeyValue *kv = createKeyValue(keys[i], token);
        if (!head) 
        {
          head = kv;
        } 
        else 
        {
          tail->next = kv;
        }
        tail = kv;
      }
      token = strtok(NULL, ";");
    }
      BSTNode *node = createBSTNode(id, head);
      root = insertIntoBST(root, node, sortKey);
      id++;
    }
  *total_nodes = id;
  fclose(file);
  for (int i = 0; i < numKeys; i++) 
  {
    free(keys[i]);
  }
  return root;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function to search for key inside key value
///
/// @param kv key value
/// @param key key to look for
///
/// @return desired value
//
KeyValue* searchKeyValue(KeyValue *kv, const char *key) 
{
  while (kv && strcmp(kv->key, key) != 0) 
  {
    kv = kv->next;
  }
  return kv;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Search function. Used for SELECT command
///
/// @param root root node
/// @param key key to be searched with
/// @param value value to search
/// @param sortKey either 1 or 0, depends on if key is sort or not
/// @param found found or not indicator
///
/// @return 1, if found, 0 if not
//
int searchBST(BSTNode *root, const char *key, const char *value, int sortKey, int *found) 
{
  if (!root) return 0;

  // Always print the current node's id
  printf("id: %d\n", root->id + 1);

  if (*found) return 1; // If already found, skip further processing
    
  KeyValue *kv = searchKeyValue(root->data, key);
    
  if (kv && strcmp(kv->value, value) == 0) 
  {
    *found = 1; // Mark as found
    printf("Found: ");
    KeyValue *result = root->data;
    while (result) 
    {
      if (result->next) 
      {
        printf("%s=%s ", result->key, result->value);
      } 
      else 
      {
        printf("%s=%s", result->key, result->value);
      }
      result = result->next;
    }
    printf("\n");
    return 1; // Found the node
  }

  if (sortKey) 
  {
    if (kv && strcmp(kv->value, value) < 0) 
    {
      if (searchBST(root->right, key, value, sortKey, found)) return 1;
    } 
    else 
    {
      if (searchBST(root->left, key, value, sortKey, found)) return 1;
    }
  } 
  else 
  {
    if (searchBST(root->left, key, value, sortKey, found)) return 1;
    if (searchBST(root->right, key, value, sortKey, found)) return 1;
  }
    
  return 0; // Node not found in this path
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function used to handle insert command
///
/// @param map a BSTmap
/// @param values values to be inserted
/// @param numValues number of values
///
//
void insertKeyValuePairs(BSTMap *map, char *values[], int numValues) 
{
  KeyValue *head = NULL, *tail = NULL;
    
  // Get key count from the first node's data
  if (map->bst) 
  {
    KeyValue *temp = map->bst->data;
    while (temp) 
    {
      temp = temp->next;
    }
  }

  // Create key-value pairs using keys from the first node
  KeyValue *currentKey = map->bst->data;
  for (int i = 0; i < numValues; i++) 
  {
    KeyValue *kv = createKeyValue(currentKey->key, values[i]);
    if (!head) 
    {
      head = kv;
    } 
    else 
    {
      tail->next = kv;
    }
    tail = kv;
    currentKey = currentKey->next;
  }

  BSTNode *node = createBSTNode(map->totalNodes, head);
  map->bst = insertIntoBST(map->bst, node, map->sortKey);
  map->totalNodes++;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Function to find smallest node
///
/// @param node BSTnode
///
/// @return min node
//
BSTNode* findMinValueNode(BSTNode* node) 
{
  BSTNode* current = node;
  while (current && current->left != NULL) 
  {
    current = current->left;
  }
  return current;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Delete node function
///
/// @param root root bst
/// @param key key with which to delete
/// @param value a value to delete
/// @param sortKey sorting key
///
/// @return 1, if values are same, 0 if not
//
BSTNode* deleteNode(BSTNode* root, char* key, char* value, const char* sortKey) 
{
  if (root == NULL) return root;
  printf("id: %d\n", root->id + 1);

  if (compareKeyValue(root->data, createKeyValue(key, value), sortKey) > 0) 
  {
    root->left = deleteNode(root->left, key, value, sortKey);
  } 
  else if (compareKeyValue(root->data, createKeyValue(key, value), sortKey) < 0) 
  {
    root->right = deleteNode(root->right, key, value, sortKey);
  } 
  else 
  {
    printf("Found! - Delete: ");
    printf("id=%d ", root->id + 1);
    KeyValue *result = root->data;
    while (result) 
    {
      if (result->next) 
      {
        printf("%s=%s ", result->key, result->value);
      } 
      else 
      {
        printf("%s=%s", result->key, result->value);
      }
      result = result->next;
    }
    if (root->left == NULL) 
    {
      BSTNode *temp = root->right;
      free(root);
      return temp;
    } 
    else if (root->right == NULL) 
    {
      BSTNode *temp = root->left;
      free(root);
      return temp;
    }

    BSTNode* temp = findMinValueNode(root->right);
    root->data = temp->data;
    root->id = temp->id;
    root->right = deleteNode(root->right, temp->data->key, temp->data->value, sortKey);
  }
  return root;
}






