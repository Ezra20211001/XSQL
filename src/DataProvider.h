//--------------------------------------------------------------------
// 文件名:		DataProvider.h
// 内  容:		数据库连接池
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#pragma once
#include "DataConnector.h"
#include <mutex>
#include <list>

class DataProvider : public IDataProvider
{
public:
    DataProvider();
    virtual ~DataProvider();

    // 设置连接字符串
    virtual bool SetConnectionString(const char *value) override;

    virtual IDataConnector *GetConnector() override;

    virtual void FreeConnector(IDataConnector *pConnector) override;

private:
    int m_nPort;            // 数据库端口号
    std::string m_strAddr;  // 数据库地址
    std::string m_strUser;  // 数据库用户
    std::string m_strPaswd; // 登录密码
    std::string m_strName;  // 数据库名称

    std::mutex m_Mutex;
    std::list<DataConnector*> m_Connectors; // 活跃连接
};
