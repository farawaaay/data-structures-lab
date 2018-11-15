/* Linear Table On Sequence Structure */
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

template <typename T>
struct LinkedListNode {
  T data;
  LinkedListNode* next;
};

template <typename T>
struct LinkedList {
  int length;
  LinkedListNode<T>* head;
  LinkedList* next;
  int ListID = -1;
};

/*-----page 19 on textbook ---------*/
template <typename T>
status InitList(LinkedList<T>& L);
template <typename T>
status DestroyList(LinkedList<T>& L);
template <typename T>
status ClearList(LinkedList<T>& L);
template <typename T>
bool ListEmpty(const LinkedList<T> L);
template <typename T>
size_t ListLength(const LinkedList<T> L);
template <typename T>
status GetElem(const LinkedList<T> L, size_t i, T& e);
template <typename T>
size_t LocateElem(const LinkedList<T> L, const T e);
template <typename T>
status PriorElem(LinkedList<T> L, T cur, T& pre_e);
template <typename T>
status NextElem(LinkedList<T> L, T cur, T& next_e);
template <typename T>
status ListInsert(LinkedList<T>& L, size_t i, T e);
template <typename T>
status ListDelete(LinkedList<T>& L, size_t i, T& e);
template <typename T>
status ListTraverse(LinkedList<T> L);

template <typename T>
status IntiaList(LinkedList<T>& L) {
  L.head = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
  if (L.head == NULL)
    return ERROR;
  L.head->next = NULL;
  L.length = 0;
  return OK;
}

template <typename T>
status DestroyList(LinkedList<T>& L) {
  LinkedListNode<T>* tmp = L.head->next;
  LinkedListNode<T>* tmp2 = tmp;
  while (tmp != NULL) {
    tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  free(L.head);
  L.head = NULL;
  L.length = 0;
  return OK;
}

template <typename T>
status ClearList(LinkedList<T>& L) {
  LinkedListNode<T>* tmp = L.head->next;
  LinkedListNode<T>* tmp2 = tmp;
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
bool ListEmpty(const LinkedList<T>& L) {
  if (L.length == 0)
    return true;
  else
    return false;
}

template <typename T>
int ListLength(const LinkedList<T>& L) {
  return L.length;
}

template <typename T>
status GetElem(const LinkedList<T>& L, int i, T& e) {
  if (i < 1 || i > L.length) {
    return ERROR;
  }
  LinkedListNode<T>* ele = L.head;
  for (int j = 0; j < i; j++) {
    ele = ele->next;
  }
  e = ele->data;
  return OK;
}

template <typename T>
size_t LocateElem(const LinkedList<T> L, const T e) {
  LinkedListNode<T>* ele = L.head;
  for (int i = 0; i < L.length; i++) {
    ele = ele->next;
    if (ele->data == e)
      return i + 1;
  }
  return 0;
}

template <typename T>
status PriorElem(const LinkedList<T>& L, const T& cur_e, T& pre_e) {
  int loc = LocateElem(L, cur_e);
  if (loc == 0 || loc == 1)
    return ERROR;
  else {
    loc--;
    GetElem(L, loc, pre_e);
    return OK;
  }
}

template <typename T>
status NextElem(const LinkedList<T>& L, const T& cur_e, T& next_e) {
  int loc = LocateElem(L, cur_e);
  if (loc == L.length || loc == 0)
    return ERROR;
  else {
    loc++;
    GetElem(L, loc, next_e);
    return OK;
  }
}

template <typename T>
status ListInsert(LinkedList<T>& L, size_t i, T& e) {
  if (i < 1 || i > L.length + 1)
    return ERROR;
  LinkedListNode<T>* ele = L.head;
  for (int j = 0; j < i - 1; j++) {
    ele = ele->next;
  }
  LinkedListNode<T>* tmp = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
  tmp->data = e;
  tmp->next = ele->next;
  ele->next = tmp;
  L.length++;
  return OK;
}

template <typename T>
status ListDelete(LinkedList<T>& L, size_t i, T& e) {
  if (i < 1 || i > L.length)
    return ERROR;
  L.length--;
  GetElem(L, i, e);
  LinkedListNode<T>* ele = L.head;
  for (int j = 0; j < i - 1; j++) {
    ele = ele->next;
  }
  LinkedListNode<T>* tmp = ele->next;
  ele->next = tmp->next;
  free(tmp);
  return OK;
}

template <typename T>
status ListTraverse(const LinkedList<T>& L) {
  LinkedListNode<T>* ele = L.head->next;
  printf("\n-----------all elements -----------------------\n");
  while (ele != NULL) {
    printf("%d ", ele->data);
    ele = ele->next;
  }
  printf("\n------------------ end ------------------------\n");
  return OK;
}