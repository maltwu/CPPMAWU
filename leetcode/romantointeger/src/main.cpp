#include <iostream>
#include <string.h>
#include <limits>
using namespace std;
#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000
#define HUNDRED 100
class Solution
{
private:
    const string dicSingle[9] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven",
                                 "Eight", "Nine"};
    const string dicDecimal1[10] = {"Ten", "Eleven", "Twelve", "Thirteen",
                                    "Fourteen", "Fifteen", "Sixteen", "Seventing", "Eighting", "Nineteen"};
    const string dicDecimal2[8] = {"Twenty", "Thirty", "Fourty", "Fifty", "Sixty",
                                   "Seventy", "Eighty", "Ninety"};

public:
    string numberToWords(int num)
    {
        string rets;
        int org = num;
        int opr = org / BILLION;
        if (opr > 0)
        {
            solute3figure(rets, opr);
            safeappend(rets, "Billion");
        }
        org -= opr * BILLION;
        opr = org / MILLION;
        if (opr > 0)
        {
            solute3figure(rets, opr);
            safeappend(rets, "Million");
        }
        org -= opr * MILLION;
        opr = org / THOUSAND;
        if (opr > 0)
        {
            solute3figure(rets, opr);
            safeappend(rets, "Thousand");
        }
        org -= opr * THOUSAND;
        if (org > 0)
            solute3figure(rets, org);
        return rets;
    }
    /**
    a<=999
    处理百位数
    */
    void solute3figure(string &s, int a)
    {
        int org = a;
        if (a > 0 && a <= 999)
        {
            int opr = org / HUNDRED;
            org -= opr * HUNDRED;
            if (opr > 0)
            {
                safeappend(s, dicSingle[opr - 1]);
                safeappend(s, "Hundred");
            }
            if (org >= 20)
            {
                opr = org / 10;
                org -= opr * 10;
                safeappend(s, dicDecimal2[opr - 2]);
                if (org > 0)
                    safeappend(s, dicSingle[org - 1]);
            }
            else if (org >= 10)
            {
                org = org % 10;
                safeappend(s, dicDecimal1[org]);
            }
            else if (org > 0)
                safeappend(s, dicSingle[org - 1]);
        }
    }
    void safeappend(string &s, string input)
    {
        if (s.length() > 0)
        {
            s.append(" ");
            s.append(input);
        }
        else
            s.append(input);
    }
};

int main(int argc, char *argv[])
{
    int cc;
    Solution s1;
    cin >> cc;
    cout<<numeric_limits<int>::max()<<endl<<sizeof(int)<<endl;
    cout << s1.numberToWords(cc) << endl;
}