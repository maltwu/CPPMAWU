#include <iostream>
#include <stack>
#include <sys/time.h>
#include <cmath>
using namespace std;

int init_ = []() -> int
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	return 0;
}();

int main(int argc, char *argv[])
{
	timeval nowtime;	
    gettimeofday(&nowtime, NULL);
	freopen("output/input.in", "r", stdin);
	int citys, roads;
	stack<int> parents;
	while (cin >> citys >> roads)
	{
		int CITY[citys + 1];
		for (int i = 1; i <= citys; ++i)
			CITY[i] = i;
		for (int i = 0; i < roads; ++i)
		{
			int city1, city2;
			cin >> city1 >> city2;
			//关联城市，偏向小序号城市为根
			if (city1 > city2)
				CITY[city1] = city2;
			else
				CITY[city2] = city1;
		}
		//整理集合分类
		for (int i = 1; i <= citys; ++i)
		{
			int parent = i;
			while (CITY[parent] != parent)
			{						  //非自身父节点
				parents.push(parent); //登记要处理的父亲节点脚标
				parent = CITY[parent];
			}
			while (!parents.empty())
			{
				CITY[parents.top()] = parent;
				parents.pop();
			}
		}
		//统计集合数量,统计根的数量
		int cnt = 0;
		for (int i = 1; i <= citys; ++i)
		{
			if (i == CITY[i]) //当前节点是否为根节点（仅根节点内存放的是自身标号，其它节点内存放的是其父节点的标号)
				cnt++;
		}
		cout << cnt - 1 << endl;
	}
	timeval endtime;
	gettimeofday(&endtime, NULL);
	auto difftimebetween=[](timeval start,timeval end)->double {
		double startt = start.tv_sec+(double)start.tv_usec/1000/1000;
		double endt = end.tv_sec+(double)end.tv_usec/1000/1000;
		return endt-startt;
	};
	printf("Procedure costed %.8f secs \n",difftimebetween(nowtime,endtime));
	return 0;
}