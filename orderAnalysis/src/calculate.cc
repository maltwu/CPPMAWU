#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "calculate.h"

void insertEvalByDesc(DATACALCHAIN arr[], DATACALCHAIN unit, int pos);

void insertStdByAsc(DATACALCHAIN arr[], DATACALCHAIN unit, int pos);

void sortEvalByDesc(DATACALCHAIN arr[]);

void sortStdByAsc(DATACALCHAIN arr[]);

using namespace std;

/**
 * @brief 释放统计数据列表
 *
 * @param pFirst
 */
void clearDataChain(PDATACALCHAIN pFirst)
{
    size_t nCount = 0;
    PDATACALCHAIN interP = pFirst, freeP = NULL;
    while (interP)
    {
        if (interP->data.plable)
            free(interP->data.plable);
        freeP = interP;
        interP = freeP->pNext;
        free(freeP);
        nCount++;
    }
    printf("已释放统计项目%d个\n", nCount);
}
/**
 * @brief 按人头计算每月收益率期望,以及对此期望的标准差
 *
 * @param proot 待计算原始数据
 * @param at_year 要统计的年份
 * @return PDATACALCHAIN 按人头输出针对某年份数据的统计结果
 */
PDATACALCHAIN calExpectionByPersons(PDATAITEM proot, char *at_year)
{
    PDATACALCHAIN pRet = NULL;  //链表头指针
    PDATACALCHAIN pData = NULL; //当前处置位置
    if (proot)
    {
        PDATAITEM pos = proot; //记录开始遍历原始数据位置

        while (pos)
        {
            PDATACALCHAIN pNew = (PDATACALCHAIN)malloc(sizeof(DATACALCHAIN)); //分配一个新链表单元
            memset(pNew, 0, sizeof(DATACALCHAIN));                            //清空单元
            calExpectionOnPerson(pNew->data, pos, at_year);                   //计算单行期望，并填充到列表
            if (!pRet)
            { //首行
                pRet = pNew;
                pData = pNew;
                pData->number = 1;
            }
            else
            {
                pData->pNext = pNew;
                pNew->number = pData->number + 1;
                pData = pNew;
            }
            pos = pos->pNext; //原数据链表向下一项移动
        }
    }
    return pRet;
}
/**
 * @brief 计算单人每月期望，及对期望的标准差
 *
 * @param des  接收计算结果的单元
 * @param org  输入待计算单人数据
 * @param at_year 指定年份。为空时，为全部年份
 */
void calExpectionOnPerson(DATACALCELL &des, PDATAITEM org, char *at_year)
{
    if (org)
    {
        if (des.plable)
            free(des.plable);
        des.plable = (char *)malloc(org->lablesize);
        memcpy(des.plable, org->plable, org->lablesize); //获取本行数据标签
        //确定统计范围
        size_t cpos = -1, nStart = 0, nEnd = org->valuesize;
        if (at_year)
        {
            string syear = string(at_year), slabel;
            char label[50];
            nStart = -1;
            nEnd = -1;
            for (size_t i = 0; i < org->valuesize; i++)
            {
                memcpy(label, org->pvaluemark + i * 50, 50 * sizeof(char)); //复制对应标题
                slabel = string(label);
                cpos = slabel.find(syear);
                if (cpos != syear.npos)
                {
                    if (nStart == -1)
                        nStart = i;
                    if (nEnd == -1)
                        nEnd = i + 1;
                    else
                        nEnd++;
                }
            }
        }
        //计算本行数据期望
        double sumData = 0;
        size_t stcnt = 0; //统计次数
        for (size_t i = nStart; i < nEnd; i++)
        {
            sumData += *(org->pValue + i);
            stcnt++;
        }
        if (stcnt)
            des.eVal = sumData / stcnt;
        else
        {
            des.eVal = 0;
            des.sTd = 0;
            return; //无目标数据，直接返回
        }
        sumData = 0;
        //计算本行数据的标准差
        for (size_t i = nStart; i < nEnd; i++)
            sumData += pow(*(org->pValue + i) - des.eVal, 2);
        des.sTd = sqrt(sumData / stcnt);
    }
    else
        des.plable = NULL;
} // end of calExpectionOnPerson

/**
 * @brief Get the Top10 object
 *
 * @param pRoot
 * @param exp
 * @param std
 */
void getTop10(PDATACALCHAIN pRoot, DATACALCHAIN exp[], DATACALCHAIN std[])
{
    if (!pRoot)
        return;
    memset(exp, 0, sizeof(DATACALCHAIN) * TOPCOUNTS);
    memset(std, 0, sizeof(DATACALCHAIN) * TOPCOUNTS);
    PDATACALCHAIN pStart = pRoot;
    int initial = 0;
    while (pStart)
    {

        if (initial < TOPCOUNTS)
        { //初始化前10項
            memcpy(&exp[initial], pStart, sizeof(DATACALCHAIN));
            memcpy(&std[initial], pStart, sizeof(DATACALCHAIN));
            initial++;
            if (initial == TOPCOUNTS)
            {
                sortEvalByDesc(exp);
                sortStdByAsc(std);
            }
        }
        else
        {
            //從大到小順序插入期望
            insertEvalByDesc(exp, (*pStart), 0);
            //從小到大順序插入標準差項
            insertStdByAsc(std, (*pStart), 0);
        }
        pStart = pStart->pNext;
    }
} // end of getTop10

/**
 * @brief 從大到小順序插入期望項
 *
 * @param arr 待插入數組
 * @param unit 待插入單元
 * @param pos  嘗試插入的第一開始位置
 * @note 數組長度為TOPCOUNTS,數組從0位開始按從大到小排序
 */
void insertEvalByDesc(DATACALCHAIN arr[], DATACALCHAIN unit, int pos = 0)
{
    if (pos < TOPCOUNTS)
    {
        if (unit.data.eVal > arr[pos].data.eVal)
        { //輸入一個數，比最大值大，依次後移
            insertEvalByDesc(arr, arr[pos], pos + 1);
            memcpy(&arr[pos], &unit, sizeof(DATACALCHAIN));
        }
        else if (unit.data.eVal < arr[pos].data.eVal)
            //輸入數比當前數小,依次測試數據應插入位置
            insertEvalByDesc(arr, unit, pos + 1);
    }
} // end of insertEvalByDesc
/**
 * @brief 從小到大順序插入標準差項
 *
 * @param arr 待插入數組
 * @param unit 待插入單元
 * @param pos  嘗試插入的第一開始位置
 * @note 數組長度為TOPCOUNTS,數組從0位開始按從小到大排序
 */
void insertStdByAsc(DATACALCHAIN arr[], DATACALCHAIN unit, int pos = 0)
{
    if (pos < TOPCOUNTS)
    {
        if (unit.data.sTd < arr[pos].data.sTd)
        { //輸入一個數，比最小值小，依次後移
            insertStdByAsc(arr, arr[pos], pos + 1);
            memcpy(&arr[pos], &unit, sizeof(DATACALCHAIN));
        }
        else if (unit.data.sTd > arr[pos].data.sTd)
            //輸入數比當前數大,依次測試數據應插入位置
            insertStdByAsc(arr, unit, pos + 1);
    }
} // end of insertStdByAsc
/**
 * @brief 按降序排列期望數組
 *
 * @param arr
 */
void sortEvalByDesc(DATACALCHAIN arr[])
{
    bool trans;
    do
    {
        trans = false;
        for (int i = 0; i < TOPCOUNTS - 1; i++)
        {
            if (arr[i].data.eVal < arr[i + 1].data.eVal)
            { //後數大，則交換
                DATACALCHAIN temp;
                memcpy(&temp, &arr[i], sizeof(DATACALCHAIN));
                memcpy(&arr[i], &arr[i + 1], sizeof(DATACALCHAIN));
                memcpy(&arr[i + 1], &temp, sizeof(DATACALCHAIN));
                trans = true;
            }
        }
    } while (trans);
} // end of sortEvalByDesc
/**
 * @brief 按升序排列標準差數組
 *
 * @param arr
 */
void sortStdByAsc(DATACALCHAIN arr[])
{
    bool trans;
    do
    {
        trans = false;
        for (int i = 0; i < TOPCOUNTS - 1; i++)
        {
            if (arr[i].data.sTd > arr[i + 1].data.sTd)
            { //前數大，則交換
                DATACALCHAIN temp;
                memcpy(&temp, &arr[i], sizeof(DATACALCHAIN));
                memcpy(&arr[i], &arr[i + 1], sizeof(DATACALCHAIN));
                memcpy(&arr[i + 1], &temp, sizeof(DATACALCHAIN));
                trans = true;
            }
        }
    } while (trans);
} // end of sortStdByAsc
/**
 * @brief 获取给定单元的指定年月数据
 *
 * @param pUnit
 * @param at_year
 * @param month
 * @return double
 */
double getValueOnMonth(PDATAITEM pUnit, char *at_year, int month)
{
    string smonth[12] = {"年1月", "年2月", "年3月", "年4月", "年5月", "年6月", "年7月", "年8月", "年9月", "年10月", "年11月", "年12月"};
    if (pUnit && at_year && month >= 1 && month <= 12)
    {
        //确定年份
        string year = string(at_year);
        //遍历所有收益率数据
        for (size_t index = 0; index < pUnit->valuesize; index++)
        {
            string label = string(pUnit->pvaluemark + index * 50);
            size_t pos = label.find(year); //确实是否本年度数据

            if (pos != label.npos)
            {
                pos = label.find(smonth[month - 1]);
                if (pos != label.npos)
                {                                    //确定具体月份数据发现
                    return *(pUnit->pValue + index); //返回收益率数据
                }
            }
        }
    }
    return nan("-1");
}
/**
 * @brief 获取给定单位年内的收益率和
 *
 * @param pUnit
 * @param at_year
 * @return double
 */
double getExpValueOnMonth(PDATAITEM pUnit, char *at_year)
{
    int i = 1;
    double r = 0, t = 0;
    do
    {
        t = getValueOnMonth(pUnit, at_year, i);
        if (t != nan("-1"))
            r += t;
        else
            break;
    } while (++i <= 12);
    return r;
}
/**
 * @brief 整理指定年份数据按月分配
 *
 * @param proot 原始数据链表
 * @param at_year 参与排名数据年份
 * @param cnts 参与排序的人员数目
 * @return PRANKINMONTHY 排名链表（年内按月分配)
 */
PRANKINMONTHY AssignByProfitsPerM(PDATAITEM proot, char *at_year, size_t cnts)
{
    string month[12] = {"年1月", "年2月", "年3月", "年4月", "年5月", "年6月", "年7月", "年8月", "年9月", "年10月", "年11月", "年12月"};
    PRANKINMONTHY pRet = NULL;
    size_t lines = 0; //累计处理行数
    if (proot && at_year && cnts > 0)
    {
        //确定年份
        string year = string(at_year);
        do
        {
            //遍历第一行数据
            for (size_t index = 0; index < proot->valuesize; index++)
            {
                string label = string(proot->pvaluemark + index * 50);
                size_t pos = label.find(year); //确实是否本年度数据
                if (pos != label.npos)
                {
                    for (int m = 0; m < 12; m++)
                    { //按月分配此行数据
                        pos = label.find(month[m]);
                        if (pos != label.npos)
                        { //确定具体月份数据发现
                            //将此人员数据填入对应月份位置
                            if (!pRet)
                            {
                                pRet = (PRANKINMONTHY)malloc(sizeof(RANKINMONTHY));
                                memset(pRet, 0, sizeof(RANKINMONTHY));
                                strcpy(pRet->year, at_year);
                            }
                            if (!pRet->rank[m])
                            {                                                               //对应月份无数据列
                                pRet->rank[m] = (PRANKMARK)malloc(sizeof(RANKMARK) * cnts); //按参与统计人员数分配内存
                                memset(pRet->rank[m], 0, sizeof(RANKMARK) * cnts);
                            }
                            //在指定位置插入该人员数据
                            (pRet->rank[m] + lines)->pUnit = proot; //记录人员
                            (pRet->rank[m] + lines)->pos = index;   //记录人员该年月数据在原始数据行中的位置
                        }
                    }
                }
            }

            proot = proot->pNext;
            lines++;
        } while (proot);
    }
    return pRet;
}
/**
 * @brief 释放AssignByProfitsPerM函数分配的空间
 *
 * @param p
 */
void ClearAssignByProfitsPerM(PRANKINMONTHY p)
{
    if (p)
    {
        for (int i = 0; i < 12; i++)
        {
            if (p->rank[i])
                free(p->rank[i]);
        }
        free(p);
    }
}

/**
 * @brief 根据条件对指定的收益率按年月统计数据进行排序
 * 1
 * @param pArray 待排序数组首指针
 * @param len 待排序数组长度
 * @param dirc 排序方向 true 降序 false 升序
 */
void makePopSort(PRANKMARK pArray, size_t len, bool dirc)
{
    if (pArray && len > 0)
    {
        PRANKMARK pRankPerMonth = pArray; //获取待排名数据
        size_t n = 0;
        size_t start, end = len - 1; //确定开始指针位置

        do
        {
            start = 0;
            do
            {
                printf("%d\n", ++n);
                RANKMARK temp;
                //确定比较数
                double value_n = *((pRankPerMonth + start)->pUnit->pValue + (pRankPerMonth + start)->pos);
                double value_n_1 = *((pRankPerMonth + start + 1)->pUnit->pValue + (pRankPerMonth + start + 1)->pos);
                if ((dirc ? value_n_1 > value_n : value_n_1 < value_n))
                { //判断是否需要交换
                    memcpy(&temp, pRankPerMonth + start, sizeof(RANKMARK));
                    memcpy(pRankPerMonth + start, pRankPerMonth + start + 1, sizeof(RANKMARK));
                    memcpy(pRankPerMonth + start + 1, &temp, sizeof(RANKMARK));
                }
                start++;
            } while (start < end);
        } while ((end--) > 0);
    }
}

/**
 * @brief 根据条件对指定的收益率按年月统计数据进行排序
 *
 * @param pArray 待排序数组首指针
 * @param len 待排序数组长度
 * @param method
 * @param dirc 排序方向 true 降序 false 升序
 */
void SortOfPrankInmonthY(PRANKINMONTHY pArray, size_t len, SORTMETHODS method, bool dirc)
{
    if (pArray && len > 0)
    {
        for (int month = 0; month < 12; month++) //按月份处理排名
        {
            size_t deep = 0;
            PRANKMARK pRankPerMonth = pArray->rank[month]; //获取待排名数据
            if (pRankPerMonth)
            {
                switch (method)
                {
                case SORTMETHODS::POP:
                    makePopSort(pRankPerMonth, len, dirc);
                    break;
                case SORTMETHODS::HEAP:
                    makeHeapSort(pRankPerMonth, len, dirc);
                    break;
                case SORTMETHODS::QUICK:
                    makequicksort(pRankPerMonth, 0, len, deep, dirc);
                    break;
                case SORTMETHODS::COMBINE:
                    MergeSort(pRankPerMonth, 0, len, dirc);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

/**
 * @brief 按月排名，统计指定范围内的收益率数据（排名在#%之後，？%之前的人員）
 *
 * @param pOut 接收整理結果的對象
 * @param pArray 已排序數據
 * @param len 排序數組總長度
 * @param startPos 開始排名百分數 <1
 * @param endPos 結束排名百分數 <=1
 * @return int 0 成功，非零 失敗
 */
int staticsInRange(PRANGERANKS pOut, PRANKINMONTHY pArray, size_t len, double startPos, double endPos)
{
    if (pOut && pArray && startPos < 1 && endPos <= 1 && startPos < endPos)
    {
        pOut->pSortedData = pArray;             //保存數據來源指針
        pOut->start = len * startPos;           //區間開始位置
        pOut->len = len * endPos - pOut->start; //區間長度(人次)
        sprintf(pOut->pos, "%.0f%% ~ %.0f%%", round(startPos * 100), round(endPos * 100));
        for (int j = 0; j < 12; j++)
        {
            pOut->M[j] = true;   //标记本月有效
            if (pArray->rank[j]) //當月存在數據
            {
                double value = 0, tpval = 0;
                size_t ncnt = 0;
                for (size_t i = pOut->start; i < (pOut->len + pOut->start - 1) && pOut->len > 0; i++)
                {
                    PDATAITEM pUnit = (pArray->rank[j] + i)->pUnit;
                    pUnit->ncounts = 0;      //清零计数器，为getPersonsRankedTimes调用做准备
                    pUnit->npointed = false; //关闭指向标记
                    //清空达标月份记录
                    memset(pUnit->M, 0, sizeof(bool) * 12);
                    tpval = *(pUnit->pValue + (pArray->rank[j] + i)->pos); //累積參量
                    value += tpval;
                    ncnt++;
                    if (i == pOut->start || tpval < pOut->MINE[j])
                        pOut->MINE[j] = tpval; //首值
                    if (i == pOut->start || tpval > pOut->MAXE[j])
                        pOut->MAXE[j]=tpval;
                }
                pOut->E[j] = value / ncnt; //紀錄範圍期望
            }
            else
                pOut->M[j] = false;
        }
        return 0;
    }
    else
        return -1;
}

/**
 * @brief 返回年内指定排序并指定之排名范围内，累计出现次数高于指定阈值的人员清单
 *
 * @param pRanks 输入按指定排名范围统计人员清单情况
 * @param nOccurs 出现次数之阈值（需大于1次)
 * @return PPERSONINRANGE
 * @note 调用此函数前必须调用函数staticsInRange，否则可能得到错误的结果
 */
PDATAITEM *getPersonsRankedTimes(PRANGERANKS pRanks, int nOccurs)
{
    PRANKINMONTHY pArray = pRanks->pSortedData; //获取源数据
    PDATAITEM pUnit = NULL;
    PDATAITEM *pTemp = NULL;
    size_t LEN = 0;
    if (pRanks && nOccurs > 1)
    {
        for (int j = 0; j < 12; j++)
        { //按月统计
            for (size_t i = pRanks->start; i - pRanks->start < pRanks->len - 1 && pRanks->len > 0; i++)
            {
                if (pRanks->M[j])
                {
                    pUnit = (pArray->rank[j] + i)->pUnit;
                    pUnit->ncounts++;   //累计出现次数
                    pUnit->M[j] = true; //标记该月份落在区间内
                }
            }
        } //完成按月统计
        //只取符合标准的人
        for (int j = 0; j < 12; j++)
        { //按月统计
            for (size_t i = pRanks->start; i - pRanks->start < pRanks->len - 1 && pRanks->len > 0; i++)
            {
                if (pRanks->M[j])
                {
                    pUnit = (pArray->rank[j] + i)->pUnit;
                    if (pUnit->ncounts >= nOccurs && !pUnit->npointed)
                    {
                        pUnit->npointed = true;
                        LEN++;
                        pTemp = (PDATAITEM *)realloc(pTemp, sizeof(PDATAITEM) * LEN); //登记
                        *(pTemp + LEN - 1) = pUnit;
                    }
                }
            }
        }
        if (pTemp) //补结尾空指针
        {
            LEN++;
            pTemp = (PDATAITEM *)realloc(pTemp, sizeof(PDATAITEM) * LEN);
            *(pTemp + LEN - 1) = NULL;
        }
        return pTemp; //返回链表
    }
    return NULL;
}

#define COMPLEMENTARYLEN 3
/**
 * @brief 按降序插入互补项目
 *
 */
void insertComplementaryByDesc(PDATAITEM *proot, double *Ep, PDATAITEM pWaitIn, char *year)
{
    double ep, temp;
    PDATAITEM ptemp;
    proot++; //重新定义proot起始位置，让开首项占位
    for (int i = 0; i < COMPLEMENTARYLEN; i++)
    {
        double ep = getExpValueOnMonth(pWaitIn, year); //待插入项目收益率和
        if (ep >= Ep[i])
        { //新项目比当前项目高，新项目插入原位，旧项目向后移动,尾部丢弃

            for (int j = COMPLEMENTARYLEN - 1; j > i; j--)
            {
                Ep[j] = Ep[j - 1];
                *(proot + j) = *(proot + j - 1);
            }
            Ep[i] = ep;
            *(proot + i) = pWaitIn;
            return; //完成交换，终止循环
        }
    }
}
/**
 * @brief 在输入的序列内找到互补序列
 *
 * @param pQuee
 * @param factor_one
 * @param ncounts 计数互补次数
 * @return PDATAITEM* 返回互补序列
 */
PDATAITEM *getComplementaryQuee(PDATAITEM needPartner, PDATAITEM pRoot, char *year, int ncounts)
{
    size_t LEN = COMPLEMENTARYLEN + 2; //只取有限个最佳值
    PDATAITEM *pTemp = (PDATAITEM *)malloc(sizeof(PDATAITEM) * (LEN));
    double Ep[LEN - 2];
    memset(Ep, 0, sizeof(double) * (LEN - 2));
    *pTemp = needPartner; //首项式
    *(pTemp + 1) = NULL;  //尾项式
    *(pTemp + LEN - 1) = NULL;
    int counts = 0;
    double rootval = 0, partnerval[12];
    for (int i = 0; i < 12; i++)
        partnerval[i] = getValueOnMonth(needPartner, year, i + 1);
    while (pRoot)
    {
        counts = 0;
        if (pRoot->number != needPartner->number)
        {
            for (int i = 0; i < 12; i++)
            {
                rootval = getValueOnMonth(pRoot, year, i + 1);
                if (!needPartner->M[i] && partnerval[i] < 0)
                { //当月排名靠后时

                    if (rootval + partnerval[i] > partnerval[i])
                        counts++; //当月互补
                }
                else //排名靠前时
                {
                    if (rootval + partnerval[i] < 0.1)            //如不能在赢月保持10%以上收益率，记为反互补
                        counts = (counts - 1 < 0) ? 0 : --counts; //当月互补
                }
            }
            if (counts >= ncounts) //互补月数超过指定值
                insertComplementaryByDesc(pTemp, Ep, pRoot, year);
        }
        pRoot = pRoot->pNext; //指向下一项
    }
    return pTemp;
}
//在rangeperson范围内找
PDATAITEM *getComplementaryQuee(PDATAITEM needPartner, PDATAITEM *pRoot, char *year, int ncounts)
{
    size_t LEN = 1;
    PDATAITEM *pTemp = NULL;
    pTemp = (PDATAITEM *)realloc(pTemp, sizeof(PDATAITEM) * LEN); //互补首项
    *pTemp = needPartner;
    int counts = 0;
    double rootval = 0, partnerval[12];
    for (int i = 0; i < 12; i++)
        partnerval[i] = getValueOnMonth(needPartner, year, i + 1);
    while (*pRoot)
    {
        counts = 0;
        if ((*pRoot)->number != needPartner->number)
        {
            for (int i = 0; i < 12; i++)
            {
                rootval = getValueOnMonth(*pRoot, year, i + 1);
                if (!needPartner->M[i] && partnerval[i] < 0)
                { //当月排名靠后时

                    if (rootval + partnerval[i] >= 0)
                        counts++; //当月互补
                }
                else //排名靠前时
                {
                    if (rootval + partnerval[i] < 0.1)            //如不能在赢月保持10%以上收益率，记为反互补
                        counts = (counts - 1 < 0) ? 0 : --counts; //当月互补
                }
            }
            if (counts >= ncounts) //互补月数超过指定值
            {
                LEN++;
                pTemp = (PDATAITEM *)realloc(pTemp, sizeof(PDATAITEM) * LEN); //登记
                *(pTemp + LEN - 1) = *pRoot;                                  //登记该项为互补项
            }
        }
        pRoot++; //指向下一项
    }
    if (pTemp) //补结尾空指针
    {
        LEN++;
        pTemp = (PDATAITEM *)realloc(pTemp, sizeof(PDATAITEM) * LEN);
        *(pTemp + LEN - 1) = NULL;
    }
    return pTemp;
}