#include "gcn.h"
#include "TsMatrix.h"
#include<math.h>


Graph* createGraph(){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = 0;
    graph->featureNum = 0;
    return graph;
}

// DFS辅助函数
static void dfs(Graph* graph, bool* visited, int node) {
    visited[node] = true;
    for(int i = 0; i < graph->numNodes; i++) {
        if(graph->adjacencyMatrix[node][i] && !visited[i]) {
            dfs(graph, visited, i);
        }
    }
}

bool isConnected(Graph* graph){
    if(graph->numNodes == 0) return true;  // 空图被认为是连通的
    
    // 用于标记节点是否被访问过
    bool visited[MAX_NODES] = {false};
    
    // 从节点0开始DFS
    dfs(graph, visited, 0);
    
    // 检查是否所有节点都被访问过
    for(int i = 0; i < graph->numNodes; i++) {
        if(!visited[i]) return false;
    }
    
    return true;
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
    printf("\n添加自环后的邻接矩阵：\n");
    printAdjMatrix(graph);

    //计算度矩阵
    computeDegreeMatrix(graph);
    printf("\n度矩阵：\n");
    printDegreeMatrix(graph);

    //计算度矩阵的逆矩阵的平方根
    computeDegreeMatrix2(graph);
    //printf("\n度矩阵的逆矩阵平方根：\n");
    //for(int i = 0; i < graph->numNodes; i++){
    //    for(int j = 0; j < graph->numNodes; j++){
    //        printf("%.6f ", graph->degreeMatrix2[i][j]);
    //    }
    //    printf("\n");
    //}

    //计算归一化邻接矩阵
    //将邻接矩阵转换为三元组顺序表
    printf("----------计算归一化邻接矩阵----------\n");
    TsMatrix* tsMatrix1 = createTsMatrix();
    intMatrixToTsMatrix(tsMatrix1, graph->adjacencyMatrix);
    printf("\n邻接矩阵的三元组表示：\n");
    printTsMatrix(tsMatrix1);

    //将度矩阵的逆矩阵的平方根转换为三元组顺序表
    TsMatrix* tsMatrix2 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix2, graph->degreeMatrix2);
    printf("\n度矩阵逆平方根的三元组表示：\n");
    printTsMatrix(tsMatrix2);

    //计算归一化邻接矩阵 第一步乘法
    TsMatrix* result1 = createTsMatrix();
    tsMatrixMul(tsMatrix1, tsMatrix2, result1);
    printf("\n第一步乘法结果：\n");
    printTsMatrix(result1);

    //计算归一化邻接矩阵 第二步乘法
    TsMatrix* result2 = createTsMatrix();
    tsMatrixMul(result1, tsMatrix2, result2);
    printf("\n第二步乘法结果：\n");
    printTsMatrix(result2);

    //将归一化邻接矩阵转换为二维数组
    tsMatrixToDoubleMatrix(result2, graph->normalizedAdjMatrix);
    
    //打印最终的归一化邻接矩阵
    printf("\n最终的归一化邻接矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->numNodes; j++){
            printf("%.6f ", graph->normalizedAdjMatrix[i][j]);
        }
        printf("\n");
    }
}

void randomWeightMatrix(Graph* graph){
    srand(time(NULL));
    for(int i = 0; i < graph->featureNum; i++){
        for(int j =0; j < OUT_FEATURES; j++){
            // 使用Xavier初始化
            double scale = sqrt(2.0 / (graph->featureNum + OUT_FEATURES));
            graph->weightMatrix[i][j] = ((double)rand() / RAND_MAX) * 2 * scale - scale;
        }
    }
}

// 添加动量项
static double momentum[MAX_FEATURES][OUT_FEATURES] = {0};
#define MOMENTUM_RATE 0.9

void computeGradients(Graph* graph, double output[MAX_NODES][OUT_FEATURES], double gradients[MAX_FEATURES][OUT_FEATURES]){
    // 初始化梯度矩阵
    for(int i = 0; i < graph->featureNum; i++){
        for(int j = 0; j < OUT_FEATURES; j++){
            gradients[i][j] = 0.0;
        }
    }

    // 计算每个权重的梯度
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < OUT_FEATURES; j++){
            double error = output[i][j] - graph->nodes[i].labels[j];
            // 考虑Leaky ReLU的导数
            double relu_derivative = (output[i][j] > 0) ? 1.0 : 0.01;
            
            for(int k = 0; k < graph->featureNum; k++){
                gradients[k][j] += error * relu_derivative * graph->featureMatrix[i][k] / graph->numNodes;
            }
        }
    }

    // 添加L2正则化
    double lambda = 0.01;  // 正则化系数
    for(int i = 0; i < graph->featureNum; i++){
        for(int j = 0; j < OUT_FEATURES; j++){
            gradients[i][j] += lambda * graph->weightMatrix[i][j] / graph->numNodes;
        }
    }

    // 打印梯度信息（调试用）
    //printf("\n梯度信息：\n");
    //for(int i = 0; i < graph->featureNum; i++){
    //    for(int j = 0; j < OUT_FEATURES; j++){
    //          printf("梯度[%d][%d]: %.6f\n", i, j, gradients[i][j]);
    //    }
    //}
}

// 添加批量归一化
void batchNormalize(Graph* graph, double output[MAX_NODES][OUT_FEATURES]) {
    for(int j = 0; j < OUT_FEATURES; j++) {
        // 计算均值
        double mean = 0;
        for(int i = 0; i < graph->numNodes; i++) {
            mean += output[i][j];
        }
        mean /= graph->numNodes;

        // 计算方差
        double variance = 0;
        for(int i = 0; i < graph->numNodes; i++) {
            variance += (output[i][j] - mean) * (output[i][j] - mean);
        }
        variance /= graph->numNodes;
        variance += 1e-8;  // 防止除零

        // 归一化
        for(int i = 0; i < graph->numNodes; i++) {
            output[i][j] = (output[i][j] - mean) / sqrt(variance);
        }
    }
}

void aggregateFeatureMatrix(Graph* graph){
    // 保存原始特征
    double originalFeatures[MAX_NODES][MAX_FEATURES];
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->featureNum; j++){
            originalFeatures[i][j] = graph->nodes[i].features[j];
        }
    }

    // 将原始特征复制到特征矩阵
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->featureNum; j++){
            graph->featureMatrix[i][j] = originalFeatures[i][j];
        }
    }

    // 使用三元组矩阵进行特征聚合
    TsMatrix* tsMatrix1 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix1, graph->normalizedAdjMatrix);
    printf("\n归一化邻接矩阵的三元组表示：\n");
    printTsMatrix(tsMatrix1);

    TsMatrix* tsMatrix2 = createTsMatrix();
    doubleMatrixToTsMatrix(tsMatrix2, graph->featureMatrix);
    printf("\n特征矩阵的三元组表示：\n");
    printTsMatrix(tsMatrix2);

    // 打印矩阵维度信息
    //printf("\n矩阵维度信息：\n");
    //printf("归一化邻接矩阵：(%d,%d)\n", tsMatrix1->rows, tsMatrix1->cols);
    //printf("特征矩阵：(%d,%d)\n", tsMatrix2->rows, tsMatrix2->cols);

    TsMatrix* result = createTsMatrix();
    tsMatrixMul(tsMatrix1, tsMatrix2, result);
    printf("\n特征聚合结果的三元组表示：\n");
    printTsMatrix(result);
    
    // 将结果转换回特征矩阵
    tsMatrixToDoubleMatrix(result, graph->featureMatrix);

    // 打印聚合后的特征矩阵（调试用）
    printf("\n聚合后的特征矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < graph->featureNum; j++){
            printf("%.6f ", graph->featureMatrix[i][j]);
        }
        printf("\n");
    }
}

void linearTransform(Graph* graph, double output[MAX_NODES][OUT_FEATURES]){
    // 线性变换
    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            output[i][j] = 0;
            for(int k = 0; k < graph->featureNum; k++) {
                output[i][j] += graph->featureMatrix[i][k] * graph->weightMatrix[k][j];
            }
        }
    }

    // 打印线性变换后的输出（调试用）
    //printf("\n线性变换后的输出：\n");
    //for(int i = 0; i < graph->numNodes; i++) {
    //    for(int j = 0; j < OUT_FEATURES; j++) {
    //        printf("%.6f ", output[i][j]);
    //    }
    //    printf("\n");
    //}

    // 应用批量归一化
    batchNormalize(graph, output);

    // 使用Leaky ReLU
    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            if(output[i][j] < 0) {
                output[i][j] = 0.01 * output[i][j];
            }
        }
    }
}

// 计算损失函数 均方误差
double computeLoss(Graph* graph, double output[MAX_NODES][OUT_FEATURES]){
    double loss = 0;
    for(int i = 0; i < graph->numNodes; i++){
        for(int j=0; j<OUT_FEATURES; j++){
            double diff = output[i][j] - graph->nodes[i].labels[j];
            loss += diff * diff;
        }
    }
    loss /= (2.0 * graph->numNodes * OUT_FEATURES);  // 修正损失计算
    return loss;
}

void backward(Graph* graph, double output[MAX_NODES][OUT_FEATURES]) {
    double loss;
    double gradients[MAX_FEATURES][OUT_FEATURES];

    // 计算损失
    loss = computeLoss(graph, output);
    //printf("当前损失值: %.6f\n", loss);

    // 计算梯度
    computeGradients(graph, output, gradients);

    // 更新权重
    updateWeight(graph, gradients);
}

void updateWeight(Graph* graph, double gradients[MAX_FEATURES][OUT_FEATURES]) {
    for(int i = 0; i < graph->featureNum; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            // 添加梯度裁剪
            if(gradients[i][j] > 1.0) gradients[i][j] = 1.0;
            if(gradients[i][j] < -1.0) gradients[i][j] = -1.0;
            
            // 更新动量
            momentum[i][j] = MOMENTUM_RATE * momentum[i][j] + LEARNING_RATE * gradients[i][j];
            
            // 更新权重
            graph->weightMatrix[i][j] -= momentum[i][j];
            
            // 权重衰减
            graph->weightMatrix[i][j] *= (1.0 - 0.0001);  // L2正则化
            
            // 打印更新信息（调试用）
            //printf("权重[%d][%d] 更新: %.6f (梯度: %.6f, 动量: %.6f)\n", 
            //       i, j, momentum[i][j], gradients[i][j], momentum[i][j]);
        }
    }
}
