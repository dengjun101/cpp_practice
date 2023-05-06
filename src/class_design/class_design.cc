#include <iostream>
#include <memory>

class B {
public:
    explicit B(std::string val): val_(val) {}
    void DoSomeThing() {
        std::cout << __FUNCTION__ << "\t" << val_ << std::endl;
    }
private:
    std::string val_;
};

// A1拥有B，那么就用unique_ptr
class A1 {
public:
    A1(std::unique_ptr<B> unique_b)
    : owned_b_(std::move(unique_b)){}

    void UsingB() {
        owned_b_->DoSomeThing();
    }

    void ReplaceB(std::unique_ptr<B> b) {
        owned_b_ = std::move(b);
        // owned_b_.swap(b);
    }

private:
    std::unique_ptr<B> owned_b_;
};

// A2和其他对象共享B的生命周期，那么就用shared_ptr
class A2 {
public:
    A2(std::shared_ptr<B> shared_b)
    : shared_b_(shared_b){}
    void UsingB() {
        shared_b_->DoSomeThing();
    }

private:
    std::shared_ptr<B> shared_b_;
};

// A3依赖B，且A3的生命周期明确的比B长，则使用引用作为成员变量
class A3 {
public:
    A3(B& b):ref_b_(b) {}
    void UsingB() {
        ref_b_.DoSomeThing();
    }
private:
    B& ref_b_;
};

// A4和B没有从属关系，使用引用参数，如果是const方法，则使用const引用
class A4 {
public:
    void UsingB(B& b) {
        b.DoSomeThing();
    }
};

// A5依赖B，但是B是可选的，仅当作参数处理，根据指针是否有效处理不同逻辑
class A5 {
public:
    A5() = default;
    void UsingB(B* b) {
        if(b) {
            b->DoSomeThing();
        } else {
            std::cout << "DoOtherThing";
        }
    }

};

int main(int argc, char* argv[]) {

    // a1明确拥有b1的所有权，所以使用unique_ptr
    A1 a1(std::make_unique<B>("b1"));
    a1.UsingB();
    auto b1 = std::make_unique<B>("b1_replace");
    a1.ReplaceB(std::move(b1));
    a1.UsingB();

    // a2和a2_other共享b2_shared的所有权，使用shared_ptr
    auto b2_shared = std::make_shared<B>("b2_shared");
    A2 a2(b2_shared);
    a2.UsingB();
    A2 a2_other(b2_shared);
    a2_other.UsingB();

    // A3的生命周期明确比b3短，可以使用引用作为成员
    // 但是容易出错，不推荐
    B b3("b3");
    {
        A3 a3(b3);
        a3.UsingB();
    }

    //
    A4 a4;
    B b4("b4");
    a4.UsingB(b4);

    A5 a5;
    B b5("b5");
    // 指针是可选参数，有效指针和无效指针使用不同的逻辑
    a5.UsingB(&b5);
    a5.UsingB(nullptr);
    return 0;
}