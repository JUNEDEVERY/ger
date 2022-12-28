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
	// 2 ����� � �������
	HINSTANCE FileHandle = CreateFile(PATH, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LPWSTR ReadString = calloc(100500, sizeof(WCHAR));
	ReadFile(FileHandle, ReadString, 100500 * sizeof(WCHAR), &d, NULL);
	CloseHandle(FileHandle); // ��������� ����������
	ReadString[d / 2] = '\n';
	ReadString[d / 2 + 1] = '\0'; // ��� �� �� �������� '\n' �� '\0' �� � 60\2 ������ +1
	return ReadString;
}

DWORD LpwstrToDword(LPWSTR str) // ������� ���������� ��������
{
	DWORD dw = 0;
	// ����� 1 � ������� 49.
	// 7 - 55. 55 - 48 = 7.
	for (size_t i = 0; i < wcslen(str); i++)
	{
		dw += (str[i] - '0'); // 0 ��������� �� ������ �������
		dw *= 10; // ��������, ����� ���� ���������� �����������
	}
	return dw / 10; // �����, �.�. �� ����� ���������� �� ���������� ���������
}
DWORD CountUsers(LPWSTR str)
{
	DWORD count = 0;
	for (size_t i = 0; i < wcslen(str); i++)
	{
		if (str[i] == L'\n') count++; // ���������� ������ 
	}
	return count;
}
/// <summary>
/// ���������� ������� �������� �� ������.
/// </summary>
/// <param name="users"></param>
/// <param name="count"></param>
/// <returns></returns>
__declspec(dllexport) float AvgVoztUserArray(U* users, LPDWORD count)
{
	float avg = 0;
	for (DWORD i = 1; i < *count; i++)
	{
		avg += users[i].vozr; // ���� �� ���� ������
	}
	avg = avg / (*count - 1); // ����� �� ���������� -1 �.� ������ � �������
	return avg;
}
/// <summary>
/// ��������� ����� ������ ������ �� ������������� � ������� ������� ���������� ������.
/// </summary>
/// <param name="users"></param>
/// <param name="count"></param>
/// <param name="str"></param>
/// <returns></returns>
__declspec(dllexport) U* SortUserArray(U* users, LPDWORD count, LPWSTR str)
{
	DWORD ind = 0, indUser = 1, con = 0, f = 0;
	DWORD countF = wcslen(str); // countf ���������� �������� � �������
	for (DWORD i = 0; i < *count; i++) // ����� ���������� ������� �� ������� ��� ������������ ����� ���������
	{
		for (DWORD j = 0; j < countF; j++)
		{
			if (users[i].f[j] == str[j]) // ���� �������  ��������� +1
				ind++;
			else ind--;
		}
		if (ind == countF) // � ��� ����� (������� � ������� ������ = �������, �� ������ != ������, �.� ������� ��� � 1 �������)
		{
			con++; // ��� �� ����������
		}
		ind = 0;
	}
	con++;
	U* UsersSort = calloc(con, sizeof(U)); // ��������� ����� ������ �������
	UsersSort[0].f[0] = (WCHAR)65279; // � ����� ������ ������ ����� �������� ����� �������� ���������
	// 65279 ��� ��� ������������ ������
	for (DWORD i = 0; i < *count; i++) //����� ����� � ����������� ��
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
	//i � 1 ��� ���� ��� �� ���������� ������ 65279 ������� � ������ �����
	// 65279 ������ ��� ������. �� �� ����� ������� ���������� ����� � ����� ���������� ������
	for (size_t i = 1; i < wcslen(str); i++) // wcslen ���� ����� ��� get lenght
	{
		if (str[i] == '\n')
		{
			strvozr = calloc(3, sizeof(TCHAR));
			zap++;
			poz = 0;
			ind = 0;
		}
		else // ������� ���� � else
		{
			if (str[i] == ';') // ���� �����������, �������� �������
			{
				poz++;
				ind = 0;
			}
			else // ������ ������� ������������ � ���, � ����� ��� = 3 - �������
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
						strvozr[ind] = '\0'; // ��������� ������
						users[zap].vozr = LpwstrToDword(strvozr); // ��������� �������, �.� lpwrstr �������� � \0
						free(strvozr);
					}
					else strvozr[ind] = str[i]; // ���� �� ����� ������������ ��� tchar
				}
				ind++; // ����� ���� �� ����� ����� ������������ 
			}
		}
	}
	*n = count; // ����� ������� count � U* users = UserList(PATH, &count);
	return users;
}