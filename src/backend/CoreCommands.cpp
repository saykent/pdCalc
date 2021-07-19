#include <cassert>
#include <vector>
#include <cmath>
#include "CoreCommands.h"
#include "Stack.h"
#include "../utilities/Exception.h"
#include "../utilities/UserInterface.h"
#include "CommandRepository.h"

using std::vector;
using std::string;

namespace pdCalc {

namespace {

double eps = 1e-12;

bool topOfStackIsBetween(double lb, double ub)
{
    assert(lb <= ub);

    auto v = Stack::Instance().getElements(1);
    auto d = v.back();

    return d >= lb && d <= ub;
}

bool passesPowerTest(double y, double x)
{
    auto pass = true;

    if (y == 0 && x < 0)
    {
        pass = false;
    }

    double intPart;
    if (y < 0 && std::modf(x, &intPart) != 0.0)
    {
        pass = false;
    }

    return pass;
}

void registerCommand(UserInterface& ui, const string& label, CommandPtr c)
{
    try
    {
        CommandRepository::Instance().registerCommand(label, std::move(c));
    }
    catch (Exception& e)
    {
        ui.postMessage(e.what());
    }
}

}

EnterNumber::EnterNumber(double d)
: Command(), number_{d}
{
}

EnterNumber::EnterNumber(const EnterNumber& rhs)
: Command{rhs}, number_{rhs.number_}
{
}

EnterNumber::~EnterNumber()
{
}

void EnterNumber::executeImpl() noexcept
{
    Stack::Instance().push(number_);
}

void EnterNumber::undoImpl() noexcept
{
    Stack::Instance().pop();
}

EnterNumber* EnterNumber::cloneImpl() const
{
    return new EnterNumber{*this};
}

const char* EnterNumber::helpMessageImpl() const noexcept
{
    return "Adds a number to the stack";
}

SwapTopOfStack::SwapTopOfStack(const SwapTopOfStack& rhs)
: Command{rhs}
{
}

SwapTopOfStack::~SwapTopOfStack()
{
}

void SwapTopOfStack::checkPreconditionsImpl() const
{
    if (Stack::Instance().size() < 2)
        throw Exception{"Stack must have 2 elements"};
}

void SwapTopOfStack::executeImpl() noexcept
{
    Stack::Instance().swapTop();
}

void SwapTopOfStack::undoImpl() noexcept
{
    Stack::Instance().swapTop();
}

SwapTopOfStack* SwapTopOfStack::cloneImpl() const
{
    return new SwapTopOfStack{*this};
}

const char* SwapTopOfStack::helpMessageImpl() const noexcept
{
    return "Swap the top two elements of the stack";
}

DropTopOfStack::DropTopOfStack(const DropTopOfStack& rhs)
: Command{rhs}, droppedNumber_{rhs.droppedNumber_}
{
}

DropTopOfStack::~DropTopOfStack()
{
}

void DropTopOfStack::checkPreconditionsImpl() const
{
    if (Stack::Instance().size() < 1)
        throw Exception{"Stack must have 1 element"};
}

void DropTopOfStack::executeImpl() noexcept
{
    droppedNumber_ = Stack::Instance().pop();
}

void DropTopOfStack::undoImpl() noexcept
{
    Stack::Instance().push(droppedNumber_);
}

DropTopOfStack* DropTopOfStack::cloneImpl() const
{
    return new DropTopOfStack{*this};
}

const char* DropTopOfStack::helpMessageImpl() const noexcept
{
    return "Drop the top element from the stack";
}

ClearStack::ClearStack(const ClearStack& rhs)
: Command{rhs}, stack_{rhs.stack_}
{
}

ClearStack::~ClearStack()
{
}

void ClearStack::executeImpl() noexcept
{
    const auto n = Stack::Instance().size();
    if (n == 0) return;

    for (auto i = 0u; i  < n - 1; ++i)
    {
        stack_.push(Stack::Instance().pop(true));
    }
    stack_.push(Stack::Instance().pop(false));
}

void ClearStack::undoImpl() noexcept
{
    const auto n = stack_.size();
    if (n == 0) return;

    for (auto i = 0u; i < n - 1; ++i)
    {
        Stack::Instance().push(stack_.top(), true);
        stack_.pop();
    }
    Stack::Instance().push(stack_.top(), false);
    stack_.pop();
}

ClearStack* ClearStack::cloneImpl() const
{
    return new ClearStack{*this};
}

const char* ClearStack::helpMessageImpl() const noexcept
{
    return "Clear the stack";
}

Add::Add(const Add& rhs)
: BinaryCommand{rhs}
{
}

Add::~Add()
{
}

double Add::binaryOperation(double next, double top) const noexcept
{
    return next + top;
}

Add* Add::cloneImpl() const
{
    return new Add{*this};
}

const char* Add::helpMessageImpl() const noexcept
{
    return "Replace first two elements on the stack with their sum";
}

Subtract::Subtract(const Subtract& rhs)
: BinaryCommand{rhs}
{
}

Subtract::~Subtract()
{
}

double Subtract::binaryOperation(double next, double top) const noexcept
{
    return next - top;
}

Subtract* Subtract::cloneImpl() const
{
    return new Subtract{*this};
}

const char* Subtract::helpMessageImpl() const noexcept
{
    return "Replace first two elements on the stack with their difference";
}

Multiply::Multiply(const Multiply& rhs)
: BinaryCommand{rhs}
{
}

Multiply::~Multiply()
{
}

double Multiply::binaryOperation(double next, double top) const noexcept
{
    return next * top;
}

Multiply* Multiply::cloneImpl() const
{
    return new Multiply{*this};
}

const char* Multiply::helpMessageImpl() const noexcept
{
    return "Replace first two elements on the stack with their multiplication";
}

Divide::Divide(const Divide& rhs)
: BinaryCommand{rhs}
{
}

Divide::~Divide()
{
}

void Divide::checkPreconditionsImpl() const
{
    BinaryCommand::checkPreconditionsImpl();
    auto v = Stack::Instance().getElements(1);
    if (v.front() == 0. || v.front() == -0.)
        throw Exception{"Division by zero"};
}

double Divide::binaryOperation(double next, double top) const noexcept
{
    return next / top;
}

Divide* Divide::cloneImpl() const
{
    return new Divide{*this};
}

const char* Divide::helpMessageImpl() const noexcept
{
    return "Replace first two elements on the stack with their quotient";
}

Power::Power(const Power& rhs)
: BinaryCommand{rhs}
{
}

Power::~Power()
{
}

void Power::checkPreconditionsImpl() const
{
    BinaryCommand::checkPreconditionsImpl();

    auto v = Stack::Instance().getElements(2);
    if (!passesPowerTest(v[1], v[0]))
        throw Exception{"Invalid result"};
}

double Power::binaryOperation(double next, double top) const noexcept
{
    return std::pow(next, top);
}

Power* Power::cloneImpl() const
{
    return new Power{*this};
}

const char* Power::helpMessageImpl() const noexcept
{
    return "Replace first two elements on the stack, y, x, with y^x. Note, x is top of stack";
}

Root::Root(const Root& rhs)
: BinaryCommand{rhs}
{
}

Root::~Root()
{
}

void Root::checkPreconditionsImpl() const
{
    BinaryCommand::checkPreconditionsImpl();

    auto v = Stack::Instance().getElements(2);
    if (!passesPowerTest(v[1], 1. / v[0]) || v[0] == 0.0)
        throw Exception{"Invalid result"};
}

double Root::binaryOperation(double next, double top) const noexcept
{
    return std::pow(next, 1. / top);
}

Root* Root::cloneImpl() const
{
    return new Root{*this};
}

const char* Root::helpMessageImpl() const noexcept
{
    return "Replace first two elements on teh stack, y, x, with xth root of y. Note, x is top of stack";
}

Sine::Sine(const Sine& rhs)
        : UnaryCommand{rhs}
{ }

Sine::~Sine()
{ }

double Sine::unaryOperation(double top) const noexcept
{
    return std::sin(top);
}

Sine* Sine::cloneImpl() const
{
    return new Sine{*this};
}

const char* Sine::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with sin(x). x must be in radians";
}

Cosine::Cosine(const Cosine& rhs)
        : UnaryCommand{rhs}
{ }

Cosine::~Cosine()
{ }

double Cosine::unaryOperation(double top) const noexcept
{
    return std::cos(top);
}

Cosine* Cosine::cloneImpl() const
{
    return new Cosine{*this};
}

const char* Cosine::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with cos(x). x must be in radians";
}

Tangent::Tangent(const Tangent& rhs)
        : UnaryCommand{rhs}
{ }

Tangent::~Tangent()
{ }

double Tangent::unaryOperation(double top) const noexcept
{
    return std::tan(top);
}

Tangent* Tangent::cloneImpl() const
{
    return new Tangent{*this};
}

const char* Tangent::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with tan(x). x must be in radians";
}

void Tangent::checkPreconditionsImpl() const
{
    UnaryCommand::checkPreconditionsImpl();

    auto v = Stack::Instance().getElements(1);
    double d{ v.back() + M_PI / 2. };
    double r{ std::fabs(d) / std::fabs(M_PI) };
    int w{ static_cast<int>(std::floor(r + eps)) };
    r = r - w;
    if(r < eps && r > -eps)
        throw Exception{"Infinite result"};

    return;
}

Arcsine::Arcsine(const Arcsine& rhs)
        : UnaryCommand{rhs}
{ }

Arcsine::~Arcsine()
{ }

double Arcsine::unaryOperation(double top) const noexcept
{
    return std::asin(top);
}

Arcsine* Arcsine::cloneImpl() const
{
    return new Arcsine{*this};
}

const char* Arcsine::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arcsin(x). Returns result in radians";
}

void Arcsine::checkPreconditionsImpl() const
{
    UnaryCommand::checkPreconditionsImpl();

    if(!topOfStackIsBetween(-1, 1))
        throw Exception{"Invalid argument"};

    return;
}

Arccosine::Arccosine(const Arccosine& rhs)
        : UnaryCommand{rhs}
{ }

Arccosine::~Arccosine()
{ }

double Arccosine::unaryOperation(double top) const noexcept
{
    return std::acos(top);
}

Arccosine* Arccosine::cloneImpl() const
{
    return new Arccosine{*this};
}

const char* Arccosine::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arccos(x). Returns result in radians";
}

void Arccosine::checkPreconditionsImpl() const
{
    UnaryCommand::checkPreconditionsImpl();

    if(!topOfStackIsBetween(-1, 1))
        throw Exception{"Invalid argument"};

    return;
}

Arctangent::Arctangent(const Arctangent& rhs)
        : UnaryCommand{rhs}
{ }

Arctangent::~Arctangent()
{ }

double Arctangent::unaryOperation(double top) const noexcept
{
    return std::atan(top);
}

Arctangent* Arctangent::cloneImpl() const
{
    return new Arctangent{*this};
}

const char* Arctangent::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arctan(x). Returns result in radians";
}

Negate::Negate(const Negate& rhs)
        : UnaryCommand{rhs}
{ }

Negate::~Negate()
{ }

double Negate::unaryOperation(double top) const noexcept
{
    return -top;
}

Negate* Negate::cloneImpl() const
{
    return new Negate{*this};
}

const char* Negate::helpMessageImpl() const noexcept
{
    return "Negates the top number on the stack";
}

Duplicate::Duplicate(const Duplicate& rhs)
        : Command{rhs}
{ }

Duplicate::~Duplicate()
{ }

void Duplicate::checkPreconditionsImpl() const
{
    if( Stack::Instance().size() < 1 )
        throw Exception{"Stack must have 1 element"};
}

void Duplicate::executeImpl() noexcept
{
    auto v = Stack::Instance().getElements(1);
    Stack::Instance().push( v.back() );
}

void Duplicate::undoImpl() noexcept
{
    Stack::Instance().pop();
}

Duplicate* Duplicate::cloneImpl() const
{
    return new Duplicate{*this};
}

const char* Duplicate::helpMessageImpl() const noexcept
{
    return "Duplicates the top number on the stack";
}

void RegisterCoreCommands(UserInterface& ui)
{
    registerCommand( ui, "swap", MakeCommandPtr<SwapTopOfStack>() );
    registerCommand( ui, "drop", MakeCommandPtr<DropTopOfStack>() );
    registerCommand( ui, "clear", MakeCommandPtr<ClearStack>() );
    registerCommand( ui, "+", MakeCommandPtr<Add>() );
    registerCommand( ui, "-", MakeCommandPtr<Subtract>() );
    registerCommand( ui, "*", MakeCommandPtr<Multiply>() );
    registerCommand( ui, "/", MakeCommandPtr<Divide>() );
    registerCommand( ui, "pow", MakeCommandPtr<Power>() );
    registerCommand( ui, "root", MakeCommandPtr<Root>() );
    registerCommand( ui, "sin", MakeCommandPtr<Sine>() );
    registerCommand( ui, "cos", MakeCommandPtr<Cosine>() );
    registerCommand( ui, "tan", MakeCommandPtr<Tangent>() );
    registerCommand( ui, "arcsin", MakeCommandPtr<Arcsine>() );
    registerCommand( ui, "arccos", MakeCommandPtr<Arccosine>() );
    registerCommand( ui, "arctan", MakeCommandPtr<Arctangent>() );
    registerCommand( ui, "neg", MakeCommandPtr<Negate>() );
    registerCommand( ui, "dup", MakeCommandPtr<Duplicate>() );
}

}