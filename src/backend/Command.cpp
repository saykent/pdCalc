#include "Command.h"
#include "Stack.h"
#include "../utilities/Exception.h"

namespace pdCalc {

void Command::execute()
{
    checkPreconditionsImpl();
    executeImpl();
}

void Command::undo()
{
    undoImpl();
}

Command* Command::clone() const
{
    return cloneImpl();
}

const char* Command::helpMessage() const
{
    return helpMessageImpl();
}

void Command::deallocate()
{
    delete this;
}

void Command::checkPreconditionsImpl() const
{

}

BinaryCommand::BinaryCommand(const BinaryCommand& rhs)
: Command(rhs), top_{rhs.top_}, next_{rhs.next_}
{
}

BinaryCommand::~BinaryCommand()
{}

void BinaryCommand::checkPreconditionsImpl() const
{
    if (Stack::Instance().size() < 2)
        throw Exception{"Stack must have 2 elements"};
}

void BinaryCommand::executeImpl() noexcept
{
    top_ = Stack::Instance().pop(true);
    next_ = Stack::Instance().pop(true);
    Stack::Instance().push(binaryOperation(next_, top_));
}

void BinaryCommand::undoImpl() noexcept
{
    Stack::Instance().pop(true);
    Stack::Instance().push(next_, true);
    Stack::Instance().push(top_);
}

UnaryCommand::UnaryCommand(const UnaryCommand& rhs)
: Command(rhs), top_{rhs.top_}
{
}

UnaryCommand::~UnaryCommand()
{}

void UnaryCommand::checkPreconditionsImpl() const
{
    if (Stack::Instance().size() < 1)
        throw Exception{"Stack must have one element"};
}

void UnaryCommand::executeImpl() noexcept
{
    top_ = Stack::Instance().pop(true);
    Stack::Instance().push(unaryOperation(top_));
}

void UnaryCommand::undoImpl() noexcept
{
    Stack::Instance().pop(true);
    Stack::Instance().push(top_);
}

}
