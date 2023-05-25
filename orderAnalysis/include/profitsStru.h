/**
 * @file profitsStru.h
 * @author your name (you@domain.com)
 * @brief 定义收益率统计所需的数据结构
 * @version 0.1
 * @date 2022-11-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PROFITSSTRUCT
#define PROFITSSTRUCT
#include <iostream>
/**
 * @brief 原始行数据存储链表
 *
 */
typedef struct _data_item
{
    size_t number;               //该条目在创建时序号
    size_t lablesize, valuesize; //数据指针大小（与数据类型相关）
    char *plable;                //标签数据
    char *pvaluemark;            //统计数据对应标签数组
    double *pValue;              //统计数据
    bool M[12];                  //统计落在区间内的月份
    int ncounts;                 //统计用户在序列中出现的次数
    bool npointed;               //标记该数据已被指向
    _data_item *pNext;           //指向下一条数据的指针
} DATAITEM, *PDATAITEM;
/**
 * @brief 排名标记，指向排名在指定位置的具体数据
 * @note  数据依赖基础数据DATAITEM存在而存在
 */
typedef struct _rank_mark
{
    PDATAITEM pUnit; //排名位置对应的data_item
    size_t pos;      //参与排名的数据在统计数据数组中的位置
} RANKMARK, *PRANKMARK;
/**
 * @brief 指定年份的各月排名
 *
 */
typedef struct _rank_in_month_a_year
{
    char year[6];
    PRANKMARK rank[12]; //按月保存，排名人员顺序数组
} RANKINMONTHY, *PRANKINMONTHY;
/**
 * @brief 紀錄指定年份各月指定範圍人員站隊位置及收益率期望
 * @note 数据依赖基础数据RANKINMONTHY存在而存在
 */
typedef struct _range_of_ranks
{
    size_t len;                //佔據區間的人數
    size_t start;              //佔據區間人群的開始位置
    char pos[10];            //紀錄開始結束百分比
    double E[12];              //區間收益率期望
    double MINE[12];           //区间最低收益率
    double MAXE[12];
    bool M[12];                //標記有有效數據的月
    PRANKINMONTHY pSortedData; //指向數據來源的指針
} RANGERANKS, *PRANGERANKS;
/**
 * @brief 按项统计数据存储单元
 *
 */
typedef struct _data_calcu_cell
{
    char *plable; //数据条目标签数据
    double eVal;  //期望
    double sTd;   //标准差
} DATACALCELL, *PDATACALCELL;
/**
 * @brief 按项统计数据存储链表
 *
 */
typedef struct _data_calcu_chain
{
    size_t number;
    _data_calcu_cell data;
    _data_calcu_chain *pNext;
} DATACALCHAIN, *PDATACALCHAIN;
#endif