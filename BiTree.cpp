#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType;

typedef struct BiTreeNode {
  ElemType data;
  int index;
  BiTreeNode* parent = NULL;
  BiTreeNode* left = NULL;
  BiTreeNode* right = NULL;
} BiTreeNode;

typedef struct BiTree {
  int TreeID = -1;
  int length = 0;
  BiTreeNode* root;
  BiTree* next;
} BiTree;

enum TraverseMethod {
  PRE,
  IN,
  POST,
  LEVEL
};

bool isvalid(int* pre, int* in, int length);
void write(BiTreeNode* root, TraverseMethod method, bool isindex, FILE* fp);
status InitBiTree(BiTree& T);
status DestroyBiTree(BiTree& T);
status CreateBiTree(BiTree& T, int length, int* preorder, int* inorder, ElemType* data);
status ClearBiTree(BiTree& T);
bool BiTreeEmpty(const BiTree& T);
int BiTreeDepth(const BiTree& T);
BiTreeNode* Root(const BiTree& T);
status Value(const BiTree& T, int index, ElemType& value);
status Assign(BiTree& T, int index, ElemType& value);
BiTreeNode* Parent(const BiTree& T, int index);
BiTreeNode* LeftChild(const BiTree& T, int index);
BiTreeNode* RightChild(const BiTree& T, int index);
BiTreeNode* LeftSibling(const BiTree& T, int index);
BiTreeNode* RightSibling(const BiTree& T, int index);
status InsertChild(BiTree& T, int index, int LR, BiTree& c);
status DeleteChild(BiTree& T, int index, int LR);
status PreOrderTraverse(const BiTree& T);
status InOrderTraverse(const BiTree& T);
status PostOrderTraverse(const BiTree& T);
status LevelOrderTraverse(const BiTree& T);

void write(BiTreeNode* root, TraverseMethod method, bool isindex, FILE* fp) {
  if (root == NULL)
    return;
  if (isindex) {
    switch (method) {
      case PRE:
        fwrite(&root->index, sizeof(int), 1, fp);
        write(root->left, method, isindex, fp);
        write(root->right, method, isindex, fp);
        break;
      case IN:
        write(root->left, method, isindex, fp);
        fwrite(&root->index, sizeof(int), 1, fp);
        write(root->right, method, isindex, fp);
        break;
    }
  } else {
    write(root->left, method, isindex, fp);
    fwrite(&root->data, sizeof(ElemType), 1, fp);
    write(root->right, method, isindex, fp);
  }
}

bool isvalid(int* pre, int* in, int length) {
  vector<int> prev;
  vector<int> inv;
  for (int i = 0; i < length; i++) {
    prev.push_back(*(pre + i));
    inv.push_back(*(in + i));
  }
  sort(prev.begin(), prev.end());
  sort(inv.begin(), inv.end());
  for (int i = 0; i < length; i++) {
    if (prev[i] != inv[i])
      return false;
  }
  return true;
}
void FreeNodes(BiTreeNode* root) {
  if (root == NULL)
    return;
  if (root->left != NULL)
    FreeNodes(root->left);
  if (root->right != NULL)
    FreeNodes(root->right);
  free(root);
}

int search(int value, int* string, int length) {
  if (length <= 0)
    return -1;
  for (int i = 0; i < length; i++) {
    if (string[i] == value)
      return i;
  }
  return -1;
}

BiTreeNode* Create(int* pre, int* in, int length, ElemType* data) {
  int rootindex = search(pre[0], in, length);
  if (rootindex == -1)
    return NULL;
  BiTreeNode* root = (BiTreeNode*)malloc(sizeof(BiTreeNode));
  root->data = data[rootindex];
  root->index = in[rootindex];
  root->left = Create(pre + 1, in, rootindex + 1, data);
  if (root->left != NULL)
    root->left->parent = root;
  root->right = Create(pre + rootindex + 1, in + rootindex + 1, length - rootindex - 1, data + rootindex + 1);
  if (root->right != NULL)
    root->right->parent = root;
  return root;
}

void Traverse(BiTreeNode* root, TraverseMethod method) {
  if (root == NULL)
    return;
  switch (method) {
    case PRE:
      cout << root->data << " ";
      Traverse(root->left, PRE);
      Traverse(root->right, PRE);
      break;
    case IN:
      Traverse(root->left, IN);
      cout << root->data << " ";
      Traverse(root->right, IN);
      break;
    case POST:
      Traverse(root->left, POST);
      Traverse(root->right, POST);
      cout << root->data << " ";
      break;
    case LEVEL:
      queue<BiTreeNode*> q;
      q.push(root);
      while (q.size() != 0) {
        BiTreeNode* n = q.front();
        cout << n->data << " ";
        q.pop();
        if (n->left != NULL)
          q.push(n->left);
        if (n->right != NULL)
          q.push(n->right);
      }
      break;
  }
}

void increaseindex(BiTreeNode* root, int length) {
  if (root == NULL)
    return;
  root->index += length;
  increaseindex(root->left, length);
  increaseindex(root->right, length);
}
int size(BiTreeNode* root) {
  if (root == NULL)
    return 0;
  return 1 + size(root->left) + size(root->right);
}

int Depth(BiTreeNode* root) {
  if (root == NULL)
    return 0;
  int depthleft = Depth(root->left);
  int depthright = Depth(root->right);
  int depth = depthleft > depthright ? depthleft : depthright;
  return depth + 1;
}

BiTreeNode* FindNode(BiTreeNode* root, int index) {
  if (root == NULL)
    return NULL;
  if (root->index == index)
    return root;
  else {
    BiTreeNode* left = FindNode(root->left, index);
    BiTreeNode* right = FindNode(root->right, index);
    if (left != NULL)
      return left;
    if (right != NULL)
      return right;
    return NULL;
  }
}

status InitBiTree(BiTree& T) {
  T.next = NULL;
  T.root = NULL;
  T.length = 0;
  return OK;
}

status DestroyBiTree(BiTree& T) {
  FreeNodes(T.root);
  T.root = NULL;
  return OK;
}

status CreateBiTree(BiTree& T, int length, int* preorder, int* inorder, ElemType* data) {
  T.root = Create(preorder, inorder, length, data);
  T.length = length;
  return OK;
}

status ClearBiTree(BiTree& T) {
  if (T.root == NULL)
    return OK;
  FreeNodes(T.root->left);
  FreeNodes(T.root->right);
  free(T.root);
  T.root = NULL;
  T.length = 0;
  return OK;
}

bool BiTreeEmpty(const BiTree& T) {
  if (T.root == NULL)
    return true;
  else
    return false;
}

int BiTreeDepth(const BiTree& T) {
  return Depth(T.root);
}

BiTreeNode* Root(const BiTree& T) {
  return T.root;
}

status Value(const BiTree& T, int index, ElemType& value) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return ERROR;
  else
    value = node->data;
  return OK;
}

status Assign(BiTree& T, int index, ElemType& value) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return ERROR;
  else
    node->data = value;
  return OK;
}

BiTreeNode* Parent(const BiTree& T, int index) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return NULL;
  else
    return node->parent;
}

BiTreeNode* LeftChild(const BiTree& T, int index) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return NULL;
  else
    return node->left;
}

BiTreeNode* RightChild(const BiTree& T, int index) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return NULL;
  else
    return node->right;
}

BiTreeNode* LeftSibling(const BiTree& T, int index) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return NULL;
  if (node->parent == NULL)
    return NULL;
  else {
    if (node->parent->left == node)
      return NULL;
    if (node->parent->right == node)
      return node->parent->left;
  }

  return NULL;
}

BiTreeNode* RightSibling(const BiTree& T, int index) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return NULL;
  if (node->parent == NULL)
    return NULL;
  else {
    if (node->parent->right == node)
      return NULL;
    if (node->parent->left == node)
      return node->parent->right;
  }

  return NULL;
}

status InsertChild(BiTree& T, int index, int LR, BiTree& c) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL || c.root == NULL || c.root->right != NULL)
    return ERROR;
  if (LR == 0) {
    BiTreeNode* tmp = node->left;
    increaseindex(c.root, T.length);
    c.root->parent = node;
    node->left = c.root;
    if (tmp != NULL) {
      c.root->right = tmp;
      tmp->parent = c.root;
    }
    T.length += c.length;
    return OK;
  }
  if (LR == 1) {
    BiTreeNode* tmp = node->right;
    increaseindex(c.root, T.length);
    c.root->parent = node;
    node->right = c.root;
    if (tmp != NULL) {
      c.root->right = tmp;
      tmp->parent = c.root;
    }
    T.length += c.length;
    return OK;
  } else
    return ERROR;
}

status DeleteChild(BiTree& T, int index, int LR) {
  BiTreeNode* node = FindNode(T.root, index);
  if (node == NULL)
    return ERROR;
  if (LR == 0) {
    if (node->left == NULL)
      return ERROR;
    T.length -= size(node->left);
    FreeNodes(node->left);
    node->left = NULL;
    return OK;
  }
  if (LR == 1) {
    if (node->right == NULL)
      return ERROR;
    T.length -= size(node->right);
    FreeNodes(node->right);
    node->right = NULL;
    return OK;
  } else
    return ERROR;
}

status PreOrderTraverse(const BiTree& T) {
  Traverse(T.root, PRE);
  return OK;
}

status InOrderTraverse(const BiTree& T) {
  Traverse(T.root, IN);
  return OK;
}

status PostOrderTraverse(const BiTree& T) {
  Traverse(T.root, POST);
  return OK;
}

status LevelOrderTraverse(const BiTree& T) {
  Traverse(T.root, LEVEL);
  return OK;
}

void PrintMenu(void) {
}

status LoadData(BiTree** head) {
  int *preorder = NULL, *inorder = NULL;
  ElemType* data = NULL;
  FILE* fp = fopen("SLDB", "r");
  if (fp == NULL)
    return ERROR;

  int size = 0;
  int count = 0;
  BiTree* tmp = (BiTree*)malloc(sizeof(BiTree));
  size = fread(tmp, sizeof(BiTree), 1, fp);
  if (size == 0) {
    free(tmp);
    return OK;
  }
  count++;
  size = tmp->length;
  if (size != 0) {
    preorder = (int*)malloc(size * sizeof(int));
    inorder = (int*)malloc(size * sizeof(int));
    data = (ElemType*)malloc(size * sizeof(ElemType));
    fread(preorder, sizeof(int), size, fp);
    fread(inorder, sizeof(int), size, fp);
    fread(data, sizeof(ElemType), size, fp);
    CreateBiTree(*tmp, size, preorder, inorder, data);
    free(preorder);
    free(inorder);
    free(data);
  }

  *head = tmp;

  while (1) {
    BiTree* tmp = (BiTree*)malloc(sizeof(BiTree));
    size = fread(tmp, sizeof(BiTree), 1, fp);
    if (size == 0) {
      free(tmp);
      break;
    }
    count++;
    size = tmp->length;
    if (size != 0) {
      preorder = (int*)malloc(size * sizeof(int));
      inorder = (int*)malloc(size * sizeof(int));
      data = (ElemType*)malloc(size * sizeof(ElemType));
      fread(preorder, sizeof(int), size, fp);
      fread(inorder, sizeof(int), size, fp);
      fread(data, sizeof(ElemType), size, fp);
      CreateBiTree(*tmp, size, preorder, inorder, data);
      free(preorder);
      free(inorder);
      free(data);
    }
    (*head)->next = tmp;
    *head = (*head)->next;
  }
  (*head)->next = NULL;
  *head = tmp;
  fclose(fp);
  return OK;
}

status SaveData(BiTree* head) {
  FILE* fp = fopen("SLDB", "w");
  if (fp == NULL)
    return ERROR;
  BiTree *L = head, *p = head;
  while (L != NULL) {
    fwrite(L, sizeof(BiTree), 1, fp);
    write(L->root, PRE, true, fp);
    write(L->root, IN, true, fp);
    write(L->root, IN, false, fp);
    p = L->next;
    DestroyBiTree(*L);

    L = p;
  }

  fclose(fp);
  return OK;
}

int main() {
  int selection = -1;
  BiTree* head = NULL;
  while (selection != 0) {
    system("cls");
    printf("\n\n");
    printf("   Menu for Linear Table On Sequence Structure  \n");
    printf("------------------------------------------------\n");
    printf("      1.InitBiTree       11.LeftChild           \n");
    printf("      2.DestroyBiTree    12.RightChild          \n");
    printf("      3.CreateBiTree     13.LeftSibling         \n");
    printf("      4.ClearBiTree      14.RightSibling        \n");
    printf("      5.BiTreeEmpty      15.InsertChild         \n");
    printf("      6.BiTreeDepth      16.DeleteChild         \n");
    printf("      7.Root             17.PreOrderTraverse    \n");
    printf("      8.Value            18.InOrderTraverse     \n");
    printf("      9.Assign           19.PostOrderTraverse   \n");
    printf("      10.Parent          20.LevelOrderTraverse  \n");
    printf("------------------------------------------------\n");
    printf("\n");
    printf("ÇëÑ¡ÔñÄãµÄ²Ù×÷[0~20]:");
    scanf("%d", &selection);
    LoadData(&head);
    BiTree* L = head;
    BiTree* tmp = head;
    int tree_index;
    switch (selection) {
      case -1:
        while (head != NULL) {
          printf("TreeID:%d\tTreelength:%d\t", head->TreeID, head->length);
          cout << "Preorder Traverse: ";
          PreOrderTraverse(*head);
          cout << endl;
          head = head->next;
        }
        head = L;
        break;
      case 1:
        printf("* Function Name: InitBiTree\n");
        printf("* Parameter: BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: initialize the binary tree\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head != NULL) {
          printf("Error, the tree %d already exist.\n", tree_index);
        } else {
          BiTree* new_tree = (BiTree*)malloc(sizeof(BiTree));
          if (InitBiTree(*new_tree) == OK) {
            printf("Inital the tree %d succeed.\n", tree_index);
            new_tree->TreeID = tree_index;
            new_tree->next = L;
            head = new_tree;
          } else {
            printf("ERROR, something wrong with the RAM\n");
          }
        }
        printf("\n");
        break;
      case 2:
        printf("* Function Name: DestroyBiTree\n");
        printf("* Parameter: BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: destroy the binary tree\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        if (head == NULL) {
          printf("Error, the Tree %d does not exist.\n", tree_index);
          break;
        }
        if (head->TreeID == tree_index) {
          head = head->next;
          DestroyBiTree(*L);
          printf("Tree %d has been removed\n", tree_index);
          break;
        }
        while (head->next != NULL) {
          if (head->next->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head->next == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          L = head->next;
          head->next = head->next->next;
          DestroyBiTree(*L);
          printf("Tree %d has been removed\n", tree_index);
          head = tmp;
        }
        printf("\n");
        break;
      case 3:
        printf("* Function Name: CreateBiTree\n");
        printf("* Parameter: BiTree &T, int length, int *preorder, int *inorder, ElemType * data\n");
        printf("* Return: Status(int)\n");
        printf("* Use: create the tree using the defination data\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          int length = 0;
          cout << "please insert the length" << endl;
          cin >> length;
          int* pre = (int*)malloc(sizeof(int) * length);
          int* in = (int*)malloc(sizeof(int) * length);
          ElemType* data = (ElemType*)malloc(sizeof(ElemType) * length);
          cout << "please insert the preorder index" << endl;
          for (int i = 0; i < length; i++)
            cin >> pre[i];
          cout << "please insert the inorder index" << endl;
          for (int i = 0; i < length; i++)
            cin >> in[i];
          cout << "please insert the inorder data" << endl;
          for (int i = 0; i < length; i++)
            cin >> data[i];
          ClearBiTree(*head);
          if (!isvalid(pre, in, length)) {
            cout << "the index is not valid." << endl;
            free(pre);
            free(in);
            free(data);
            head = L;
            break;
          }
          CreateBiTree(*head, length, pre, in, data);
          free(pre);
          free(in);
          free(data);
          head = L;
        }

        printf("\n");
        break;
      case 4:
        printf("* Function Name: ClearBiTree\n");
        printf("* Parameter: BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: clear the BiTree\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          ClearBiTree(*head);
          head = L;
          printf("The Tree %d has been cleared.\n", tree_index);
        }
        break;
      case 5:
        printf("* Function Name: BiTreeEmpty\n");
        printf("* Parameter: const BiTree &T\n");
        printf("* Return: bool\n");
        printf("* Use: check whether the tree is empty\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          bool empty = BiTreeEmpty(*head);
          head = L;
          if (empty) {
            printf("The tree %d is empty!", tree_index);
          } else {
            printf("The tree %d is not empty", tree_index);
          }
        }
        break;
      case 6:
        printf("* Function Name: BiTreeDepth\n");
        printf("* Parameter: const BiTree &T\n");
        printf("* Return: int\n");
        printf("* Use: calculate the depth of the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          printf("The depth of tree %d is %d", tree_index, BiTreeDepth(*head));
          head = L;
        }
        break;
      case 7:
        printf("* Function Name: Root\n");
        printf("* Parameter: const BiTree &T\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the root node of the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* root = Root(*head);
          head = L;
          if (root != NULL)
            printf("The index of the root is %d,the data is %d\n", root->index, root->data);
          else
            printf("The root is empty!");
        }
        break;
      case 8:
        printf("* Function Name: Value\n");
        printf("* Parameter: const BiTree &T, int index,ElemType &value\n");
        printf("* Return: status\n");
        printf("* Use: return the value of the node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          ElemType value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          if (Value(*head, index, value) == OK) {
            cout << "The value is " << value << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 9:
        printf("* Function Name: Assign\n");
        printf("* Parameter: BiTree &T, int index, ElemType &value\n");
        printf("* Return: status\n");
        printf("* Use: assign given value to given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          ElemType value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          cout << "Please insert the desired value!" << endl;
          cin >> value;
          if (Assign(*head, index, value) == OK) {
            cout << "The value " << value << "is successfully inserted into the node " << index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 10:
        printf("* Function Name: Parent\n");
        printf("* Parameter: const BiTree &T, int index\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the parent of the given node \n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          value = Parent(*head, index);
          if (value != NULL) {
            cout << "The parent data is " << value->data << " and the index is" << value->index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 11:
        printf("* Function Name: LeftChild\n");
        printf("* Parameter: const BiTree &T, int index\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the LeftChild of the given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          value = LeftChild(*head, index);
          if (value != NULL) {
            cout << "The left child data is " << value->data << " and the index is" << value->index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 12:
        printf("* Function Name: RightChild\n");
        printf("* Parameter: const BiTree &T, int index\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the RightChild of the given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          value = RightChild(*head, index);
          if (value != NULL) {
            cout << "The right child data is " << value->data << " and the index is" << value->index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 13:
        printf("* Function Name: LeftSibling\n");
        printf("* Parameter: const BiTree &T, int index\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the LeftSibling of the given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          value = LeftSibling(*head, index);
          if (value != NULL) {
            cout << "The left sibling data is " << value->data << " and the index is" << value->index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 14:
        printf("* Function Name: RightSibling\n");
        printf("* Parameter: const BiTree &T, int index\n");
        printf("* Return: BiTreeNode *\n");
        printf("* Use: return the RightSibling of the given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          BiTreeNode* value;
          int index = 0;
          cout << "Please insert the desired index!" << endl;
          cin >> index;
          value = RightSibling(*head, index);
          if (value != NULL) {
            cout << "The right sibling data is " << value->data << " and the index is" << value->index << endl;
          } else
            cout << "Sorry, we encounter an error." << endl;
        }
        head = L;
        break;
      case 15:
        printf("* Function Name: InsertChild\n");
        printf("* Parameter: BiTree &T, int index, int LR, BiTree &c\n");
        printf("* Return: status\n");
        printf("* Use: Insert the BiTree to the given node \n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          int instree_index = 0;
          cout << "please enter the id of the inserted tree" << endl;
          cin >> instree_index;
          BiTree* head2 = L;
          BiTree* pre = L;
          while (head2 != NULL) {
            if (pre->next->TreeID != instree_index)
              pre = pre->next;
            if (head2->TreeID == instree_index)
              break;
            head2 = head2->next;
          }
          if (head2 == NULL) {
            printf("Error, the tree %d does not exist.\n", tree_index);
            break;
          } else {
            int index = 0, LR = 0;
            cout << "please insert the node index" << endl;
            cin >> index;
            cout << "L or R? (L = 0 R = 1)" << endl;
            cin >> LR;
            if (LR != 0 && LR != 1) {
              cout << "invalid input." << endl;
              head = L;
              break;
            }
            if (InsertChild(*head, index, LR, *head2) != OK)
              cout << "Sorry, we encounter an error." << endl;
            else {
              cout << "insert complete." << endl;
              if (L != head2)
                pre->next = pre->next->next;
              else {
                L = head2->next;
                head = L;
              }
            }
          }
        }
        head = L;
        break;
      case 16:
        printf("* Function Name: DeleteChild\n");
        printf("* Parameter: BiTree &T, int index, int LR\n");
        printf("* Return: status\n");
        printf("* Use: delete the child tree of the given node\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          int index = 0;
          int LR = 0;
          cout << "please insert the desiredc index" << endl;
          cin >> index;
          cout << "L or R? (L = 0 R = 1)" << endl;
          cin >> LR;
          if (DeleteChild(*head, index, LR) == OK) {
            cout << "delete complete." << endl;
          } else {
            cout << "Sorry, we encounter an error." << endl;
          }
        }
        head = L;
        break;
      case 17:
        printf("* Function Name: PreOrderTraverse\n");
        printf("* Parameter:const BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: pre order traverse the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          cout << "The pre-order traverse of the tree" << tree_index << " is:" << endl;
          PreOrderTraverse(*head);
        }
        head = L;
        break;
      case 18:
        printf("* Function Name: InOrderTraverse\n");
        printf("* Parameter:const BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: in order traverse the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          cout << "The in-order traverse of the tree" << tree_index << " is:" << endl;
          InOrderTraverse(*head);
        }
        head = L;
        break;
      case 19:
        printf("* Parameter:const BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: in order traverse the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          cout << "The post-order traverse of the tree" << tree_index << " is:" << endl;
          PostOrderTraverse(*head);
        }
        head = L;
        break;
      case 20:
        printf("* Function Name: LevelOrderTraverse\n");
        printf("* Parameter:const BiTree &T\n");
        printf("* Return: Status(int)\n");
        printf("* Use: level order traverse the tree.\n");
        printf("please enter the id of the tree:");
        scanf("%d", &tree_index);
        while (head != NULL) {
          if (head->TreeID == tree_index)
            break;
          head = head->next;
        }
        if (head == NULL) {
          printf("Error, the tree %d does not exist.\n", tree_index);
          head = L;
        } else {
          cout << "The level-order traverse of the tree" << tree_index << " is:" << endl;
          LevelOrderTraverse(*head);
        }
        head = L;
        break;
      case 0:
        cout << "Thanks for using.";
        break;
      default:
        cout << "No such selection.";
        break;
    }
    SaveData(head);
  }
}