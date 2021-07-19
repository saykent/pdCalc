#ifndef CORECOMMANDS_H
#define CORECOMMANDS_H

#include <string>
#include <stack>
#include "Command.h"

namespace pdCalc {

class EnterNumber : public Command
{
public:
    explicit EnterNumber(double d);
    explicit EnterNumber(const EnterNumber&);
    ~EnterNumber();

private:
    EnterNumber(EnterNumber&&) = delete;
    EnterNumber& operator=(const EnterNumber&) = delete;
    EnterNumber& operator=(EnterNumber&&) = delete;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    EnterNumber* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    double number_;
};

class SwapTopOfStack : public Command
{
public:
    SwapTopOfStack() {}
    explicit SwapTopOfStack(const SwapTopOfStack&);
    ~SwapTopOfStack();

    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;

private:
    void checkPreconditionsImpl() const override;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    SwapTopOfStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class DropTopOfStack : public Command
{
public:
    DropTopOfStack() {}
    explicit DropTopOfStack(const DropTopOfStack&);
    ~DropTopOfStack();

    DropTopOfStack(DropTopOfStack&&) = delete;
    DropTopOfStack& operator=(const DropTopOfStack&) = delete;
    DropTopOfStack& operator=(DropTopOfStack&&) = delete;

private:
    void checkPreconditionsImpl() const override;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    DropTopOfStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    double droppedNumber_;
};

class ClearStack : public Command
{
public:
    ClearStack() = default;
    explicit ClearStack(const ClearStack&);
    ~ClearStack();

    ClearStack(ClearStack&&) = delete;
    ClearStack& operator=(const ClearStack&) = delete;
    ClearStack& operator=(ClearStack&&) = delete;

private:
    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    ClearStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    std::stack<double> stack_;
};

class Add : public BinaryCommand
{
public:
    Add() {}
    explicit Add(const Add& rhs);
    ~Add();

    Add(Add&&) = delete;
    Add& operator=(const Add&) = delete;
    Add& operator=(Add&&) = delete;

private:
    double binaryOperation(double next, double top) const noexcept override;

    Add* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class Subtract : public BinaryCommand
{
public:
    Subtract() {}
    explicit Subtract(const Subtract& rhs);
    ~Subtract();

    Subtract(Subtract&&) = delete;
    Subtract& operator=(const Subtract&) = delete;
    Subtract& operator=(Subtract&&) = delete;

private:
    double binaryOperation(double next, double top) const noexcept override;

    Subtract* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class Multiply : public BinaryCommand
{
public:
    Multiply() {}
    explicit Multiply(const Multiply& rhs);
    ~Multiply();

    Multiply(Multiply&&) = delete;
    Multiply& operator=(const Multiply&) = delete;
    Multiply& operator=(Multiply&&) = delete;

private:
    double binaryOperation(double next, double top) const noexcept override;

    Multiply* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class Divide : public BinaryCommand
{
public:
    Divide() {}
    explicit Divide(const Divide& rhs);
    ~Divide();

    Divide(Divide&&) = delete;
    Divide& operator=(const Divide&) = delete;
    Divide& operator=(Divide&&) = delete;

private:
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Divide* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class Power : public BinaryCommand
{
public:
    Power() {}
    explicit Power(const Power& rhs);
    ~Power();

    Power(Power&&) = delete;
    Power& operator=(const Power&) = delete;
    Power& operator=(Power&&) = delete;

private:
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Power* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

class Root : public BinaryCommand
{
public:
    Root() {}
    explicit Root(const Root& rhs);
    ~Root();

    Root(Root&&) = delete;
    Root& operator=(const Root&) = delete;
    Root& operator=(Root&&) = delete;

private:
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Root* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};
// takes the sine of a number on the stack in radians
// precondition: at least one number on the stack
class Sine : public UnaryCommand
{
public:
    Sine() { }
    explicit Sine(const Sine&);
    ~Sine();

private:
    Sine(Sine&&) = delete;
    Sine& operator=(const Sine&) = delete;
    Sine& operator=(Sine&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Sine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the cosine of a number on the stack in radians
// precondition: at least one number on the stack
class Cosine : public UnaryCommand
{
public:
    Cosine() { }
    explicit Cosine(const Cosine&);
    ~Cosine();

private:
    Cosine(Cosine&&) = delete;
    Cosine& operator=(const Cosine&) = delete;
    Cosine& operator=(Cosine&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Cosine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the tangent of a number on the stack in radians
// preconditions: 1) at least one number on the stack
//                2) number cannot be a multiple of pi/2 +/- pi
class Tangent : public UnaryCommand
{
public:
    Tangent() { }
    explicit Tangent(const Tangent&);
    ~Tangent();

private:
    Tangent(Tangent&&) = delete;
    Tangent& operator=(const Tangent&) = delete;
    Tangent& operator=(Tangent&&) = delete;

    // checks unary condition
    // throws if a multiple of pi/2 +/- pi
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Tangent* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arcsine of a number on the stack and returns radians
// preconditions: 1) at least one number on the stack
//                2) number, x, must satisfy -1 <= x <= 1
class Arcsine : public UnaryCommand
{
public:
    Arcsine() { }
    explicit Arcsine(const Arcsine&);
    ~Arcsine();

private:
    Arcsine(Arcsine&&) = delete;
    Arcsine& operator=(const Arcsine&) = delete;
    Arcsine& operator=(Arcsine&&) = delete;

    // checks unary condition
    // throws if value outside [-1, 1]
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Arcsine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arccosine of a number on the stack and returns radians
// preconditions: 1) at lesat one number on the stack
//                2) number, x, must satisfy -1 <= x <= 1
class Arccosine : public UnaryCommand
{
public:
    Arccosine() { }
    explicit Arccosine(const Arccosine&);
    ~Arccosine();

private:
    Arccosine(Arccosine&&) = delete;
    Arccosine& operator=(const Arccosine&) = delete;
    Arccosine& operator=(Arccosine&&) = delete;

    // check unary condition
    // throws if value outside [-1, 1]
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Arccosine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arctangent of a number ron the stack and returns radians
// precondition: at least one number on the stack
class Arctangent : public UnaryCommand
{
public:
    Arctangent() { }
    explicit Arctangent(const Arctangent&);
    ~Arctangent();

private:
    Arctangent(Arctangent&&) = delete;
    Arctangent& operator=(const Arctangent&) = delete;
    Arctangent& operator=(Arctangent&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Arctangent* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the top of the stack and negates it
// precondition: at least one number on the stack
class Negate : public UnaryCommand
{
public:
    Negate() { }
    explicit Negate(const Negate&);
    ~Negate();

private:
    Negate(Negate&&) = delete;
    Negate& operator=(const Negate&) = delete;
    Negate& operator=(Negate&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Negate* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the top of the stack and duplicates it
// precondition: at least one number on the stack
class Duplicate : public Command
{
public:
    Duplicate() { }
    explicit Duplicate(const Duplicate&);
    ~Duplicate();

private:
    Duplicate(Duplicate&&) = delete;
    Duplicate& operator=(const Duplicate&) = delete;
    Duplicate& operator=(Duplicate&&) = delete;

    void checkPreconditionsImpl() const override;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    Duplicate* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

};

class UserInterface;
void RegisterCoreCommands(UserInterface& ui);
}
#endif //CORECOMMANDS_H
