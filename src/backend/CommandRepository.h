#ifndef COMMAND_REPOSITORY_H
#define COMMAND_REPOSITORY_H

#include <memory>
#include <string>
#include <iostream>
#include <set>
#include "Command.h"

namespace pdCalc {

class CommandRepository
{
    class CommandRepositoryImpl;

public:
    static CommandRepository& Instance();

    void registerCommand(const std::string& name, CommandPtr c);

    CommandPtr deregisterCommand(const std::string& name);

    size_t getNumberCommands() const;

    CommandPtr allocateCommand(const std::string& name) const;

    bool hasKey(const std::string& s) const;

    std::set<std::string> getAllCommandNames() const;

    void printHelp(const std::string& command, std::ostream&) const;

    void clearAllCommands();

    CommandRepository(CommandRepository&) = delete;
    CommandRepository& operator=(const CommandRepository&) = delete;
    CommandRepository& operator=(CommandRepository&&) = delete;

private:
    CommandRepository();
    ~CommandRepository();

    std::unique_ptr<CommandRepositoryImpl> pimpl_;
};

}
#endif //COMMAND_REPOSITORY_H
