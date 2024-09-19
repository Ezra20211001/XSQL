//--------------------------------------------------------------------
// 文件名:		IDataProvider.h
// 内  容:
// 说  明:		数据库接口
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#pragma once
#include <stdint.h>

enum DATA_FIELD
{
    DATA_FIELD_UNKNOWN,  // 未知类型
    DATA_FIELD_INT,      // 整形
    DATA_FIELD_INT64,    // 长整型
    DATA_FIELD_DOUBLE,   // 浮点型
    DATA_FIELD_STRING,   // 字符串类型
    DATA_FIELD_DATETIME, // 时间类型
};

class IDataQuery
{
public:
    virtual ~IDataQuery() = default;

    // 获得数据库名
    virtual const char *GetAccessName() = 0;

    // 获得数据表名
    virtual const char *GetTableName() = 0;

    // 返回影响的行数
    virtual uint64_t GetAffectRows() = 0;

    // 返回自增ID
    virtual uint64_t GetInsertID() = 0;

    // 获得查询行数
    virtual uint64_t GetRows() = 0;

    // 获得查询列数
    virtual uint32_t GetCols() = 0;

    // 获得列类型
    virtual int GetColType(uint32_t col) = 0;

    // 获得列名称
    virtual const char *GetColName(uint32_t col) = 0;

    // 获得指定行列数据
    virtual char *GetData(uint32_t row, uint32_t col) = 0;

    // 获得数据长度
    virtual uint32_t GetDataLen(uint32_t row, uint32_t col) = 0;
};

class IDataConnector
{
public:
    virtual ~IDataConnector() = default;

    // 执行ping命令
    virtual bool Ping() = 0;

    // 获得错误描述
    virtual const char *GetLastError() = 0;

    virtual bool ExecuteSQL(const char *sql, uint32_t len) = 0;

    // 获得查询接口
    virtual IDataQuery *GetDataQuery() = 0;

    // 释放查询接口
    virtual void ReleaseQuery(IDataQuery *pQuery) = 0;
};

class IDataProvider
{
public:
    virtual ~IDataProvider() = default;

    // 设置连接字符串
    // Data Source=127.0.0.1:3306;Password=123456;User ID=root;Initial Catalog=xxx
    virtual bool SetConnectionString(const char *value) = 0;

    // 获取一个MySQL连接
    virtual IDataConnector *GetConnector() = 0;

    // 释放MySQL连接
    virtual void FreeConnector(IDataConnector *pConnector) = 0;
};