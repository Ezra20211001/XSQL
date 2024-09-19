//--------------------------------------------------------------------
// 文件名:		StringUtilss.cpp
// 内  容:		字符串工具
// 说  明:		
// 创建日期:	
// 创建人:		
// 版权所有:	
//--------------------------------------------------------------------

#include "StringUtils.h"
// #include <codecvt>

#ifdef _WIN32
#include <Windows.h>
#else
#include <string.h>
// #include <strings.h>
#endif

//字符串是否为空
bool StringUtils::CharIsNull(const char* value)
{
    if (!value || value[0] == '\0')
    {
        return true;
    }

    return false;
}

unsigned char StringUtils::HexAsInt(unsigned char hex)
{
    const int DEC = 10;
    if (('0' <= hex) && ('9' >= hex))
    {
        //减去'0'转换为整数
        return (hex - '0');
    }
    else if (('A' <= hex) && ('F' >= hex))
    {
        return (hex - 'A' + DEC);
    }
    else if (('a' <= hex) && ('f' >= hex))
    {
        return (hex - 'a' + DEC);
    }

    return 0;
}

std::string StringUtils::HexAsString(const unsigned char *value, int len)
{
    std::string result(len *2, '\0');

    for (int i = 0; i < len; ++i)
    {
        char *buff = (char *)result.data() + i * 2;
        sprintf(buff, "%02x", value[i]);
    }

    return std::move(result);
}

//把一个bool值转换成一个字符串
std::string StringUtils::BoolAsString(bool value)
{
    if (value)
    {
        return "true";
    }

    return "false";
}


//整数转换为字符串
std::string StringUtils::IntAsString(int value)
{
    return std::move(std::to_string(value));
}

//无符号整型转换为字符串
std::string StringUtils::UIntAsString(unsigned int value)
{
    return std::move(std::to_string(value));
}

//长整型转换成字符串
std::string StringUtils::Int64AsString(long long value)
{
    return std::move(std::to_string(value));
}

std::string StringUtils::UInt64AsString(unsigned long long value)
{
    return std::move(std::to_string(value));
}

// 浮点数转换为字符串(单精度小数有效位数6-7位)
std::string StringUtils::FloatAsString(float value, int decimals)
{
    std::string str = std::to_string(value);
    if (0 >= decimals)
    {
        return std::move(str);
    }

    size_t pos = str.find('.');
    if (str.length() - pos > (size_t)decimals)
    {
        return std::move(str.substr(0, pos + 1 + decimals));
    }
    else
    {
        return std::move(str);
    }
}

// 双精度浮点数转换成字符串
std::string StringUtils::DoubleAsString(double value, int decimals)
{
    std::string str = std::to_string(value);
    if (0 >= decimals)
    {
        return std::move(str);
    }

    size_t pos = str.find('.');
    if (str.length() - pos > (size_t)decimals)
    {
        return std::move(str.substr(0, pos + 1 + decimals));
    }
    else
    {
        return std::move(str);
    }
}

//把一个字符串转换成bool,如果其串为空则返回def_value
bool StringUtils::StringAsBool(const char* str, bool def_value)
{
    if (CharIsNull(str))
    {
        return def_value;
    }

#ifdef _WIN32
    return (_stricmp(str, "true") == 0);
#else
    return 0 == strcasecmp(str, "true");
#endif
}

// 字符串转换为整数
int StringUtils::StringAsInt(const char* value, int def_value)
{
    if (CharIsNull(value))
    {
        return def_value;
    }
    return atoi(value);
}

int64_t StringUtils::StringAsInt64(const char *value, int64_t def_value)
{
    if (CharIsNull(value))
    {
        return def_value;
    }

    return atoll(value);  
}

// 字符串转换为浮点数
float StringUtils::StringAsFloat(const char* value, float def_value)
{
    if (CharIsNull(value))
    {
        return def_value;
    }

    return atof(value);
}

//字符串转换为浮点数
double StringUtils::StringAsDouble(const char * value, double def_value)
{
    if (CharIsNull(value))
    {
        return def_value;
    }

    return atof(value);
}

//将英文字符串转换成16进制字符串
std::string StringUtils::StringAsHex(const char *value)
{
    if (CharIsNull(value))
    {
        return "";
    }

    //二个十六进制数表示一个字符串，i每次加2
    int len = strlen(value);
    if (len % 2 != 0)
    {
        return "";
    }

    std::string result(len / 2, '\0');
    for (int i = 0; i < len; i += 2)
    {
        result[i / 2] = HexAsInt(value[i]) * 16 + HexAsInt(value[i + 1]);
    }

    return std::move(result);
}

// 取子串，pos从0开始
std::string StringUtils::Substr(const char* str, size_t pos, size_t len)
{
    if (len == 0)
    {
        return std::string();
    }

    size_t i = pos + len;

    if (i > strlen(str))
    {
        return std::string();
    }

    std::string s(str);

    return s.substr(pos, len);
}


// 把str字符串里的pat都换成replace
void StringUtils::StrReplace(std::string& str, const std::string& pat, const std::string& replace)
{
    size_t pos = 0;

    while (pos < str.size())
    {
        pos = str.find(pat, pos);

        if (pos == std::string::npos)
        {
            break;
        }

        str.replace(pos, pat.size(), replace);

        pos += replace.size();
    }
}

// 去除str字符串中前后的指定字符
std::string StringUtils::Trim(const std::string& str, const char ch)
{
    if (str.empty())
    {
        return "";
    }

    if (str[0] != ch && str[str.size()-1] != ch)
    {
        return str;
    }

    size_t pos_begin = str.find_first_not_of(ch, 0);
    size_t pos_end = str.find_last_not_of(ch, str.size());

    if (pos_begin == std::string::npos || pos_end == std::string::npos)
    {
        //这表示这个字符串里的字符全都是ch组成，把它设置为空串
        return "";
    }

    return str.substr(pos_begin, pos_end - pos_begin);
}

// 分割字符串
bool StringUtils::SplitString(const std::string& src, const char ch, std::vector<std::string>& vec)
{
    if (src.empty())
    {
        return false;
    }

    vec.clear();

    size_t nLast = 0;
    size_t nCur = 0;
    while (true)
    {
        nCur = src.find(ch, nLast);
        if (nCur == std::string::npos)
        {
            break;
        }

        std::string sSub = src.substr(nLast, nCur - nLast);
        if (!sSub.empty())
        {
            vec.push_back(sSub);
        }
        nLast = nCur + 1;
    }

    std::string sSub = src.substr(nLast);
    if (!sSub.empty())
    {
        vec.push_back(sSub);
    }

    return true;
}

// 宽字符转字符串
std::string StringUtils::WideStrAsString(const std::wstring& wstr)
{
    if (wstr.empty())
    {
        return std::string();
    }

    // 字符串最大长度
#define MAX_STRING_LEN	1024

    char s[MAX_STRING_LEN] = { '\0' };
    int nLen = 0;

#ifdef _WIN32

    nLen = WideCharToMultiByte(CP_ACP, NULL, wstr.c_str(), -1, NULL, 0, NULL, FALSE);
    if (nLen > MAX_STRING_LEN)
    {
        return std::string();
    }

    WideCharToMultiByte(CP_OEMCP, NULL, wstr.c_str(), -1, s, nLen, NULL, FALSE);
    s[nLen - 1] = '\0';

#else 
    nLen = wcstombs(NULL, wstr.c_str(), 0);
    if (nLen >= MAX_STRING_LEN)
    {
        return std::string();
    }

    wcstombs(s, wstr.c_str(), nLen);
    s[nLen] = '\0';

#endif 

    return std::string(s);
}

// 字符串转宽字符串
std::wstring StringUtils::StringAsWideStr(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring();
    }

// 字符串最大长度
#define MAX_STRING_LEN	1024

    wchar_t	ws[MAX_STRING_LEN] = {L'\0'};
    int nLen = 0;

#ifdef _WIN32

    nLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), 0, 0);
    if (nLen >= MAX_STRING_LEN)
    {
        return std::wstring();
    }
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), ws, nLen); 

#else 
    setlocale(LC_CTYPE, ""); 
    nLen = mbstowcs(NULL, str.c_str(), str.length());
    if (nLen >= MAX_STRING_LEN)
    {
        return std::wstring();
    }

    int nRet = mbstowcs(ws, str.c_str(), str.length());
    if (nRet < nLen || nRet == -1)
    {
        return std::wstring();
    }

#endif 

    ws[nLen] = '\0';

    return std::wstring(ws);
}

// std::string StringUtils::WideStrAsUTF8(const std::wstring & wstr)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//     return conv.to_bytes(wstr);
// }

// std::wstring StringUtils::UTF8AsWideStr(const std::string & src)
// {
//     std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//     return conv.from_bytes(src);
// }
