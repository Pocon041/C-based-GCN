#include "gcn.h"
#include "TsMatrix.h"
#include<math.h>

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

void printNormalizedAdjMatrix(Graph* graph){
    printf("归一化邻接矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->numNodes; j++){
            printf("%.2lf ", graph->normalizedAdjMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void addEdge(Graph* graph, int node1, int node2){
    graph->adjacencyMatrix[node1][node2] = 1;
    graph->adjacencyMatrix[node2][node1] = 1;
}

void computeFeatureMatrix(Graph* graph){
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->featureNum; j++){
            graph->featureMatrix[i][j] = graph->nodes[i].features[j];
        }
    }
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

void printWeightMatrix(Graph* graph){
    printf("权重矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->numNodes; j++){
            printf("%.2lf ", graph->weightMatrix[i][j]);
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

void computeDegreeMatrix2(Graph* graph){
    for(int i = 0; i < graph->numNodes; i++){
        graph->degreeMatrix2[i][i] = 1/sqrt(graph->degreeMatrix[i][i]);
    }
}

void normalizeAdjMatrix(Graph* graph){
    //加上自环
    selfLoop(graph);
    //printAdjMatrix(graph);

    //计算度矩阵
    computeDegreeMatrix(graph);
    //printDegreeMatrix(graph);
    //计算度矩阵的逆矩阵的平方根
    computeDegreeMatrix2(graph);
    //printDegreeMatrix2(graph);

    //计算归一化邻接矩阵
    //将邻接矩阵转换为三元组顺序表
    TsMatrix* tsMatrix1 = createTsMatrix();
    intMatrixToTsMatrix(tsMatrix1, graph->adjacencyMatrix);
    printTsMatrix(tsMatrix1);

    //将度矩阵的逆矩阵的平方根转换为三元组顺序表
    TsMatrix* tsMatrix2 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix2, graph->degreeMatrix2);
    printTsMatrix(tsMatrix2);

    //计算归一化邻接矩阵 第一步乘法
    TsMatrix* result1 = createTsMatrix();
    tsMatrixMul(tsMatrix1, tsMatrix2, result1);
    printTsMatrix(result1);

    //计算归一化邻接矩阵 第二步乘法
    TsMatrix* result2 = createTsMatrix();
    tsMatrixMul(result1, tsMatrix2, result2);
    printTsMatrix(result2);

    //将归一化邻接矩阵转换为二维数组
    tsMatrixToDoubleMatrix(result2, graph->normalizedAdjMatrix);
    printNormalizedAdjMatrix(graph);
}

void randomWeightMatrix(Graph* graph){
    srand(time(NULL));
    for(int i = 0; i < graph->featureNum; i++){
        for(int j = 0; j < OUT_FEATURES; j++){
            // 生成-0.05到0.05之间的随机数
            graph->weightMatrix[i][j] = ((double)rand() / RAND_MAX) * 0.1 - 0.05;
        }
    }
}


void aggregateFeatureMatrix(Graph* graph){
    computeFeatureMatrix(graph);

    TsMatrix* tsMatrix1 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix1, graph->normalizedAdjMatrix);
    //printTsMatrix(tsMatrix1);

    TsMatrix* tsMatrix2 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix2, graph->featureMatrix);
    //printTsMatrix(tsMatrix2);

    TsMatrix* result = createTsMatrix();
    tsMatrixMul(tsMatrix1, tsMatrix2, result);
    printTsMatrix(result);
    

    tsMatrixToDoubleMatrix(result, graph->featureMatrix);
    printFeatureGraph(graph);
}

void linearTransform(Graph* graph,double output[MAX_NODES][OUT_FEATURES]){
    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            output[i][j] = 0;
            for(int k = 0; k < graph->featureNum; k++) {
                output[i][j] += graph->featureMatrix[i][k] * graph->weightMatrix[k][j];
            }
        }
    }
}