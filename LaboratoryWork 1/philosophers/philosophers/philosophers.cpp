#include "pch.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

#define LEFT (i+4)%5 // філософ зліва
#define RIGHT (i+1)%5 // філософ зправа

const int P = 5; // кількість філософів

// кожний філософ знаходиться в одному з трьох станів:
const int HUNGRY = 0;
const int EATING = 1;
const int THINKING = 2;

thread threads[P]; // масив потоків, вони ж по суті філософи 

int state[P]; // масив станів філософів
int s[P] = { 0,0,0,0,0 }; // масив семафор, використовуваються для очікування звільнення ресурсів(виделок)

// Мьютекси, які використовуваються для організвації
// ексклюзивного доступу до критичних ресурсів: виделки та консоль
mutex forkm; 
mutex coutm;

void think(int id) {
	coutm.lock();
	//cout << "\n Философ " << id << " начинает думать \n";
	coutm.unlock();

	this_thread::sleep_for(chrono::milliseconds(rand() % 1000));

	coutm.lock();
	//cout << "\n Философ " << id << " заканчивает думать \n";
	coutm.unlock();
}

void eat(int id) {
	coutm.lock();
	cout << "\n Философ " << id << " кушает \n";
	coutm.unlock();

	this_thread::sleep_for(chrono::milliseconds(rand() % 1000));

	coutm.lock();
	//cout << "\n Философ " << id << " заканчивает кушать \n";
	coutm.unlock();
}

void up(int i) {
	s[i]++;
}

void down(int i) {
	if (s[i] > 0) {
		s[i]--;
		return;
	}

	// Чекаємо поки сусіди зліва та зправа не доїдять 
	while (s[i] <= 0) {
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	
	s[i]--;
}

void test(int i) {
	// Якщо філософ Голодний та сусіди зправа та зліва не їдять
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		state[i] = EATING;	// Змінюємо стан філософа на Їсть
		up(i);				// Дозволяємо перейти до їжі
	}
}

void put_forks(int i) {
	forkm.lock();		// Захоплюємо доступ до критичних даних
	state[i] = THINKING;// Змінюємо стан філософа на Думає
	test(LEFT);			// Намагаємося нагодувати філософа зліва
	test(RIGHT);		// Намагаємося нагодувати філософа зправа
	forkm.unlock();		// Звільняємо доступ до критичних даних
}

void take_forks(int i) {
	forkm.lock();		// Захоплюємо доступ до критичних даних
	state[i] = HUNGRY;	// Змінюємо стан філософа на Голодний
	test(i);			// Намагаємося взяти дві вилки
	forkm.unlock();		// Звільняємо доступ до критичних даних
	down(i);			// Блокуємося, якщо не вдалося взяти виделки
}

void philosopher(int i) {
	while (true) {
		think(i);		// Думає
		take_forks(i);	// Бере дві виделки або блокується
		eat(i);			// Їсть
		put_forks(i);	// Кладе на стіл дві виделки
	}
}

int main()
{
	setlocale(LC_ALL, "ru");

	// Створюємо та запускаемо потоки на виконання
	for (int i = 0; i < P; i++) {
		threads[i] = thread(philosopher, i);
	}

	// Очікуємо завершення роботи всіх потоків
	for (int i = 1; i <= P; i++) {
		threads[i - 1].join();
	}

	return 0;
}

