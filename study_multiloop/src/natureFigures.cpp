#include <iostream>
#include <natureFigures>
#include <math.h>
using namespace std;

void printNatureFiguresSumEqM()
{
    long long M;
    cin >> M;
    long long start = 1, count = 0, end = 0;
    //自然數是等差數列 其和滿足 S = [n(a1+an)]/2
    while (start < M / 2)
    {
        end = start + 1;
        while (end < M)
        {
            long long n = end - start + 1; //保留等差數列長度
            if (M == (n * (start + end)) / 2)
            {
                cout << "解" << ++count << ":[" << start << "," << end << "]";
                count % 5 == 0 ? cout << endl : cout << "  ";
            }
            end++;
        }
        start++;
    }
    cout<<endl;
}

void printNatureFiguresSumEqMOptimized1st()
{
    long long M;
    cin >> M;
    long long start = 1, count = 0, end = 2;
    //自然數是等差數列 其和滿足 S = [n(a1+an)]/2
    while (start < M / 2)
    {
        while (end < M)
        {
            long long n = end - start + 1; //保留等差數列長度
            if (M == (n * (start + end)) / 2)
            {
                cout << "解" << ++count << ":[" << start << "," << end << "]";
                count % 5 == 0 ? cout << endl : cout << "  ";
            }
            else if ((n * (start + end)) > 2 * M)
                break;
            end++;
        }
        start++;
    }
    cout<<endl;
}

void printNatureFiguresSumEqMOptimized2nd()
{
    int M;
    cin >> M;
    int count = 0, n = sqrt(2 * M);
    //自然數是等差數列 其和滿足 S = [(2*start+n-1)n]/2  end = start+n-1
    //當 end 和 start 滿足條件時 start = (2*M/n+1-n)/2 -----式1
    for (int n = sqrt(2 * M); n > 1; n--)
    {
        if ( 2*M%n==0&&(2 * M / n + 1 - n) % 2 == 0) // n整除2M，且start有由式1確定的整數解，及給定n找到配合start使S=M成立
        {
            cout << "解" << ++count
             << ":[" << (2 * M / n + 1 - n) / 2 << "," 
             << (2 * M / n + 1 - n) / 2 + n - 1 << "]";
            count % 5 == 0 ? cout << endl : cout << "  ";
        }
    }
    cout<<endl;
}