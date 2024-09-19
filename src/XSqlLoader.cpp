#include "XSqlLoader.h"
#include "Logger.h"
#include "StringUtils.h"
#include "IDataProvider.h"
#include "tinyxml.h"
#include <sstream>
#include <vector>

struct xaction_t
{
    xrecord_t *record;
    std::string actName;
    std::string actType;
    // std::string sqlFormat;
    std::vector<size_t> where;
    std::vector<size_t> variant;
};

struct xrecord_t
{
    std::string recordName;
    std::vector<std::string> fieldNames;
    std::vector<VARIANT_TYPE> fieldTypes;
    std::vector<xaction_t> actions;

    int get_column(const char *fieldName)
    {
        for (int i = 0; i < (int)fieldNames.size(); i++)
        {
            if (fieldNames[i] == fieldName)
            {
                return i;
            }
        }

        return -1;
    }
};

extern IDataProvider *g_pDataProvider;

XSqlLoader::XSqlLoader()
{
}

XSqlLoader::~XSqlLoader()
{
    m_XAction.clear();

    for (auto iter = m_XRecord.begin(); iter != m_XRecord.end(); ++iter)
    {
        delete iter->second;
    }
    m_XRecord.clear();
}

bool XSqlLoader::Query(const char *action, const CVarList &where, const CVarList &args)
{
    auto iter = m_XAction.find(action);
    if (iter == m_XAction.end())
    {
        LogError("[XSqlLoader::Query] unknown action: %s", action);
        return false;
    }

    std::string sql = FormatSQL(iter->second, where, args);
    if (sql.empty())
    {
        return false;
    }

    IDataConnector *pConnector = g_pDataProvider->GetConnector();
    if (!pConnector->ExecuteSQL(sql.c_str(), sql.length()))
    {
        LogError("[XSqlLoader::Query] %s: %s", sql.c_str(), pConnector->GetLastError());
    }

    g_pDataProvider->FreeConnector(pConnector);

    return true;
}

bool XSqlLoader::Query(const char *action, const CVarList &where, const CVarList &args, CVarList &result)
{
    auto iter = m_XAction.find(action);
    if (iter == m_XAction.end())
    {
        LogError("[XSqlLoader::Query] unknown action: %s", action);
        return false;
    }

    std::string sql = FormatSQL(iter->second, where, args);
    if (sql.empty())
    {
        return false;
    }

    IDataConnector *pConnector = g_pDataProvider->GetConnector();
    if (!pConnector->ExecuteSQL(sql.c_str(), sql.length()))
    {
        LogError("[XSqlLoader::Query] %s: %s", sql.c_str(), pConnector->GetLastError());
        g_pDataProvider->FreeConnector(pConnector);
        return false;
    }

    IDataQuery *pQuery = pConnector->GetDataQuery();
    for (uint64_t uRows = 0; uRows < pQuery->GetRows(); ++uRows)
    {
        for (uint32_t uCols = 0; uCols < pQuery->GetCols(); ++uCols)
        {
            switch (pQuery->GetColType(uCols))
            {
            case DATA_FIELD_INT:
                result.AddInt(StringUtils::StringAsInt(pQuery->GetData(uRows, uCols)));
                break;
            case DATA_FIELD_INT64:
                result.AddInt64(StringUtils::StringAsInt64(pQuery->GetData(uRows, uCols)));
                break;
            case DATA_FIELD_DOUBLE:
                result.AddDouble(StringUtils::StringAsDouble(pQuery->GetData(uRows, uCols)));
                break;
            case DATA_FIELD_STRING:
                result.AddString((const char *)pQuery->GetData(uRows, uCols));
                break;
            case DATA_FIELD_DATETIME:
                result.AddString((const char *)pQuery->GetData(uRows, uCols));
                break;
            default:
                LogError("[XSqlLoader::Query] unknown type, %s", sql.c_str());
                break;
            }
        }
    }

    // 释放结果集
    pConnector->ReleaseQuery(pQuery);
    // 最后要释放连接
    g_pDataProvider->FreeConnector(pConnector);

    return true;
}

bool XSqlLoader::Load(const char *file)
{
    TiXmlDocument document;
    if (!document.LoadFile(file))
    {
        LogError("[XSqlLoader::Load] cannot find %s", file);
        return false;
    }

    TiXmlElement *root = document.RootElement();
    if (nullptr == root)
    {
        LogError("[XSqlLoader::Load] read root element error!");
        return false;
    }

    TiXmlElement *pRecordEle = root->FirstChildElement("record");
    while (pRecordEle)
    {
        if (!ParseRecord(pRecordEle))
        {
            return false;
        }

        pRecordEle = pRecordEle->NextSiblingElement("record");
    }

    return true;
}

int XSqlLoader::ConvertVarType(const char *szVarType)
{
    int nVarType = VTYPE_UNKNOWN;
    if (strcasecmp(szVarType, "byte") == 0 || strcasecmp(szVarType, "int32") == 0)
    {
        nVarType = VTYPE_INT;
    }
    else if (strcasecmp(szVarType, "int64") == 0)
    {
        nVarType = VTYPE_INT64;
    }
    else if (strcasecmp(szVarType, "float") == 0)
    {
        nVarType = VTYPE_FLOAT;
    }
    else if (strcasecmp(szVarType, "double") == 0)
    {
        nVarType = VTYPE_DOUBLE;
    }
    else if (strcasecmp(szVarType, "string") == 0)
    {
        nVarType = VTYPE_STRING;
    }

    return nVarType;
}

bool XSqlLoader::ParseRecord(TiXmlElement *pRecordEle)
{
    const char *name = pRecordEle->Attribute("name");

    if (m_XRecord.find(name) != m_XRecord.end())
    {
        LogError("[XSqlLoader::ParseRecord] multiple definition of record '%s'", name);
        return false;
    }

    xrecord_t record;
    record.recordName = name;

    // 读取列
    TiXmlElement *pColumn = pRecordEle->FirstChildElement("column");
    while (pColumn)
    {
        const char *fieldName = pColumn->Attribute("field");
        const char *fieldType = pColumn->Attribute("type");

        int varType = ConvertVarType(fieldType);
        if (VTYPE_UNKNOWN == varType)
        {
            LogError("[XSqlLoader::ParseRecord] unknown type[%s]", fieldType);
            return false;
        }

        record.fieldNames.push_back(fieldName);
        record.fieldTypes.push_back((VARIANT_TYPE)varType);

        pColumn = pColumn->NextSiblingElement("column");
    }

    if (!ParseAction("insert", pRecordEle, &record))
    {
        return false;
    }

    if (!ParseAction("delete", pRecordEle, &record))
    {
        return false;
    }

    if (!ParseAction("update", pRecordEle, &record))
    {
        return false;
    }

    if (!ParseAction("select", pRecordEle, &record))
    {
        return false;
    }

    // 检查action是否冲突
    for (size_t i = 0; i < record.actions.size(); i++)
    {
        if (m_XAction.find(record.actions[i].actName) != m_XAction.end())
        {
            LogError("[XSqlLoader::ParseRecord] multiple definition of action '%s'", record.actions[i].actName.c_str());
            return false;
        }
    }

    xrecord_t *pRecord = new xrecord_t;
    *pRecord = record;
    m_XRecord.insert(std::make_pair(name, pRecord));

    for (size_t i = 0; i < pRecord->actions.size(); i++)
    {
        xaction_t *pAction = &pRecord->actions[i];
        pAction->record = pRecord;
        m_XAction.insert(std::make_pair(pAction->actName, pAction));
    }

    return true;
}

bool XSqlLoader::ParseAction(const char *actType, TiXmlElement *pRecordEle, xrecord_t *pRecord)
{
    TiXmlElement *pActEle = pRecordEle->FirstChildElement(actType);
    while (pActEle)
    {
        xaction_t action;
        action.actType = actType;
        action.actName = pActEle->Attribute("action");

        // 读取条件
        TiXmlElement *pWhereEle = pActEle->FirstChildElement("where");
        if (pWhereEle)
        {
            TiXmlElement *pColEle = pWhereEle->FirstChildElement("column");
            while (pColEle)
            {
                const char *fieldName = pColEle->Attribute("field");
                int col = pRecord->get_column(fieldName);
                if (-1 == col)
                {
                    LogError("[XSqlLoader::ParseAction] unknown filed[%s]", fieldName);
                    return false;
                }

                action.where.push_back(col);
                pColEle = pColEle->NextSiblingElement("column");
            }
        }

        // 读取位域
        TiXmlElement *pFieldEle = pActEle->FirstChildElement("fields");
        if (pFieldEle)
        {
            TiXmlElement *pColEle = pFieldEle->FirstChildElement("column");
            while (pColEle)
            {
                const char *fieldName = pColEle->Attribute("field");
                int col = pRecord->get_column(fieldName);
                if (-1 == col)
                {
                    LogError("[XSqlLoader::ParseAction] unknown filed[%s]", fieldName);
                    return false;
                }

                action.variant.push_back(col);
                pColEle = pColEle->NextSiblingElement("column");
            }
        }

        pRecord->actions.push_back(action);

        pActEle = pActEle->NextSiblingElement(actType);
    }

    return true;
}

static inline bool format_args(std::stringstream &ss, const CVarList &args, size_t pos, const char *link)
{
    switch (args.GetType(pos))
    {
    case VTYPE_INT:
        ss << args.IntVal(pos);
        break;
    case VTYPE_INT64:
        ss << args.Int64Val(pos);
        break;
    case VTYPE_FLOAT:
        ss << args.FloatVal(pos);
        break;
    case VTYPE_DOUBLE:
        ss << args.DoubleVal(pos);
    case VTYPE_STRING:
        ss << "'" << args.StrintVal(pos) << "'";
        break;
    default:
        LogError("unknown args type: %d", args.GetType(pos));
        return false;
    }

    if (pos != args.GetCount() - 1)
    {
        ss << link;
    }

    return true;
}

std::string XSqlLoader::FormatSQL(xaction_t *pAction, const CVarList &where, const CVarList &args)
{
    std::stringstream ss;
    if (0 == strcasecmp(pAction->actType.c_str(), "insert"))
    {
        ss << "INSERT INTO " << pAction->record->recordName << " ";
        if (pAction->variant.size() > 0)
        {
            ss << "(";
            for (size_t i = 0; i < pAction->variant.size(); i++)
            {
                ss << pAction->record->fieldNames[pAction->variant[i]];
                if (i != pAction->variant.size() - 1)
                {
                    ss << ", ";
                }
            }
            ss << ") ";
        }

        ss << "VALUES (";
        for (size_t i = 0; i < args.GetCount(); i++)
        {
            format_args(ss, args, i, ", ");
        }
        ss << ")";
    }
    else if (0 == strcasecmp(pAction->actType.c_str(), "delete"))
    {
        ss << "DELETE FROM " << pAction->record->recordName;
        if (pAction->where.size() > 0)
        {
            ss << " WHERE ";
            for (size_t i = 0; i < pAction->where.size(); i++)
            {
                ss << pAction->record->fieldNames[pAction->where[i]] << " = ";
                format_args(ss, where, i, " AND ");
            }
        }
    }
    else if (0 == strcasecmp(pAction->actType.c_str(), "update"))
    {
        ss << "UPDATE " << pAction->record->recordName << " SET ";
        for (size_t i = 0; i < pAction->variant.size(); i++)
        {
            ss << pAction->record->fieldNames[pAction->variant[i]] << " = ";
            format_args(ss, args, i, " , ");
        }

        // 检查where
        if (pAction->where.size() > 0)
        {
            ss << " WHERE ";
            for (size_t i = 0; i < pAction->where.size(); i++)
            {
                ss << pAction->record->fieldNames[pAction->where[i]] << " = ";
                format_args(ss, where, i, " AND ");
            }
        }
    }
    else if (0 == strcasecmp(pAction->actType.c_str(), "select"))
    {
        ss << "SELECT ";
        if (pAction->variant.size() > 0)
        {
            for (size_t i = 0; i < pAction->variant.size(); i++)
            {
                ss << pAction->record->fieldNames[pAction->variant[i]];
                if (i != pAction->variant.size() - 1)
                {
                    ss << ", ";
                }
            }
        }
        else
        {
            ss << "* ";
        }
        ss << " FROM " << pAction->record->recordName;

        // 检查where
        if (pAction->where.size() > 0)
        {
            ss << " WHERE ";
            for (size_t i = 0; i < pAction->where.size(); i++)
            {
                ss << pAction->record->fieldNames[pAction->where[i]] << " = ";
                format_args(ss, where, i, " AND ");
            }
        }
    }

    return ss.str();
}