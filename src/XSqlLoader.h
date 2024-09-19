#include "TVarList.h"
#include <map>

struct xaction_t;
struct xrecord_t;
class TiXmlElement;
class XSqlLoader
{
public:
    XSqlLoader();
    ~XSqlLoader();

    bool Load(const char *file);

    bool Query(const char *action, const CVarList &where, const CVarList &args);

    bool Query(const char *action, const CVarList &where, const CVarList &args, CVarList &result);

private:
    int ConvertVarType(const char *szVarType);

    // 解析record
    bool ParseRecord(TiXmlElement *pRecordEle);

    // 解析action
    bool ParseAction(const char *actType, TiXmlElement *pRecordEle, xrecord_t *pRecord);

    // 生成SQL语句
    std::string FormatSQL(xaction_t* pAction, const CVarList &where, const CVarList &args);

private:
    std::map<std::string, xaction_t*> m_XAction;
    std::map<std::string, xrecord_t*> m_XRecord;
};
