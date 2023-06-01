#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
	int N;
	cin >> N;
	int carriage[N];
	for (int i = 0; i < N; ++i)
	{
		cin >> carriage[i];
	}
	auto swap = [](int &a, int &b) -> bool {
		if(a>b){
			int temp = b;
			b = a;
			a = temp;
			return true;
		}
		return false;
	};
	int swaptimes = 0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N - 1 - i; ++j)
		{
			if(swap(carriage[j],carriage[j+1]))
				swaptimes++;
		}
	}
	printf("%d\n",swaptimes);
}
