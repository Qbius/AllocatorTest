#ifndef __ANALIZE_H__
#define __ANALIZE_H__

#include <string>
#include <map>

struct call_stats
{
    unsigned alloc_count = 0;
    unsigned dealc_count = 0;
    unsigned cnstr_count = 0;
    unsigned destr_count = 0;
};

template <template <typename, class> class TContainer, template <typename> class TAllocator = std::allocator>
class analize
{
    static call_stats access_stats;

    template <class TBaseAlloc>
    class allocator_wrapper
    {
        TBaseAlloc allocator;

    public:

        typedef typename TBaseAlloc::value_type      value_type;
        typedef typename TBaseAlloc::pointer         pointer;
        typedef typename TBaseAlloc::const_pointer   const_pointer;
        typedef typename TBaseAlloc::size_type       size_type;

        value_type* address(value_type& value) const
        {
            return &value;
        }

        const value_type* address(const value_type& value) const
        {
            return &value;
        }


        allocator_wrapper() throw()
        {
        }

        allocator_wrapper(const allocator_wrapper&) throw()
        {
        }

        template <class U>
        allocator_wrapper(const allocator_wrapper<U>&) throw()
        {
        }

        ~allocator_wrapper() throw()
        {
        }

        size_t max_size() const throw()
        {
            return allocator.max_size();
        }

        value_type* allocate(size_t size, const void* = 0)
        {
            ++access_stats.alloc_count;
            return allocator.allocate(size);
        }

        void construct(value_type* p, const value_type& value)
        {
            ++access_stats.cnstr_count;
            allocator.construct(p, value);
        }

        void destroy(value_type* p)
        {
            ++access_stats.destr_count;
            allocator.destroy(p);
        }

        void deallocate(value_type* p, size_type num)
        {
            ++access_stats.dealc_count;
            allocator.deallocate(p, num);
        }

        template <class UBaseAlloc>
        bool operator== (const allocator_wrapper<UBaseAlloc>&)
        {
            return true;
        }

        template <class UBaseAlloc>
        bool operator!= (const allocator_wrapper<UBaseAlloc>&)
        {
            return false;
        }
    };

public:

    call_stats saved_stats;
    std::string type_name;

    template<typename TValueType>
    analize(const std::initializer_list<TValueType>& i_list) : type_name("placeholder")
    {
        TContainer<TValueType, allocator_wrapper<TAllocator<TValueType>>> c{ i_list };
        std::swap(access_stats, saved_stats);
    }
};

// dunno if necessary tbh...
//template <template <typename, class> class TContainer, template <typename> class TAllocator>
//typename analize<TContainer, TAllocator>::call_stats analize<TContainer, TAllocator>::stats = analize<TContainer, TAllocator>::call_stats{};

struct type_data
{
    std::string container_type;
    std::string value_type;
    std::string allocator_type;
};

class data_dump
{
    std::map<type_data, call_stats> all_stats;

public:

    template <template <typename, class> class TContainer, template <typename> class TAllocator>
    data_dump& operator>>(const analize<TContainer, TAllocator>& a)
    {
        all_stats.insert(std::make_pair(type_data{ "placeholder", a.type_name, "placeholder" }, a.saved_stats));
        return *this;
    }
};

#endif