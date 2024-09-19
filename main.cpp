
#if 0
#include "DataProvider.h"
#include "StringUtilss.h"
#include "XSqlLoader.h"
#include <string.h>

IDataProvider *g_pDataProvider = nullptr;

int32_t main(int32_t argc, char *argv[])
{
    const char *szURL = "Data Source=127.0.0.1:3306;Password=Admin@1234;User ID=root;Initial Catalog=nmp_proxy";
    g_pDataProvider = new DataProvider();

    if (!g_pDataProvider->SetConnectionString(szURL))
    {
        return 0;
    }

    IDataConnector *pConnector = g_pDataProvider->GetConnector();
    if (!pConnector)
    {
        return 0;
    }

    const char *szSQL = "SELECT * FROM bs_key_record";
    if (!pConnector->ExecuteSQL(szSQL, strlen(szSQL)))
    {
        printf("execute sql failed: %s\n", pConnector->GetLastError());
        return 0;
    }

    // 获得结果集并打印
    IDataQuery *pQuery = pConnector->GetDataQuery();

    for (uint32_t nCols = 0; nCols < pQuery->GetCols(); ++nCols)
    {
        printf("    %s    ", pQuery->GetColName(nCols));
    }
    printf("\n");

    for (uint64_t nRow = 0; nRow < pQuery->GetRows(); ++nRow)
    {
        for (uint32_t nCols = 0; nCols < pQuery->GetCols(); ++nCols)
        {
            switch (pQuery->GetColType(nCols))
            {
            case DATA_FIELD_INT:
                printf("%d  ", StringUtils::StringAsInt(pQuery->GetData(nRow, nCols)));
                break;
            case DATA_FIELD_INT64:
                printf("%ld  ", StringUtils::StringAsInt64(pQuery->GetData(nRow, nCols)));
                break;
            case DATA_FIELD_DOUBLE:
                printf("%f  ", StringUtils::StringAsDouble(pQuery->GetData(nRow, nCols)));
                break;
            case DATA_FIELD_STRING:
                printf("%s  ", (const char *)pQuery->GetData(nRow, nCols));
                break;
            case DATA_FIELD_DATETIME:
                printf("%s  ", (const char *)pQuery->GetData(nRow, nCols));
                break;
            default:
                printf("unknown type");
                break;
            }
        }
        printf("\n");
    }

    // 释放结果集
    pConnector->ReleaseQuery(pQuery);
    // 最后要释放连接
    g_pDataProvider->FreeConnector(pConnector);
    return 0;
}
#endif

#if 1
#include "DataProvider.h"
#include "XSqlLoader.h"
#include <string.h>

IDataProvider *g_pDataProvider = nullptr;

int32_t main(int32_t argc, char *argv[])
{
    const char *szURL = "Data Source=127.0.0.1:3306;Password=Admin@1234;User ID=root;Initial Catalog=bbs_proxy";
    g_pDataProvider = new DataProvider();

    if (!g_pDataProvider->SetConnectionString(szURL))
    {
        return 0;
    }

    XSqlLoader xsql;
    if (!xsql.Load("../MySQL.xml"))
    {
        return false;
    }

    const char *deviceID = "abcdefg";
    const char *hash = "aaabbbccc";
    int nArea = 1;
    int nType = 1;
    int nFileIndex = 100;
    int nLength = 134217728;
    int nStatus = 0;

    CVarList where, args, result;

    // 增
    args << deviceID << nArea << nType << nFileIndex << nLength << nStatus << hash;
    xsql.Query("addKey", where, args);

    // 删
    where.Clear();
    args.Clear();
    where << deviceID;
    // xsql.Query("deleteByID", where, args);

    // 改
    where.Clear();
    args.Clear();
    where << deviceID;
    args << "AAABBBCCC";
    xsql.Query("updateByDeviceID", where, args);

    // 查
    where.Clear();
    args.Clear();
    where << deviceID;
    xsql.Query("selectByDeviceID", where, args, result);

    printf("Id=%ld, DeviceID=%s, Hash=%s, Type=%d, Status=%d\n",
           result.Int64Val(0), result.StrintVal(1), result.StrintVal(2), result.IntVal(3), result.IntVal(4));

    return 0;
}

#endif