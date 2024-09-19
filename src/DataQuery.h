//--------------------------------------------------------------------
// 文件名:		DataQuery.h
// 内  容:		MySQL查询结果集
// 说  明:		
// 创建日期:	
// 创建人:		
// 版权所有:	
//--------------------------------------------------------------------
#pragma once
#include "IDataProvider.h"
#include <mysql/mysql.h>

class DataQuery :public IDataQuery
{
public:
    DataQuery();

    virtual ~DataQuery();

    virtual const char* GetAccessName() override;

    virtual const char* GetTableName() override;

    virtual uint64_t GetAffectRows() override;

    virtual uint64_t GetInsertID() override;

    virtual uint64_t GetRows() override;

    virtual uint32_t GetCols() override;

    virtual int GetColType(uint32_t col) override;

    virtual const char* GetColName(uint32_t col) override;

    virtual char* GetData(uint32_t row, uint32_t col) override;

    virtual uint32_t GetDataLen(uint32_t row, uint32_t col) override;

public:
    // 转储结果
    bool StoreResult(MYSQL* pMySql);

    // 释放结果
    void FreeResult();

private:
    MYSQL* m_pMySql;		// 连接对象
    MYSQL_RES* m_pMyRes;	// 结果集和
    MYSQL_FIELD* m_pFields;	// 列信息
    uint64_t m_nRows;
    uint32_t m_nCols;
    uint64_t m_nCurRowPos;
    MYSQL_ROW m_CurRowVal;
};

