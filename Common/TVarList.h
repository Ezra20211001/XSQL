//--------------------------------------------------------------------
// 文件名:      TVarList.h
// 内  容:      不定参数列表
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#ifndef _TVARLIST_H_
#define _TVARLIST_H_
#include <stdlib.h>
#include <string.h>
#include <string>

enum VARIANT_TYPE
{
    VTYPE_UNKNOWN, // 未知
    VTYPE_BOOL,    // 布尔
    VTYPE_INT,     // 32位整数
    VTYPE_INT64,   // 64位整数
    VTYPE_FLOAT,   // 单精度浮点数
    VTYPE_DOUBLE,  // 双精度浮点数
    VTYPE_STRING,  // 字符串
    VTYPE_MAX,
};

template <size_t DATA_SIZE, size_t BUFFER_SIZE>
class TVarList
{
    struct var_data_t
    {
        int nType;
        union
        {
            bool boolVal;
            int32_t intVal;
            int64_t int64Val;
            float floatVal;
            double doubleVal;
            size_t stringVal;
        };
    };

public:
    TVarList(const TVarList &src) = delete;
    TVarList& operator=(const TVarList& src) = delete;

    TVarList()
    {
        m_pData = m_DataStack;
        m_nDataSize = DATA_SIZE;
        m_nDataUsed = 0;
        m_pBuffer = m_BufferStack;
        m_nBufferSize = BUFFER_SIZE;
        m_nBufferUsed = 0;
    }

    ~TVarList()
    {
        if (m_nDataSize > DATA_SIZE)
        {
            ::free(m_pData);
        }

        if (m_nBufferSize > BUFFER_SIZE)
        {
            ::free(m_pBuffer);
        }
    }

    bool AddBool(bool value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_BOOL;
        p->boolVal = value;

        return true;
    }

    bool AddInt(int32_t value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_INT;
        p->intVal = value;

        return true;
    }

    bool AddInt64(int64_t value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_INT64;
        p->int64Val = value;

        return true;
    }

    bool AddFloat(float value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_FLOAT;
        p->floatVal = value;

        return true;
    }

    bool AddDouble(double value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_DOUBLE;
        p->doubleVal = value;

        return true;
    }

    bool AddString(const char *value)
    {
        var_data_t *p = AllocVarData();

        p->nType = VTYPE_STRING;
        p->stringVal = m_nBufferUsed;

        size_t len = strlen(value) + 1;
        char *buf = AllocBuffer(len);

        memcpy(buf, value, len);

        return true;
    }

    bool BoolVal(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_BOOL == m_pData[index].nType)
        {
            return m_pData[index].boolVal;
        }

        return false;
    }

    int32_t IntVal(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_INT == m_pData[index].nType)
        {
            return m_pData[index].intVal;
        }

        return 0;
    }

    int64_t Int64Val(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_INT64 == m_pData[index].nType)
        {
            return m_pData[index].intVal;
        }

        return 0;
    }

    float FloatVal(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_FLOAT == m_pData[index].nType)
        {
            return m_pData[index].floatVal;
        }

        return 0.0f;
    }

    double DoubleVal(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_DOUBLE == m_pData[index].nType)
        {
            return m_pData[index].doubleVal;
        }

        return 0.0;
    }

    const char *StrintVal(size_t index) const
    {
        if (index < m_nDataUsed && VTYPE_STRING == m_pData[index].nType)
        {
            return m_pBuffer + m_pData[index].stringVal;
        }

        return "";
    }

    size_t GetCount() const
    {
        return m_nDataUsed;
    }

    int32_t GetType(size_t index) const
    {
        if (index < m_nDataUsed)
        {
            return m_pData[index].nType;
        }

        return VTYPE_UNKNOWN;
    }

    void Clear()
    {
        m_nDataUsed = 0;
        m_nBufferUsed = 0;
    }

    TVarList &operator<<(bool value)
    {
        AddBool(value);
        return *this;
    }
    TVarList &operator<<(char value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(unsigned char value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(short value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(unsigned short value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(int value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(unsigned int value)
    {
        AddInt(value);
        return *this;
    }
    TVarList &operator<<(int64_t value)
    {
        AddInt64(value);
        return *this;
    }
    TVarList &operator<<(uint64_t value)
    {
        AddInt64(value);
        return *this;
    }
    TVarList &operator<<(float value)
    {
        AddFloat(value);
        return *this;
    }
    TVarList &operator<<(double value)
    {
        AddDouble(value);
        return *this;
    }
    TVarList &operator<<(const char *value)
    {
        AddString(value);
        return *this;
    }
    TVarList &operator<<(const std::string &value)
    {
        AddString(value.c_str());
        return *this;
    }
private:
    var_data_t *AllocVarData()
    {
        if (m_nDataUsed >= m_nDataSize)
        {
            size_t nSize = m_nDataSize * 2;
            var_data_t *p = (var_data_t *)::malloc(nSize * sizeof(var_data_t));

            memcpy(p, m_pData, m_nDataUsed * sizeof(var_data_t));

            if (m_nDataSize > DATA_SIZE)
            {
                ::free(m_pData);
            }

            m_pData = p;
            m_nDataSize = nSize;
        }

        return m_pData + m_nDataUsed++;
    }

    char *AllocBuffer(size_t size)
    {
        size_t new_used = m_nBufferUsed + size;

        if (new_used > m_nBufferSize)
        {
            size_t new_size = m_nBufferSize * 2;

            if (new_used > new_size)
            {
                new_size = new_used * 2;
            }

            char *p = (char *)::malloc(new_size);
            memcpy(p, m_pBuffer, m_nBufferUsed);

            if (m_nBufferSize > BUFFER_SIZE)
            {
                ::free(m_pBuffer);
            }

            m_pBuffer = p;
            m_nBufferSize = new_size;
        }

        char *ret = m_pBuffer + m_nBufferUsed;

        m_nBufferUsed = new_used;

        return ret;
    }

private:
    var_data_t m_DataStack[DATA_SIZE];
    var_data_t *m_pData;
    size_t m_nDataSize;
    size_t m_nDataUsed;

    char m_BufferStack[BUFFER_SIZE];
    char *m_pBuffer;
    size_t m_nBufferSize;
    size_t m_nBufferUsed;
};

typedef TVarList<8, 128> CVarList;

#endif
