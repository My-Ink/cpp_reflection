#ifndef REFLECT_H_
#define REFLECT_H_

#include <cstring>

namespace my {

struct reflect
{
 public:
    template<class C>
    class Instance;

    template<class C>
    class Class
    {
        using class_type = C;
     public:

        Class() = default;

        Instance<C> get_instance();

        template<class T>
        bool operator==(Class<T>& rhs);
    };

    template<class C>
    class Instance
    {
     public:
        using class_type = C;
        using size_type = std::size_t;

        Instance();

        ~Instance();

        Instance& operator=(const Instance& rhs);

        C& operator*();

        template<typename T>
        const T& get_field(T C::*mp);

        template<typename T>
        void set_field(T C::*mp, T value);

        constexpr Class<C> get_class();

        template<class T>
        bool is_instance_of(Class<T>& _class);

     private:
        Class<C> _class;
        class_type* _p;
        char      * _mask;
        size_type _i_size;
    };

 private:
    template<class C>
    struct _meta
    {
        using _class_pointer = C*;
        static _class_pointer _get_pointer(void* p);

    };

};


template<class C>
typename reflect::_meta<C>::_class_pointer
reflect::_meta<C>::_get_pointer(void* p)
{
    return reinterpret_cast<_class_pointer>(p);
}


template<class C>
reflect::Instance<C>::Instance()
{
    _i_size = sizeof(class_type);
    _mask   = new char[_i_size];
    _p      = _meta<C>::_get_pointer(_mask);
}


template<class C>
reflect::Instance<C>::~Instance()
{
    delete[] _mask;
}


template<class C>
reflect::Instance<C>&
reflect::Instance<C>::operator=(const reflect::Instance<C>& rhs)
{
    if (this != &rhs) {
        delete[] _mask;
        _i_size = rhs._i_size;

        _mask = new char[_i_size];
        _p    = _meta<C>::_get_pointer(_mask);

        memccpy(_p, rhs._p, _i_size, 1);
    }
    return *this;
}


template<class C>
C& reflect::Instance<C>::operator*()
{
    return *_p;
}


template<class C>
constexpr reflect::Class<C> reflect::Instance<C>::get_class()
{
    return _class;
}


template<class C>
template<typename T>
const T& reflect::Instance<C>::get_field(T C::* mp)
{
    return _p->*mp;
}


template<class C>
template<typename T>
void reflect::Instance<C>::set_field(T C::* mp, T value)
{
    _p->*mp = value;
}


template<class C>
template<class T>
bool reflect::Instance<C>::is_instance_of(reflect::Class<T>& other_class)
{
    return _class == other_class;
}


template<class C>
reflect::Instance<C> reflect::Class<C>::get_instance()
{
    return Instance<class_type>();
}


template<class C>
template<class T>
bool reflect::Class<C>::operator==(reflect::Class<T>&)
{
    return std::is_same_v<class_type, T>;
}

}
#endif //REFLECT_H_
