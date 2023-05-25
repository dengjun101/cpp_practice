
#include <cstddef>
#include <iostream>
#include <memory>
#include <algorithm>

namespace NSVarTypeDict {
    struct NullParameter;
    template<size_t N, template<typename...> class TCont, typename...T>
    struct Create_ {
        using type = typename Create_<N - 1, TCont, NullParameter, T...>::type;
    };

    template<template<typename...> class TCont, typename...T>
    struct Create_<0, TCont, T...> {
        using type = TCont<T...>;
    };

    template<typename TVal, size_t N, size_t M, typename TProcessedTypes, typename...TRemainTypes>
    struct NewTupleType_;

    template<typename TVal, size_t N, size_t M, template<typename...> class TCont, typename...TModifiedTypes, typename TCurType, typename...TRemainTypes>
    struct NewTupleType_<TVal, N, M, TCont<TModifiedTypes...>, TCurType, TRemainTypes...> {
        using type = typename NewTupleType_<TVal, N, M + 1, TCont<TModifiedTypes..., TCurType>, TRemainTypes...>::type;
    };

    template<typename TVal, size_t N, template<typename...> class TCont, typename...TModifiedTypes, typename TCurType, typename...TRemainTypes>
    struct NewTupleType_<TVal, N, N, TCont<TModifiedTypes...>, TCurType, TRemainTypes...> {
        using type = TCont<TModifiedTypes..., TVal, TRemainTypes...>;
    };

    template<typename TVal, size_t TagPos, typename TCont, typename... TRemainTypes>
    using NewTupleType = typename NewTupleType_<TVal, TagPos, 0, TCont, TRemainTypes...>::type;

    template<typename TTag, typename... TParameters>
    struct Tag2ID_;

    template<typename TTag, typename TTag1, typename...TRemainTypes>
    struct Tag2ID_<TTag, TTag1, TRemainTypes...> {
        constexpr static size_t id = Tag2ID_<TTag, TRemainTypes...>::id + 1;
    };

    template<typename TTag, typename...TRemainTypes>
    struct Tag2ID_<TTag, TTag, TRemainTypes...> {
        constexpr static size_t id = 0;
    };

    template<typename TTag, typename...TParameters>
    constexpr size_t Tag2ID = Tag2ID_<TTag, TParameters...>::id;

    template<size_t ID, typename... TTypes>
    struct ID2Type_;

    template<size_t ID, typename TType, typename...TRemainTypes>
    struct ID2Type_<ID, TType, TRemainTypes...> {
        using type = typename ID2Type_<ID - 1, TRemainTypes...>::type;
    };

    template<typename TType, typename...TRemainTypes>
    struct ID2Type_<0, TType, TRemainTypes...> {
        using type = TType;
    };

    template<size_t ID, typename...TTypes>
    using ID2Type = typename ID2Type_<ID, TTypes...>::type;

}
template<typename...TParameters>
struct VarTypeDict {
    template<typename...TTypes>
    struct Values {
        Values() = default;

        Values(std::shared_ptr<void>(&&input)[sizeof...(TTypes)]) {
            for (size_t i = 0; i < sizeof...(TTypes); ++i) {
                m_tuple[i] = std::move(input[i]);
            }
        }

    public:
        template<typename TTag, typename TVal>
        auto Set(TVal &&val) &&{
            using namespace NSVarTypeDict;
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;

            using rawVal = std::decay_t<TVal>;
            rawVal *tmp = new rawVal(std::forward<TVal>(val));
            m_tuple[TagPos] = std::shared_ptr<void>(tmp, [](void *ptr) {
                rawVal *nptr = static_cast<rawVal *>(ptr);
                delete nptr;
            });

            using new_type = NewTupleType<rawVal, TagPos, Values<>, TTypes...>;
            return new_type(std::move(m_tuple));
        }

        template<typename TTag>
        const auto &Get() const {
            using namespace NSVarTypeDict;
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;
            using ThisType = ID2Type<TagPos, TTypes...>;
            return *static_cast<ThisType *>(m_tuple[TagPos].get());
        }

    private:
        std::shared_ptr<void> m_tuple[sizeof...(TTypes)];
    };

public:
    static auto Create() {
        using namespace NSVarTypeDict;
        using type = typename Create_<sizeof...(TParameters), Values>::type;

        return type{};
    }
};

using FParams = VarTypeDict<struct A, struct B, struct Weight, struct X, struct Y, struct Z>;

template<typename TIn>
float fun(const TIn &in) {
    auto a = in.template Get<A>();
    auto b = in.template Get<B>();
    auto weight = in.template Get<Weight>();

    auto x = in.template Get<X>();
    auto y = in.template Get<Y>();
    auto z = in.template Get<Z>();
    std::cout << x << std::endl << y << std::endl << z << std::endl << (typeid(z) == typeid(bool)) << std::endl;

    return a * weight + b * (1 - weight);
}

int main() {
    std::cerr << fun(FParams::Create().Set<B>(2.4f).Set<Weight>(0.1f).Set<A>(1.3f).Set<X>("string").Set<Y>(123).Set<Z>(
            false));
}