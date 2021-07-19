#ifndef STACK_H
#define STACK_H

#include <vector>
#include <memory>
#include <string>
#include "../utilities/Publisher.h"

namespace pdCalc {

class StackEventData : public EventData
{
public:
    enum class ErrorConditions {Empty, TooFewArguments };
    explicit StackEventData(ErrorConditions e) : err_{e} {}

    static const char* Message(ErrorConditions ec);
    const char* message() const;
    ErrorConditions error() const { return err_; }

private:
    ErrorConditions err_;
};

class Stack : private Publisher
{
    class StackImpl;

public:
    static Stack& Instance();
    void push(double d, bool suppressChangeEvent = false);
    double pop(bool suppressChangeEvent = false);
    void swapTop();

    std::vector<double> getElements(size_t n) const;
    void getElements(size_t n, std::vector<double>&) const;

    using Publisher::attach;
    using Publisher::detach;

    size_t size() const;
    void clear() const;

    static const std::string StackChanged;
    static const std::string StackError;

private:
    Stack();
    ~Stack();
    Stack(const Stack&) = delete;
    Stack(const Stack&&) = delete;
    Stack& operator=(const Stack&) = delete;
    Stack& operator=(const Stack&&) = delete;

    std::unique_ptr<StackImpl> pimpl_;
};

}
#endif //STACK_H
