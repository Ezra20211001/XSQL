//--------------------------------------------------------------------
// 文件名:		DataConnector.h
// 内  容:
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#pragma once
#include "DataQuery.h"

class DataConnector : public IDataConnector
{
public:
    DataConnector();

    virtual ~DataConnector();

    virtual bool Ping() override;

    virtual const char *GetLastError() override;

    virtual bool ExecuteSQL(const char *sql, uint32_t len) override;

    virtual IDataQuery *GetDataQuery() override;

    virtual void ReleaseQuery(IDataQuery *pQuery) override;

public:
    bool Open(const char *addr, int port, const char *db, const char *user, const char *pwd);

    bool Close();

private:
    MYSQL *m_pMySQL;
    DataQuery m_DataQuery;
};
