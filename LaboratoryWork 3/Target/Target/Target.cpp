#include <iostream>

using namespace std;

string test = "hello world"; // ця змінна буде зчитана іншим процесом(програмою), вона знаходиться в глобальній
// області пам'яті програми, тому є висока ймовірніть того, що її адреса не зміниться,
// навіть коли address space layout randomization is used

int main()
{
	cout << "Adress of test is: " << &test << endl; // виводимо адресу змінної
	cin.get();
	return 0;
}
