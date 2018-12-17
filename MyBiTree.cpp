#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <queue>
#include <vector>
#define MAX_HEIGHT 1000
#define INFINITY (1 << 20)

using namespace std;
using status = void;
using ValueType = char;

struct ElemType {
  ValueType value;
  size_t index;
  bool null = true;
};

struct BiTreeNode {
  ElemType data;
  BiTreeNode* parent = NULL;
  BiTreeNode* left = NULL;
  BiTreeNode* right = NULL;
};

struct BiTree {
  size_t length = 0;
  bool init = false;
  BiTreeNode* root = NULL;
};

enum TraverseMethod {
  PRE,
  IN,
  POST,
  LEVEL
};

enum Error {
  OK,
  EXISTS,
  NOT_EXISTS,
  WRONG_DEF,
  NO_SUCH_NODE,
};

status InitBiTree(BiTree& T);
status DestroyBiTree(BiTree& T);
status CreateBiTree(BiTree& T, vector<ElemType> def);
status ClearBiTree(BiTree& T);
bool BiTreeEmpty(const BiTree& T);
int BiTreeDepth(const BiTree& T);
BiTreeNode* Root(const BiTree& T);
status Value(const BiTree& T, size_t index, ElemType& value);
status Assign(BiTree& T, size_t index, ElemType& value);
BiTreeNode* Parent(const BiTree& T, size_t index);
BiTreeNode* LeftChild(const BiTree& T, size_t index);
BiTreeNode* RightChild(const BiTree& T, size_t index);
BiTreeNode* LeftSibling(const BiTree& T, size_t index);
BiTreeNode* RightSibling(const BiTree& T, size_t index);
status InsertChild(BiTree& T, size_t index, bool LR, BiTree& c);
status DeleteChild(BiTree& T, size_t index, bool LR);
status PreOrderTraverse(const BiTree& T, function<void(BiTreeNode*)>);
status InOrderTraverse(const BiTree& T, function<void(BiTreeNode*)>);
status PostOrderTraverse(const BiTree& T, function<void(BiTreeNode*)>);
status LevelOrderTraverse(const BiTree& T);

BiTreeNode* _Find(const BiTree& T, size_t index);
void _PrintNode(BiTreeNode* n);

status InitBiTree(BiTree& T) {
  if (T.init == false) {
    T.init = true;

  } else {
    throw Error::EXISTS;
  }
}

status DestroyBiTree(BiTree& T) {
  if (T.init != false) {
    ClearBiTree(T);
    T.init = false;
  } else {
    throw Error::NOT_EXISTS;
  }
}

status CreateBiTree(BiTree& T, vector<ElemType> def) {
  if (def.size() == 0) {
    throw Error::WRONG_DEF;
  }

  function<void(BiTreeNode*, size_t, size_t)> Create = [&](BiTreeNode* n, size_t s, size_t e) -> void {
    n->data = def[s];
    size_t v = (e - s) - (e - s) / 2;
    if (s + 1 <= v + s && def[s + 1].null == false)
      Create(n->left = new BiTreeNode(), s + 1, v + s);
    if (v + s + 1 <= e && def[v + s + 1].null == false)
      Create(n->right = new BiTreeNode(), v + s + 1, e);
  };

  Create(T.root = new BiTreeNode(), 0, def.size() - 1);
}

status ClearBiTree(BiTree& T) {
  PostOrderTraverse(T, [](BiTreeNode* pn) -> void {
    delete pn;
  });
  T.root = NULL;
}

status Value(const BiTree& T, size_t index, ElemType& value) {
  value = _Find(T, index)->data;
}

status Assign(BiTree& T, int index, ElemType& value) {
  _Find(T, index)->data = value;
}

BiTreeNode* _Find(const BiTree& T, size_t index) {
  bool stop = false;
  BiTreeNode* pRtn = NULL;
  PreOrderTraverse(T, [&](BiTreeNode* pn) -> void {
    if (stop == true)
      return;

    if (pn->data.index == index) {
      pRtn = pn;
      stop = true;
    }
  });

  if (pRtn == NULL) {
    throw Error::NO_SUCH_NODE;
  }
  return pRtn;
}

void _PrintNode(BiTreeNode* n) {
  printf("(%d,%c)\n", n->data.index, n->data.value);
}

BiTreeNode* Parent(const BiTree& T, size_t index) {
  bool stop = false;
  BiTreeNode* pRtn = NULL;
  PreOrderTraverse(T, [&](BiTreeNode* pn) -> void {
    if (stop == true)
      return;

    if (
        (pn->left && pn->left->data.index == index) ||
        (pn->right && pn->right->data.index == index)) {
      pRtn = pn;
      stop = true;
    }
  });

  if (pRtn == NULL)
    throw Error::NO_SUCH_NODE;

  return pRtn;
}

BiTreeNode* LeftChild(const BiTree& T, size_t index) {
  return _Find(T, index)->left;
}

BiTreeNode* RightChild(const BiTree& T, size_t index) {
  return _Find(T, index)->right;
}

BiTreeNode* LeftSibling(const BiTree& T, size_t index) {
  BiTreeNode* parent = Parent(T, index);
  BiTreeNode* self = _Find(T, index);
  return parent->left == self ? NULL : parent->left;
}

BiTreeNode* RightSibling(const BiTree& T, size_t index) {
  BiTreeNode* parent = Parent(T, index);
  BiTreeNode* self = _Find(T, index);
  return parent->left == self ? NULL : parent->left;
}

BiTreeNode* Root(const BiTree& T) {
  return T.root;
}

bool BiTreeEmpty(const BiTree& T) {
  return T.root == NULL;
}

int BiTreeDepth(const BiTree& T) {
  function<int(BiTreeNode*)> Depth = [&](BiTreeNode* root) -> int {
    if (root == NULL)
      return 0;
    int depthLeft = Depth(root->left);
    int depthRight = Depth(root->right);
    int depth = depthLeft > depthRight ? depthLeft : depthRight;
    return depth + 1;
  };

  return Depth(T.root);
}

status InsertChild(BiTree& T, size_t index, bool LR, BiTree& c) {
  BiTreeNode* self = _Find(T, index);
  BiTreeNode* original = NULL;
  if (LR == true) {
    original = self->left;
    self->left = c.root;
    c.root->right = original;
  } else {
    original = self->right;
    self->right = c.root;
    c.root->left = original;
  }
}

status DeleteChild(BiTree& T, size_t index, bool LR) {
  BiTreeNode* pn = NULL;
  if (LR == true) {
    pn = _Find(T, index)->left;
  } else {
    pn = _Find(T, index)->right;
  }
  function<void(BiTreeNode*)> Traverse = [&](BiTreeNode* root) -> void {
    if (root == NULL || root->data.null == true)
      return;
    Traverse(root->left);
    Traverse(root->right);
    delete root;
  };

  Traverse(pn);
}

status PreOrderTraverse(const BiTree& T, function<void(BiTreeNode*)> Visit) {
  function<void(BiTreeNode*)> Traverse = [&](BiTreeNode* root) -> void {
    if (root == NULL)
      return;
    Visit(root);
    Traverse(root->left);
    Traverse(root->right);
  };

  Traverse(T.root);
}

status InOrderTraverse(const BiTree& T, function<void(BiTreeNode*)> Visit) {
  function<void(BiTreeNode*)> Traverse = [&](BiTreeNode* root) -> void {
    if (root == NULL)
      return;
    Traverse(root->left);
    Visit(root);
    Traverse(root->right);
  };

  Traverse(T.root);
}

status PostOrderTraverse(const BiTree& T, function<void(BiTreeNode*)> Visit) {
  function<void(BiTreeNode*)> Traverse = [&](BiTreeNode* root) -> void {
    if (root == NULL)
      return;
    Traverse(root->left);
    Traverse(root->right);
    Visit(root);
  };

  Traverse(T.root);
}

status LevelOrderTraverse(const BiTree& T, function<void(BiTreeNode*)> Visit) {
  queue<BiTreeNode*> q;
  q.push(T.root);
  while (q.size() != 0) {
    BiTreeNode* n = q.front();
    Visit(n);
    q.pop();
    if (n->left != NULL)
      q.push(n->left);
    if (n->right != NULL)
      q.push(n->right);
  }
}

struct asciinode {
  asciinode *left, *right;

  int edge_length;

  int height;

  int lablen;

  int parent_dir;

  char label[11];
};

int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
int gap = 3;
int print_next;

int MIN(int X, int Y) {
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX(int X, int Y) {
  return ((X) > (Y)) ? (X) : (Y);
}

asciinode* build_ascii_tree_recursive(BiTreeNode* t) {
  asciinode* node;

  if (t == NULL || t->data.null == true)
    return NULL;

  node = (asciinode*)malloc(sizeof(asciinode));
  node->left = build_ascii_tree_recursive(t->left);
  node->right = build_ascii_tree_recursive(t->right);

  if (node->left != NULL) {
    node->left->parent_dir = -1;
  }

  if (node->right != NULL) {
    node->right->parent_dir = 1;
  }

  sprintf(node->label, "(%d,%c)", t->data.index, t->data.value);
  node->lablen = strlen(node->label);

  return node;
}

asciinode* build_ascii_tree(BiTreeNode* t) {
  asciinode* node;
  if (t == NULL)
    return NULL;
  node = build_ascii_tree_recursive(t);
  node->parent_dir = 0;
  return node;
}

void free_ascii_tree(asciinode* node) {
  if (node == NULL)
    return;
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  free(node);
}

void compute_lprofile(asciinode* node, int x, int y) {
  int i, isleft;
  if (node == NULL)
    return;
  isleft = (node->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
  if (node->left != NULL) {
    for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
      lprofile[y + i] = MIN(lprofile[y + i], x - i);
    }
  }
  compute_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
  compute_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_rprofile(asciinode* node, int x, int y) {
  int i, notleft;
  if (node == NULL)
    return;
  notleft = (node->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
  if (node->right != NULL) {
    for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++) {
      rprofile[y + i] = MAX(rprofile[y + i], x + i);
    }
  }
  compute_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
  compute_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_edge_lengths(asciinode* node) {
  int h, hmin, i, delta;
  if (node == NULL)
    return;
  compute_edge_lengths(node->left);
  compute_edge_lengths(node->right);

  if (node->right == NULL && node->left == NULL) {
    node->edge_length = 0;
  } else {
    if (node->left != NULL) {
      for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++) {
        rprofile[i] = -INFINITY;
      }
      compute_rprofile(node->left, 0, 0);
      hmin = node->left->height;
    } else {
      hmin = 0;
    }
    if (node->right != NULL) {
      for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++) {
        lprofile[i] = INFINITY;
      }
      compute_lprofile(node->right, 0, 0);
      hmin = MIN(node->right->height, hmin);
    } else {
      hmin = 0;
    }
    delta = 4;
    for (i = 0; i < hmin; i++) {
      delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }

    if (((node->left != NULL && node->left->height == 1) ||
         (node->right != NULL && node->right->height == 1)) &&
        delta > 4) {
      delta--;
    }

    node->edge_length = ((delta + 1) / 2) - 1;
  }

  h = 1;
  if (node->left != NULL) {
    h = MAX(node->left->height + node->edge_length + 1, h);
  }
  if (node->right != NULL) {
    h = MAX(node->right->height + node->edge_length + 1, h);
  }
  node->height = h;
}

void print_level(asciinode* node, int x, int level) {
  int i, isleft;
  if (node == NULL)
    return;
  isleft = (node->parent_dir == -1);
  if (level == 0) {
    for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2)); i++) {
      printf(" ");
    }
    print_next += i;
    printf("%s", node->label);
    print_next += node->lablen;
  } else if (node->edge_length >= level) {
    if (node->left != NULL) {
      for (i = 0; i < (x - print_next - (level)); i++) {
        printf(" ");
      }
      print_next += i;
      printf("/");
      print_next++;
    }
    if (node->right != NULL) {
      for (i = 0; i < (x - print_next + (level)); i++) {
        printf(" ");
      }
      print_next += i;
      printf("\\");
      print_next++;
    }
  } else {
    print_level(node->left,
                x - node->edge_length - 1,
                level - node->edge_length - 1);
    print_level(node->right,
                x + node->edge_length + 1,
                level - node->edge_length - 1);
  }
}

void print_ascii_tree(BiTreeNode* t) {
  asciinode* proot;
  int xmin, i;
  if (t == NULL)
    return;
  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
    lprofile[i] = INFINITY;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
    xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++) {
    print_next = 0;
    print_level(proot, -xmin, i);
    printf("\n");
  }
  if (proot->height >= MAX_HEIGHT) {
    printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot);
}

int main() {
  BiTree T1;
  InitBiTree(T1);
  CreateBiTree(T1, {
                       ElemType({'a', 0, false}),
                       ElemType({'g', 1, false}),
                       ElemType({'a', 2, false}),
                       ElemType({'g', 3, false}),
                       ElemType({'y', 4, false}),
                       ElemType({'1', 5, false}),
                       ElemType({'@', 6, false}),
                       ElemType({'t', 7, false}),
                       ElemType({'i', 8, false}),
                       ElemType({'p', 9, false}),
                   });

  BiTree T2;
  InitBiTree(T2);
  CreateBiTree(T2, {ElemType({'h', 10, false}),
                    ElemType({'z', 11, false}),
                    ElemType({'y', 12, false}),
                    ElemType(),
                    ElemType()});

  // print_ascii_tree(T1.root);
  // print_ascii_tree(T2.root);
  InsertChild(T1, 6, true, T2);
  print_ascii_tree(T1.root);
  // ClearBiTree(T1);
  // print_ascii_tree(T1.root);

  printf("%c\n", Parent(T1, 9)->data.value);
  printf("%c\n", LeftSibling(T1, 9)->data.value);
  printf("%c\n", RightSibling(T1, 9)->data.value);
  // printf("%c\n", LeftChild(T1, 12));
  // printf("%c\n", RightChild(T1, 12));
  printf("%d\n", BiTreeDepth(T1));

  PreOrderTraverse(T1, _PrintNode);
  printf("\n");
  InOrderTraverse(T1, _PrintNode);
  printf("\n");
  LevelOrderTraverse(T1, _PrintNode);
  printf("\n");
}