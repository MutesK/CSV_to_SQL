#include "Parser.h"
#include "ConvertSQL.h"
#include <iostream>
using namespace std;


int main()
{
	CParser Parser(L"CSV_to_SQL.ini");

	// CONFIG �� ���´�.
	int MaxFile;
	int MaxColumn;
	WCHAR DBName[80];

	Parser.GetValue(L"MAX_FILE", &MaxFile, L"CONFIG");
	Parser.GetValue(L"MAX_COLUMN", &MaxColumn, L"CONFIG");
	Parser.GetValue(L"DB_NAME", DBName, L"CONFIG");

	ConvertSQL Sql(L"CSV_to_SQL.sql", DBName, MaxColumn);
	Sql.FirstTransectionCode();

	for (int FileIndex = 0; FileIndex < MaxFile; FileIndex++)
	{
		// File_%d
		WCHAR Part[80];
		WCHAR OpenFile[80];
		WCHAR DBTable[80];

		wsprintf(Part, L"FILE_%d", FileIndex + 1);

		// �ش� TXT_FILE�� DB_TABLE�� �����´�.
		if (!Parser.GetValue(L"TXT_FILE", OpenFile, Part))
			break;		
		// ��ã�Ҵٸ� ������ ����.
		if (!Parser.GetValue(L"DB_TABLE", DBTable, Part))
			break;

		// �̾Ƴ� Į���� �����´�.
		vector<wstring> Columns;

		for (int ColumnIndex = 0; ColumnIndex < MaxColumn; ColumnIndex++)
		{
			WCHAR Column[80];
			WCHAR ColumnName[80];
			wsprintf(Column, L"COLUMN_%d", ColumnIndex + 1);

			if (!Parser.GetValue(Column, ColumnName, Part))
				break;

			Columns.push_back(ColumnName);
		}

		Sql.ReadFile(OpenFile, Columns);
		Sql.WriteSQLFile(DBTable);
	}

	Sql.LastTransectionCode();
	return 0;
}

