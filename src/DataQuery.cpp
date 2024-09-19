//--------------------------------------------------------------------
// 文件名:		DataQuery.cpp
// 内  容:		MySQL查询结果集
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#include "DataQuery.h"

DataQuery::DataQuery() : m_pMySql(nullptr),
                         m_pMyRes(nullptr),
                         m_pFields(nullptr),
                         m_nRows(0),
                         m_nCols(0),
                         m_nCurRowPos(-1),
                         m_CurRowVal(nullptr)
{
}

DataQuery::~DataQuery()
{
}

const char *DataQuery::GetAccessName()
{
    return m_pFields ? m_pFields->db : "";
}

const char *DataQuery::GetTableName()
{
    return m_pFields ? m_pFields->table : "";
}

uint64_t DataQuery::GetAffectRows()
{
    return mysql_affected_rows(m_pMySql);
}

uint64_t DataQuery::GetInsertID()
{
    return mysql_insert_id(m_pMySql);
}

uint64_t DataQuery::GetRows()
{
    return m_nRows;
}

uint32_t DataQuery::GetCols()
{
    return m_nCols;
}

int DataQuery::GetColType(uint32_t col)
{
    int type = DATA_FIELD_UNKNOWN;
    switch (m_pFields[col].type)
    {
    case FIELD_TYPE_TINY:
    case FIELD_TYPE_SHORT:
        type = DATA_FIELD_INT;
        break;
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_LONGLONG:
        type = DATA_FIELD_INT64;
        break;
    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
    case FIELD_TYPE_DECIMAL:
        type = DATA_FIELD_DOUBLE;
        break;
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_VAR_STRING:
        type = DATA_FIELD_STRING;
        break;
    case FIELD_TYPE_TIMESTAMP:
    case FIELD_TYPE_DATETIME:
        type = DATA_FIELD_DATETIME;
        break;
    default:
        break;
    }

    return type;
}

const char *DataQuery::GetColName(uint32_t col)
{
    if (col < m_nCols)
    {
        // MYSQL_FIELD* pField =  mysql_fetch_field_direct(m_pMyRes, col);
        return m_pFields[col].name ? m_pFields[col].name : "";
    }

    return "";
}

char *DataQuery::GetData(uint32_t row, uint32_t col)
{
    static char s_NULL = 0;
    if (row < m_nRows && col < m_nCols)
    {
        if (row != m_nCurRowPos)
        {
            if (row == m_nCurRowPos + 1)
            {
                mysql_row_seek(m_pMyRes, m_pMyRes->data_cursor);
            }
            else
            {
                mysql_data_seek(m_pMyRes, row);
            }

            m_CurRowVal = mysql_fetch_row(m_pMyRes);
            m_nCurRowPos = row;
        }

        return m_CurRowVal[col] ? m_CurRowVal[col] : &s_NULL;
    }

    return &s_NULL;
}

uint32_t DataQuery::GetDataLen(uint32_t row, uint32_t col)
{
    if (row < m_nRows && col < m_nCols)
    {
        if (row != m_nCurRowPos)
        {
            if (row == m_nCurRowPos + 1)
            {
                mysql_row_seek(m_pMyRes, m_pMyRes->data_cursor);
            }
            else
            {
                mysql_data_seek(m_pMyRes, row);
            }

            m_nCurRowPos = row;
        }

        unsigned long *pLength = mysql_fetch_lengths(m_pMyRes);
        return (uint32_t)pLength[col];
    }

    return 0;
}

bool DataQuery::StoreResult(MYSQL *pMySql)
{
    FreeResult();

    m_pMySql = pMySql;
    m_pMyRes = mysql_store_result(m_pMySql);
    if (!m_pMyRes)
    {
        return false;
    }

    m_nRows = mysql_num_rows(m_pMyRes);
    m_nCols = mysql_num_fields(m_pMyRes);
    m_pFields = mysql_fetch_fields(m_pMyRes);

    return true;
}

void DataQuery::FreeResult()
{
    if (m_pMyRes)
    {
        mysql_free_result(m_pMyRes);
        m_pMyRes = nullptr;
    }

    m_pMySql = nullptr;
    m_pFields = nullptr;
    m_nRows = 0;
    m_nCols = 0;
    m_nCurRowPos = -1;
    m_CurRowVal = nullptr;
}
