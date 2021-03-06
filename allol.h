#ifndef __ALLOL_H__
#define __ALLOL_H__

#include <limits>

template <typename T>
class allol
{
public:
    
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef size_t   size_type;

    T* address(T& value) const
    {
        return &value;
    }

    const T* address(const T& value) const
    {
        return &value;
    }


    allol() throw()
    {
    }

    allol(const allol&) throw()
    {
    }

    template <class U>
    allol(const allol<U>&) throw()
    {
    }

    ~allol() throw()
    {
    }

    size_t max_size() const throw()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    T* allocate(size_t size, const void* = 0)
    {
        ++stats.alloc_count;
        return static_cast<T*>(operator new(size * sizeof(T)));
    }

    void construct(T* p, const T& value)
    {
        ++stats.cnstr_count;
        new (p) T{ value };
    }

    void destroy(T* p)
    {
        ++stats.destr_count;
        p->~T();
    }

    void deallocate(pointer p, size_type num)
    {
        ++stats.dealc_count;
        operator delete(p);
    }
};

template <class T1, class T2>
bool operator==(const allol<T1>&, const allol<T2>&) throw()
{
    return true;
}

template <class T1, class T2>
bool operator!= (const allol<T1>&, const allol<T2>&) throw()
{
    return false;
}

#endif