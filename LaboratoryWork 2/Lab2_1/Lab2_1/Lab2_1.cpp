#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

using namespace std;

vector<int> v;
mutex mv;
mutex mcout;
mutex mblocked;
bool fBlocked = false;
bool flag = true;
bool flagF = true;
bool flagG = true;

struct my_struct
{
	bool uninitialized = true;
	bool value;
};

void push(int x)
{
	mv.lock();
	v.push_back(x);
	mv.unlock();
}

bool pop(int& x)
{
	mv.lock();
	if (v.size() == 0)
	{
		mv.unlock();
		return false;
	}
	x = v.at(0);
	mv.unlock();
	return true;
}

void block()
{
	mblocked.lock();
	fBlocked = true;
	mblocked.unlock();
}

bool fIsBlocked()
{
	mblocked.lock();
	if (fBlocked)
	{
		mblocked.unlock();
		return true;
	}
	mblocked.unlock();
	return false;
}

int f(int x) 
{
	this_thread::sleep_for(chrono::milliseconds(3000));
	if (x == 2)
	{
		mcout.lock();
		cout << "Thread f is blocked\n";
		mcout.unlock();
		block();
		while (true); // blocked
	}
	push(x);
	return x;
}

int g(int y) 
{
	//this_thread::sleep_for(chrono::milliseconds(1000));
	int x;
	if (y == 4)
		return 4;
	if (y == 3)
	{
		mcout.lock();
		cout << "Thread g is blocked\n";
		mcout.unlock();
		while (true); // blocked
	}
	while (!pop(x)) // blocking
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		if (fIsBlocked() && flag)
		{
			flag = false;
			mcout.lock();
			cout << "Thread g is blocked\n";
			mcout.unlock();
		}
	}
	return y + x;
}

int main()
{
	int x;
	int y;

	cout << "Value for f: ";
	cin >> x;
	cout << "Value for g: ";
	cin >> y;

	my_struct result_f;
	my_struct result_g;

	thread thr_f([&result_f, &x]() {
		result_f.value = f(x);
		result_f.uninitialized = false;
	});
	thread thr_g([&result_g, &y]() {
		result_g.value = g(y);
		result_g.uninitialized = false;
	});

	thr_f.detach();
	thr_g.detach();

	this_thread::sleep_for(chrono::milliseconds(1000));
	while (true) 
	{
		if ((result_f.uninitialized & result_g.uninitialized) || (result_f.value == false && result_g.uninitialized) || (result_g.value == false && result_f.uninitialized)) 
		{
			if (!result_f.uninitialized && !result_f.value && flagF)
			{
				flagF = false;
				mcout.lock();
				cout << "Thread f returned false\n";
				mcout.unlock();
			}
			if (!result_g.uninitialized && !result_g.value && flagG)
			{
				flagG = false;
				mcout.lock();
				cout << "Thread g returned false\n";
				mcout.unlock();
			}
			mcout.lock();
			cout << "The program is not responding. Wait for 5 seconds? (y/n): \n";
			mcout.unlock();
			char answer;
			cin >> answer;
			if (answer == 'y') 
			{
				this_thread::sleep_for(chrono::milliseconds(5000));
			}
			else {
				break;
			}
		}
		else {
			
			bool result = false;
			if (!result_f.uninitialized && result_f.value == true)
				result = true;
			if (!result_g.uninitialized && result_g.value == true)
				result = true;

			mcout.lock();
			cout << "f(x) || g(x) is " << result << endl;
			mcout.unlock();
			break;
		}
	}

	return 0;
}
