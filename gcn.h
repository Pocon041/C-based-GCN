#ifndef GCN_H
#define GCN_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX_NODES 100
#define MAX_FEATURES 100

//数据集
typedef struct Node{
    double features[MAX_FEATURES];  //特征向量
}Node;

typedef struct Graph{
    int featureNum; //特征维度
    int numNodes; //节点数量
    Node nodes[MAX_NODES]; //特征数组
    int adjacencyMatrix[MAX_NODES][MAX_NODES]; //邻接矩阵
    double normalizedAdjMatrix[MAX_NODES][MAX_NODES]; //归一化邻接矩阵
    int degreeMatrix[MAX_NODES][MAX_NODES]; //度矩阵
}Graph;

//操作集
Graph* createGraph(void);

void printFeatureGraph(Graph* graph);
void printAdjMatrix(Graph* graph);
void printDegreeMatrix(Graph* graph);

void addEdge(Graph* graph, int node1, int node2);

void selfLoop(Graph* graph);
void computeDegreeMatrix(Graph* graph);
void normalizeAdjMatrix(Graph* graph);

void freeGraph(Graph* graph);

#endif