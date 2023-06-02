#include <iostream>
using namespace std;

int main()
{
    int T, N, M;
    string msg;
    cin >> T;
    while (T-- > 0)
    {
        cin >> N >> M;
        int group[2 * N + 1], high[2 * N + 1]; //对应影子集合的概念为 指定成员的敌对成员集合
        for (int i = 0; i < 2 * N + 1; ++i)
        {
            group[i] = i;
            high[i] = 0;
        }
        while (M-- > 0)
        {
            char cmd;
            int a, b;
            cin >> cmd >> a >> b;
            auto getunionroot = [&](int x) -> int
            {
                int *num = nullptr;
                int pos = -1;
                while (group[x] != x)
                {
                    num = (int *)realloc(num, (++pos) << 2);
                    num[pos] = x;
                    x = group[x];
                }
                while (pos >= 0)
                    group[num[pos--]] = x;
                if (num)
                    free(num);
                return x;
            };
            /**
             *启发式合并，矮树合并高树，此做法对于整理整个路径上所有节点的父都直接
             *指向根的算法来说，其实是多余的操作（因每次查询都直接降低树高度到1）
             *此处保留此处理逻辑，唯提醒有此方式来处理归并集的一个流派（查询时不做
             *逆溯变更各节点指向父亲的指针 
             */           
            auto addenemyunion = [&](int x, int y) -> void
            {
                int xroot = getunionroot(x);
                int yroot = getunionroot(y);
                if (xroot != yroot)
                {
                    if (high[xroot] > high[yroot])
                        group[yroot] = xroot;
                    else
                        group[xroot] = yroot;
                    if (high[xroot] == high[yroot])
                        high[yroot]++;
                }
                return;
            };
            auto judge = [&](int a, int b) -> void
            {
                int aroot = getunionroot(a);
                int broot = getunionroot(b);
                int aEroot = getunionroot(a + N);
                int bEroot = getunionroot(b + N);
                if (aroot == broot || aEroot == bEroot)
                    msg.append("In the same gangs\n");
                else if (aroot == bEroot || broot == aEroot)
                    msg.append("In different gangs\n");
                else
                    msg.append("Not sure yet\n");
            };
            switch (cmd)
            {
            case 'D':
                // a与b对立，则分别加入各自的对立集合
                addenemyunion(a, b + N);
                addenemyunion(b, a + N);
                break;
            case 'A':
                judge(a, b);
                break;
            }
        }
    }
    if (!msg.empty())
        cout << msg;
}