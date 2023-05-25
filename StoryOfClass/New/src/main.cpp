#include <iostream>
struct s_person{
	char name;
	int  age;
	int  height;
};
class c_person{
	private:
		char name;
		int age;  
		int height;
	public:
		c_person();
		c_person(char name,int age,int height);
		~c_person();
		char getName(){
			return this->name;
		}
};
c_person::c_person(){
	name ='W';
	age =10;
	height = 140;
}
c_person::c_person(char name,int age,int height){
	this->name=name;
	this->age=age;
	this->height=height;
}
c_person::~c_person(){
	std::cout<<"I am destoried"<<std::endl;
}
int main(int argc, char *argv[])
{
	c_person spers('B',35,160);
	// std::cout << "Size of int = "<<sizeof(int) << std::endl;
	// std::cout << "Size of char = "<<sizeof(char) << std::endl;
	std::cout << "Size of c_person = "<<sizeof(c_person) << std::endl;
	void* ppers = &spers;
	// void* pppers = &spers.name;
	std::cout<<*((int*)(ppers)+1)<<std::endl;  //spers.age	
	// std::cout<<spers.age<<std::endl;
	std::cout<<spers.getName()<<std::endl;

}