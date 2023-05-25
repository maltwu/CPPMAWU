#include <iostream>
//结构体
struct hello
{
	int first;
	char interp;
	int second;
};
class Chello
{
private:
	/* data */
	// int first;

public:
	int first;
	char interp;
	int second;

public:
	Chello(/* args */);
	~Chello();
	virtual int atest(void);
	virtual void virtualmethod(){};
};

Chello::Chello(/* args */)
{
	std::cout << "Init Chello" << std::endl;
	first = 10;
	interp = 'H';
	second = 97;
}

Chello::~Chello()
{
}
int Chello::atest()
{
	std::cout << "I am Chello::" << std::endl;
	return 0;
}

class A : public Chello
{
public:
	A();
	int atest(void);
};
A::A()
{
	std::cout << "Init A" << std::endl;
}
int A::atest()
{
	std::cout << "I am A::" << std::endl;
	// Chello::atest();
	return 0;
}

class B : public A
{
public:
	int atest(void);
};
int B::atest(void)
{
	std::cout << "I am B::" << std::endl;
}
int main(int argc, char *argv[])
{
	std::cout << "Size of Int " << sizeof(int) << "Bytes" << std::endl;
	std::cout << "Size of char " << sizeof(char) << "Bytes" << std::endl;
	std::cout << "Size of hello " << sizeof(hello) << "Bytes" << std::endl;
	hello testhel;
	testhel.first = 90;
	testhel.second = 50;
	testhel.interp = 'U';
	std::cout << testhel.interp << std::endl;
	char *ptest = (char *)&testhel;
	std::cout << (void *)ptest << std::endl;
	std::cout << *(char *)(ptest + 4) << std::endl;
	std::cout << "Size of Chello " << sizeof(Chello) << "Bytes" << std::endl;
	std::cout << "Size of A " << sizeof(A) << "Bytes" << std::endl;
	std::cout << "Size of B " << sizeof(B) << "Bytes" << std::endl;
	A ss;
	std::cout << "ss::";
	ss.atest();
	std::cout << "((Chello)ss)::";
	((Chello)ss).atest();
	std::cout << "((Chello*)&ss)::";
	((Chello *)&ss)->atest();
	B nn;
	std::cout << "nn::";
	nn.atest();
	std::cout << "((A*)nn)::";
	((A *)&nn)->atest();
	std::cout << "((Chello*)nn)::";
	((Chello *)&nn)->atest();
	std::cout << "Chello's first " << ss.first << std::endl;
	char *pss = (char *)&ss;
	int* p = (int*)(pss+8+12);
	std::cout << "Chello's first by reference " << *((int *)(pss + 8)) << std::endl;
}