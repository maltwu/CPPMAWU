#include <iostream>
#include <hundred>
using namespace std;
void hundredChicks()
{
    float n, m;
    int columns = 0;
    cin >> n >> m;
    //花n钱，买m只鸡的问题
    for (float hen = 1; hen <= m; hen++)
    {
        for (float cock = 1; cock <= m; cock++)
        {
            float chick = 100 - hen - cock; //留意小鸡是1文钱3只，故整买时无法只买到1只小鸡
            // 5*cock+3*hen+(1/3)*chick
            if (5 * cock + 3 * hen + chick / 3 == 100 && (int)chick % 3 == 0 && chick >= 1)
                cout << cock << ',' << hen << ',' << chick << endl;
        }
    }
    //求解 n~m之间所有回文素数
    /* if (n > m)
    { //保证小值在前
        int temp = n;
        n = m;
        m = temp;
    }
    for (int i = n; i <= m; i++)
    {
        int flag = 1;
        for (int j = 2; j * j <= i; j++)
        { //判断素数
            if (i % j == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1)
        { //确定素数再判断是否回文
            int wei = 0, value = i;
            do
            {
                wei = wei * 10 + value % 10;
                value /= 10;
            } while (value > 0);
            /* int *pbase = NULL;
            do
            {
                wei++;
                pbase = (int *)realloc(pbase, sizeof(int) * wei);
                *(pbase + wei - 1) = value % 10; //从低到高保留到个位
                value /= 10;					 //每次降低10倍
            } while (value > 0);
            for (int k = 0, w = wei - 1; k < wei; k++, w--)
            {								//逆向重组数字
                int current = *(pbase + k); //取原数字的第k+1位【从低到高排序】
                for (int cnt = w; cnt > 0; cnt--)
                    current *= 10; //移动到对应高位
                value += current;  //组合成逆序数字
            }
            free(pbase); //释放临时内存 */
    /* 			if (wei == i)
                { //找到回文数输出
                    cout << i;
                    if (++columns % 10 == 0) //每10个元素换行
                        cout << endl;
                    else
                        cout << " ";
                }
            }
        } */
    cout << endl;
}