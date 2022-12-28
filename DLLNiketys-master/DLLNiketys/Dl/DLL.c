#include "../header.h"

BOOL WINAPI DllMain(HINSTANCE hlnstDll, DWORD dwReason, LPVOID IpReserved)
{
	BOOL bAllWentWell = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	if (bAllWentWell)
		return TRUE;
	else
		return FALSE;
}

LPWSTR Reading(LPWSTR path)
{
	
	DWORD d = 0; // weight
	// 2 байта в юникоде
	HINSTANCE FileHandle = CreateFile(PATH, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LPWSTR ReadString = calloc(100500, sizeof(WCHAR));
	ReadFile(FileHandle, ReadString, 100500 * sizeof(WCHAR), &d, NULL);
	CloseHandle(FileHandle); // закрываем дескриптор
	ReadString[d / 2] = '\n';
	ReadString[d / 2 + 1] = '\0'; // что бы не заменить '\n' на '\0' мы к 60\2 делаем +1
	return ReadString;
}

DWORD LpwstrToDword(LPWSTR str) // функция нахождения возраста
{
	DWORD dw = 0;
	// число 1 в юникоде 49.
	// 7 - 55. 55 - 48 = 7.
	for (size_t i = 0; i < wcslen(str); i++)
	{
		dw += (str[i] - '0'); // 0 вычитание из юникод таблицы
		dw *= 10; // умножаем, чтобы была нормальная разрядность
	}
	return dw / 10; // делим, т.к. не можем избавиться от последнего умножения
}
DWORD CountUsers(LPWSTR str)
{
	DWORD count = 0;
	for (size_t i = 0; i < wcslen(str); i++)
	{
		if (str[i] == L'\n') count++; // количество юзеров 
	}
	return count;
}
/// <summary>
/// Возвращает среднее значение из списка.
/// </summary>
/// <param name="users"></param>
/// <param name="count"></param>
/// <returns></returns>
__declspec(dllexport) float AvgVoztUserArray(U* users, LPDWORD count)
{
	float avg = 0;
	for (DWORD i = 1; i < *count; i++)
	{
		avg += users[i].vozr; // идем по всем юзерам
	}
	avg = avg / (*count - 1); // делим на количество -1 т.к начали с первого
	return avg;
}
/// <summary>
/// Формирует новый список только из пользователей в фамилии которых содержится строка.
/// </summary>
/// <param name="users"></param>
/// <param name="count"></param>
/// <param name="str"></param>
/// <returns></returns>
__declspec(dllexport) U* SortUserArray(U* users, LPDWORD count, LPWSTR str)
{
	DWORD ind = 0, indUser = 1, con = 0, f = 0;
	DWORD countF = wcslen(str); // countf количество символов в фамилии
	for (DWORD i = 0; i < *count; i++) // Поиск количества фамилий по условию для формирования новой структуры
	{
		for (DWORD j = 0; j < countF; j++)
		{
			if (users[i].f[j] == str[j]) // если символы  совпадают +1
				ind++;
			else ind--;
		}
		if (ind == countF) // и это число (символы в порядке ИВАНОВ = ИВАНОВА, НО ИВАНОВ != ПЕТРОВ, Т.К ОТЛИЧИЯ УЖЕ В 1 СИМВОЛЕ)
		{
			con++; // кол во совпадений
		}
		ind = 0;
	}
	con++;
	U* UsersSort = calloc(con, sizeof(U)); // указатель после поиска фамилий
	UsersSort[0].f[0] = (WCHAR)65279; // в самое начало первой буквы вставили чтобы получить кодировку
	// 65279 бом или таинственный символ
	for (DWORD i = 0; i < *count; i++) //Поиск строк и записывание их
	{
		for (DWORD j = 0; j < countF; j++)
		{
			if (users[i].f[j] == str[j]) 
				ind++;
		}
		if (ind == countF)
		{
			for (DWORD j = 0; j < wcslen(users[i].o); j++)
			{
				UsersSort[indUser].f[j] = users[i].f[j];
			}
			for (DWORD j = 0; j < wcslen(users[i].i); j++)
			{
				UsersSort[indUser].i[j] = users[i].i[j];
			}
			for (DWORD j = 0; j < wcslen(users[i].o); j++)
			{
				UsersSort[indUser].o[j] = users[i].o[j];
			}
			UsersSort[indUser].vozr = users[i].vozr;
			indUser++;
		}
		ind = 0;
	}
	*count = con;
	return UsersSort;
}

__declspec(dllexport) U* CreateUserArray(LPWSTR path, LPDWORD n)
{
	LPWSTR str = Reading(path);
	DWORD count = CountUsers(str);
	U* users = calloc(count, sizeof(U));
	DWORD poz = 0, zap = 0, ind = 0;
	LPWSTR strvozr = calloc(3, sizeof(TCHAR));
	//i с 1 для того что бы пропустить символ 65279 стоящий в начале файла
	// 65279 пробел без ширины. из за этого символа сдвигались байты и хавал предыдущий символ
	for (size_t i = 1; i < wcslen(str); i++) // wcslen тоже самое как get lenght
	{
		if (str[i] == '\n')
		{
			strvozr = calloc(3, sizeof(TCHAR));
			zap++;
			poz = 0;
			ind = 0;
		}
		else // сначала идем в else
		{
			if (str[i] == ';') // ищем разделитель, сдвигаем позицию
			{
				poz++;
				ind = 0;
			}
			else // каждую позицию приравниваем к ФИО, в конце поз = 3 - возраст
				// 
			{
				if (poz == 0)
					users[zap].f[ind] = str[i];
				if (poz == 1)
					users[zap].i[ind] = str[i];
				if (poz == 2)
					users[zap].o[ind] = str[i];
				if (poz == 3)
				{
					if (str[i] == '\r')
					{
						strvozr[ind] = '\0'; // завершаем строку
						users[zap].vozr = LpwstrToDword(strvozr); // конвертим возраст, т.к lpwrstr работает с \0
						free(strvozr);
					}
					else strvozr[ind] = str[i]; // если не конец переписываем как tchar
				}
				ind++; // чтобы друг на друга слова наслаивались 
			}
		}
	}
	*n = count; // чтобы вернуть count в U* users = UserList(PATH, &count);
	return users;
}