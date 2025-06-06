#include "gcn.h"

Graph* createGraph(){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = 0;
    graph->featureNum = 0;
    return graph;
}

void printFeatureGraph(Graph* graph){
    printf("节点数：%d\n", graph->numNodes);
    printf("特征维度：%d\n", graph->featureNum);
    printf("特征矩阵：");
    for(int i = 0; i < graph->numNodes; i++){
        printf("\n");
        for(int j = 0; j < graph->featureNum; j++){
            printf("%.2lf ", graph->nodes[i].features[j]);
        }
    }
    printf("\n");
}

void printAdjMatrix(Graph* graph){
    printf("邻接矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->numNodes; j++){
            printf("%d ", graph->adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void addEdge(Graph* graph, int node1, int node2){
    graph->adjacencyMatrix[node1][node2] = 1;
    graph->adjacencyMatrix[node2][node1] = 1;
}

void selfLoop(Graph* graph){
    for(int i = 0; i < graph->numNodes; i++){
        graph->adjacencyMatrix[i][i] = 1;
    }
}

void printDegreeMatrix(Graph* graph){
    printf("度矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->numNodes; j++){
            printf("%d ", graph->degreeMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void computeDegreeMatrix(Graph* graph){
    for(int i = 0; i < graph->numNodes; i++){
        int degree = 0;
        for(int j = 0; j < graph->numNodes; j++){
            degree += graph->adjacencyMatrix[i][j];
        }
        graph->degreeMatrix[i][i] = degree;
    }
}