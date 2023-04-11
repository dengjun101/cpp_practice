#include <iostream>

// 工厂函数

enum class WidgetType
{
    Slider = 0,
    Button = 1,
};

class Widget
{
public:
    Widget(WidgetType type) : type_(type) {}

    void PrintType()
    {
        switch (type_)
        {
        case WidgetType::Slider:
            std::cout << "this is a Slider" << std::endl;
            break;
        case WidgetType::Button:
            std::cout << "this is a Button" << std::endl;
            break;
        default:
            std::cout << "error type" << std::endl;
            break;
        }
    }

private:
    WidgetType type_;
};

[[nodiscard]] std::unique_ptr<Widget> make_widget1(WidgetType type)
{
    return std::make_unique<Widget>(type);
}

class BaseWidget
{
};

class SliderWidget : public BaseWidget
{
};

class ButtonWidget : public BaseWidget
{
};

class InvalidWidget
{
};

template <typename WidgetTypeT>
[[nodiscard]] WidgetTypeT make_widget2()
    requires(std::is_base_of_v<BaseWidget, WidgetTypeT>)
{
    return WidgetTypeT();
}