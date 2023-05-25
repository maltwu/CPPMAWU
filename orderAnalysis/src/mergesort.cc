#include "profitsStru.h"
#include "calculate.h"
/**
 * @brief 歸併左右集
 *
 * @param prank
 * @param left
 * @param mid
 * @param right
 * @param isBig true 降序排列 false 升序排列
 */
void MergeData(PRANKMARK prank, size_t left, size_t mid, size_t right, bool isBig)
{
    PRANKMARK ptemp = (PRANKMARK)malloc(sizeof(RANKMARK) * (right - left));
    size_t begin1 = left, end1 = mid, begin2 = mid, end2 = right, idxtmp = 0;
    while (begin1 < end1 && begin2 < end2)
    {
        double begin1val = getRankOrgVal(prank + begin1);
        double begin2val = getRankOrgVal(prank + begin2);
        if (isBig ? begin1val >= begin2val : begin1val <= begin2val)
            memcpy(ptemp + idxtmp++, prank + begin1++, sizeof(RANKMARK));
        else
            memcpy(ptemp + idxtmp++, prank + begin2++, sizeof(RANKMARK));
    }
    if (begin1 < end1) //序列1有余剩，则都为大于或小于前序列值，则剩余序列都填入尾部
        memcpy(ptemp + idxtmp, prank + begin1, sizeof(RANKMARK) * (end1 - begin1));
    if (begin2 < end2) //序列2有余剩，则都为大于或小于前序列值，则剩余序列都填入尾部
        memcpy(ptemp + idxtmp, prank + begin2, sizeof(RANKMARK) * (end2 - begin2));
    //整理后的序列都填回原数组
    memcpy(prank + left, ptemp, sizeof(RANKMARK) * (right - left));
    free(ptemp); //释放借用内存
}
/**
 * @brief 归并排序指定数组
 * 
 * @param prank 
 * @param left 
 * @param right 
 * @param isBig 
 */
void MergeSort(PRANKMARK prank, size_t left, size_t right, bool isBig)
{
    if (right - left > 1)
    {
        size_t mid = (left + right) >> 1;
        MergeSort(prank, left, mid, isBig);  //排序左序列
        MergeSort(prank, mid, right, isBig); //排序右序列
        //归并左右序列
        MergeData(prank, left, mid, right, isBig);
    }
}