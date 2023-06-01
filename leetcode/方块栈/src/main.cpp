#include <iostream>
#include <queue>
using namespace std;
int main(int argc, char *argv[])
{
	//方块列
	int blocks[30001][2] = {{0}};
	for (int i = 1; i < 30001; ++i)
	{
		blocks[i][0] = i;
		blocks[i][1] = i;
	}

	int n, X, Y;
	char ins;
	queue<pair<char, pair<int, int>>> instruction; //指令集
	cin >> n;
	//读取指令集
	while (n-- > 0)
	{
		cin >> ins;
		if (ins == 'M')
		{
			cin >> X >> Y;
			instruction.push(pair<char, pair<int, int>>{ins, pair<int, int>{X, Y}});
		}
		else
		{
			cin >> X;
			instruction.push(pair<char, pair<int, int>>{ins, pair<int, int>{X, 0}});
		}
	}
	auto findTop = [&blocks](int x) -> int
	{
		while (x != blocks[x][0])
			x = blocks[x][0];
		return x;
	};
	auto findBottom = [&blocks](int x) -> int
	{
		while (x != blocks[x][1])
			x = blocks[x][1];
		return x;
	};
	auto MoveTo = [&blocks](int wait, int to) -> void
	{
		// top连接
		blocks[to][0] = wait;
		// bottom连接
		blocks[wait][1] = to;
		return;
	};
	auto getBlocksFrom = [&blocks](int x) -> int
	{
		int ret = 0;
		while (x != blocks[x][1])
		{
			x = blocks[x][1];
			++ret;
		}
		return ret;
	};
	while (!instruction.empty())
	{
		pair<char, pair<int, int>> instruct = instruction.front();
		instruction.pop();
		if (instruct.first == 'M')
		{
			int wait = findBottom(instruct.second.first);
			int to = findTop(instruct.second.second);
			MoveTo(wait, to);
		}
		else
			cout << getBlocksFrom(instruct.second.first) << endl;
	}
	// printf("0");
	return 0;
}