#ifndef TS_MATRIX_H
#define TS_MATRIX_H

#include<stdio.h>
#include<stdlib.h>
#include"gcn.h"

#define MAX_NODES 100
#define MAX_ROW_NUM 100

// 三元组定义
typedef struct TupNode
{
    int cow;    //非零元素行号
    int col;    //非零元素列号
    double d;   //元素值
}TupNode;

// 三元组顺序表
typedef struct TsMatrix
{
    int rows;   //矩阵行数
    int cols;   //矩阵列数
    int nums;   //非零元素个数
    TupNode data[MAX_NODES]; //三元组表
    int rpos[MAX_ROW_NUM];   //每行第一个非零元素的位置
}TsMatrix;

TsMatrix* createTsMatrix(void);
void printTsMatrix(TsMatrix* tsMatrix);

void intMatrixToTsMatrix(TsMatrix* tsMatrix, int matrix[MAX_NODES][MAX_NODES]);
void tsMatrixToIntMatrix(TsMatrix* tsMatrix, int matrix[MAX_NODES][MAX_NODES]);

void doubleMatrixToTsMatrix(TsMatrix* tsMatrix, double matrix[MAX_NODES][MAX_NODES]);
void tsMatrixToDoubleMatrix(TsMatrix* tsMatrix, double matrix[MAX_NODES][MAX_NODES]);

void tsMatrixMul(TsMatrix* tsMatrix1, TsMatrix* tsMatrix2, TsMatrix* result);




#endif