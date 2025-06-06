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


int main(){
    system("chcp 65001");

    //初始化

    Graph* graph = createGraph();

    inputFeatureMatrix(graph);

    inputAdjMatrix(graph);

    printFeatureGraph(graph);
    printAdjMatrix(graph);
    
    normalizeAdjMatrix(graph);


    //图卷积运算
    aggregateFeatureMatrix(graph);

    randomWeightMatrix(graph);
    printWeightMatrix(graph);

    double output[MAX_NODES][OUT_FEATURES];
    linearTransform(graph, output);


    //ReLU激活函数
    for(int i = 0; i < graph->numNodes; i++){
        for(int j = 0; j < OUT_FEATURES; j++){
            if(output[i][j] < 0) output[i][j] = 0;
        }
    }

    // 打印结果
    printf("线性变换后的特征矩阵：\n");
    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < OUT_FEATURES; j++) {
            printf("%.2lf ", output[i][j]);
        }
        printf("\n");
    }


    return 0;
}
