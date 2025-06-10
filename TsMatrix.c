#include "TsMatrix.h"

TsMatrix* createTsMatrix(void){
    TsMatrix* tsMatrix = (TsMatrix*)malloc(sizeof(TsMatrix));
    tsMatrix->rows = 0;
    tsMatrix->cols = 0;
    tsMatrix->nums = 0;
    // 初始化rpos数组
    for(int i = 0; i < MAX_ROW_NUM; i++) {
        tsMatrix->rpos[i] = 0;
    }
    return tsMatrix;
}

void printTsMatrix(TsMatrix* tsMatrix){
    printf("三元组矩阵：\n");
    for(int i = 0; i < tsMatrix->nums; i++){
        printf("(行：%d, 列：%d, 值：%.2lf)\n", tsMatrix->data[i].cow, tsMatrix->data[i].col, tsMatrix->data[i].d);
    }
}

void intMatrixToTsMatrix(TsMatrix* tsMatrix, int matrix[MAX_NODES][MAX_NODES]){
    // 设置矩阵的行数和列数
    tsMatrix->rows = MAX_NODES;
    tsMatrix->cols = MAX_NODES;
    
    // 初始化rpos数组
    for(int i = 0; i < MAX_ROW_NUM; i++) {
        tsMatrix->rpos[i] = 0;
    }
    
    // 第一次遍历，计算每行非零元素的个数
    int rowCount[MAX_ROW_NUM] = {0};
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            if(matrix[i][j] != 0){
                rowCount[i]++;
            }
        }
    }
    
    // 计算每行第一个非零元素的位置
    tsMatrix->rpos[0] = 0;
    for(int i = 1; i < tsMatrix->rows; i++){
        tsMatrix->rpos[i] = tsMatrix->rpos[i-1] + rowCount[i-1];
    }
    
    // 存储非零元素
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            if(matrix[i][j] != 0){
                tsMatrix->data[tsMatrix->nums].cow = i;
                tsMatrix->data[tsMatrix->nums].col = j;
                tsMatrix->data[tsMatrix->nums].d = matrix[i][j];
                tsMatrix->nums++;
            }
        }
    }
}

void tsMatrixToIntMatrix(TsMatrix* tsMatrix, int matrix[MAX_NODES][MAX_NODES]){
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            matrix[i][j] = 0;
        }
    }
    for(int i = 0; i < tsMatrix->nums; i++){
        matrix[tsMatrix->data[i].cow][tsMatrix->data[i].col] = tsMatrix->data[i].d;
    }   
}

void doubleMatrixToTsMatrix(TsMatrix* tsMatrix, double matrix[MAX_NODES][MAX_NODES]){
    // 设置矩阵的行数和列数
    tsMatrix->rows = MAX_NODES;
    tsMatrix->cols = MAX_NODES;
    tsMatrix->nums = 0;  // 重置非零元素计数
    
    // 初始化rpos数组
    for(int i = 0; i < MAX_ROW_NUM; i++) {
        tsMatrix->rpos[i] = 0;
    }
    
    // 第一次遍历，计算每行非零元素的个数
    int rowCount[MAX_ROW_NUM] = {0};
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            if(fabs(matrix[i][j]) > 1e-10){  // 使用一个小的阈值来判断非零
                rowCount[i]++;
            }
        }
    }
    
    // 计算每行第一个非零元素的位置
    tsMatrix->rpos[0] = 0;
    for(int i = 1; i < tsMatrix->rows; i++){
        tsMatrix->rpos[i] = tsMatrix->rpos[i-1] + rowCount[i-1];
    }
    
    // 存储非零元素
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            if(fabs(matrix[i][j]) > 1e-10){  // 使用一个小的阈值来判断非零
                tsMatrix->data[tsMatrix->nums].cow = i;
                tsMatrix->data[tsMatrix->nums].col = j;
                tsMatrix->data[tsMatrix->nums].d = matrix[i][j];
                tsMatrix->nums++;
            }
        }
    }
    
    // 打印调试信息
    //printf("\n矩阵转换信息：\n");
    //printf("矩阵维度：(%d,%d)\n", tsMatrix->rows, tsMatrix->cols);
    //printf("非零元素个数：%d\n", tsMatrix->nums);
}

void tsMatrixToDoubleMatrix(TsMatrix* tsMatrix, double matrix[MAX_NODES][MAX_NODES]){
    for(int i = 0; i < tsMatrix->rows; i++){
        for(int j = 0; j < tsMatrix->cols; j++){
            matrix[i][j] = 0;
        }
    }
    for(int i = 0; i < tsMatrix->nums; i++){
        matrix[tsMatrix->data[i].cow][tsMatrix->data[i].col] = tsMatrix->data[i].d;
    }   
}

void tsMatrixMul(TsMatrix* tsMatrix1, TsMatrix* tsMatrix2, TsMatrix* result) {
    // 检查矩阵是否可以相乘
    if(tsMatrix1->cols != tsMatrix2->rows) {
        printf("矩阵维度不匹配：(%d,%d) * (%d,%d)\n", 
               tsMatrix1->rows, tsMatrix1->cols, 
               tsMatrix2->rows, tsMatrix2->cols);
        return;
    }
    
    // 检查是否有零矩阵
    if(tsMatrix1->nums == 0 || tsMatrix2->nums == 0) {
        printf("存在零矩阵\n");
        return;
    }
    
    result->rows = tsMatrix1->rows;
    result->cols = tsMatrix2->cols;
    result->nums = 0;
    
    // 初始化结果矩阵的rpos
    for(int i = 0; i < MAX_ROW_NUM; i++) {
        result->rpos[i] = 0;
    }
    
    // 临时数组存储当前行的计算结果
    double ctemp[MAX_ROW_NUM] = {0};
    
    // 对tsMatrix1的每一行进行处理
    for(int arow = 0; arow < tsMatrix1->rows; arow++) {
        // 清空临时数组
        for(int i = 0; i < result->cols; i++) {
            ctemp[i] = 0;
        }
        
        // 记录当前行在结果矩阵中的起始位置
        result->rpos[arow] = result->nums;
        
        // 确定当前行的结束位置
        int tp;
        if(arow < tsMatrix1->rows - 1) {
            tp = tsMatrix1->rpos[arow + 1];
        } else {
            tp = tsMatrix1->nums;
        }
        
        // 处理当前行的每个非零元素
        for(int p = tsMatrix1->rpos[arow]; p < tp; p++) {
            int brow = tsMatrix1->data[p].col;
            double weight = tsMatrix1->data[p].d;  // 获取权重
            
            // 确定tsMatrix2中对应行的结束位置
            int t;
            if(brow < tsMatrix2->rows - 1) {
                t = tsMatrix2->rpos[brow + 1];
            } else {
                t = tsMatrix2->nums;
            }
            
            // 计算乘积
            for(int q = tsMatrix2->rpos[brow]; q < t; q++) {
                int ccol = tsMatrix2->data[q].col;
                ctemp[ccol] += weight * tsMatrix2->data[q].d;
            }
        }
        
        // 将非零元素存入结果矩阵
        for(int ccol = 0; ccol < result->cols; ccol++) {
            if(fabs(ctemp[ccol]) > 1e-10) {  // 使用一个小的阈值来判断非零
                if(result->nums >= MAX_NODES) {
                    printf("结果矩阵超出最大容量\n");
                    return;
                }
                result->data[result->nums].cow = arow;
                result->data[result->nums].col = ccol;
                result->data[result->nums].d = ctemp[ccol];
                result->nums++;
            }
        }
    }
    
    // 打印调试信息
    //printf("\n矩阵乘法结果：\n");
    //printf("结果矩阵维度：(%d,%d)\n", result->rows, result->cols);
    //printf("非零元素个数：%d\n", result->nums);
}


