#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include <functional>

namespace pdCalc {

class Command
{
public:
    virtual ~Command() {};

    void execute();

    void undo();

    Command* clone() const;

    const char* helpMessage() const;

    virtual void deallocate();

protected:
    Command() {}
    Command(const Command&) {}

private:
    virtual void checkPreconditionsImpl() const;

    virtual void executeImpl() noexcept = 0;
    virtual void undoImpl() noexcept = 0;
    virtual Command* cloneImpl() const = 0;

    virtual const char* helpMessageImpl() const noexcept = 0;

    Command(Command&&) = delete;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&&) = delete;
};

class BinaryCommand : public Command
{
public:
    virtual ~BinaryCommand();

protected:
    void checkPreconditionsImpl() const override;

    BinaryCommand() {}
    BinaryCommand(const BinaryCommand&);

private:
    BinaryCommand(BinaryCommand&&) = delete;
    BinaryCommand& operator=(const BinaryCommand&) = delete;
    BinaryCommand& operator=(BinaryCommand&&) = delete;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    virtual double binaryOperation(double next, double top) const noexcept = 0;

    double top_;
    double next_;
};

class UnaryCommand : public Command
{
public:
    virtual ~UnaryCommand();

protected:
    void checkPreconditionsImpl() const override;

    UnaryCommand() {}
    UnaryCommand(const UnaryCommand&);

private:
    UnaryCommand(UnaryCommand&&) = delete;
    UnaryCommand& operator=(const UnaryCommand&) = delete;
    UnaryCommand& operator=(UnaryCommand&&) = delete;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    virtual double unaryOperation(double top) const noexcept = 0;

    double top_;
};

inline void CommandDeleter(Command* p)
{
    p->deallocate();
}

using CommandPtr = std::unique_ptr<Command, decltype(&CommandDeleter)>;

template<typename T, typename ... Args>
auto MakeCommandPtr(Args&&... args)
{
    return CommandPtr{new T{std::forward<Args>(args)...}, &CommandDeleter};
}

inline auto MakeCommandPtr(Command* p)
{
    return CommandPtr{p, &CommandDeleter};
}

}

#endif //COMMAND_H
