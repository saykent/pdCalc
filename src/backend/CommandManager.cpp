#include <stack>
#include <vector>
#include <list>
#include "CommandManager.h"
#include "Command.h"

using std::unique_ptr;
using std::make_unique;
using std::stack;
using std::vector;
using std::list;

namespace pdCalc {

class CommandManager::CommandManagerImpl
{
public:
    virtual ~CommandManagerImpl() {}

    virtual size_t getUndoSize() const = 0;
    virtual size_t getRedoSize() const = 0;

    virtual void executeCommand(CommandPtr c) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class CommandManager::UndoRedoStackStrategy : public CommandManager::CommandManagerImpl
{
public:
    size_t getUndoSize() const override { return undoStack_.size(); }
    size_t getRedoSize() const override { return redoStack_.size(); }

    void executeCommand(CommandPtr c) override;
    void undo() override;
    void redo() override;

private:
    void flushStack(stack<CommandPtr>& st);

    stack<CommandPtr> undoStack_;
    stack<CommandPtr> redoStack_;
};

void CommandManager::UndoRedoStackStrategy::executeCommand(CommandPtr c)
{
    c->execute();

    undoStack_.push(std::move(c));
    flushStack(redoStack_);
}

void CommandManager::UndoRedoStackStrategy::undo()
{
    if (getUndoSize() == 0) return;

    auto& c = undoStack_.top();
    c->undo();

    redoStack_.push(std::move(c));
    undoStack_.pop();
}

void CommandManager::UndoRedoStackStrategy::redo()
{
    if (getRedoSize() == 0) return;

    auto& c = redoStack_.top();
    c->execute();

    undoStack_.push(std::move(c));
    redoStack_.pop();
}

void CommandManager::UndoRedoStackStrategy::flushStack(stack<CommandPtr>& st)
{
    while (!st.empty())
    {
        st.pop();
    }
}

class CommandManager::UndoRedoListStrategyVector : public CommandManager::CommandManagerImpl
{
public:
    UndoRedoListStrategyVector() : cur_{-1}, undoSize_{0}, redoSize_{0} {}

    size_t getUndoSize() const override { return undoSize_; }
    size_t getRedoSize() const override { return redoSize_; }

    void executeCommand(CommandPtr c) override;
    void undo() override;
    void redo() override;

private:
    void flush();

    int cur_;
    size_t undoSize_;
    size_t redoSize_;
    vector<CommandPtr> undoRedoList_;
};

void CommandManager::UndoRedoListStrategyVector::executeCommand(CommandPtr c)
{
    c->execute();

    flush();
    undoRedoList_.emplace_back(std::move(c));
    cur_ = undoRedoList_.size() - 1;
    ++undoSize_;
    redoSize_ = 0;
}

void CommandManager::UndoRedoListStrategyVector::undo()
{
    if (getUndoSize() == 0) return;

    undoRedoList_[cur_]->undo();
    --cur_;
    --undoSize_;
    ++redoSize_;
}

void CommandManager::UndoRedoListStrategyVector::redo()
{
    if (getRedoSize() == 0) return;

    ++cur_;
    undoRedoList_[cur_]->execute();
    --redoSize_;
    ++undoSize_;
}

void CommandManager::UndoRedoListStrategyVector::flush()
{
    if (!undoRedoList_.empty()) undoRedoList_.erase(undoRedoList_.begin() + cur_ + 1, undoRedoList_.end());
}

class CommandManager::UndoRedoListStrategy : public CommandManager::CommandManagerImpl
{
public:
    UndoRedoListStrategy() : undoSize_{0}, redoSize_{0}, cur_{undoRedoList_.end()} {}

    size_t getUndoSize() const override { return undoSize_; }
    size_t getRedoSize() const override { return redoSize_; }

    void executeCommand(CommandPtr c) override;
    void undo() override;
    void redo() override;

private:
    void flush();

    size_t undoSize_;
    size_t redoSize_;
    list<CommandPtr> undoRedoList_;
    list<CommandPtr>::iterator cur_;
};

void CommandManager::UndoRedoListStrategy::executeCommand(CommandPtr c)
{
    c->execute();

    flush();
    undoRedoList_.emplace_back(std::move(c));
    ++undoSize_;
    redoSize_ = 0;
    cur_ = undoRedoList_.end();
    --cur_;
}

void CommandManager::UndoRedoListStrategy::undo()
{
    if (undoSize_ == 0) return;

    --undoSize_;
    ++redoSize_;
    (*cur_)->undo();
    --cur_;
}

void CommandManager::UndoRedoListStrategy::redo()
{
    if (redoSize_ == 0) return;

    --redoSize_;
    ++undoSize_;
    ++cur_;
    (*cur_)->execute();
}

void CommandManager::UndoRedoListStrategy::flush()
{
    auto it = cur_;
    ++it;
    if (!undoRedoList_.empty()) undoRedoList_.erase(it, undoRedoList_.end());
}

CommandManager::CommandManager(CommandManager::UndoRedoStrategy st)
{
    switch (st)
    {
        case UndoRedoStrategy::ListStrategy:
            pimpl_ = make_unique<UndoRedoListStrategy>();
            break;

        case UndoRedoStrategy::StackStrategy:
            pimpl_ = make_unique<UndoRedoStackStrategy>();
            break;

        case UndoRedoStrategy::ListStrategyVector:
            pimpl_ = make_unique<UndoRedoListStrategyVector>();
            break;
    }
}

CommandManager::~CommandManager()
{
}

size_t CommandManager::getUndoSize() const
{
    return pimpl_->getUndoSize();
}

size_t CommandManager::getRedoSize() const
{
    return pimpl_->getRedoSize();
}

void CommandManager::executeCommand(CommandPtr c)
{
    pimpl_->executeCommand(std::move(c));
}

void CommandManager::undo()
{
    pimpl_->undo();
}

void CommandManager::redo()
{
    pimpl_->redo();
}

}
