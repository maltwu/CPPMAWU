#include "profitsStru.h"
#include "calculate.h"
#include <cmath>

/**
 * @brief 打印區間內的排名數據
 *
 * @param pArray
 * @param counts
 */
void printRangeStatics(PRANGERANKS pArray, int counts)
{
    PRANGERANKS cur = pArray;
    if (pArray && counts > 0)
    {
        for (int i = 0; i < counts; i++, cur++)
        {
            for (int j = 0; j < 12; j++)
            {
                if (cur->M[j])
                {
                    printf("%s年%d月 ", cur->pSortedData->year, j + 1);
                    printf("區間:%s 共%d人次 ", cur->pos, cur->len);
                    printf("收益期望:%.5f 最大收益:%.5f 最小收益:%.5f\n", cur->E[j],cur->MAXE[j],cur->MINE[j]);
                }
            }
        }
    }
}
/**
 * @brief 打印区域内人员多次出现情况
 *
 * @param ppPerson 要打印的人员列表
 * @param rank 配套人员列表对应的统计数据
 * @param lossable 是否允许亏损
 */
void printPersonInRangeByYear(PDATAITEM *ppPerson, RANGERANKS rank, bool lossable)
{
    double val;
    int n = 0;
    if (ppPerson && (*ppPerson)&&*(ppPerson+1) && rank.len > 0)//仅打印有互补项的数据
    {
        while (*ppPerson)
        {
            n++;
            if (!lossable)
            { //不允许亏损，则需要预检
                for (int i = 0; i < 12; i++)
                {
                    val = getValueOnMonth((*ppPerson), rank.pSortedData->year, i + 1);
                    if (val < 0 && !(*ppPerson)->M[i])
                    {
                        goto NEXT; //任月数据小于0且该月非胜利则换下一个
                        break;
                    }
                }
            }
            printf("%s年 ", rank.pSortedData->year);
            printf("区间:%s id:%#6d %d个月在区间内 ", rank.pos, (*ppPerson)->number, (*ppPerson)->ncounts);
            for (int i = 0; i < 12; i++)
            {
                val = getValueOnMonth((*ppPerson), rank.pSortedData->year, i + 1);
                printf("M%.2d_%c_%.3f ", i + 1, (*ppPerson)->M[i] ? 'Y' : 'N', val);
            }
            printf("\n");
        NEXT:
            ppPerson++;
        }
        printf("***************************\n");
    }
}