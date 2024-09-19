//--------------------------------------------------------------------
// 文件名:		DataProvider.cpp
// 内  容:		数据库连接池
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#include "DataProvider.h"
#include "StringUtils.h"
#include "Logger.h"

DataProvider::DataProvider()
{
}

DataProvider::~DataProvider()
{
}

bool DataProvider::SetConnectionString(const char *value)
{
    if (0 == value || '\0' == value[0])
    {
        return false;
    }

    // Data Source=127.0.0.1:3306;User ID=root;Password=123456;Initial Catalog=xxx
    std::vector<std::string> result;
    StringUtils::SplitString(value, ';', result);
    if (result.size() < 4)
    {
        LogError("[DataProvider::SetConnectionString] Prase connect string failed!");
        return false;
    }

    std::string strDataSource;
    std::string strUserID;
    std::string strPassword;
    std::string strCatalog;

    for (size_t i = 0; i < result.size(); ++i)
    {
        std::string tmp = result[i];
        std::vector<std::string> tmpresult;

        StringUtils::SplitString(tmp, '=', tmpresult);
        if (2 != tmpresult.size())
        {
            LogError("[DataProvider::SetConnectionString] Prase sub string failed!");
            return false;
        }

        if (tmpresult[0] == "Data Source")
        {
            strDataSource = tmpresult[1];
        }
        else if (tmpresult[0] == "User ID")
        {
            strUserID = tmpresult[1];
        }
        else if (tmpresult[0] == "Password")
        {
            strPassword = tmpresult[1];
        }
        else if (tmpresult[0] == "Initial Catalog")
        {
            strCatalog = tmpresult[1];
        }
    }

    if (strDataSource.empty() || strUserID.empty() ||
        strPassword.empty() || strCatalog.empty())
    {
        // 解析连接字符串出错
        LogError("[DataProvider::SetConnectionString] Prase string failed!");
        return false;
    }

    std::vector<std::string> addr_result;
    StringUtils::SplitString(strDataSource, ':', addr_result);
    if (2 != addr_result.size())
    {
        // 解析地址出错
        LogError("[DataProvider::SetConnectionString] Prase string failed!");
        return false;
    }

    std::string strAddr = addr_result[0];
    int nPort = StringUtils::StringAsInt(addr_result[1].c_str(), 0);
    if (nPort <= 0)
    {
        LogError("[DataProvider::SetConnectionString] Prase string failed!");
        return false;
    }

    m_nPort = nPort;
    m_strAddr = strAddr;
    m_strUser = strUserID;
    m_strPaswd = strPassword;
    m_strName = strCatalog;

    return true;
}

IDataConnector *DataProvider::GetConnector()
{
    std::lock_guard<std::mutex> locker(m_Mutex);

    DataConnector *pConnector;
    if (!m_Connectors.empty())
    {
        pConnector = m_Connectors.front();
        m_Connectors.pop_front();
    }
    else
    {
        pConnector = new DataConnector();
    }

    if (!pConnector->Ping())
    {
        if (!pConnector->Open(m_strAddr.c_str(), m_nPort, m_strName.c_str(), m_strUser.c_str(), m_strPaswd.c_str()))
        {
            LogError("[DataProvider::GetConnector] Open mysql failed: %s", pConnector->GetLastError());

            delete pConnector;
            pConnector = nullptr;
        }
    }

    return pConnector;
}

void DataProvider::FreeConnector(IDataConnector *pConnector)
{
    if (pConnector)
    {
        std::lock_guard<std::mutex> locker(m_Mutex);
        m_Connectors.push_back((DataConnector *)pConnector);
    }
}
