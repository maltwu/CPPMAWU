
#include "profitsStru.h"

#define TOPCOUNTS 10

enum SORTMETHODS
{
    POP,
    HEAP,
    QUICK,
    COMBINE
};
enum DATAINDEX
{
    D2020,
    D2021,
    D2022
};

void clearDataChain(PDATACALCHAIN);
PDATACALCHAIN calExpectionByPersons(PDATAITEM, char *at_year = NULL);
void calExpectionOnPerson(DATACALCELL &, PDATAITEM, char *at_year = NULL);
void getTop10(PDATACALCHAIN, DATACALCHAIN[], DATACALCHAIN[]);
double getValueOnMonth(PDATAITEM, char *, int);
PRANKINMONTHY AssignByProfitsPerM(PDATAITEM, char *, size_t);
void ClearAssignByProfitsPerM(PRANKINMONTHY);

void SortOfPrankInmonthY(PRANKINMONTHY, size_t, SORTMETHODS, bool dirc = true);

int staticsInRange(PRANGERANKS, PRANKINMONTHY, size_t, double startPos = 0, double endPos = 1);

PDATAITEM* getPersonsRankedTimes(PRANGERANKS, int);
PDATAITEM *getComplementaryQuee(PDATAITEM , PDATAITEM , char *, int );
PDATAITEM *getComplementaryQuee(PDATAITEM , PDATAITEM* , char *, int);

// qucksort.cc
void makequicksort(PRANKMARK, size_t, size_t, size_t &, bool isBig = true);
// heapsort.cc
void makeHeapSort(PRANKMARK prank, size_t nlength, bool isBig = true);
double getRankOrgVal(PRANKMARK);
void heapsort_swap(PRANKMARK, PRANKMARK);
// mergesort.cc
void MergeSort(PRANKMARK, size_t, size_t, bool isBig = true);

// printInfo.cc
void printRangeStatics(PRANGERANKS, int);
void printPersonInRangeByYear(PDATAITEM *, RANGERANKS,bool lossable);