#include <iostream>
#include <time.h>
#include <classAobjBFS.cc>
using namespace std;

int init_ = []() -> int
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	return 0;
}();

int main(int argc, char *argv[])
{
	freopen("output/cpp.in", "r", stdin);
	string s;
	vector<vector<char>> input;
	vector<char> content;
	cin >> s;
	
	for (char c : s)
	{
		if (c == ']' && !content.empty())
		{
			input.push_back(content);
			content.clear();
		}
		if (c == '#' || c == '.' || c == 'B' || c == 'T' || c == 'S')
		{
			content.push_back(c);
		}
	}

	Solution solved;
	solved.printGraph(input);
	time_t nowtime;
	time(&nowtime);
	cout << solved.minPushBox(input) << endl;
	time_t endtime;
	time(&endtime);
	cout << difftime(endtime, nowtime) << "secs" << endl;
	return 0;
}
