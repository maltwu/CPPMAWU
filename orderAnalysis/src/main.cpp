#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <sys/resource.h>
#include "calculate.h"
#define ORIGINPATH 0

using namespace std;

static const string originpath[2] = {"/Users/malt/Desktop/原始數據/老王_回调加仓_每月数据.csv",
                                     "/Users/malt/Desktop/原始數據/老王_回调加仓_每月数据_Short.csv"};

/**
 * @brief 本地字符串连接函数
 *
 * @param des
 * @param src
 */
void localstrcat(char *des, char *src)
{
    if (src[0] != '\0')
    {
        strcat(des, src);
        strcat(des, "_");
    }
}
/**
 * @brief 访问系统资源情报
 *
 *
 */
void setStackSize()
{
    rlimit syslimit;
    int ret = getrlimit(RLIMIT_CPU, &syslimit);
    printf("CPU rlimit:(%llu) (%llu)\n", syslimit.rlim_cur, syslimit.rlim_max);
    ret = getrlimit(RLIMIT_STACK, &syslimit);
    printf("STACK rlimit:(%llu) (%llu)\n", syslimit.rlim_cur, syslimit.rlim_max);
}
/**
 * @brief 安全释放数据链
 * @note  不可使用递归方式，栈空间不够
 */
void FREEPDATAITEM(PDATAITEM p)
{
    size_t nCount = 0;
    PDATAITEM interP = p, freeP = NULL;
    while (interP)
    {
        if (interP->pValue)
            free(interP->pValue);
        if (interP->plable)
            free(interP->plable);
        if (interP->pvaluemark)
            free(interP->pvaluemark);
        freeP = interP;
        interP = freeP->pNext;
        free(freeP);
        nCount++;
    }
    printf("已释放备处置项目%d个\n", nCount);
}
/**
 * @brief 获得标签数据索引位置
 *
 * @param ptitle 標題數組
 * @return int*  索引组数【0】数组长度，即有多少关重字段项
 */
int *getKeyIndex(char ptitle[][50])
{
    string keyWords[] = {
        "策略ID", "模拟杠杆", "加仓比例", "回撤比例", "留存比例", "下单杠杆" /*, "模拟杠杆余额",
         "最终余额", "最大回撤", "最大余额", "策略简码"*/
    };
    int *pRet = (int *)malloc(sizeof(int));
    *pRet = 0;
    for (int i = 0; i < 150; i++)
    {
        bool finded = false;
        for (int j = 0; j < 6; j++)
        {

            string s = string(ptitle[i]);
            int cpos = keyWords[j].find(s);
            if (cpos != s.npos && cpos >= 0)
            {
                finded = true;
                break;
            }
        }
        if (finded)
        {
            (*pRet) += 1;
            pRet = (int *)realloc(pRet, sizeof(int) * (*pRet + 1));
            *(pRet + (*pRet)) = i;
        }
    }
    return pRet;
}
/**
 * @brief 获取统计数据字段对应位置
 *
 * @param ptitle
 * @return int* 索引组数【0】数组长度，即有多少关重字段项
 */
int *getKeyDataIndex(char ptitle[][50])
{
    string keyword = "收益率";
    int *pRet = (int *)malloc(sizeof(int));
    *pRet = 0;
    for (int i = 0; i < 150; i++)
    {
        string s = string(ptitle[i]);
        size_t cpos = s.find(keyword);
        if (cpos != s.npos && cpos >= 0)
        {
            (*pRet) += 1;
            pRet = (int *)realloc(pRet, sizeof(int) * (*pRet + 1));
            *(pRet + (*pRet)) = i;
        }
    }
    return pRet;
}
/**
 * @brief 字符串转双精度浮点
 *
 * @param s
 * @return double
 */
double strTodbl(string s)
{
    double ret;
    stringstream ss;
    ss << s;
    ss >> setprecision(16) >> ret; //<iomapip>
    return ret;
}

int main()
{
    string fPath = originpath[ORIGINPATH];

    //读取指定文件
    FILE *fstream = fopen((char *)fPath.data(), "r");
    //关键字索引数组
    int *pKwIndex = NULL, *pKwDaIndex = NULL;
    //待统计运算数据链指针
    PDATAITEM pDataLink = NULL; //链表所有者指针
    PDATAITEM pDataPos = NULL;  //链表当前处理位置指针
    if (fstream)
    {
        char buffer[1000];
        char title[150][50]; //標題數組
        char data[150][50];  //數據數組
        int strPos = 0,
            charPos = 0, lines = 0;
        bool isQuoting = false /*引號標記*/, isJustReturn = false /*連續換行符*/;
        memset(title, 0, sizeof(char) * 150 * 50);
        memset(data, 0, sizeof(char) * 150 * 50);
        int readBytes = 0, validUnits = 0;
        while (!feof(fstream))
        {
            memset(buffer, 0, sizeof(char) * 1000); //清理讀緩衝
            readBytes = fread(buffer, sizeof(char), 1000, fstream);
            if (ferror(fstream))
            {
                printf("文件块读取出错");
                break;
            }
            else
            {
                //获取本次读取有效单元数
                validUnits = readBytes / sizeof(char);
                for (int i = 0; i < validUnits; i++)
                {
                    if ((buffer[i] != ',' && buffer[i] != '\r' && buffer[i] != '\n') || isQuoting)
                    {
                        isJustReturn = false;       //換行後取得意義字符，即確認本次操作還行完成，可接受下一次還行
                        if (buffer[i] == '"')       //有引號時視分隔符號為普通符號，該算法不能處理“”“”形式的引號嵌套
                            isQuoting = !isQuoting; //反轉引號
                        else
                        {
                            if (lines == 0) //標題行
                                title[strPos][charPos++] = buffer[i];
                            else //數據行
                                data[strPos][charPos++] = buffer[i];
                        }
                    }
                    else
                    {
                        if (lines == 0)                      //標題行
                            title[strPos++][charPos] = '\0'; //插入字符串结束符号
                        else                                 //數據行
                            data[strPos++][charPos] = '\0';
                        charPos = 0; //子下标归位
                        if (buffer[i] == '\r' || buffer[i] == '\n')
                        {                      //行結束
                            if (!isJustReturn) //確認為一次換行
                            {
                                lines++;
                                isJustReturn = true; //標記已觸發一次換行，避免連續換行行為發生
                                if (lines == 1 && !pKwIndex && !pKwDaIndex)
                                {                                        //處理标题栏，确定关键字段索引
                                    pKwIndex = getKeyIndex(title);       //关键标题位置
                                    pKwDaIndex = getKeyDataIndex(title); //关键数据位置
                                }
                                else if (lines > 1)
                                { //整理数据列表
                                    if (!pDataLink)
                                    {
                                        pDataLink = (PDATAITEM)malloc(sizeof(DATAITEM));
                                        memset(pDataLink, 0, sizeof(DATAITEM));
                                        pDataLink->number = 1;
                                        pDataLink->pNext = NULL;
                                        pDataPos = pDataLink; //登記當前鏈表位置
                                    }
                                    else
                                    {
                                        pDataPos->pNext = (PDATAITEM)malloc(sizeof(DATAITEM));
                                        memset(pDataPos->pNext, 0, sizeof(DATAITEM));
                                        pDataPos->pNext->number = pDataPos->number + 1;
                                        pDataPos->pNext->pNext = NULL;
                                        pDataPos = pDataPos->pNext;
                                    }
                                    //登記標籤數據
                                    pDataPos->lablesize = 50 * (*pKwIndex);
                                    pDataPos->plable = (char *)malloc(sizeof(char) * pDataPos->lablesize);
                                    pDataPos->valuesize = (*pKwDaIndex);
                                    pDataPos->pvaluemark = (char *)malloc(sizeof(char) * 50 * (*pKwDaIndex));
                                    pDataPos->pValue = (double *)malloc(sizeof(double) * (*pKwDaIndex));
                                    //填充標籤數據
                                    for (int i = 0; i < (*pKwIndex); i++)
                                        localstrcat(pDataPos->plable, data[*(pKwIndex + i + 1)]);
                                    for (int i = 0; i < (*pKwDaIndex); i++)
                                    {
                                        strcpy((pDataPos->pvaluemark + i * 50), title[*(pKwDaIndex + i + 1)]); //复制数据对应标签
                                        string svalue = string(data[*(pKwDaIndex + i + 1)]);
                                        double value = strTodbl(svalue);
                                        *(pDataPos->pValue + i) = value;
                                    }
                                    memset(&data[0][0], 0, sizeof(char) * 150 * 50); //清空行
                                }                                                    //数据列表整理完毕
                            }
                            strPos = 0;
                        } //换行判断结束
                    }
                } // for end
            }
        } // while end
        if (strPos && pDataLink && pDataPos && lines > 1)
        { //处理尾行漏读
            lines++;
            pDataPos->pNext = (PDATAITEM)malloc(sizeof(DATAITEM));
            memset(pDataPos->pNext, 0, sizeof(DATAITEM));
            pDataPos->pNext->number = pDataPos->number + 1;
            pDataPos->pNext->pNext = NULL;
            pDataPos = pDataPos->pNext;
            pDataPos->lablesize = 50 * (*pKwIndex);
            pDataPos->plable = (char *)malloc(sizeof(char) * pDataPos->lablesize);
            pDataPos->valuesize = (*pKwDaIndex);
            pDataPos->pvaluemark = (char *)malloc(sizeof(char) * 50 * (*pKwDaIndex));
            pDataPos->pValue = (double *)malloc(sizeof(double) * (*pKwDaIndex));
            //填充標籤數據
            for (int i = 0; i < (*pKwIndex); i++)
                localstrcat(pDataPos->plable, data[*(pKwIndex + i + 1)]);
            for (int i = 0; i < (*pKwDaIndex); i++)
            {
                strcpy((pDataPos->pvaluemark + i * 50), title[*(pKwDaIndex + i + 1)]); //复制数据对应标签
                string svalue = string(data[*(pKwDaIndex + i + 1)]);
                double value = strTodbl(svalue);
                *(pDataPos->pValue + i) = value;
            }
        }
        printf("读取数据计%d行\n", lines - 1);

        PRANKINMONTHY Data_Year[3]{
            AssignByProfitsPerM(pDataLink, "2020", lines - 1), //获取指定年份月列表
            AssignByProfitsPerM(pDataLink, "2021", lines - 1),
            AssignByProfitsPerM(pDataLink, "2022", lines - 1)};
        for (int i = 0; i < 3; i++) //排序
            SortOfPrankInmonthY(Data_Year[i], lines - 1, SORTMETHODS::COMBINE);
        RANGERANKS Range_Data[3];
        PDATAITEM *Range_Person[3];
        PDATAITEM *Range_Complementary = NULL;
        int thred = 6;
        for (int i = 0; i < 3; i++)
        {
            staticsInRange(&Range_Data[i], Data_Year[i], lines - 1, 0, 0.01);
            printRangeStatics(Range_Data, 3);
            Range_Person[i] = getPersonsRankedTimes(&Range_Data[i], thred);
            // printPersonInRangeByYear(Range_Person[i], Range_Data[i], true);

         /*    size_t nCount = 0;
            while (Range_Person[i] && *(Range_Person[i] + nCount))
            { //从第一项开始在全局找互补项
                Range_Complementary = getComplementaryQuee(*(Range_Person[i] + nCount),
                                                        //    Range_Person[i],
                                                           pDataLink,
                                                           Data_Year[i]->year, (12-thred)*0.4);
                if (Range_Complementary)
                {
                    printPersonInRangeByYear(Range_Complementary, Range_Data[i], true);
                    free(Range_Complementary);
                    Range_Complementary = NULL;
                }
                nCount++;
            } */
        }
        fclose(fstream);
        if (pKwIndex)
            free(pKwIndex);
        if (pKwDaIndex)
            free(pKwDaIndex);
        FREEPDATAITEM(pDataLink);
        // clearDataChain(pStatisticData);
        for (int i = 0; i < 3; i++)
        {
            ClearAssignByProfitsPerM(Data_Year[i]);
            free(Range_Person[i]);
        }
    }
}