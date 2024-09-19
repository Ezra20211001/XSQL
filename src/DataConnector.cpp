//--------------------------------------------------------------------
// 文件名:		DataConnector.cpp
// 内  容:
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#include "DataConnector.h"

DataConnector::DataConnector() : m_pMySQL(nullptr)
{
}

DataConnector::~DataConnector()
{
}

bool DataConnector::Ping()
{
    if (m_pMySQL)
    {
        return 0 == mysql_ping(m_pMySQL);
    }

    return false;
}

const char *DataConnector::GetLastError()
{
    return mysql_error(m_pMySQL);
}

bool DataConnector::ExecuteSQL(const char *sql, uint32_t len)
{
    return 0 == mysql_real_query(m_pMySQL, sql, len);
}

IDataQuery *DataConnector::GetDataQuery()
{
    m_DataQuery.StoreResult(m_pMySQL);
    return (IDataQuery *)&m_DataQuery;
}

void DataConnector::ReleaseQuery(IDataQuery *pQuery)
{
    m_DataQuery.FreeResult();
}

bool DataConnector::Open(const char *addr, int port, const char *db, const char *user, const char *pwd)
{
    Close();

    m_pMySQL = mysql_init(nullptr);
    if (!m_pMySQL)
    {
        return false;
    }

    // 设置如果连接失败，不自动连接
    char value = 1;
    mysql_options(m_pMySQL, MYSQL_OPT_RECONNECT, &value);

    // 设置连接等待时间，这里设置的是5秒
    value = 5;
    mysql_options(m_pMySQL, MYSQL_OPT_CONNECT_TIMEOUT, &value);

    // 设置字符集
    mysql_options(m_pMySQL, MYSQL_SET_CHARSET_NAME, "utf8");

    // utf8mb4
    // mysql_set_character_set(m_pMySQL, "utf8");

    if (!mysql_real_connect(m_pMySQL, addr, user, pwd, db, port, NULL, 0))
    {
        return false;
    }

    return true;
}

bool DataConnector::Close()
{
    if (m_pMySQL)
    {
        mysql_close(m_pMySQL);
        m_pMySQL = nullptr;
    }

    return true;
}
