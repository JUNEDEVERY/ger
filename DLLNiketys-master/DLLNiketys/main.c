#include "header.h"
#define PATHDLL L"Dl.dll"
#define PATHLIST L"Users.csv"
typedef U* (_cdecl* MyFunction)(LPWSTR, LPDWORD); // прототипы
typedef U* (_cdecl* MyFunction1)(U*, LPDWORD, LPWSTR);
typedef float(_cdecl* MyFunction2) (U*, LPDWORD);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	HINSTANCE MyDLL;
	if (!(MyDLL = LoadLibrary(PATHDLL))) {
		MessageBox(NULL, L"Произошла ошибка при открытии DLL библиотеки", L"Что-то не так", MB_OK);
	}
	else {
		// создаем переменную пользовательского типа
		// В скобках приводим к нужному типу
		MyFunction UserList = (MyFunction)GetProcAddress(MyDLL, "CreateUserArray"); // получаем адрес написанной функции
		MyFunction1 UserList1 = (MyFunction1)GetProcAddress(MyDLL, "SortUserArray"); // дескриптор и имя функцииz`
		MyFunction2 AvgUserList = (MyFunction1)GetProcAddress(MyDLL, "AvgVoztUserArray");
		DWORD count = 0;
		DWORD w;
		LPWSTR str = L"Иванов";
		LPWSTR str2 = L"Иванова";
		U* users = UserList(PATH, &count);
		DWORD count2 = count;
		HINSTANCE FileHandle1 = CreateFile(L"EndResult.csv", GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		U* uses = UserList1(users, &count2, str);
		TCHAR otvFile[512]; 
		WriteFile(FileHandle1, uses[0].f, wcslen(uses[0].f) * sizeof(WCHAR), &w, NULL);//Для записи символа начала строки 65279
		for (size_t i = 1; i < count2; i++)
		{
			uses[i].f[wcslen(uses[i].f)] = (WCHAR)';';
			WriteFile(FileHandle1, uses[i].f, wcslen(uses[i].f) * sizeof(WCHAR), &w, NULL);
			uses[i].i[wcslen(uses[i].i)] = (WCHAR)';';
			WriteFile(FileHandle1, uses[i].i, wcslen(uses[i].i) * sizeof(WCHAR), &w, NULL);
			uses[i].o[wcslen(uses[i].o)] = (WCHAR)';';
			WriteFile(FileHandle1, uses[i].o, wcslen(uses[i].o) * sizeof(WCHAR), &w, NULL);
			// \r начало строки \n конец
			swprintf(otvFile, 512, TEXT("%d\r\n"), uses[i].vozr); // просто записываем в переменную \р\н
			DWORD len = 0;
			while (otvFile[len] != '\0')
			{
				len++;
			}
			DWORD dz = 0;
			/// <summary>
			/// Запись в файл
			/// </summary>
			/// <param name="FileHandle1">Дескриптор открытого окна</param>
			/// <param name="otvFile">указываем буфер для записи</param>
			/// <param name="len * (sizeof(char) * 2)">указываем, сколько байт мы хотим записать</param>
			/// <param name="&dz">передаем указатель на DWORD</param>
			/// <param name="NULL">синхронный режим записи</param>
			/// <returns></returns>
			WriteFile(FileHandle1, 
				otvFile,
				len * (sizeof(char) * 2),
				&dz,
				NULL
			);
		}

		float avg = AvgUserList(uses, &count2);
		swprintf(otvFile, 512, TEXT("Средний возраст:%.10f\n\0"), avg);
		DWORD len = 0;
		while (otvFile[len] != '\0')
		{
			len++;
		}
		DWORD dz = 0;
		WriteFile(FileHandle1,
			otvFile,
			len * (sizeof(char) * 2),
			&dz,
			NULL
		);
		CloseHandle(FileHandle1);
		return 0;
	}
	
}