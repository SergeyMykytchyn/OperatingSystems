#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <chrono>
#include <stdio.h> 
#include <time.h> 

using namespace std;

int n, m, k;
int** a;
int** b;
int** c;
ifstream fin("input.txt");
mutex cm;
mutex coutm;
thread* threads;

const int NUMBER_OF_INCREMENTS = 1000000;
int counter1 = 0;
int counter2 = 0;
mutex counter2_mutex;

void doThingsWithLocking()
{
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++)
	{
		counter2_mutex.lock();
		counter2++;
		counter2_mutex.unlock();
	}
}

void doThingsWithoutLocking()
{

	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++)
	{
		counter1++;
	}
}

void malocMatrix(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = new int[m];
	}
}

void loadMatrix(int** arr, int n, int m)
{
	malocMatrix(arr, n, m);
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			fin >> arr[i][j];
		}
	}
}

void printMatrix(int** arr, int n, int m)
{
	cout << "\n";
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			cout << arr[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void generateRandomMatrix(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			arr[i][j] = (rand() % 10);
	}
}

void fill_INT_MAX(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			arr[i][j] = INT_MAX;
		}
	}
}

int multiplyTwoVectors(int i, int j)
{
	int prod = 0;
	for (int t = 0; t < m; t++)
	{
		prod += a[i][t] * b[t][j];
	}
	return prod;
}

void printIntermediateResult(int** c, int n, int k)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			if (c[i][j] == INT_MAX) {
				cout << "NaN ";
			}
			else {
				cout << c[i][j] << " ";
			}
		}
		cout << "\n";
	}
}

void exec(int i, int j)
{
	this_thread::sleep_for(chrono::milliseconds(rand() % 100000));
	int res = multiplyTwoVectors(i, j);
	cm.lock();
	c[i][j] = res;
	cout << "------------------------------------------------\n";
	cout << "c[" << i << "," << j << "] = " << c[i][j] << "\n\n";
	printIntermediateResult(c, n, k);
	cm.unlock();
}

int main()
{
	srand(time(0));
	char answer;
	cout << "Do you want to generate matrices randomly(r) or load them from file(f)? (r/f): \n";
	cin >> answer;
	if (answer == 'f') {
		fin >> n >> m >> k;
		a = new int*[n];
		b = new int*[m];
		c = new int*[n];

		loadMatrix(a, n, m);
		loadMatrix(b, m, k);
	}
	else if (answer == 'r') {
		cout << "Enter dimensions of matrices: n m k \n";
		cin >> n >> m >> k;
		a = new int*[n];
		b = new int*[m];
		c = new int*[n];

		malocMatrix(a, n, m);
		malocMatrix(b, m, k);

		generateRandomMatrix(a, n, m);
		generateRandomMatrix(b, m, k);
	}
	else {
		cout << "Incorrect input, error \n";
		return 0;
	}
	
	cout << "Matrix a[" << n << " x " << m << "]";
	printMatrix(a, n, m);
	cout << "Matrix b[" << m << " x " << k << "]";
	printMatrix(b, m, k);

	malocMatrix(c, n, k);
	fill_INT_MAX(c, n, k);

	cout << "Computing elements of matrix c[" << n << " x " << k << "]\n";

	threads = new thread[n * k];


	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			threads[i * k + j] = thread(exec, i, j);
		}
	}

	for (int i = 0; i < n * k; i++) 
	{
		threads[i].join();
	}
	end = chrono::high_resolution_clock::now();
	int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();
	cout << "\nMatrix c computed during: " << elapsed_seconds << " milliseconds\n";

	// 22 вариант

	// без использования критической секциии
	start = chrono::high_resolution_clock::now();
	thread t1(doThingsWithLocking);
	thread t2(doThingsWithLocking);
	t1.join();
	t2.join();
	end = chrono::high_resolution_clock::now();
	int elapsed_seconds1 = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();
	cout << "The count: " << counter2 << " without critical section\n";
	cout << "Time passed: " << elapsed_seconds1 << " milliseconds\n";

	// с использованием критической секции
	start = chrono::high_resolution_clock::now();
	thread t3(doThingsWithoutLocking);
	thread t4(doThingsWithoutLocking);
	t3.join();
	t4.join();
	end = chrono::high_resolution_clock::now();
	int elapsed_seconds2 = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();
	cout << "The count: " << counter1 << " with critical section\n";
	cout << "Time passed: " << elapsed_seconds2 << " milliseconds\n";

	return 0;
}
