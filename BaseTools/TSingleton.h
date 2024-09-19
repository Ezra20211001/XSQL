//--------------------------------------------------------------------
// 文件名:      TSingleton.h
// 内  容:      单一实例模板
// 说  明:
// 创建日期:
// 创建人:
// 版权所有:
//--------------------------------------------------------------------
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <class T>
class TSingleton
{
public:
    TSingleton(const TSingleton<T> &) = delete;
    TSingleton(TSingleton<T> &&) = delete;
    TSingleton &operator=(const TSingleton<T> &) = delete;

    static T &Instance()
    {
        static T instace;
        return instace;
    };

protected:
    TSingleton(void) = default;
    virtual ~TSingleton(void) = default;
};

#endif