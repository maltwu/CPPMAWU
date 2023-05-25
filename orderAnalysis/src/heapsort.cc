/**
 * @file heapsort.cc
 * @author your name (you@domain.com)
 * @brief 针对收益率的排序算法-堆排序
 * @version 0.1
 * @date 2022-11-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "profitsStru.h"
/**
 * @brief 获取指定项目的收益率
 *
 */
double getRankOrgVal(PRANKMARK unit)
{
    return *(unit->pUnit->pValue + unit->pos);
}
/**
 * @brief 交换数据
 *
 * @param unit_1
 * @param unit_2
 */
void heapsort_swap(PRANKMARK unit_1, PRANKMARK unit_2)
{
    if (unit_1 != unit_2)
    {
        RANKMARK temp;
        int size = sizeof(RANKMARK);
        memcpy(&temp, unit_1, size);
        memcpy(unit_1, unit_2, size);
        memcpy(unit_2, &temp, size);
    }
}
/**
 * @brief 从指定节点开始向后调整元素构建堆
 *
 * @param prank
 * @param startnode
 * @param length
 * @param isBig 为真，按大顶建构(升序），为假，按小顶建构（降序）
 */
void adjustHeap(PRANKMARK prank, size_t startnode, size_t length, bool isBig)
{
    RANKMARK temp;
    double valleft = 0, valright = 0, valtemp = 0, keyvalue = 0; //左右支,与中间量等对应收益值
    memcpy(&temp, prank + startnode, sizeof(temp));
    valtemp = getRankOrgVal(prank + startnode);
    for (size_t k = startnode * 2 + 1; k < length; k = 2 * k + 1)
    {                                       //依次整理当前节点和下级节点
        valleft = getRankOrgVal(prank + k); //初始假定上浮左支
        keyvalue = valleft;
        if (k + 1 < length)
        {
            valright = getRankOrgVal(prank + k + 1);
            if (isBig ? valleft < valright : valleft > valright) //大顶 取两者大值上浮，小顶取两者小值上浮
            {
                k++;
                keyvalue = valright; //变更上浮右支
            }
        }
        if (isBig ? valtemp < keyvalue : valtemp > keyvalue) //确定上浮到节点？
        {
            // K对应数据填充到节点处,单向不交换,利用每次比较都是temp值的方式，可只记录要交换的位置的移动，减少内存操作
            memcpy(prank + startnode, prank + k, sizeof(RANKMARK));
            startnode = k; //记录该位置待交换
        }
    }
    memcpy(prank + startnode, &temp, sizeof(temp)); //按记录的交换位置，完成最终交换
}
/**
 * @brief 堆算法排序
 *
 * @param prank 堆结构非配数组
 * @param nlength 待结构化数组长度
 * @param isBig 为真降序（建小顶），为假升序，建大顶
 */
void makeHeapSort(PRANKMARK prank, size_t nlength, bool isBig)
{
    //最后一片叶子为数组尾数据，下标为
    size_t lastLeafIndx = nlength - 1;
    //则最后一个父节点下标为
    size_t lastNodeIndx = (lastLeafIndx - 1) / 2;
    for (size_t start = 0; start <= lastNodeIndx; start++)
    {
        //从最后一个父节点开始，上浮极值，整理二叉树结构
        adjustHeap(prank, lastNodeIndx - start, nlength, !isBig);
    }
    for (size_t idx = 0; idx < nlength; idx++)
    {
        size_t ilen = nlength - 1 - idx;
        //交换头尾数据
        heapsort_swap(prank, prank + ilen);
        adjustHeap(prank, 0, ilen, !isBig);
    }
}
