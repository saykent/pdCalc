#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <memory>
#include "Command.h"

namespace pdCalc {

class CommandManager
{
    class CommandManagerImpl;
    class UndoRedoStackStrategy;
    class UndoRedoListStrategyVector;
    class UndoRedoListStrategy;

public:
    enum class UndoRedoStrategy { ListStrategy, StackStrategy, ListStrategyVector };

    explicit CommandManager(UndoRedoStrategy st = UndoRedoStrategy::StackStrategy);
    ~CommandManager();

    size_t getUndoSize() const;
    size_t getRedoSize() const;

    void executeCommand(CommandPtr c);

    void undo();

    void redo();

    CommandManager(CommandManager&) = delete;
    CommandManager(CommandManager&&) = delete;
    CommandManager& operator=(CommandManager&) = delete;
    CommandManager& operator=(CommandManager&&) = delete;

private:
    std::unique_ptr<CommandManagerImpl> pimpl_;
};

}
#endif //COMMAND_MANAGER_H
