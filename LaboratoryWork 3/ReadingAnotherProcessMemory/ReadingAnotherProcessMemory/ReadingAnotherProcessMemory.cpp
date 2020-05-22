#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

int main()
{
	// адреса комірки пам'яті іншого процесу
	LPCVOID Adress = (LPCVOID)0x00EF5018;

	// створюємо буфер для зчитування
	char buffer[64];

	// Отримуємо id процесора за його назвою
	DWORD ProcessId = FindProcessId(L"Target.exe");

	/*
		Перший парамерт вказує на право доступу до процеса, в нашому випадку, для читання пам'яті нам потрібен PROCESS_VM_READ.
		Другий булевий параметр вказує чи потрібно успадковувати управління, нам це не потріно ми ставимо 0.
		Третій параметр вказує на id процесу.
		Отримуємо екземпляр об'єкту для управління процесом.
	*/
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ, 0, ProcessId);

	// зчитуємо значення з комірки пам'яті
	ReadProcessMemory(hProcess, Adress, &buffer, sizeof(buffer), 0);

	// виводимо це значення
	cout << "Data read from remote process: " << buffer << endl;

	cin.get();
	return 0;
}


