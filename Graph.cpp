#include <cstdio>
#include <functional>
#include <list>
#include <tuple>
#include <vector>

using namespace std;
using u_int = unsigned int;
using status = void;

enum Error {
  NO_SUCH_VEX,
  NO_ADJ_VEX,
};

struct VertexType {
  u_int index;
  char value;
};

struct ArcNode {
  u_int adjvex;
  int* info = NULL;
  ArcNode* next = NULL;
};

struct VNode {
  VertexType data;
  ArcNode* arcs = NULL;
};

struct Graph {
  vector<VNode> vertices;
  size_t vexNum, arcNum;
  int8_t kind;
};

status CreateGraph(Graph& G, vector<VertexType> nodes, vector<ArcNode> arcs);
status DestroyGraph(Graph& G);
size_t LocateVex(Graph& G, u_int index);
VertexType GetVex(Graph& G, u_int index);
status PutVex(Graph& G, u_int index, VertexType& data);
ArcNode* FirstAdjVex(Graph& G, u_int index);
ArcNode* NextAdjVex(Graph& G, u_int v, u_int w);
status InsertVex(Graph& G, VertexType data);
status DeleteVex(Graph& G, u_int index);
status InsertArc(Graph& G, u_int v, u_int w, int* info);
status DeleteArc(Graph& G, u_int v, u_int w);
status DFSTraverse(Graph& G, function<void(VNode&)> Visit);
status BFSTraverse(Graph& G, function<void(VNode&)> Visit);

status CreateGraph(Graph& G, vector<VertexType> nodes, vector<tuple<u_int, u_int, int*>> arcs) {
  G.arcNum = arcs.size();
  G.vexNum = nodes.size();
  G.vertices = {};
  for (auto node : nodes)
    InsertVex(G, node);
  for (auto arc : arcs)
    InsertArc(G, get<0>(arc), get<1>(arc), get<2>(arc));
}

status DestroyGraph(Graph& G) {
  for (auto v : G.vertices) {
    ArcNode* arc = v.arcs;
    while (arc != NULL) {
      ArcNode* next = arc->next;
      delete arc->info;
      delete arc;
      arc = next;
    }
  }
  G.vertices.clear();
}

size_t LocateVex(Graph& G, u_int index) {
  size_t i = 0;
  for (auto v : G.vertices) {
    if (v.data.index == index) {
      return i;
    }
    i++;
  }
  throw Error::NO_SUCH_VEX;
}

VertexType GetVex(Graph& G, u_int index) {
  return G.vertices[LocateVex(G, index)].data;
}

status PutVex(Graph& G, u_int index, VertexType& data) {
  G.vertices[LocateVex(G, index)].data = data;
}

ArcNode* FirstAdjVex(Graph& G, u_int index) {
  ArcNode* head = G.vertices[LocateVex(G, index)].arcs;
  return head;
}

ArcNode* NextAdjVex(Graph& G, u_int v, u_int w) {
  ArcNode* arc = G.vertices[LocateVex(G, v)].arcs;
  while (arc != NULL) {
    if (arc->adjvex == w)
      return arc->next;
    arc = arc->next;
  }
  return NULL;
}

status InsertVex(Graph& G, VertexType data) {
  G.vertices.push_back({data, NULL});
}

status DeleteVex(Graph& G, u_int index) {
  size_t i = LocateVex(G, index);
  ArcNode* arc = G.vertices[i].arcs;
  while (arc != NULL) {
    ArcNode* next = arc->next;
    delete arc->info;
    delete arc;
    arc = next;
  }
  G.vertices.erase(G.vertices.begin() + i);
}

status InsertArc(Graph& G, u_int v, u_int w, int* info) {
  size_t i = LocateVex(G, v);
  ArcNode* head = G.vertices[i].arcs;
  if (head == NULL) {
    G.vertices[i].arcs = head = new ArcNode();
  } else {
    while (head->next != NULL)
      head = head->next;
    head = head->next = new ArcNode();
  }
  head->adjvex = w;
  head->info = info;
  head->next = NULL;
}

status DeleteArc(Graph& G, u_int v, u_int w) {
  VNode vex = G.vertices[LocateVex(G, v)];
  ArcNode* __head = new ArcNode();
  ArcNode* head = __head;
  __head->next = vex.arcs;

  while (head != NULL && head->next != NULL) {
    if (head->next->adjvex == w) {
      head->next = head->next->next;
    }
    head = head->next;
  }

  vex.arcs = __head->next;
}

status DFSTraverse(Graph& G, function<void(VNode&)> Visit) {
  bool* visited = new bool[G.vertices.size()];
  for (size_t i = 0; i < G.vertices.size(); i++)
    visited[i] = false;

  function<void(size_t)> DFS = [&](size_t i) -> void {
    visited[i] = true;
    Visit(G.vertices[i]);
    ArcNode* arc = G.vertices[i].arcs;
    while (arc != NULL) {
      size_t ii = LocateVex(G, arc->adjvex);
      if (visited[ii] == false)
        DFS(ii);
      arc = arc->next;
    }
  };

  DFS(0);
  delete[] visited;
}

status BFSTraverse(Graph& G, function<void(VNode&)> Visit) {
  bool* visited = new bool[G.vertices.size()];
  for (size_t i = 0; i < G.vertices.size(); i++)
    visited[i] = false;

  size_t s = 0;
  list<size_t> queue;
  visited[s] = true;
  queue.push_back(s);

  while (!queue.empty()) {
    s = queue.front();
    VNode v = G.vertices[s];
    Visit(v);
    queue.pop_front();

    ArcNode* arc = v.arcs;
    while (arc != NULL) {
      size_t ii = LocateVex(G, arc->adjvex);
      if (visited[ii] == false) {
        visited[ii] = true;
        queue.push_back(ii);
      }
      arc = arc->next;
    }
  }
  delete[] visited;
}

int main() {
  int selection = -1;
  size_t I = -1;
  vector<Graph> G = {};
  while (selection != 0) {
    system("cls");
    printf("\n");
    printf("    Graph With Adjacency List Representation    \n");
    printf("------------------------------------------------\n");
    printf("      1.CreateGraph       10.InsertArc          \n");
    printf("      2.DestroyGraph      11.DeleteArc          \n");
    printf("      3.LocateVex         12.DFSTraverse        \n");
    printf("      4.GetVex            13.BFSTraverse        \n");
    printf("      5.PutVex            14.AddGraph           \n");
    printf("      6.FirstAdjVex                             \n");
    printf("      7.NextAdjVex                              \n");
    printf("      8.InsertVex                               \n");
    printf("      9.DeleteVex                               \n");
    printf("                                                \n");
    printf("      0.Exit              15.Write              \n");
    printf("     -1.Debug             16.Read               \n");
    printf("------------------------------------------------\n");
    printf("\n");
    printf("请选择你的操作[0~22]:");
    scanf("%d", &selection);
    try {
      switch (selection) {
        case -1:
          I = 0;
          for (auto _G : G) {
            printf("图%llu\n", I++);
            for (auto v : _G.vertices) {
              printf("(%u,%c)", v.data.index, v.data.value);
              ArcNode* arc = v.arcs;
              if (arc != NULL)
                printf("-");

              while (arc != NULL) {
                if (arc->info == NULL)
                  printf("-> %u ", arc->adjvex);
                else
                  printf("-> (%u,%d) ", arc->adjvex, *arc->info);

                arc = arc->next;
              }
              printf("\n");
            }
            printf("\n\n");
          }
          break;
        case 0:
          printf("欢迎下次再使用本系统！\n");
          break;
        case 14:
          G.push_back(Graph());
          printf("创建成功!当前id范围:[0, %llu]\n", G.size() - 1);
          break;
        case 15: {
          size_t s = G.size();
          if (s <= 0) {
            printf("没有图!\n");
            break;
          }
          FILE* fp = fopen("G", "w");
          fwrite(&s, sizeof(size_t), 1, fp);
          for (auto _G : G) {
            size_t s = _G.vertices.size();
            fwrite(&s, sizeof(size_t), 1, fp);
            vector<tuple<u_int, u_int, bool, int>> arcs = {};
            for (auto vex : _G.vertices) {
              fwrite(&vex.data, sizeof(VertexType), 1, fp);
              ArcNode* arc = vex.arcs;
              while (arc != NULL) {
                arcs.push_back({vex.data.index,
                                arc->adjvex,
                                arc->info == NULL,
                                arc->info == NULL ? 0 : *arc->info});
                arc = arc->next;
              }
            }
            s = arcs.size();
            fwrite(&s, sizeof(size_t), 1, fp);
            for (auto arc : arcs)
              fwrite(&arc, sizeof(arc), 1, fp);
          }
          fclose(fp);
          printf("写出成功!\n");
          break;
        }
        case 16: {
          G = {};
          FILE* fp = fopen("G", "r");
          size_t s;
          fread(&s, sizeof(size_t), 1, fp);
          for (size_t i = 0; i < s; i++) {
            size_t _s;
            fread(&_s, sizeof(size_t), 1, fp);
            vector<VertexType> vexs = {};
            for (size_t i = 0; i < _s; i++) {
              VertexType* vex = new VertexType();
              fread(vex, sizeof(VertexType), 1, fp);
              vexs.push_back(*vex);
              delete vex;
            }
            fread(&_s, sizeof(size_t), 1, fp);
            vector<tuple<u_int, u_int, int*>> arcs = {};
            for (size_t i = 0; i < _s; i++) {
              auto arc = new tuple<u_int, u_int, bool, int>();
              fread(arc, sizeof(*arc), 1, fp);
              bool null = get<2>(*arc);
              arcs.push_back({
                  get<0>(*arc),
                  get<1>(*arc),
                  null ? NULL : new int(get<3>(*arc)),
              });
              delete arc;
            }
            Graph _G;
            CreateGraph(_G, vexs, arcs);
            G.push_back(_G);
          }
          fclose(fp);
          printf("读入成功!\n");
          break;
        }
        default:
          if (selection >= 1 && selection <= 13) {
            printf("输入图的Id:");
            if (scanf("%llu", &I) != 0) {
              if (G.size() != 0 && I <= G.size() - 1) {
                switch (selection) {
                  case 1: {
                    // u_int index, v, w;
                    // char value;
                    // vector<VertexType> vex = {};
                    // vector<tuple<u_int, u_int, int*>> arc = {};
                    // printf("输入节点的index和value:\n");
                    // while (scanf("%u %c", &index, &value) > 0)
                    //   vex.push_back({index, value});
                    // printf("输入边的v w:\n");
                    // while (scanf("%u %u", &v, &w) > 0)
                    //   arc.push_back({v, w, NULL});
                    // printf("输入上述边的info:\n");
                    // for (size_t i = 0; i < arc.size(); i++)
                    //   scanf("%d", get<2>(arc[i]));
                    // CreateGraph(G[I], vex, arc);

                    CreateGraph(G[I],
                                {
                                    {1, 'a'},
                                    {2, 'b'},
                                    {3, 'c'},
                                    {4, 'd'},
                                    {5, 'e'},
                                    {6, 'f'},
                                    {7, 'g'},
                                    {8, 'h'},
                                },
                                {
                                    {1, 2, NULL},
                                    {1, 4, NULL},
                                    {3, 2, NULL},
                                    {4, 2, NULL},
                                    {5, 1, NULL},
                                    {5, 6, NULL},
                                    {8, 1, NULL},
                                    {7, 1, NULL},
                                    {4, 1, NULL},
                                    {7, 5, NULL},
                                    {3, 6, NULL},
                                    {3, 5, NULL},
                                    {4, 3, NULL},
                                });
                    printf("创建成功!");
                    break;
                  }
                  case 2: {
                    DestroyGraph(G[I]);
                    G.erase(G.begin() + I);
                    printf("销毁成功!\n");
                    break;
                  }
                  case 8: {
                    VertexType v;
                    printf("输入index和value:\n");
                    if (scanf("%u %c", &v.index, &v.value)) {
                      InsertVex(G[I], v);
                    }
                    printf("插入成功!\n");
                    break;
                  }
                  case 3:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                  case 9:
                  case 10:
                  case 11: {
                    u_int index;
                    u_int w;
                    VertexType v;
                    ArcNode* pArc = NULL;
                    printf("输入节点的index:");
                    if (scanf("%u", &index) == 0) {
                      printf("无效输入!");
                      break;
                    }
                    switch (selection) {
                      case 3:
                        printf("该顶点在数组中的索引: %llu\n", LocateVex(G[I], index));
                        break;
                      case 4:
                        v = GetVex(G[I], index);
                        printf("(%u,%c)\n", v.index, v.value);
                        break;
                      case 5:
                        printf("输入新的index和value:");
                        if (scanf("%u %c", &v.index, &v.value)) {
                          PutVex(G[I], index, v);
                        }
                        break;
                      case 6:
                        pArc = FirstAdjVex(G[I], index);
                        if (pArc == NULL) {
                          printf("未找到边!");
                          break;
                        }
                        if (pArc->info == NULL)
                          printf("(%u,%u)\n", index, pArc->adjvex);
                        else
                          printf("(%u,%u,%d)\n", index, pArc->adjvex, *pArc->info);
                        break;
                      case 7:
                        printf("输入w:");
                        if (scanf("%u", &w)) {
                          pArc = NextAdjVex(G[I], index, w);
                          if (pArc == NULL) {
                            printf("未找到边!\n");
                            break;
                          }
                          if (pArc->info == NULL)
                            printf("(%u,%u)\n", index, pArc->adjvex);
                          else
                            printf("(%u,%u,%d)\n", index, pArc->adjvex, *pArc->info);
                        }
                        break;
                      case 9:
                        DeleteVex(G[I], index);
                        printf("删除成功!\n");
                        break;
                      case 10:
                        printf("输入w:");
                        if (scanf("%u", &w)) {
                          int* info = new int();
                          printf("输入info:\n");
                          if (scanf("%d", info))
                            InsertArc(G[I], index, w, info);
                          else
                            InsertArc(G[I], index, w, NULL);
                        }
                        break;
                      case 11:
                        printf("输入w:");
                        if (scanf("%u", &w)) {
                          DeleteArc(G[I], index, w);
                        }
                        break;
                    }
                    break;
                  }
                  case 12:
                    DFSTraverse(G[I], [](VNode& v) -> void {
                      printf("(%u,%c)\n", v.data.index, v.data.value);
                    });
                    break;

                  case 13:
                    BFSTraverse(G[I], [](VNode& v) -> void {
                      printf("(%u,%c)\n", v.data.index, v.data.value);
                    });
                    break;
                }
                break;
              }
              printf("无效输入\n");
            }
          }
          break;
      }
    } catch (Error e) {
    }

    getchar();
    getchar();
  }
}
