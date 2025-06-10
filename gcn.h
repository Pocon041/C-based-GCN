#ifndef GCN_H
#define GCN_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>

#define MAX_NODES 100
#define MAX_FEATURES 100

#define OUT_FEATURES 4 //输出特征维度
#define LEARNING_RATE 0.01 //学习率
#define MOMENTUM_RATE 0.9   // 保持动量率

//数据集

typedef struct Node{
    double features[MAX_FEATURES];  //特征向量
    double labels[OUT_FEATURES];    //标签
}Node;


typedef struct Graph{
    int featureNum; //特征维度
    int numNodes; //节点数量
    Node nodes[MAX_NODES]; //特征数组
    int adjacencyMatrix[MAX_NODES][MAX_NODES]; //邻接矩阵
    double featureMatrix[MAX_NODES][MAX_FEATURES]; //特征矩阵
    double normalizedAdjMatrix[MAX_NODES][MAX_NODES]; //归一化邻接矩阵
    int degreeMatrix[MAX_NODES][MAX_NODES]; //度矩阵
    double degreeMatrix2[MAX_NODES][MAX_NODES]; //度矩阵的逆矩阵的平方根
    double weightMatrix[MAX_FEATURES][OUT_FEATURES]; //权重矩阵
}Graph;

//操作集
Graph* createGraph(void);

bool isConnected(Graph* graph);

void printFeatureGraph(Graph* graph);
void printAdjMatrix(Graph* graph);
void printDegreeMatrix(Graph* graph);
void printNormalizedAdjMatrix(Graph* graph);
void printWeightMatrix(Graph* graph);

void addEdge(Graph* graph, int node1, int node2);

void computeFeatureMatrix(Graph* graph);
void selfLoop(Graph* graph);
void computeDegreeMatrix(Graph* graph);
void computeDegreeMatrix2(Graph* graph);
void normalizeAdjMatrix(Graph* graph);
void randomWeightMatrix(Graph* graph);


void aggregateFeatureMatrix(Graph* graph);
void linearTransform(Graph* graph,double output[MAX_NODES][OUT_FEATURES]); 

// 反向传播相关函数
double computeLoss(Graph* graph, double output[MAX_NODES][OUT_FEATURES]);
void computeGradients(Graph* graph, double output[MAX_NODES][OUT_FEATURES], double gradients[MAX_FEATURES][OUT_FEATURES]);
void updateWeight(Graph* graph, double gradients[MAX_FEATURES][OUT_FEATURES]);
void backward(Graph* graph, double output[MAX_NODES][OUT_FEATURES]);

void freeGraph(Graph* graph);

#endif