// ConnectMsSql.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, (SQLWCHAR*)(const wchar_t *)SQLState, NULL, (SQLWCHAR*)(const wchar_t *)message, 1024, NULL))
		cout << "SQL driver message : " << message << "\nSQL state :  " << SQLState << "." << endl;
}

int main()
{
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	SQLWCHAR SQLQuery[] = L"SELECT * FROM memberTbl";

	do {
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
			break;

		if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
			break;

		if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			break;

		SQLCHAR recConString[1024];

		switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLWCHAR*)L"DRIVER={SQL Server}; SERVER=localhost,1433; DATABASE=shopDB; UID=superrotc; PWD=1234;", SQL_NTS, (SQLWCHAR*)(const wchar_t *)recConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
			case SQL_SUCCESS:
				break;
			case SQL_SUCCESS_WITH_INFO:
				break;
			case SQL_NO_DATA_FOUND:
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				retCode = -1;
				break;
			case SQL_INVALID_HANDLE:
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				retCode = -1;
				break;
			case SQL_ERROR:
				showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
				retCode = -1;
				break;
			default:
				break;
		}

		if (retCode == -1)
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
			break;




		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, SQLQuery, SQL_NTS))
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		else
		{
			char memberID[256];
			char memberName[256];
			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
			{
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &memberID, sizeof(memberID), NULL);
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &memberName, sizeof(memberName), NULL);

				cout << memberID << " " << memberName << endl;


			}
		}


	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	getchar();

	return 0;
}

//연동완료