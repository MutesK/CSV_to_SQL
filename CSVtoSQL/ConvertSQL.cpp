#include "ConvertSQL.h"
#include <iostream>


ConvertSQL::ConvertSQL(WCHAR *pWriteFile, WCHAR *pDBName, int MaxColumn)
{

	lstrcpyW(WriteFile, pWriteFile);
	lstrcpyW(DBName, pDBName);

	ColumnVector.reserve(MaxColumn);
	_wfopen_s(&SqlSave, WriteFile, L"w,ccs=UTF-8");
	rewind(SqlSave);

}


ConvertSQL::~ConvertSQL()
{
	fclose(SqlSave);
}

void ConvertSQL::FirstTransectionCode()
{
	fwprintf(SqlSave, L"SET AUTOCOMMIT = 0;\n");
	fwprintf(SqlSave, L"START TRANSACTION;\n\n");
}
void ConvertSQL::LastTransectionCode()
{
	fwprintf(SqlSave, L"COMMIT;\n");
	fwprintf(SqlSave, L"SET AUTOCOMMIT = 1;\n");
}

void ConvertSQL::SplitStr(WCHAR *Str, int *pOutStrCount, WCHAR pOutArray[][400])
{
	WCHAR *index = Str;
	WCHAR *pStart = Str;
	*pOutStrCount = 0;

	while (*index != L'\n')
	{
		if (*index == L',')
		{
			index++;
			lstrcpynW(pOutArray[*pOutStrCount], pStart, index - pStart);
			pOutArray[*pOutStrCount][index - pStart] = L'\0';
			(*pOutStrCount)++;
			pStart = index;
			continue;
		}
		else
			index++;
	}

	index++;
	lstrcpynW(pOutArray[*pOutStrCount], pStart, index - pStart);
	(*pOutStrCount)++;
}


void ConvertSQL::ReadFile(WCHAR *OpenFile, vector<wstring> &PrintColumn)
{
	FILE *CsvOpen;

	_wfopen_s(&CsvOpen, OpenFile, L"rt");
	ColumnVector.clear();
	ZeroMemory(ColumnArray, sizeof(WCHAR) * 100 * 400);
	ZeroMemory(OpenStr, sizeof(WCHAR) * 400);

	int ColumnCount = 0;


	fgetws(OpenStr, 400, CsvOpen);

	SplitStr(OpenStr, &ColumnCount, ColumnArray);

	

	// 필요없는 부분은 여기서 제치게 해야된다.
	for (int i = 0; i < ColumnCount; i++)
	{
		for (int P =0; P<PrintColumn.size(); P++)
		{
			if (lstrcmpW(ColumnArray[i], PrintColumn[P].c_str()) == 0)
			{
				Column Columns;
				Columns.index = i;

		
				lstrcpyW(Columns.ColumnName, ColumnArray[i]);
				ColumnVector.push_back(Columns);
				break;
			}
		}

	}
	PrintCount = ColumnVector.size();

	// 4.다음 행 한줄을 읽어서 컴마 단위로 분리.
	while (!feof(CsvOpen))
	{
		fgetws(OpenStr, 400, CsvOpen);
		SplitStr(OpenStr, &ColumnCount, ColumnArray);

		for (int i = 0; i < ColumnCount; i++)
		{
			for (int P = 0; P < PrintCount; P++)
			{
				if(ColumnVector[P].index == i)
				{
					wcout << ColumnArray[i] << " ";
					ColumnVector[P].ColumnValue.push_back(ColumnArray[i]);
					break;
				}
			}
		}
		wcout << endl;
	}

	fclose(CsvOpen);
}
void ConvertSQL::WriteSQLFile(WCHAR *DBTable)
{
	int Size = ColumnVector[0].ColumnValue.size();

	fwprintf(SqlSave, L"\nDELETE FROM `%s`.`%s`;\n\n", DBName, DBTable);

	for (int i = 0; i < Size; i++)
	{
		fwprintf(SqlSave, L"INSERT INTO `%s`.`%s` (", DBName, DBTable);
		// 칼럼
		int j = 0;
		for (; j < PrintCount - 1; j++)
			fwprintf(SqlSave, L"`%s`, ", ColumnVector[j].ColumnName);

		fwprintf(SqlSave, L"`%s`) VALUES (", ColumnVector[j].ColumnName);


		// Value
		j = 0;
		for (; j < PrintCount - 1; j++)
			fwprintf(SqlSave, L"'%s', ", ColumnVector[j].ColumnValue[i].c_str());

		fwprintf(SqlSave, L"'%s');\n", ColumnVector[j].ColumnValue[i].c_str());

	}
	fwprintf(SqlSave, L"\n\n");
}