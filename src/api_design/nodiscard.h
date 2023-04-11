// [[nodiscard]] && noexcept

[[nodiscard("10086 can not be discard")]] int get_chinamobile()
{
    return 10086;
}

struct [[nodiscard]] ErrorType
{
};

ErrorType get_error_info()
{
    return ErrorType{};
}

struct FDHolder
{
    [[nodiscard]] FDHolder(int FD) {}

    FDHolder() {}
};

void throw_chinatelecom() noexcept
{
    // 终止程序
    throw 10000;
}