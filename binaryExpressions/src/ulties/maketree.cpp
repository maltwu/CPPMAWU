#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maketree.h"

#define N 1e3 + 5;
//内部方法声明
bool isLeaf(PSTLOGIC);
PSTLOGIC makeLeafByValue(char, PSTLOGIC, PSTLOGIC);

/**
 * @brief 根据逻辑运算表达式，创建逻辑运算树
 *
 * @param expression 输入表达式
 * @return PSTLOGIC 根节点
 */
PSTLOGIC makeTreeByExpr(const char *expression)
{
    size_t limitn = N;
    PSTLOGIC tree[limitn];         //操作数处理堆栈
    char op[limitn];               //操作符处理堆栈
    int Pos_op = 0, Pos_tree = 0;  //操作符堆栈位置,初始化指向堆栈底部
    size_t n = strlen(expression); //确定表达式长度

    op[0] = 'N';
    if (n > limitn || n < 3)
        return NULL;

    for (size_t i = 0; i < n; i++)
    {
        if (expression[i] == '1' || expression[i] == '0')
            tree[Pos_tree++] = makeLeafByValue(expression[i], NULL, NULL);
        else
        {
            while ((expression[i] == '|' && op[Pos_op - 1] == '|') ||
                   (expression[i] == '&' && op[Pos_op - 1] == '&') ||
                   (expression[i] == '|' && op[Pos_op - 1] == '&') ||
                   (expression[i] == ')' && op[Pos_op - 1] != '('))
            {
                PSTLOGIC right = tree[--Pos_tree];
                PSTLOGIC left = tree[--Pos_tree];
                tree[Pos_tree++] = makeLeafByValue(op[(Pos_op--) - 1], left, right);
            }
            if (expression[i] == ')' && op[Pos_op - 1] == '(')
                Pos_op--;
            else
                op[Pos_op++] = expression[i];
        }
    }
    return tree[0]; //返回树根节点
}
/**
 * @brief 计算树结果，并返回短路策略执行情况
 *
 * @param pTree  根节点
 * @param nAndSt 累积“与”短路次数，初始给定变量值应为0
 * @param nOrSt 累积“或”短路次数，初始给定变量值应为0
 * @return char 计算正确时返回逻辑运算结果字符“0”或“1”，错误时返回字符“E”
 */
char AcculateTree(PSTLOGIC pTree, int &nAndSt, int &nOrSt)
{
    char lnode='E', rnode='E';
    if (pTree)
    {
        //树叶子直接返回
        if (isLeaf(pTree))
            return pTree->opr; //
        //如果树有左节点，先计算左节点
        if (pTree->pleft)
            lnode = AcculateTree(pTree->pleft, nAndSt, nOrSt);
        //判断短路策略
        if (pTree->opr == '&' && lnode == '0')
        {
            nAndSt++;
            return '0';
        }
        else if (pTree->opr == '|' && lnode == '1')
        {
            nOrSt++;
            return '1';
        }
        else if (lnode == 'E'|| (pTree->opr != '&' && pTree->opr != '|'))
            return 'E';
        //如果树有右节点，计算右节点
        if (pTree->pright)
            rnode = AcculateTree(pTree->pright, nAndSt, nOrSt);
        if (rnode == 'E')
            return 'E';
        //逻辑运算实现
        if (pTree->opr == '&')
        {
            if (rnode == '1' && lnode == '1')
                return '1';
            else
                return '0';
        }else {
            if (rnode == '1' || lnode == '1')
                return '1';
            else
                return '0';
        }
    }
    else
        return 'E';
}
/**
 * @brief 释放树空间
 *
 * @param pTree
 */
void freeTree(PSTLOGIC pTree)
{
    if (pTree)
    {
        if (isLeaf(pTree))
        {
            free(pTree); //释放自身
            return;
        }
        else
        {
            if (pTree->pleft)
                freeTree(pTree->pleft);
            if (pTree->pright)
                freeTree(pTree->pright);
            free(pTree);
            return;
        }
    }
    return;
}
/**
 * @brief 打印树结构
 *
 * @param ptree  树根节点
 * @param hoffset 水平偏移
 */
void printTree(PSTLOGIC ptree, int hoffset)
{
    if (ptree)
    {
        int i = hoffset;
        printf("%c \n", ptree->opr);
        if (!isLeaf(ptree))
        {
            //处理左分支间隔
            while (i)
            {
                printf("  ");
                i--;
            }
            printf("__");
            printTree(ptree->pleft, hoffset + 1);
            //处理右分支间隔
            i = hoffset;
            while (i)
            {
                printf("  ");
                i--;
            }
            printf("__");
            printTree(ptree->pright, hoffset + 1);
        }
    }
    else
        printf("空树");
}
/**
 * @brief 创建一个节点
 *
 * @param opr 分支操作数
 * @param l 左侧子分支
 * @param r 右侧子分支
 * @return PSTLOGIC
 */
PSTLOGIC makeLeafByValue(char opr, PSTLOGIC l, PSTLOGIC r)
{
    PSTLOGIC p = (PSTLOGIC)malloc(sizeof(STLOGIC));
    p->opr = opr;
    p->pleft = l;
    p->pright = r;
    return p;
}
/**
 * @brief 判定是否为叶子
 *
 * @param node 待判定节点
 * @return true
 * @return false
 */
bool isLeaf(PSTLOGIC node)
{
    if (node)
    {
        if (!node->pleft || !node->pright)
            return true;
    }
    return false;
}
