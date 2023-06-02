#include <iostream>
#include <vector>
using namespace std;
struct INFO
{
	int parent;
	int level; //食物链内的等级初始为0阶
};
int getObjCatagory(int x, vector<INFO> &Animals, int &level)
{
	if (Animals[x].parent != x)
	{
		/*
		2-》1-》0-》2 循环
		*/
		Animals[x].parent = getObjCatagory(Animals[x].parent, Animals, level);
		level = Animals[x].level = (level + Animals[x].level) % 3;
	}
	else
		level = Animals[x].level;
	return Animals[x].parent;
}
//以食物链来分类动物，而非以同类来分类动物
int main(int argc, char *argv[])
{
	int N, K;
	cin >> N >> K;
	vector<INFO> Animals(1, INFO{0, 0});
	for (int i = 1; i <= N; ++i)
	{
		Animals.push_back(INFO({i, 0}));
	}
	int fakes = 0;
	while (K-- > 0)
	{
		int c, x, y;
		cin >> c >> x >> y;
		if (x > N || y > N || x < 1 || y < 1)
			fakes++;
		else
		{
			int level;
			int xroot = getObjCatagory(x, Animals,level);
			int yroot = getObjCatagory(y, Animals,level);
			if (xroot == yroot)
			{ /*
				  x,y在一條食物鏈中。
				  1、若c=1 需x.level=y.level時同类之说为真话，(x.level-y.level)%3=0
				  2、若c=2 若 x 吃 y 为真，则组合为(1,0),(2,1),(0,2) 即 x.level-y.level = 1 或 -2 时为真话
				  3、考虑统一算式 (x.level-y.level+m-c)%3 在任何条件为真时都为0,可知需保证条件1，2各组合下有(x.level-y.level+m-c)=3k，则在不同情况下，真话可表示为
					 c=1 x-y=0 m=3k+1;c=2 x-y=1 m=3k+1;c=2 x-y=-2 m=3k+4=3(k+1)+1=3K+1  故m的表达形式统一，其最接近的不会导致计算中产生负数的选择值可选4
			   */

				fakes += (Animals[x].level - Animals[y].level + 4 - c) % 3 != 0 ? 1 : 0;
			}
			else
			{
				//类别不同时，对x 与 y 同类，或者 x 吃 y 都考虑将x所在集合合并至y所在集合
				if (Animals[x].parent == x)
				{ //并入项为游离集合,直接合并至y节点后
					Animals[x].parent = y;
					Animals[x].level = c - 1; //此附值保证回头查询x时,更新level = (x.level+parent.level)%3时，可得到正确的食物链关系
				}
				else
				{
					/*
					1、x所在集合根对接至y所在集合根处，需根据x,y关系确定x集合根所在层级
					2、初始根层级都是0，则在集合有序的情况下（查询x,y时必然得到到x,y有序的集合）若
					   c=1 则 x.level_new-y.level = 0;
					   c=2 则 x.level_new-y.level = 1;  暂不考虑环形 可令 3-》2
					3、对x的根节点r层级,其相对x节点的层级跨度需一直保持,即在食物链环上x 和 r 移动相同的步数，故有 r.level_new-r.level = x.level_new-x.level
					   r.level =0 则对
					   c=1 有 r.level_new = (y.level-x.level)%3; 考虑周期3的循环性
					   c=2 有 r.level_new = (y.level-x.level+1)%3;
					   合并c有 r.level_new = (y.level-x.level+c-1)%3=(y.level-x.level+3+c-1)%3  加入整数因子3,使食物链完成闭环 -1：：2，-2:：1
					*/
					Animals[xroot].level = (Animals[y].level - Animals[x].level + 2 + c) % 3;
					Animals[xroot].parent = yroot;
				}
			}
		}
	}
	cout << fakes << endl;
	return 0;
}