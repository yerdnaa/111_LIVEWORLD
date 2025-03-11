

#include "../metric/metric.hpp"
#include <iostream>

#pragma once

//* Типовая арифметика:
inline bool test_metric_1() {
    
    Metric metric_1(5);
    Metric metric_2(5.7);

    Metric counter(metric_1 + metric_2 - metric_2(1.1));
    
    Metric expected_result_1(metric_2);
    Metric expected_result_2(0.0);

    expected_result_1(8.5 + expected_result_1);
    expected_result_2(expected_result_1);

    return expected_result_1 == counter and expected_result_2 == counter;
}

//* Сеттеры:
inline bool test_metric_setter() {

    const int MAX_VALUE = 5;
    const int MIN_VALUE = 2;
    
    Metric metric_1(
        MIN_VALUE, 
        M_Setters::max(MAX_VALUE)
    );
    
    Metric metric_2(
        MIN_VALUE, 
        M_Setters::min(MAX_VALUE)
    );
    
    Metric metric_3(
        metric_1 + metric_2, 
        M_Setters::clamp(MIN_VALUE, MAX_VALUE)
    );

    bool res = metric_3 == MAX_VALUE;
    metric_3(metric_2 - metric_1);

    return metric_1 == MAX_VALUE and metric_2 == MIN_VALUE and res and metric_3 == MIN_VALUE;
}

//* Прямое присваивание:
inline bool test_metric_direct() {
    
    return true;

}

//* Сравнение float:
inline bool test_metric_comparison() {
    return true;
}

//* Округление:
inline bool test_metric_round() {

   return true;
}

//* Циклы:
inline bool test_metric_loop() {

    return true;
}

//* Observer pattern:
inline bool test_metric_observer() {
    
    Metric m_listener(0);

    Metric m_observer(
        5.46,
        M_Setters::notify<int>(&m_listener)
    );
    
    std::cout << m_listener << std::endl;
    std::cout << m_observer << std::endl;

    // return m_listener == 5 and m_observer == 5.6;
    return true;
}