#include "gcn.h"


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
    Graph* graph = createGraph();

    inputFeatureMatrix(graph);

    inputAdjMatrix(graph);

    printFeatureGraph(graph);
    printAdjMatrix(graph);
    selfLoop(graph);

    printAdjMatrix(graph);
    
    computeDegreeMatrix(graph);
    printDegreeMatrix(graph);
    return 0;
}
