#include <cmath>
#include <concepts>
#include <source_location>
#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>

#pragma once


template <typename T>
concept is_numeric = std::integral<T> || std::floating_point<T>;

#define DELETE_COPY_CONSTR(NAME)\
    NAME(const NAME &) = delete;\
    NAME(NAME &&) = delete;\
    NAME &operator=(const NAME &) = delete;\
    NAME &operator=(NAME &&) = delete;


inline void INIT_ERROR(const std::source_location & loc, const std::string & message) {
    throw std::runtime_error(
        "\n\t" + message + " at " +
        std::string(loc.file_name()) + " :: line " +
        std::to_string(loc.line()) + " inside `" +
        loc.function_name() + "`"
    );
}


template<is_numeric T1, is_numeric T2> T1 M_calc_remainder(T1 arg_1, T2 arg_2) {
    unsigned int prts = arg_1 / arg_2;
    return arg_1 - prts * arg_2;
}

template <typename T> class Metric;


template <typename T> 
struct _M_SetterData {
    Metric<T>& metric;
    T value;
};

#define SETTER(NAME, ...) template<__VA_OPT__(__VA_ARGS__)> constexpr static void NAME (_M_SetterData<T> data)

class M_Setter {

    public:

    DELETE_COPY_CONSTR(M_Setter)
    
    SETTER(apply, typename T) {
        data.metric =data.value ;
    }
    

    //* Числовые операции (только is_numeric):

    SETTER(max, is_numeric T, T MAX_VALUE) {
        data.metric = std::max( data.value, MAX_VALUE ) ;
    }
    
    SETTER(min, is_numeric T, T MIN_VALUE) {
        data.metric = std::min( data.value, MIN_VALUE ) ;
    }
    
    SETTER(clamp, is_numeric T, T MIN_VALUE, T MAX_VALUE) {
        data.metric = std::clamp( data.value, MIN_VALUE, MAX_VALUE ) ;
    }

    SETTER(loop, is_numeric T, T MAX_VALUE) {
        data.metric = M_calc_remainder<T, T>(data.value, MAX_VALUE);
    }


    
    //* Округление (только floating_point):

    SETTER(round, std::floating_point T) {
        data.metric = std::round( data.value ) ;
    }
    
    SETTER(floor, std::floating_point T) {
        data.metric = std::floor( data.value ) ;
    }
    
    SETTER(ceil, std::floating_point T) {
        data.metric = std::ceil( data.value ) ;
    }
};

#undef  SETTER




template <typename T> class Metric {
private:
    
    T _value;


public:    
    
    using SETTER_P = void(*)(_M_SetterData<T>);

    SETTER_P setter;

    Metric(T value, SETTER_P setter = M_Setter::apply<T>) : setter(setter) {
        (*this)(value);
    }
    
    template <typename T2>
    Metric(Metric<T2> & other, SETTER_P setter = M_Setter::apply<T>) : Metric(static_cast<T2>(other), setter) {}


    DELETE_COPY_CONSTR(Metric)


    template <typename T2>
    T operator() (Metric<T2> other) {

        return (*this)( static_cast<T2>(other) );
    }
    
    template <typename T2>
    T operator() (T2 value) {

        setter( _M_SetterData<T> {*this, static_cast<T>(value)} );

        return _value;
    }


    operator T() const {
        return _value;
    }



    // Перегрузки для прямого присваивания:

    #define OVERLOAD(OP) template <is_numeric T2> Metric& operator OP (T2 value) { _value OP value; return *this; }
    OVERLOAD(=) OVERLOAD(+=) OVERLOAD(-=) OVERLOAD(*=) OVERLOAD(/=)
    #undef OVERLOAD
    
    template <is_numeric T2> Metric& operator %= (T2 value) {
        _value = M_calc_remainder(_value, value);
        return *this;
    }


    #define OVERLOAD(OP, ...) Metric& operator OP (__VA_ARGS__) { _value OP; return *this; }
    OVERLOAD(++) OVERLOAD(--) OVERLOAD(++, int) OVERLOAD(--, int)
    #undef OVERLOAD

    // Перегрузки для сравнения чисел с "плавающей" точкой:
    template <std::floating_point T2>
    bool operator== (T2 value) {
        
        // double и float имеют разный epsilon:
        double epsilon = (
            std::numeric_limits<T2>::epsilon() + std::numeric_limits<T>::epsilon()
        ) / 2;

        return 
            (_value >= value - epsilon and _value <= value + epsilon);
    }

    template <std::floating_point T2> bool operator != (T2 value) { return not ( (*this) == value ); }
    template <std::floating_point T2> bool operator >= (T2 value) { return _value > value or ( (*this) == value ); }
    template <std::floating_point T2> bool operator <= (T2 value) { return _value < value or ( (*this) == value ); }
};