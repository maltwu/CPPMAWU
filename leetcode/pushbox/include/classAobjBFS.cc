#include <iostream>
#include <queue>
#include <vector>
using namespace std;
struct Position
{
	int row;
	int col;
	bool operator==(const Position a)
	{
		return a.row == row && col == a.col;
	}
};
struct Step
{
	Position box;
	Position person;
	int Steps;
	pair<Position, Position> parent;
};
class Solution
{
public:
	int minPushBox(vector<vector<char>> &grid)
	{
		// BFS 广度优先 本处以步长为深度，枚举同级步长的所有可能，依次累计考察不同深度，若某个深度的某个组合与目标重合，则找到最短路径（此方案很难回溯走法)
		int rows = grid.size();
		int cols = grid[0].size();
		vector<Step> graph;
		vector<vector<bool>> history(rows * cols, vector<bool>(rows * cols, false)); //定义经过点特征数组 row对应箱子位置，col 对应人的位置
		queue<Step> startstep, poststep;											 //登记有效移动步，前一步和后一步队列
																					 //确定各要素的初始位置
		Position T, B, S;
		for (int i = 0, valids = 0; i < rows && valids < 3; i++)
		{
			for (int j = 0; j < cols && valids < 3; j++)
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
			}
		}
		auto isOk = [&grid, rows, cols](int row, int col) -> bool
		{
			return (row < rows && row >= 0 && col < cols && col >= 0 && grid[row][col] != '#');
		};
		auto calprop = [cols](int row, int col) -> int { //特征值概念，取两值中一个，将其中一值乘以另一个值的最大值，即保证当前值非0的情况下，其组合均大于另一个值，将组合和另一个值相加，其值同两个值对的映射关系唯一
			return row * cols + col;
		};
		//初始化工作队列
		startstep.push(Step{B, S, 0});
		//标记当前状态
		history[calprop(B.row, B.col)][calprop(S.row, S.col)] = true;
		while (!startstep.empty())
		{
			Position orgPerson = startstep.front().person;
			Position orgBox = startstep.front().box;
			Position newBox, newPerson;
			int step = startstep.front().Steps; //启动点移动步数
			startstep.pop();					//取出出发序列首元素
			//将人向四个方向移动
			for (int i = 0; i < 4; ++i)
			{
				newBox = orgBox;
				newPerson = orgPerson;
				switch (i)
				{
				case 0:
					newBox.col--;
					newPerson.col--;
					break;
				case 1:
					newBox.col++;
					newPerson.col++;
					break;
				case 2:
					newBox.row--;
					newPerson.row--;
					break;
				case 3:
					newBox.row++;
					newPerson.row++;
				}

				//判断人员是否合法？
				if (isOk(newPerson.row, newPerson.col))
				{
					int X, Y;
					//人员与箱子是否重合
					if (newPerson == orgBox)
					{ //推动箱子
						X = calprop(newBox.row, newBox.col);
						Y = calprop(newPerson.row, newPerson.col);
						if (newBox == T)
						{
							printRoute(grid, graph,S);
							return step + 1;
						}
						if (isOk(newBox.row, newBox.col) &&
							!history[X][Y])
						{ //箱子就位合法，且状态没有出现过
							history[X][Y] = true;
							poststep.push({newBox, newPerson, step + 1});
							graph.push_back({newBox, newPerson, step + 1, pair<Position, Position>{orgBox, orgPerson}});
						}
					}
					else
					{ //箱子未动，只人位置变动
						X = calprop(orgBox.row, orgBox.col);
						Y = calprop(newPerson.row, newPerson.col);
						if (!history[X][Y])
						{ //首次到达 登记达到标记
							history[X][Y] = true;
							startstep.push(Step{orgBox, newPerson, step});
							graph.push_back({orgBox, newPerson, step, pair<Position, Position>{orgBox, orgPerson}});
						}
					}
				}
			}
			if (startstep.empty())
				startstep.swap(poststep); //更换到下一层状态
		}
		return -1;
	}

	void printGraph(vector<vector<char>> &input)
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
	void printRoute(vector<vector<char>> &grid, vector<Step> &graph, Position S)
	{
		vector<Step> compactgraph;
		pair<Position, Position> parent = pair<Position, Position>{graph.back().box, graph.back().person};
		for (vector<Step>::reverse_iterator it = graph.rbegin(); it != graph.rend(); ++it)
		{
			if (it->box == parent.first && it->person == parent.second)
			{
				compactgraph.push_back((*it));
				parent = it->parent;
			}
		}
		grid[S.row][S.col] = 's';
		int steps = compactgraph.back().Steps-1;
		for (vector<Step>::reverse_iterator it = compactgraph.rbegin(); it != compactgraph.rend(); ++it)
		{
			grid[it->box.row][it->box.col] = 'B';
			grid[it->person.row][it->person.col] = 'S';
			if (it->Steps > steps)
			{
				printGraph(grid);
				steps = it->Steps;
			}
			grid[it->box.row][it->box.col] = '.';
			grid[it->person.row][it->person.col] = '.';
		}
	}
};