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
        M_Setter::max<int, MAX_VALUE>
    );

    Metric metric_2(
        MIN_VALUE, 
        M_Setter::min<int, MAX_VALUE>
    );

    Metric metric_3(
        metric_1 + metric_2, 
        M_Setter::clamp<int, MIN_VALUE, MAX_VALUE>
    );

    bool res = metric_3 == MAX_VALUE;
    metric_3(metric_2 - metric_1);

    return metric_1 == MAX_VALUE and metric_2 == MIN_VALUE and res and metric_3 == MIN_VALUE;
}

//* Прямое присваивание:
inline bool test_metric_direct() {

    Metric metric(
        1.0,
        M_Setter::clamp<double, 1.0, 2.0>
    );

    #define D_T(ID, OP, RES) \
        metric OP; \
        bool res_##ID = metric == RES; \
        if ( not res_##ID ) \
            std::cout <<  #ID " FAILED -> " << metric << "\n";
    
    D_T(0, = 5, 5)

    D_T(1, += 5, 10)
    D_T(2, -= 4, 6)
    D_T(3, *= 3, 18)
    D_T(4, /= 4, 4.5)
    D_T(5, %= 2.3, 2.2)

    D_T(6, ++, 3.2)
    D_T(7, --, 2.2)
    D_T(8, --, 1.2)
    
    D_T(9, (20), 2.0)

    #undef D_T

    return res_0 and res_1 and res_2 and res_3 and res_4 and res_5 and res_6 and res_7 and res_7 and res_8 and res_9;
}

//* Сравнение float:
inline bool test_metric_comparison() {
    float var_1 = 0.1 * 3;
    float var_2 = 0.1 * 5;
    Metric metric_1 (0.1 * 3);
    Metric metric_2 (0.1 * 5);

    Metric metric_4 (metric_2);
    (++ metric_4++ ++ ++ = 2) = 8;

    Metric<int> metric_5 (metric_2);
    ++ metric_5++ ++;

    Metric<float> metric_6 (metric_5);

    Metric metric_7 (0.1f);

    float epsilon = std::numeric_limits<float>::epsilon();
    
    // Игнорируем предупреждения о сравнении float:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wliteral-range"
    
    return 
        (0.3 == 0.3)
        and (var_1 != 0.3)
        and (var_2 == 0.5)   
        and (metric_1 == 0.3)
        and (metric_2 >= 0.5)

        and (metric_2 != 0.5 + epsilon)
        and (metric_2 + epsilon == 0.5 + epsilon)
        and ((metric_2 += epsilon) == 0.5 + epsilon)

        and metric_4 == 8
        and metric_5 == 3

        and (metric_6 / 30) != 0.1
        and (metric_6 /= 30) == 0.1
        and (metric_7 *= 30) == 3

        and (metric_6 * 30 + epsilon) == 3
        and (metric_6 * 30 - epsilon) == 3
        and (metric_6 *= 30 + epsilon) == 3
        and (metric_6 += epsilon) == 3
        and (metric_6 += epsilon * 2) != 3

        and (metric_7 += epsilon) == 3
        and (metric_7 += epsilon) == 3
        and (metric_7 += epsilon * 2) != 3
    ;

    #pragma GCC diagnostic pop
}

//* Округление:
inline bool test_metric_round() {

    Metric metric_1 (0.9f, M_Setter::round<float> );
    
    bool res_1 = metric_1 == 1;
    
    metric_1(0.9f);

    res_1 = res_1 and metric_1 == 1;
    
    metric_1(metric_1);

    res_1 = res_1 and metric_1 == 1;


    Metric metric_2 (1.0f, M_Setter::round<float> );
    
    bool res_2 = metric_2 == 1;
    
    metric_2(1);

    res_2 = res_2 and metric_2 == 1;
    
    metric_2(metric_2);

    res_2 = res_2 and metric_2 == 1;
    
    metric_2(metric_1);

    res_2 = res_2 and metric_2 == 1;
    

    Metric metric_3 (0.1f, M_Setter::round<float> );

    bool res_3 = metric_3 == 0;

    metric_3(0.4);

    res_3 = res_3 and metric_3 == 0;

    metric_3(metric_3 -= 0.6); // до -1.0

    res_3 = res_3 and metric_3 == -1;

    return res_1 and res_2 and res_3;
}

//* Циклы:
inline bool test_metric_loop() {

    bool result = true;

    const int MAX_VALUE = 8;
    
    // Обычное число:
    for (int i = 0; i < 100; i++) {
        int x = i % MAX_VALUE;
        if (x >= MAX_VALUE) {
            result = false;
            goto EXIT;
        }
    }
    
    // Метрика integral:
    for (Metric i = 0; i < 100; i(i + 1)) {
        
        Metric x(
            i, 
            M_Setter::loop<int, MAX_VALUE>
        );

        if (x >= MAX_VALUE) {
            result = false;
            goto EXIT;
        }
    }
    
    // Метрика float:
    for (Metric i(0.0f); i < 95.5; i(i + 0.3)) {
        
        Metric x(
            i, 
            M_Setter::loop<float, static_cast<float>(MAX_VALUE)>
        );

        if (x >= MAX_VALUE) {
            result = false;
            goto EXIT;
        }
    }
    
    EXIT:
    return result;
}

//* Observer pattern:
inline bool test_metric_observer() {
    
    // Metric metric_listener(5, M_Setter::round<int>);

    // Metric metric_observer(9.7f, M_Setter::notify<float, int>(metric_listener));

    // std::cout << metric_listener;

    return true;
}