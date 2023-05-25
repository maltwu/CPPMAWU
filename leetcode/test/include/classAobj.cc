#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
using namespace std;
struct Position
{
	long row;
	long col;
};
enum DIR
{
	U = 0,
	D = 1,
	L = 2,
	R = 3,
	N = -1
};
class Solution
{
public:
	/**
	 * @brief
	 *1、路径一定时有穷的
	 *2、不考虑人的位置，从起点开始任选一个方向进行，选择延各方向前进的最短路径作为前进路径得到最终的最小路径
	 * @param grid
	 * @return int
	 */
	int minPushBox(vector<vector<char>> &grid)
	{
		Position T;
		Position B;
		Position S;
		long rows = grid.size();
		long cols = grid[0].size();
		int valids = 0;
		//确定各要素的初始位置
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				switch (grid[i][j])
				{
				case 'T':
					T.row = i;
					T.col = j;
					valids++;
					break;
				case 'B':
					B.row = i;
					B.col = j;
					valids++;
					break;
				case 'S':
					S.row = i;
					S.col = j;
					valids++;
					break;
				}
				if (valids == 3)
					goto START;
			}
		}
	START:
		return calculateBoxRoute(grid, T, B, S);
	}
	struct PROSTEP
	{
		Position Major;
		Position Secondary;
		int bestSteps;			//从当前箱子到目标点的最好步数
		int realSteps;			//从当前箱子到目标点的实际步数
		pair<DIR, int> dirc[4]; //按优先级从高到低进行方向排序
		int dirselect;			//当前选择的方向位置
		bool avoided;			//登记是否出现绕路
	};
	int calculateBoxRoute(vector<vector<char>> &grid, Position T, Position B, Position P)
	{
		if (T.row == B.row && T.col == B.col)
			return 0;
		int Box[20][20] = {{0}}; //累计箱子路过同一点的次数
		long rows = grid.size();
		long cols = grid[0].size();
		vector<PROSTEP> routin;
		int ret = -1;
		bool finished = false; //完成一条路径?
		// int retrys = 0;												   // 记录最小可回退的节点
		stack<int> canBack;											   //登记最小根节点
		PROSTEP startStep = initBoxStep(T, B, Position{-1, -1}, grid); //箱子准备，人员占位准备，首选方向准备
		routin.push_back(startStep);
		Position curPerson = P; //初始化人员占位
		while (!routin.empty())
		{
			while (routin.back().dirselect < 4 && !finished)
			{
				//判断人员是否可从当前位置移动到所需占位
				int personStep = calculatePersonCan(grid, routin.back().Secondary, curPerson);
				if (personStep < 0)
				{ //人员无法就位，需换方向尝试
					ChangeMovementDir(routin.back());
					continue;
				}
				curPerson = routin.back().Secondary;																		//人员就位																  //更新人员当前位置
				Position newBox = moveObject(routin.back().Major, routin.back().dirc[routin.back().dirselect].first, grid); //如果箱子可移动的下一个位置
				if (newBox.row < 0 || Box[newBox.row][newBox.col] >= 2)														//允许经过本点 判断人员能否进入优先
				{																											//同一路径不能经历2次以上,否则该点不能到达
					ChangeMovementDir(routin.back());
					continue;
				}
				else if (Box[newBox.row][newBox.col] == 1)
				{
					//退步要求，退步必须获利，否则不可退步
					double disorg = pow(routin.back().Major.row - T.row, 2) + pow(routin.back().Major.col - T.col, 2);
					double disnew = pow(newBox.row - T.row, 2) + pow(newBox.col - T.col, 2);
					if (disnew >= disorg)
					{ //未获利，不能退步
						ChangeMovementDir(routin.back());
						continue;
					}
				}

				if (newBox.row == T.row && newBox.col == T.col)
					finished = true;
				//移动箱子在图表内的位置(计算人员就位时需要)
				grid[routin.back().Major.row][routin.back().Major.col] = '.';
				//真实移动箱子
				Box[newBox.row][newBox.col]++;
				for (auto &node : routin) //更新各节点距离目标的累计步数
					node.realSteps++;
				if (newBox.row == 5 && newBox.col == 8)
					int a = 4;
				routin.push_back(initBoxStep(T, newBox, routin.back().Major, grid)); //按箱子位置准备下一节点
				grid[routin.back().Major.row][routin.back().Major.col] = 'B';
				if (routin.size() - 1 != routin.front().realSteps)
					int i = 2;
			}
			if (finished)
			{

			/* 	cout << "Counts==" << routin.front().realSteps << endl;
				printRoutin(grid, routin); */
				ret = ret == -1 ? routin.front().realSteps : min(ret, routin.front().realSteps);
				if (ret == routin.front().bestSteps)
					break;
			REPEAT:
				int backSteps = 0;
				//探查是否有其它方向可以选择
				while (!routin.empty() &&
					   (!routin.back().avoided ||
						routin.back().dirselect >= 3)) // 已经是最好路径 以及已经没有多余方向可选的直接回退
				{
					if (!canBack.empty() && canBack.top() == routin.size()) //仅当被删除点为分叉点时，弹出分叉点记录
						canBack.pop();
					Box[routin.back().Major.row][routin.back().Major.col]--; //必须在节点退出前处理，方是针对路径的处理
					grid[routin.back().Major.row][routin.back().Major.col] = '.';
					routin.pop_back();
					if (!routin.empty())
						grid[routin.back().Major.row][routin.back().Major.col] = 'B';
					backSteps++;
				}
				grid[T.row][T.col] = 'T'; //确保目标标记不被覆盖
				if (!routin.empty())
				{
					for (auto &node : routin)			 //更新各节点距离目标的累计步数
						node.realSteps -= backSteps;	 //回退到指定位置，并重新开始
					curPerson = routin.back().Secondary; //人员原始站位要回退到来时原位
					ChangeMovementDir(routin.back());	 //变更一个路径再次尝试
					canBack.push(routin.size());
					/* 	cout << "Retry " << ++retrys << endl;
						printGraph(grid); */
					finished = false; //重新开
				}
			}
			else
			{
				if (!canBack.empty() && routin.size() == canBack.top())
					//路径删除到分叉点处，不可继续删除，需重新规划处理
					goto REPEAT;
				else
				{
					//当前节点无法继续前进需关闭，并重标箱子位置
					grid[routin.back().Major.row][routin.back().Major.col] = '.';
					routin.pop_back();
					if (!routin.empty())
					{
						int cnt = 0;
						grid[routin.back().Major.row][routin.back().Major.col] = 'B';
						for (auto &node : routin) //更新各节点距离目标的累计步数
						{
							cnt++;
							node.realSteps--; //统计无效步骤
						}
						curPerson = routin.back().Secondary; //人员原始站位要回退到来时原位
						ChangeMovementDir(routin.back());	 //变更一个路径再次尝试
					}
				}
			}
		}
		return ret;
	}
	int calculatePersonCan(vector<vector<char>> &grid, Position T, Position P)
	{
		if (T.row == P.row && T.col == P.col)
			return 0;
		int Person[20][20] = {{0}}; //累计箱子路过同一点的次数,单物体移动，各点不会出现两次及以上经过情况
		long rows = grid.size();
		long cols = grid[0].size();
		vector<PROSTEP> routin;
		int ret = -1;
		bool finished = false;
		PROSTEP startStep = initPersonStep(T, P, grid);
		routin.push_back(startStep);
		Position curPerson = P; //初始化人员占位
		while (!routin.empty())
		{
			while (routin.back().dirselect < 4 && !finished)
			{
				Position newPerson = moveObject(routin.back().Major, routin.back().dirc[routin.back().dirselect].first, grid);
				if (newPerson.row < 0 || Person[newPerson.row][newPerson.col] >= 1) //允许经过本点
				{																	//同一路径不能经历1次以上,否则该点不能到达
					ChangeMovementDir(routin.back());
					continue;
				}
				if (newPerson.row == T.row && newPerson.col == T.col)
					finished = true;

				//移动
				Person[newPerson.row][newPerson.col]++;
				PROSTEP newStep = initPersonStep(T, newPerson, grid);
				for (auto &node : routin) //更新各节点距离目标的累计步数
					node.realSteps++;
				routin.push_back(newStep);
			}
			if (finished)
			{
				ret = ret == -1 ? routin.front().realSteps : min(ret, routin.front().realSteps);
				break;
			}
			else
			{
				//当前节点无法继续前进需关闭，并重标箱子位置
				routin.pop_back();
				if (!routin.empty())
				{
					for (auto &node : routin) //更新各节点距离目标的累计步数
						node.realSteps--;	  //统计无效步骤
				}
			}
		}
		return ret;
	}
	/**
	 * @brief 测算从org开始到T各个方向的权重,越小越好
	 *
	 * @param T
	 * @param org
	 * @param dirc
	 * @return int
	 * -1 移动超界；
	 * 0 移动后即与目标点重合；
	 * 1 接近目标方向 无阻碍；
	 * 2 接近目标方向 前方路径两侧有遮挡；
	 * 3 接近目标方向 第二段路径阻碍；
	 * 4 接近目标方向 双路径阻碍;
	 * 5 远离路径方向；
	 * 6 背离路径方向
	 */
	int weightofdir(Position T, Position org, DIR dirc, vector<vector<char>> &grid)
	{
		Position next = baseMove(org, dirc);
		int rows = grid.size();
		int cols = grid[0].size();
		if (T.row < 0 || T.col < 0 || T.row >= rows || T.col >= cols ||
			org.row < 0 || org.col < 0 || org.row >= rows || org.col >= cols ||
			next.row < 0 || next.col < 0 || next.row >= rows || next.col >= cols)
			return -1; //任意一点在边界外都是不可用点
		long disorg = (T.row - org.row) * (T.row - org.row) + (T.col - org.col) * (T.col - org.col);
		long disnext = (T.row - next.row) * (T.row - next.row) + (T.col - next.col) * (T.col - next.col);
		if (disnext == 0)
			return 0;
		else if (disnext < disorg)
		{
			int maxpos, minpos;
			bool one = false, two = false;
			Position beside[2] = {next};
			if (dirc == R || dirc == L)
			{ //垂直障碍物带来绕路
				maxpos = max(org.row, T.row);
				minpos = min(org.row, T.row);
				for (int i = minpos + 1; i < maxpos; ++i)
					if (grid[i][T.col] == '#')
					{
						two = true;
						break;
					}
				beside[0].row--;
				beside[1].row++;
			}
			else if (dirc == U || dirc == D)
			{ //水平
				maxpos = max(org.col, T.col);
				minpos = min(org.col, T.col);
				for (int i = minpos + 1; i < maxpos; ++i)
					if (grid[T.row][i] == '#')
					{
						two = true;
						break;
					}
				beside[0].col--;
				beside[1].col++;
			}
			if (beside[0].row < 0 || beside[0].row >= grid.size() ||
				beside[0].col < 0 || beside[0].col >= grid[0].size() ||
				beside[1].row < 0 || beside[1].row >= grid.size() ||
				beside[1].col < 0 || beside[1].col >= grid[0].size() ||
				grid[beside[0].row][beside[0].col] == '#' ||
				grid[beside[0].row][beside[0].col] == 'B' ||
				grid[beside[1].row][beside[1].col] == '#' ||
				grid[beside[1].row][beside[1].col] == 'B')
				one = true;
			return (1 + (two ? 2 : 0) + (one ? 1 : 0));
		}
		else
		{
			if (T.row == next.row || T.col == next.col)
				return 6;
			return 5;
		}
	}
	Position moveObject(Position obj, DIR dirc, vector<vector<char>> &grid)
	{
		Position next = baseMove(obj, dirc);
		// weightofdir保证超边界移动不放在考虑范围内
		if (grid[next.row][next.col] == '#' ||
			grid[next.row][next.col] == 'B')
			next.row = next.col = -1; //移动到阻碍位置，不可移动
		return next;
	}
	Position baseMove(Position obj, DIR dirc)
	{
		Position next = obj;
		switch (dirc)
		{
		case U:
			next.row--;
			break;
		case D:
			next.row++;
			break;
		case L:
			next.col--;
			break;
		case R:
			next.col++;
			break;
		}
		return next;
	}
	/**
	 * @brief 变更行进方向
	 *
	 * @param curStep
	 * @return DIR
	 */
	DIR ChangeMovementDir(PROSTEP &curStep)
	{
		int olddir = curStep.dirselect++;
		curStep.Secondary = curStep.Major;
		if (curStep.dirselect > 3)
			return N; //可选方向已经见底
		else
		{
			switch (curStep.dirc[curStep.dirselect].first)
			{
			case U:
				curStep.Secondary.row++;
				break;
			case D:
				curStep.Secondary.row--;
				break;
			case L:
				curStep.Secondary.col++;
				break;
			case R:
				curStep.Secondary.col--;
				break;
			}
			if (curStep.dirc[curStep.dirselect].second >= curStep.dirc[olddir].second)
				curStep.avoided = true; //没有能够选择第一最优路径，存在绕路可能
		}
		return curStep.dirc[curStep.dirselect].first;
	}
	PROSTEP initBoxStep(Position T, Position org, Position preBox, vector<vector<char>> &grid)
	{
		PROSTEP retStep = initPersonStep(T, org, grid);
		//调整掉头方向为最低优先级方向
		for (int i = retStep.dirselect; i < 4; ++i)
		{
			Position newPos = baseMove(retStep.Major, retStep.dirc[i].first);
			if (newPos.row == preBox.row && newPos.col == preBox.col)
			{
				pair<DIR, int> temp = retStep.dirc[i];
				for (int j = i + 1; j < 4; ++j)
					retStep.dirc[j - 1] = retStep.dirc[j];
				retStep.dirc[3] = temp;
				retStep.dirc[3].second = 500;
				break; //
			}
		}
		//判断高位是否存在原始绕路情况，即 两路径都是接近路径，且都不可直达
		if (retStep.dirselect < 3 &&
			retStep.dirc[retStep.dirselect].second == retStep.dirc[retStep.dirselect + 1].second &&
			retStep.dirc[retStep.dirselect].second > 1 &&
			retStep.dirc[retStep.dirselect].second < 5)
			retStep.avoided = true;
		//根据盒子及当前行进方向，确认人员目标站位
		retStep.Secondary = retStep.Major;
		if (retStep.dirselect <= 3)
			switch (retStep.dirc[retStep.dirselect].first)
			{
			case U:
				retStep.Secondary.row++;
				break;
			case D:
				retStep.Secondary.row--;
				break;
			case L:
				retStep.Secondary.col++;
				break;
			case R:
				retStep.Secondary.col--;
				break;
			}
		return retStep;
	}
	PROSTEP initPersonStep(Position T, Position org, vector<vector<char>> &grid)
	{
		PROSTEP retStep;
		retStep.Major = org;
		retStep.bestSteps = abs(T.col - org.col) + abs(T.row - org.row);
		retStep.realSteps = 0;
		retStep.avoided = false;
		int minus_1 = 0;
		for (int i = 0; i < 4; ++i)
		{
			retStep.dirc[i].first = (DIR)i;
			retStep.dirc[i].second = weightofdir(T, org, (DIR)i, grid);
			//累计-1出现的次数
			if (retStep.dirc[(int)i].second == -1)
				minus_1++;
		}
		//按权重排队 简单冒泡
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 3 - i; ++j)
			{
				if (retStep.dirc[j].second > retStep.dirc[j + 1].second)
				{
					pair<DIR, int> temp = retStep.dirc[j];
					retStep.dirc[j] = retStep.dirc[j + 1];
					retStep.dirc[j + 1] = temp;
				}
			}
		}
		retStep.dirselect = minus_1; //初方向选择指针指向第一个可到达方向
		return retStep;
	}
	void printGraph(vector<vector<char>> input)
	{
		int rows = 0;
		int cols = 0;
		for (auto &line : input)
		{
			if (rows == 0)
			{
				for (char c : line)
				{
					if (cols == 0)
						cout << "    ";
					printf(" %02d ", cols++);
				}
				cout << endl;
			}
			cols = 0;
			for (char c : line)
			{
				if (cols++ == 0)
					printf("%02d: ", rows);
				cout << "  " << c << " ";
			}
			cout << endl;
			rows++;
		}
	}
	void printRoutin(vector<vector<char>> grid, vector<PROSTEP> const rout)
	{
		//清除当前B点位置
		grid[rout.back().Secondary.row][rout.back().Secondary.col] = '.';
		grid[rout.back().Major.row][rout.back().Major.col] = 'T';
		printf("\n Starting ^^^^^^^^^^^^^^^^^^^^^\n");
		for (auto &node : rout)
		{
			printf("\n");
			grid[node.Secondary.row][node.Secondary.col] = 'P';
			grid[node.Major.row][node.Major.col] = 'B';
			printGraph(grid);
			grid[node.Major.row][node.Major.col] = '.';
			grid[node.Secondary.row][node.Secondary.col] = '.';
		}
		printf("\n Ended    ^^^^^^^^^^^^^^^^^^^^^\n");
	}
};