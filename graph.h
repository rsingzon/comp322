#ifndef GRAPH_ADJ_LIST_H
#define GRAPH_ADJ_LIST_H

#include "edge.h"

int compareTo(const Edge& edge1, const Edge& edge2);
Edge* append(Edge* root, int origin, int destination, int weight);
void print(Edge* root);
Edge* deleteFirst(Edge* root);
void deleteList(Edge* root);
Edge** organizeList(Edge* root, int numberVertices);
void printOrganized(Edge** adjList, int numberVertices);

#endif
