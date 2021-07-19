#include <iostream>
#include <memory>
#include "src/ui/cli/Cli.h"
#include "src/backend/CommandDispatcher.h"
#include "src/backend/Stack.h"
#include "src/backend/CoreCommands.h"
#include "src/backend/AppObservers.h"

using namespace pdCalc;

using std::cout;
using std::endl;
using std::make_unique;

int main() {
    Cli cli{std::cin, std::cout};

    CommandDispatcher ce{cli};

    RegisterCoreCommands(cli);

    cli.attach(UserInterface::CommandEntered, make_unique<CommandIssuedObserver>(ce) );

    Stack::Instance().attach(Stack::StackChanged, make_unique<StackUpdatedObserver>(cli) );

    cli.execute();

    return 0;
}
