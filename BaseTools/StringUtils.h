//--------------------------------------------------------------------
// 文件名:      StringUtilss.h
// 内  容:      字符串工具
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>

class StringUtils
{
public:
    /**
     * @brief 判断字符串是否为空
     *
     * @param value 字符串
     * @return true 字符串为空
     * @return false 字符串不为空
     */
    static bool CharIsNull(const char *value);

    /**
     * @brief 将16进制数字转换为十进制
     * 
     * @param hex 十六进制字符（'0'~'9','a'~'f','A'~'F'）
     * @return char 转换后的字符 
     */
    static unsigned char HexAsInt(unsigned char hex);

    /**
     * @brief 将十六进制转换成字符串 (0xff->"ff", 0xFF->"ff")
     * 
     * @param value 
     * @return std::string 
     */
    static std::string HexAsString(const unsigned char *value, int len);

    /**
     * @brief 把一个bool值转换成一个字符串
     *
     * @param value 将要转换的bool值
     * @return std::string "true" || "false"
     */
    static std::string BoolAsString(bool value);

    /**
     * @brief 整数转换为字符串
     *
     * @param value 将要转换的值
     * @return std::string 字符串值
     */
    static std::string IntAsString(int value);

    /**
     * @brief 无符号整型转换为字符串
     *
     * @param value 将要转换的值
     * @return std::string 字符串值
     */
    static std::string UIntAsString(unsigned int value);

    /**
     * @brief 长整型转换成字符串
     *
     * @param value 将要转换的值
     * @return std::string 字符串值
     */
    static std::string Int64AsString(long long value);

    /**
     * @brief 无符号长整型转换成字符串
     *
     * @param value 将要转换的值
     * @return std::string 字符串值
     */
    static std::string UInt64AsString(unsigned long long value);

    /**
     * @brief 单精度浮点数转换为字符串
     *
     * @param value 将要转换的值
     * @param decimals 保留位数(单精度小数有效位数6-7位)
     * @return std::string 字符串值
     */
    static std::string FloatAsString(float value, int decimals = 0);

    /**
     * @brief 双精度浮点数转换成字符串
     *
     * @param value 将要转换的值
     * @param decimals 保留位数
     * @return std::string 字符串值
     */
    static std::string DoubleAsString(double value, int decimals = 0);

    /**
     * @brief 把一个字符串转换成bool,如果其串为空则返回def_value
     *
     * @param str "true" | false,不区分大小写
     * @param def_value 缺省值
     * @return true
     * @return false
     */
    static bool StringAsBool(const char *str, bool def_value);

    /**
     * @brief 字符串转换为整数
     *
     * @param value 将要转换的值
     * @param def_value 缺省值
     * @return int
     */
    static int StringAsInt(const char *value, int def_value = 0);

    /**
     * @brief 字符串转换为长整数
     *
     * @param value 将要转换的值
     * @param def_value 缺省值
     * @return int64_t
     */
    static int64_t StringAsInt64(const char *value, int64_t def_value = 0);

    /**
     * @brief 字符串转换为浮点数
     *
     * @param value 将要转换的值
     * @param def_value 缺省值
     * @return int64_t
     */
    static float StringAsFloat(const char *value, float def_value = 0.0f);

    /**
     * @brief 字符串转换为浮点数
     *
     * @param value 将要转换的值
     * @param def_value 缺省值
     * @return int64_t
     */
    static double StringAsDouble(const char *value, double def_value = 0.0);

    /**
     * @brief 将十六进制表示字符串转换成16进制("FF"->0xFF, "ff"->0xff)
     * HexStringAsHex
     * @param value 需要转换的字符串
     * @return std::string 返回转换的结果
     */
    static std::string StringAsHex(const char *value);

    /**
     * @brief 取子串
     * 
     * @param str 原始字符串
     * @param pos 起始位置,从0开始
     * @param len 子串长度
     * @return std::string 
     */
    static std::string Substr(const char *str, size_t pos, size_t len);

    /**
     * @brief 把str字符串里的pat都换成replace
     * 
     * @param str 原始字符串
     * @param pat 需要替换的字符串
     * @param replace 用来替换的字符串
     */
    static void StrReplace(std::string &str, const std::string &pat, const std::string &replace);

    /**
     * @brief 去除str字符串中前后的指定字符
     * 
     * @param str 原始字符串
     * @param ch 需要去除的字符
     * @return std::string 结果字符串
     */
    static std::string Trim(const std::string &str, const char ch = ' ');

    /**
     * @brief 分割字符串
     * 
     * @param sour 
     * @param vec 
     * @param ch 
     * @return true 
     * @return false 
     */
    static bool SplitString(const std::string &src, const char ch, std::vector<std::string> &vec);

    /**
     * @brief 宽字符转字符串
     * 
     * @param wstr 目标字符串
     * @return std::string 返回窄字符串
     */
    static std::string WideStrAsString(const std::wstring &wstr);

    /**
     * @brief 字符串转宽字符串
     * 
     * @param str 目标字符串
     * @return std::wstring 返回宽字符串
     */
    static std::wstring StringAsWideStr(const std::string &str);


    // 宽字符转UTF8
    // static std::string WideStrAsUTF8(const std::wstring& wstr);

    // UTF8转宽字符
    // static std::wstring UTF8AsWideStr(const std::string& src);
};

#endif