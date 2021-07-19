#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include <string>
#include <set>
#include "Command.h"

namespace pdCalc {
class UserInterface;

class CommandDispatcher
{
    class CommandDispatcherImpl;

public:
    explicit CommandDispatcher(UserInterface& ui);
    ~CommandDispatcher();

    void commandEntered(const std::string& command);

    CommandDispatcher(const CommandDispatcher&) = delete;
    CommandDispatcher(CommandDispatcher&&) = delete;
    CommandDispatcher& operator=(const CommandDispatcher&) = delete;
    CommandDispatcher& operator=(CommandDispatcher&&) = delete;

private:
    std::unique_ptr<CommandDispatcherImpl> pimpl_;

};

}
#endif //COMMAND_DISPATCHER_H
