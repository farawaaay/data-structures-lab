/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <string>
#include <vector>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

using namespace std;

typedef int status;

template <typename T>
struct Node {
  T data;
  Node* next;
};

template <typename T>
struct LinkedList {
  int length;
  bool init;
  Node<T>* head;
};

// using IntLinkedList = LinkedList<int>;

/*-----page 19 on textbook ---------*/
// template <typename T>
// status InitList(LinkedList<T>&);
// template <typename T>
// status DestroyList(LinkedList<T>&);
// template <typename T>
// status ClearList(LinkedList<T>&);
// template <typename T>
// bool ListEmpty(const LinkedList<T>);
// template <typename T>
// size_t ListLength(const LinkedList<T>);
// template <typename T>
// status GetElem(const LinkedList<T>, size_t, T&);
// template <typename T>
// size_t LocateElem(const LinkedList<T>, const T);
// template <typename T>
// status PriorElem(LinkedList<T>, T cur, T& pre_e);
// template <typename T>
// status NextElem(LinkedList<T>, T cur, T& next_e);
// template <typename T>
// status ListInsert(LinkedList<T>&, size_t i, T e);
// template <typename T>
// status ListDelete(LinkedList<T>&, size_t i, T& e);
// template <typename T>
// status ListTraverse(LinkedList<T>, function<void(Node<T>)>);

template <typename T>
status InitList(LinkedList<T>& L) {
  L.head = (Node<T>*)malloc(sizeof(Node<T>));
  if (L.head == NULL)
    return ERROR;
  L.head->next = NULL;
  L.length = 0;
  L.init = true;
  return OK;
}

template <typename T>
status DestroyList(LinkedList<T>& L) {
  Node<T>* tmp = L.head->next;
  Node<T>* tmp2 = tmp;
  while (tmp != NULL) {
    tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  free(L.head);
  L.head = NULL;
  L.length = 0;
  L.init = false;
  return OK;
}

template <typename T>
status ClearList(LinkedList<T>& L) {
  Node<T>* tmp = L.head->next;
  Node<T>* tmp2 = tmp;
  while (tmp != NULL) {
    tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  L.head->next = NULL;
  L.length = 0;
  return OK;
}

template <typename T>
bool ListEmpty(const LinkedList<T> L) {
  if (L.length == 0)
    return true;
  else
    return false;
}

template <typename T>
size_t ListLength(const LinkedList<T> L) {
  return L.length;
}

template <typename T>
status GetElem(const LinkedList<T> L, size_t i, T& e) {
  if (i < 0 || i > L.length - 1) {
    return ERROR;
  }
  Node<T>* ele = L.head->next;
  for (int j = 0; j < i; j++) {
    ele = ele->next;
  }
  e = ele->data;
  return OK;
}

template <typename T>
int LocateElem(const LinkedList<T> L, const T e) {
  Node<T>* ele = L.head;
  for (int i = 0; i < L.length; i++) {
    ele = ele->next;
    if (ele->data == e)
      return i;
  }
  return -1;
}

template <typename T>
status PriorElem(const LinkedList<T> L, const T cur_e, T& pre_e) {
  int loc = LocateElem(L, cur_e);
  if (loc == 0 || loc == -1)
    return ERROR;
  else {
    loc--;
    GetElem(L, loc, pre_e);
    return OK;
  }
}

template <typename T>
status NextElem(const LinkedList<T> L, const T cur_e, T& next_e) {
  int loc = LocateElem(L, cur_e);
  if (loc == L.length - 1 || loc == -1)
    return ERROR;
  else {
    loc++;
    GetElem(L, loc, next_e);
    return OK;
  }
}

template <typename T>
status ListInsert(LinkedList<T>& L, size_t i, T e) {
  if (i == ListLength(L) && i != 0) {
    Node<T>* ele = L.head;
    while (ele->next != NULL)
      ele = ele->next;

    ele->next = new Node<T>();
    ele->next->data = e;
  }
  if (i < 0 || i > L.length)
    return ERROR;
  Node<T>* ele = L.head;
  for (int j = 0; j < i; j++) {
    ele = ele->next;
  }
  Node<T>* tmp = (Node<T>*)malloc(sizeof(Node<T>));
  tmp->data = e;
  tmp->next = ele->next;
  ele->next = tmp;
  L.length++;
  return OK;
}

template <typename T>
status ListDelete(LinkedList<T>& L, size_t i, T& e) {
  if (i < 0 || L.length == 0 || i > L.length - 1)
    return ERROR;
  L.length--;
  GetElem<T>(L, i, e);
  Node<T>* ele = L.head;
  for (int j = 0; j < i; j++) {
    ele = ele->next;
  }
  Node<T>* tmp = ele->next;
  ele->next = tmp->next;
  free(tmp);
  return OK;
}

template <typename T>
status ListTraverse(const LinkedList<T> L, function<void(Node<T>)> cb) {
  Node<T>* ele = L.head->next;
  while (ele != NULL) {
    cb(*ele);
    ele = ele->next;
  }
  return OK;
}

int main(void) {
  using ElemType = int;
  int i = 0;
  auto Lists = vector<LinkedList<ElemType>>{LinkedList<ElemType>()};
  int op = 1;
  while (op) {
    system("cls");
    printf("\n\n");
    printf("    Menu for Linear Table On Linked Structure    \n");
    printf("-------------------------------------------------\n");
    printf("       1. InitList         7. LocateElem         \n");
    printf("       2. DestroyList      8. PriorElem          \n");
    printf("       3. ClearList        9. NextElem           \n");
    printf("       4. ListEmpty       10. ListInsert         \n");
    printf("       5. ListLength      11. ListDelete         \n");
    printf("       6. GetElem         12. ListTraverse       \n");
    printf("       0. Exit            13. ReadList           \n");
    printf("                          14. WriteList          \n");
    printf("                          15. AddList            \n");
    printf("                          16. ChangeList         \n");
    printf("                          17. CurrentList        \n");
    printf("-------------------------------------------------\n");
    printf("请选择你的操作[0~12]:");
    scanf("%d", &op);
    if (op != 1 && Lists[i].init == false) {
      printf("线性表未创建！\n");
      getchar();
      getchar();
      continue;
    }
    switch (op) {
      case 1:
        //printf("\n----InitList功能待实现！\n");
        if (InitList(Lists[i]) == OK)
          printf("线性表创建成功！\n");
        else
          printf("线性表创建失败！\n");
        getchar();
        getchar();
        break;
      case 2:
        DestroyList(Lists[i]);
        getchar();
        getchar();
        break;
      case 3:
        ClearList(Lists[i]);
        getchar();
        getchar();
        break;
      case 4:
        printf("列表是否为空：%s", ListEmpty(Lists[i]) ? "是" : "否");
        getchar();
        getchar();
        break;
      case 5:
        printf("列表长度：%d", ListLength(Lists[i]));
        getchar();
        getchar();
        break;
      case 6: {
        if (ListEmpty(Lists[i])) {
          printf("列表为空！");
          getchar();
          getchar();
          break;
        }
        int __i;
        printf("输入 index：\n");
        scanf("%u", &__i);
        ElemType e;
        if (GetElem(Lists[i], __i, e) != OK) {
          printf("获取出错！\n");
        } else {
          printf("值：%d", e);
        }
        getchar();
        getchar();
        break;
      }
      case 7: {
        ElemType e;
        printf("输入值：\n");
        scanf("%d", &e);
        int index;
        if ((index = LocateElem(Lists[i], e)) == -1) {
          printf("未能找到！\n");
        } else {
          printf("Index：%d \n", index);
        }
        getchar();
        getchar();
        break;
      }
      case 8: {
        ElemType e;
        ElemType p_e;
        printf("输入值：");
        scanf("%d", &e);
        if (PriorElem(Lists[i], e, p_e) == OK) {
          printf("前驱：%d", p_e);
        } else {
          printf("失败！");
        }
        getchar();
        getchar();
        break;
      }
      case 9: {
        ElemType e;
        ElemType n_e;
        printf("输入值：");
        scanf("%d", &e);
        if (NextElem(Lists[i], e, n_e) == OK) {
          printf("后继：%d", n_e);
        } else {
          printf("失败！");
        }
        getchar();
        getchar();
        break;
      }
      case 10: {
        ElemType e;
        size_t __i;
        printf("先后输入 index 和 item：\n");
        while (scanf("%u %d", &__i, &e) && __i <= ListLength(Lists[i]) - 1) {
          if (ListInsert(Lists[i], __i, e) == OK)
            printf("成功在%d插入%d\n", __i, e);
          else
            printf("未能插入任何元素！\n");
          printf("先后输入 index 和 item：\n");
        }
        getchar();
        getchar();
        break;
      }
      case 11: {
        int __i;
        printf("输入 index：\n");
        while (scanf("%d", &__i)) {
          ElemType e;
          if (ListDelete(Lists[i], __i, e) == OK)
            printf("删除的值为：%d\n", e);
          else
            printf("未能删除任何元素！\n");
          printf("输入 index：\n");
        }
        getchar();
        getchar();
        break;
      }
      case 12:
        if (!ListTraverse<ElemType>(Lists[i],
                                    [](auto ele) -> void {
                                      printf("%d ", ele.data);
                                      return;
                                    }))

          printf("线性表是空表！\n");
        getchar();
        getchar();
        break;
      case 13: {
        FILE* fp;
        char filename[30];
        printf("Input file name: ");
        scanf("%s", filename);

        Lists[i].length = 0;
        if ((fp = fopen(filename, "r")) == NULL) {
          printf("File open error\n");
          return 1;
        }
        // while (fread(&Lists[i].elem[Lists[i].length], sizeof(int), 1, fp))
        //   Lists[i].length++;
        break;
      }
      case 14: {
        FILE* fp;
        char filename[30];
        printf("Input file name: ");
        scanf("%s", filename);

        if ((fp = fopen(filename, "w")) == NULL) {
          printf("File open error\n");
          getchar();
          getchar();
          break;
        }

        ListTraverse<ElemType>(
            Lists[i],
            [&](auto ele) -> void {
              fwrite(&ele.data, sizeof(decltype(ele.data)), Lists[i].length, fp);
            });

        fclose(fp);
        getchar();
        getchar();
        break;
      }

      case 15: {
        LinkedList<ElemType> l;
        InitList(l);
        // l.elem = new int();
        Lists.push_back(l);
        printf("添加一张空表成功！\n");
        getchar();
        getchar();
        break;
      }
      case 16: {
        int __i;
        printf("输入表编号(0~%d):", Lists.size() - 1);
        while (!(scanf("%u", &__i) && (__i <= Lists.size() - 1))) {
          printf("输入表编号(0~%d):", Lists.size() - 1);
        }
        i = __i;
        printf("切换到表%d成功！\n", __i);
        getchar();
        getchar();
        break;
      }
      case 17: {
        printf("当前是表%d\n", i);

        getchar();
        getchar();
        break;
      }

      case 0:
        break;
    }
  }
  printf("欢迎下次再使用本系统！\n");
  getchar();
}
