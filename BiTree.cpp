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
  INIT,
  NOT_INIT,
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
    throw Error::INIT;
  }
}

status DestroyBiTree(BiTree& T) {
  if (T.init != false) {
    ClearBiTree(T);
    T.init = false;
  } else {
    throw Error::NOT_INIT;
  }
}

status CreateBiTree(BiTree& T, vector<ElemType> def) {
  if (!T.init)
    throw Error::NOT_INIT;
  if (def.size() == 0)
    throw Error::WRONG_DEF;

  size_t i = 0;
  size_t l = def.size();

  function<void(BiTreeNode*&)> Create = [&](BiTreeNode*& n) -> void {
    if (i > l)
      return;
    ElemType data = def[i++];
    if (data.null == true)
      return;
    (n = new BiTreeNode())->data = data;
    Create(n->left);
    Create(n->right);
  };

  Create(T.root);
}

status ClearBiTree(BiTree& T) {
  if (!T.init)
    throw Error::NOT_INIT;

  PostOrderTraverse(T, [](BiTreeNode* pn) -> void {
    delete pn;
  });
  T.root = NULL;
}

status Value(const BiTree& T, size_t index, ElemType& value) {
  if (!T.init)
    throw Error::NOT_INIT;
  value = _Find(T, index)->data;
}

status Assign(BiTree& T, size_t index, ElemType& value) {
  if (!T.init)
    throw Error::NOT_INIT;
  _Find(T, index)->data = value;
}

BiTreeNode* _Find(const BiTree& T, size_t index) {
  if (!T.init)
    throw Error::NOT_INIT;
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
  if (!n)
    throw Error::NO_SUCH_NODE;
  printf("(%llu,%c)\n", n->data.index, n->data.value);
}

BiTreeNode* Parent(const BiTree& T, size_t index) {
  if (!T.init)
    throw Error::NOT_INIT;
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
  return parent->right == self ? NULL : parent->right;
}

BiTreeNode* Root(const BiTree& T) {
  if (!T.init)
    throw Error::NOT_INIT;
  return T.root;
}

bool BiTreeEmpty(const BiTree& T) {
  return Root(T) == NULL;
}

int BiTreeDepth(const BiTree& T) {
  if (!T.init)
    throw Error::NOT_INIT;
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
    c.root->right = original;
  }
}

status DeleteChild(BiTree& T, size_t index, bool LR) {
  if (!T.init)
    throw Error::NOT_INIT;
  function<void(BiTreeNode*&)> Traverse = [&](BiTreeNode*& root) -> void {
    if (root == NULL || root->data.null == true)
      return;
    Traverse(root->left);
    Traverse(root->right);
    delete root;
    root = NULL;
  };
  if (LR == true) {
    Traverse(_Find(T, index)->left);
  } else {
    Traverse(_Find(T, index)->right);
  }
}

status PreOrderTraverse(const BiTree& T, function<void(BiTreeNode*)> Visit) {
  if (!T.init)
    throw Error::NOT_INIT;
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
  if (!T.init)
    throw Error::NOT_INIT;
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
  if (!T.init)
    throw Error::NOT_INIT;
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
  if (!T.init)
    throw Error::NOT_INIT;
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

  sprintf(node->label, "(%llu,%c)", t->data.index, t->data.value);
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
  int selection = -1;
  size_t I = -1;
  size_t index = -1;
  vector<BiTree> trees = {};
  while (selection != 0) {
    system("cls");
    printf("\n");
    printf("  BiTree With Binary Linked List Representation \n");
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
    printf("     10.Parent           20.LevelOrderTraverse  \n");
    printf("                                                \n");
    printf("      0.Exit             21.Write               \n");
    printf("     -1.Debug            22.Read                \n");
    printf("------------------------------------------------\n");
    printf("\n");
    printf("请选择你的操作[0~22]:");
    scanf("%d", &selection);
    try {
      switch (selection) {
        case -1:
          I = 0;
          for (auto tree : trees) {
            printf("id为%llu的树:\n", I++);
            if (!BiTreeEmpty(tree))
              print_ascii_tree(tree.root);
            else
              printf("为空\n");
          }
          break;
        case 1:
          trees.push_back(BiTree());
          InitBiTree(trees.back());
          printf("创建成功!当前id范围:[0, %llu]\n", trees.size() - 1);
          break;
        case 21: {
          size_t tree_s = trees.size();
          if (tree_s <= 0) {
            printf("没有树!\n");
            break;
          }
          FILE* fp = fopen("trees", "w");
          fwrite(&tree_s, sizeof(size_t), 1, fp);
          for (auto tree : trees) {
            vector<ElemType> elems = {};
            function<void(BiTreeNode*)> Traverse = [&](BiTreeNode* root) -> void {
              if (root == NULL) {
                elems.push_back(ElemType());
                return;
              }

              elems.push_back(root->data);
              Traverse(root->left);
              Traverse(root->right);
            };

            Traverse(tree.root);
            size_t s = elems.size();
            fwrite(&s, sizeof(size_t), 1, fp);
            for (auto elem : elems) {
              fwrite(&elem, sizeof(ElemType), 1, fp);
            }
          }
          fclose(fp);
          printf("写出成功!\n");
          break;
        }
        case 22: {
          trees = {};
          FILE* fp = fopen("trees", "r");
          size_t tree_s;
          fread(&tree_s, sizeof(size_t), 1, fp);
          for (size_t i = 0; i < tree_s; i++) {
            size_t s;
            fread(&s, sizeof(size_t), 1, fp);
            vector<ElemType> elems = {};
            for (size_t i = 0; i < s; i++) {
              ElemType* elem = new ElemType();
              fread(elem, sizeof(ElemType), 1, fp);
              elems.push_back(*elem);
            }
            BiTree T;
            InitBiTree(T);
            CreateBiTree(T, elems);
            trees.push_back(T);
          }
          fclose(fp);
          printf("读取成功!\n");
          break;
        }
        case 0:
          printf("欢迎下次再使用本系统！\n");
          break;
        default:
          if (selection >= 2 && selection <= 20) {
            printf("输入这棵树的Id:");
            if (scanf("%llu", &I) != 0) {
              if (I <= trees.size() - 1) {
                switch (selection) {
                  case 2:
                    DestroyBiTree(trees[I]);
                    trees.erase(trees.begin() + I);
                    printf("删除成功!\n");
                    break;
                  case 3: {
                    vector<ElemType> elems = {};
                    while (true) {
                      int null;
                      char value;
                      size_t index;
                      printf("输入节点是否为空节点 [Y/N/END, 1/0/-1]:\n");
                      if (scanf("%d", &null) != 0 && (null == 0 || null == 1)) {
                        if (null == 0) {
                          printf("输入节点的index和value: ");
                          if (scanf("%llu %c", &index, &value) != 0) {
                            elems.push_back({value, index, null == 1});
                          }
                        } else {
                          elems.push_back(ElemType());
                        }
                      } else
                        break;
                    }
                    CreateBiTree(trees[I], elems);
                    printf("创建成功!\n");
                    print_ascii_tree(trees[I].root);
                    break;
                  }
                  case 4:
                    ClearBiTree(trees[I]);
                    printf("清空成功!\n");
                    break;
                  case 5:
                    printf("是否为空: %c\n", BiTreeEmpty(trees[I]) == true ? 'T' : 'F');
                    break;
                  case 6:
                    printf("树的深度: %d\n", BiTreeDepth(trees[I]));
                    break;
                  case 7:
                    _PrintNode(Root(trees[I]));
                    break;
                  case 9:
                    printf("输入节点的index:");
                    if (scanf("%llu", &index) != 0) {
                      printf("输入节点的index和value:");
                      size_t newIndex;
                      char value;
                      if (scanf("%llu %c", &newIndex, &value) != 0) {
                        ElemType elem = {value, newIndex, false};
                        Assign(trees[I], index, elem);
                        printf("操作成功!\n");
                        break;
                      }
                    }
                  case 8:
                    printf("输入节点的index:");
                    if (scanf("%llu", &index) != 0) {
                      _PrintNode(_Find(trees[I], index));
                      break;
                    }
                  case 10:
                  case 11:
                  case 12:
                  case 13:
                  case 14:
                    printf("输入节点的index:");
                    if (scanf("%llu", &index) != 0) {
                      switch (selection) {
                        case 10:
                          _PrintNode(Parent(trees[I], index));
                          break;
                        case 11:
                          _PrintNode(LeftChild(trees[I], index));
                          break;
                        case 12:
                          _PrintNode(RightChild(trees[I], index));
                          break;
                        case 13:
                          _PrintNode(LeftSibling(trees[I], index));
                          break;
                        case 14:
                          _PrintNode(RightSibling(trees[I], index));
                          break;
                      }
                      break;
                    }
                  case 15:
                  case 16:
                    printf("输入节点的index:");
                    if (scanf("%llu", &index) != 0) {
                      printf("左还是右 [L/R, 0/1]:");
                      int LR;
                      if (scanf("%d", &LR) != 0) {
                        if (selection == 15) {
                          printf("树c的id:");
                          size_t c_tree;
                          if (scanf("%llu", &c_tree) != 0) {
                            InsertChild(trees[I], index, LR == 0, trees[c_tree]);
                            trees.erase(trees.begin() + c_tree);
                            printf("操作成功!\n");
                            break;
                          }
                        } else {
                          DeleteChild(trees[I], index, LR == 0);
                          printf("操作成功!\n");
                          break;
                        }
                      }
                    }
                  case 17:
                    PreOrderTraverse(trees[I], _PrintNode);
                    break;
                  case 18:
                    InOrderTraverse(trees[I], _PrintNode);
                    break;
                  case 19:
                    PostOrderTraverse(trees[I], _PrintNode);
                    break;
                  case 20:
                    LevelOrderTraverse(trees[I], _PrintNode);
                    break;
                }

                break;
              }
            }

            printf("无效输入\n");
          }
          break;
      }
    } catch (Error e) {
      switch (e) {
        case Error::NO_SUCH_NODE:
          printf("该节点不存在!\n");
          break;
        case Error::WRONG_DEF:
          printf("definition错误!\n");
          break;
        case Error::NOT_INIT:
          printf("未初始化!\n");
          break;
        default:
          printf("未知错误!\n");
          break;
      }
    }
    getchar();
    getchar();
  }
}
