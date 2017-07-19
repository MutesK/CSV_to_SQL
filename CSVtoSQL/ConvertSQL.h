#pragma once


#include <Windows.h>
#include <vector>
#include <string>
using namespace std;


class ConvertSQL
{
private:
	struct Column
	{
		WCHAR ColumnName[100];
		int index;
		vector<wstring> ColumnValue;
	};

	enum
	{
		MAX_ROWLINE = 6000
	};

	
	void SplitStr(WCHAR *Str, int *pOutStrCount, WCHAR pOutArray[][400]);
	
public:
	ConvertSQL(WCHAR *pWriteFile, WCHAR *pDBName, int MaxColumn);
	~ConvertSQL();

	void FirstTransectionCode();
	void LastTransectionCode();

	void ReadFile(WCHAR *OpenFile, vector<wstring> &PrintColumn);
	void WriteSQLFile(WCHAR *DBTable);

private:
	FILE *SqlSave;

	WCHAR WriteFile[80];
	WCHAR DBName[80];
	WCHAR OpenStr[400];
	WCHAR ColumnArray[100][400];
	vector<Column> ColumnVector;
	
	int PrintCount;
};

