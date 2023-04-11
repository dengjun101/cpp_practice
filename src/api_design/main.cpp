#include <iostream>
#include <memory>

#include "auto_implicit.h"
#include "nodiscard.h"
#include "widget.h"

int main() {

    // test_explicit1("a","b"); // compile error
    // test_explicit2("a","b"); // compile error
    // test_implicit1("a","b"); // compile error
    // test_implicit2("a","b"); // compile error
    test_implicit2(implicit_string1("a"), implicit_string1("b"));
    test_auto(1,2);
    test_auto("a","b");
    test_auto(1, "a");
    test_auto("a", 1);

    // lambda need cpp23
    auto get_chinaunicom = [][[nodiscard]]()->int { return 10010; };
    get_chinaunicom(); // compiler will raise warning

    get_chinamobile(); // compiler will raise warning
    get_error_info(); // compiler will raise warning
    FDHolder{42}; // compiler will raise warning
    FDHolder h{42};
    FDHolder{};

    auto widget = make_widget1(static_cast<WidgetType>(-42));
    widget->PrintType();

    make_widget2<SliderWidget>();
    make_widget2<ButtonWidget>();

    // make_widget2<InvalidWidget>(); // compile fail

    try {
        throw_chinatelecom();
    } catch (...) {
        // catch 是无关的，不会被调用到，程序会被终止
    }
    std::cout << "this line will never executed" << std::endl;
}