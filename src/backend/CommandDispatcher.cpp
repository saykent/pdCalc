#include <sstream>
#include <regex>
#include <cassert>
#include <fstream>
#include <algorithm>
#include "CommandDispatcher.h"
#include "CommandRepository.h"
#include "CommandManager.h"
#include "CoreCommands.h"
#include "../utilities/Exception.h"
#include "../utilities/UserInterface.h"
#include "../utilities/Tokenizer.h"

using std::string;
using std::ostringstream;
using std::unique_ptr;
using std::set;
using std::istringstream;

namespace pdCalc {

class CommandDispatcher::CommandDispatcherImpl
{
public:
    explicit CommandDispatcherImpl(UserInterface& ui);

    void executeCommand(const string& command);

private:
    bool isNum(const string& s, double& d);
    void handleCommand(CommandPtr command);
    void printHelp() const;

    CommandManager manager_;
    UserInterface& ui_;
};

CommandDispatcher::CommandDispatcherImpl::CommandDispatcherImpl(UserInterface& ui)
: ui_{ui}
{
}

void CommandDispatcher::CommandDispatcherImpl::executeCommand(const string& command)
{
    double d;
    if (isNum(command, d))
        manager_.executeCommand(MakeCommandPtr<EnterNumber>(d));
    else if (command == "undo")
        manager_.undo();
    else if (command == "redo")
        manager_.redo();
    else if (command == "help")
        printHelp();
    else
    {
        auto c = CommandRepository::Instance().allocateCommand(command);
        if (!c)
        {
            ostringstream oss;
            oss << "Command " << command << " is not a known command";
            ui_.postMessage(oss.str());
        }
        else handleCommand(std::move(c));
    }
}

bool CommandDispatcher::CommandDispatcherImpl::isNum(const string& s, double& d)
{
    if (s == "+" || s == "-") return false;

    std::regex dpRegex("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
    bool isNumber{std::regex_match(s, dpRegex)};

    if (isNumber)
    {
        d = std::stod(s);
    }

    return isNumber;
}

void CommandDispatcher::CommandDispatcherImpl::handleCommand(CommandPtr command)
{
    try
    {
        manager_.executeCommand(std::move(command));
    }
    catch (Exception& e)
    {
        ui_.postMessage(e.what());
    }
}

void CommandDispatcher::CommandDispatcherImpl::printHelp() const
{
    ostringstream oss;
    set<string> allCommands = CommandRepository::Instance().getAllCommandNames();
    oss << "\n";
    oss << "undo: undo last operation\n"
        << "redo: redo last operation\n";

    for(auto i : allCommands)
    {
        CommandRepository::Instance().printHelp(i, oss);
        oss << "\n";
    }

    ui_.postMessage( oss.str() );
}

CommandDispatcher::CommandDispatcher(UserInterface& ui)
{
    pimpl_ = std::make_unique<CommandDispatcherImpl>(ui);
}

CommandDispatcher::~CommandDispatcher()
{
}

void CommandDispatcher::commandEntered(const string& command)
{
    pimpl_->executeCommand(command);
}

}
