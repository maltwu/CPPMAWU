#include "profitsStru.h"
#include "calculate.h"

/**
 * @brief 合理的中間值位置
 *
 * @param prank
 * @param first
 * @param end
 * @return size_t
 */
size_t getMidUnit(PRANKMARK prank, size_t startIdx, size_t endIdx)
{
    if (startIdx < endIdx)
    {
        size_t midIdx = ((endIdx + startIdx) >> 1);
        double startval = getRankOrgVal(prank + startIdx);
        double endval = getRankOrgVal(prank + endIdx);
        double midval = getRankOrgVal(prank + midIdx);
        if (startval < endval)
            return (midval < startval ? startIdx : (midval > endval ? endIdx : midIdx));
        else // startval>=endval
            return (midval > startval ? startIdx : (midval < endval ? endIdx : midIdx));
    }
    return -1;
}
/**
 * @brief 依據中點排序
 *
 * @param prank
 * @param startIdx 數組開始下標
 * @param endIdx 數組結束下標
 * @param isBig true 時按降序排列，false 按升序排列
 * @return size_t
 */
size_t partionsort(PRANKMARK prank, size_t startIdx, size_t endIdx, bool isBig)
{
  /*   double a[2], b[2];
    if (endIdx == 11 && startIdx == 10)
    {
        for (int i = 0; i <= (endIdx - startIdx); i++)
            a[i] = getRankOrgVal(prank + +startIdx + i);
    } */
    if (startIdx < endIdx)
    {
        size_t midIdx = getMidUnit(prank, startIdx, endIdx);
        heapsort_swap(prank + midIdx, prank + endIdx); //將基準值放在數組尾部
        double keyval = getRankOrgVal(prank + endIdx); //確定基準值
        size_t cur = startIdx, prev = cur - 1;
        while (cur <= endIdx - 1) //尾數為基準數，不可以交換
        {
            double curval = getRankOrgVal(prank + cur);
            if ((isBig ? curval >= keyval : curval <= keyval) && ++prev != cur)
                heapsort_swap(prank + cur, prank + prev); //按基準交換
            cur++;
        } // prev 停留位置為最近一次大值/小值下標
     /*    if (endIdx == 11 && startIdx == 10)
        {
            for (int i = 0; i <= (endIdx - startIdx); i++)
                b[i] = getRankOrgVal(prank + +startIdx + i);
        } */
        if (++prev != endIdx)
            heapsort_swap(prank + prev, prank + endIdx); //分隔點位置
    /*     if (endIdx == 11 && startIdx == 10)
        {
            for (int i = 0; i <= (endIdx - startIdx); i++)
                b[i] = getRankOrgVal(prank + +startIdx + i);
        } */
        return prev;
    }

    return -1;
}
/**
 * @brief 快熟排序法(含遞歸調用)
 *
 * @param prank 數組首指針
 * @param startIdx  開始下標
 * @param len 數組長度
 * @param isBig true 降序排列，false 升序排列
 */
void makequicksort(PRANKMARK prank, size_t startIdx, size_t len, size_t &deep, bool isBig)
{
    // printf("deep=%5d ", ++deep);
    if (startIdx < len - 1)
    {
        if ((len - startIdx) != 2)
        {//數組長度在2以上可進行遞歸嘗試
            size_t div = partionsort(prank, startIdx, len - 1, isBig);
            //左列
            // printf("left: %d, %d\n", startIdx, div);
            makequicksort(prank, startIdx, div, deep, isBig);
            //右列
            // printf("right: %d, %d\n", div, len);
            makequicksort(prank, div, len, deep, isBig);
        }else{
            //直接比較大小
            double left = getRankOrgVal(prank+startIdx);
            double right = getRankOrgVal(prank+startIdx+1);
            bool swap = (isBig?(left<right?true:false):(left>right?true:false));
            if(swap)
                heapsort_swap(prank+startIdx,prank+startIdx+1);
        }
    }
}
