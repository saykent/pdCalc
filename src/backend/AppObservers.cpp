#include <vector>
#include "AppObservers.h"
#include "../utilities/Exception.h"
#include "../ui/cli/Cli.h"

using std::ostringstream;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::vector;

namespace pdCalc {

CommandIssuedObserver::CommandIssuedObserver(CommandDispatcher& ce)
        : Observer("CommandIssued")
        , ce_(ce)
{ }

void CommandIssuedObserver::notifyImpl(std::shared_ptr<EventData> eventData)
{
    auto data = dynamic_pointer_cast<CommandData>(eventData);
    if(!data)
    {
        throw Exception("Could not convert CommandData to a command");
    }
    else
    {
        ce_.commandEntered( data->command() );
    }
}

StackUpdatedObserver::StackUpdatedObserver(UserInterface& ui)
        : Observer("StackUpdated")
        , ui_(ui)
{ }

void StackUpdatedObserver::notifyImpl(std::shared_ptr<EventData>)
{
    ui_.stackChanged();
}

}
