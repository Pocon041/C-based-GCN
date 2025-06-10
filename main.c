#include "gcn.h"
#include "TsMatrix.h"

// 输入特征矩阵
void inputFeatureMatrix(Graph* graph){
    printf("请输入节点数：");
    scanf("%d", &graph->numNodes);
    printf("请输入特征维度：");
    scanf("%d", &graph->featureNum);
    //printf("请输入特征矩阵：\n");
    printf("\n");
    for(int i = 0; i < graph->numNodes; i++){
        printf("请输入第%d个节点的特征：", i+1);
        for(int j = 0; j < graph->featureNum; j++){
            scanf("%lf", &graph->nodes[i].features[j]);
        }
    }
}

// 输入邻接矩阵
void inputAdjMatrix(Graph* graph){
    printf("请输入连接的节点标号：（输入-1 -1结束）\n");
    int node1, node2;
    while(1){
        scanf("%d %d", &node1, &node2);
        if(node1 == -1 && node2 == -1){
            break;
        }
        addEdge(graph, node1, node2);
    }
}

// 输入标签
void inputLabels(Graph* graph) {
    printf("请输入每个节点的标签：\n");
    for(int i = 0; i < graph->numNodes; i++) {
        printf("请输入第%d个节点的标签：", i+1);
        for(int j = 0; j < OUT_FEATURES; j++) {
            scanf("%lf", &graph->nodes[i].labels[j]);
        }
    }
}

int main(){
    system("chcp 65001");

    //初始化
    Graph* graph = createGraph();
    inputFeatureMatrix(graph);
    inputAdjMatrix(graph);
    inputLabels(graph);

    printFeatureGraph(graph);
    printAdjMatrix(graph);
    
    normalizeAdjMatrix(graph);

    // 训练参数
    int epochs = 2;
    double output[MAX_NODES][OUT_FEATURES];
    double best_loss = 1e10;
    int no_improve = 0;
    const int patience = 20;  // 早停耐心值

    // 训练循环
    for(int epoch = 0; epoch < epochs; epoch++) {
        printf("\nEpoch %d:\n", epoch + 1);
        
        // 前向传播
        aggregateFeatureMatrix(graph);
        if(epoch == 0) {  // 只在第一个epoch随机初始化权重
            randomWeightMatrix(graph);
        }
        linearTransform(graph, output);

        // ReLU激活函数
        for(int i = 0; i < graph->numNodes; i++){
            for(int j = 0; j < OUT_FEATURES; j++){
                if(output[i][j] < 0) {
                    output[i][j] = 0.01 * output[i][j];  // Leaky ReLU
                }
            }
        }

        // 打印当前输出
        printf("当前输出：\n");
        for(int i = 0; i < graph->numNodes; i++) {
            for(int j = 0; j < OUT_FEATURES; j++) {
                printf("%.6f ", output[i][j]);
            }
            printf("\n");
        }

        // 反向传播
        backward(graph, output);

        // 早停检查
        double current_loss = computeLoss(graph, output);
        if(current_loss < best_loss) {
            best_loss = current_loss;
            no_improve = 0;
        } else {
            no_improve++;
        }

        // 打印当前状态
        printf("当前损失值: %.6f (最佳: %.6f)\n", current_loss, best_loss);
        //printf("当前权重矩阵：\n");
        printWeightMatrix(graph);

        // 早停
        if(no_improve >= patience) {
            printf("\n早停：损失值在%d个epoch内没有改善\n", patience);
            break;
        }
    }

    // 打印最终结果
    printf("\n最终输出：\n");
    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            printf("%.6f ", output[i][j]);
        }
        printf("\n");
    }

    return 0;
}
