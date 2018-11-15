/* Linear Table On Sequence Structure */
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType;  //����Ԫ�����Ͷ���

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

struct SqList {  //˳���˳��ṹ���Ķ���
  ElemType* elem;
  size_t length;
  size_t listSize;
  bool operator==(SqList L2) {
    return L2.elem == this->elem &&
           L2.length == this->length &&
           L2.listSize == this->listSize;
  }
};

/*-----page 19 on textbook ---------*/
status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
bool ListEmpty(SqList L);
size_t ListLength(SqList L);
status GetElem(SqList L, size_t i, ElemType& e);
size_t LocateElem(SqList L, ElemType e);
status PriorElem(SqList L, ElemType cur, ElemType& pre_e);
status NextElem(SqList L, ElemType cur, ElemType& next_e);
status ListInsert(SqList& L, size_t i, ElemType e);
status ListDelete(SqList& L, size_t i, ElemType& e);
status ListTraverse(SqList L);  //�򻯹�

/*--------------------------------------------*/
int main(void) {
  SqList L;
  vector<SqList> Lists = {L};
  int op = 1;
  while (op) {
    system("cls");
    printf("\n\n");
    printf("Menu for Linear Table On Sequence Structure \n");
    printf("-------------------------------------------------\n");
    printf("      1. InitList         7. LocateElem\n");
    printf("      2. DestroyList      8. PriorElem\n");
    printf("      3. ClearList        9. NextElem \n");
    printf("      4. ListEmpty       10. ListInsert\n");
    printf("      5. ListLength      11. ListDelete\n");
    printf("      6. GetElem         12. ListTraverse\n");
    printf("      0. Exit            13. ReadList\n");
    printf("                         14. WriteList\n");
    printf("                         15. AddList\n");
    printf("                         16. ChangeList\n");
    printf("                         17. CurrentList\n");
    printf("-------------------------------------------------\n");
    printf("��ѡ����Ĳ���[0~12]:");
    scanf("%d", &op);
    switch (op) {
      case 1:
        //printf("\n----InitList���ܴ�ʵ�֣�\n");
        if (InitList(L) == OK)
          printf("���Ա����ɹ���\n");
        else
          printf("���Ա���ʧ�ܣ�\n");
        getchar();
        getchar();
        break;
      case 2:
        DestroyList(L);
        getchar();
        getchar();
        break;
      case 3:
        ClearList(L);
        getchar();
        getchar();
        break;
      case 4:
        printf("�б��Ƿ�Ϊ�գ�%s", ListEmpty(L) ? "��" : "��");
        getchar();
        getchar();
        break;
      case 5:
        printf("�б��ȣ�%d", ListLength(L));
        getchar();
        getchar();
        break;
      case 6: {
        if (ListEmpty(L)) {
          printf("�б�Ϊ�գ�");
          getchar();
          getchar();
          break;
        }
        int i;
        printf("���� index��\n");
        scanf("%u", &i);
        ElemType e;
        GetElem(L, i, e);
        printf("ֵ��%d", e);
        getchar();
        getchar();
        break;
      }
      case 7: {
        ElemType e;
        scanf("%d", &e);
        printf("ֵ��%d", LocateElem(L, e));
        getchar();
        getchar();
        break;
      }
      case 8: {
        ElemType e;
        ElemType p_e;
        printf("����ֵ��");
        scanf("%d", &e);
        PriorElem(L, e, p_e);
        printf("ǰ����%d", p_e);
        getchar();
        getchar();
        break;
      }
      case 9: {
        ElemType e;
        ElemType n_e;
        printf("����ֵ��");
        scanf("%d", &e);
        NextElem(L, e, n_e);
        printf("��̣�%d", n_e);
        getchar();
        getchar();
        break;
      }
      case 10: {
        ElemType e;
        size_t i;
        printf("�Ⱥ����� index �� item��\n");
        while (scanf("%u", &i) && scanf("%d", &e)) {
          if (ListInsert(L, i, e))
            printf("�ɹ���%d����%d\n", i, e);
          printf("�Ⱥ����� index �� item��\n");
        }
        getchar();
        getchar();
        break;
      }
      case 11: {
        size_t i;
        printf("���� index��\n");
        while (scanf("%u", &i)) {
          ElemType e;
          if (ListDelete(L, i, e))
            printf("ɾ����ֵΪ��%d\n", e);
          printf("���� index��\n");
        }
        getchar();
        getchar();
        break;
      }
      case 12:
        if (!ListTraverse(L))
          printf("���Ա��ǿձ�\n");
        getchar();
        getchar();
        break;
      case 13: {
        FILE* fp;
        char filename[30];
        printf("Input file name: ");
        scanf("%s", filename);

        L.length = 0;
        if ((fp = fopen(filename, "r")) == NULL) {
          printf("File open error\n");
          return 1;
        }
        while (fread(&L.elem[L.length], sizeof(ElemType), 1, fp))
          L.length++;
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
        fwrite(L.elem, sizeof(ElemType), L.length, fp);
        fclose(fp);
        getchar();
        getchar();
        break;
      }

      case 15: {
        SqList l;
        InitList(l);
        Lists.push_back(l);
        printf("���һ�ſձ�ɹ���\n");
        getchar();
        getchar();
        break;
      }
      case 16: {
        int i;
        printf("�������(0~%d):", Lists.size() - 1);
        while (!(scanf("%u", &i) && (i <= Lists.size() - 1))) {
          printf("�������(0~%d):", Lists.size() - 1);
        }
        L = Lists[i];
        printf("�л�����%d�ɹ���\n", i);
        getchar();
        getchar();
        break;
      }
      case 17: {
        int i = 0;
        for (auto l : Lists) {
          if (l == L) {
            printf("��ǰ�Ǳ�%d\n", i);
            break;
          }
          i++;
        }

        getchar();
        getchar();
        break;
      }

      case 0:
        break;
    }
  }
  printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
  getchar();
}  //end of main()

/*--------page 23 on textbook --------------------*/
status InitList(SqList& L) {
  L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
  if (!L.elem)
    exit(OVERFLOW);
  L.length = 0;
  L.listSize = LIST_INIT_SIZE;
  return OK;
}
status ListTraverse(SqList L) {
  size_t i;
  printf("\n-----------all elements -----------------------\n");
  for (i = 0; i < L.length; i++)
    printf("%d ", L.elem[i]);
  printf("\n------------------ end ------------------------\n");
  return L.length;
}
status DestroyList(SqList& L) {
  free(L.elem);
  L.length = 0;
  return OK;
}
status ClearList(SqList& L) {
  free(L.elem);
  L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
  if (!L.elem)
    exit(OVERFLOW);
  L.length = 0;
  return OK;
}
bool ListEmpty(SqList L) {
  if (L.elem != nullptr)
    return L.length == 0;
  else
    return false;
}
size_t ListLength(SqList L) {
  if (L.elem != nullptr)
    return L.length;
  return -1;
}
status GetElem(SqList L, size_t i, ElemType& e) {
  if (L.elem != nullptr)
    if (i >= 0 && i < ListLength(L)) {
      e = L.elem[i];
      return OK;
    }
  return ERROR;
}
size_t LocateElem(
    SqList L,
    ElemType e)
// std::function<bool(ElemType, ElemType)> compare =
// 	[](ElemType e1, ElemType e2) -> bool { return e1 == e2; })
{
  for (size_t i = 0; i < ListLength(L); i++) {
    ElemType _e;
    GetElem(L, i, _e);
    if (e == _e) {
      return i;
    }
  }
  return -1;
}
status PriorElem(SqList L, ElemType cur, ElemType& pre_e) {
  size_t l = LocateElem(L, cur);
  if (l != 0)
    return GetElem(L, l - 1, pre_e);
  return ERROR;
}
status NextElem(SqList L, ElemType cur, ElemType& next_e) {
  size_t l = LocateElem(L, cur);
  if (l != ListLength(L) - 1)
    return GetElem(L, l + 1, next_e);
  return ERROR;
}
status ListInsert(SqList& L, size_t i, ElemType e) {
  if (i < 0 || i >= L.length + 1) {
    return ERROR;
  }
  ElemType *p, *q;
  q = &(L.elem[i]);
  for (p = &(L.elem[L.length - 1]); p >= q; --p)
    *(p + 1) = *p;
  *q = e;
  ++L.length;
  return OK;
}

status ListDelete(SqList& L, size_t i, ElemType& e) {
  if (i < 0 || i >= L.length)
    return ERROR;

  int *p, *q;

  p = &(L.elem[i]);
  e = *p;
  q = L.elem + L.length - 1;
  for (++p; p <= q; ++p) {
    *(p - 1) = *p;
  }
  --L.length;
  return OK;
}