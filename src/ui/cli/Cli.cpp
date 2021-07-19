#include <vector>
#include <sstream>
#include "Cli.h"
#include "../utilities/Tokenizer.h"
#include "../backend/Stack.h"

#define PDCALC_VERSION "1.0.0"

using std::vector;
using std::string;
using std::istream;
using std::ostream;
using std::endl;
using std::ostringstream;

namespace pdCalc {

class Cli::CliImpl
{
public:
    CliImpl(Cli&, istream& in, ostream& out);
    void postMessage(const string& m);
    void execute(bool suppressStartupMessage, bool echo);
    void stackChanged();

private:
    void startupMessage();

    Cli& parent_;
    istream& in_;
    ostream& out_;
};

Cli::CliImpl::CliImpl(Cli& p, istream& in, ostream& out)
: parent_{p}, in_(in), out_{out}
{
}

void Cli::CliImpl::postMessage(const string& m)
{
    out_ << m << endl;
}

void Cli::CliImpl::execute(bool suppressStartupMessage, bool echo)
{
    if (!suppressStartupMessage) startupMessage();

    for (string line; std::getline(in_, line, '\n'); )
    {
        Tokenizer tokenizer{line};
        for (const auto& i : tokenizer)
        {
            if (echo) out_ << i << endl;
            if (i == "exit" || i == "quit")
            {
                return;
            }
            else
            {
                parent_.raise(UserInterface::CommandEntered, std::make_unique<CommandData>(i));
            }
        }
    }

}

void Cli::CliImpl::stackChanged()
{
    unsigned int nElements{4};
    auto v = Stack::Instance().getElements(nElements);
    ostringstream oss;
    oss.precision(12);
    size_t size = Stack::Instance().size();
    oss << "\n";
    if(size == 0)
        oss << "Stack currently empty.\n";
    else if(size == 1)
        oss << "Top element of stack (size = " << size << "):\n";
    else if(size > 1 && size <= nElements)
        oss << "Top " << size << " elements of stack (size = " << size << "):\n";
    else
        oss << "Top " << nElements << " elements of stack (size = " << size << "):\n";

    size_t j{ v.size() };
    for(auto i = v.rbegin(); i != v.rend(); ++i)
    {
        oss << j << ":\t" << *i << "\n";
        --j;
    }

    postMessage( oss.str() );
}

void Cli::CliImpl::startupMessage()
{
    out_ << "pdCalc v. " << PDCALC_VERSION << ", an RPN calculator\n"
         << "type 'help' for a list of commnads\n"
         << "'exit' to end program\n" << endl;
}

Cli::Cli(istream& in, ostream& out)
{
    pimpl_ = std::make_unique<CliImpl>(*this, in, out);
}

Cli::~Cli()
{
}

void Cli::execute(bool suppressStartUpMessage, bool echo)
{
    pimpl_->execute(suppressStartUpMessage, echo);
}

void Cli::postMessage(const string& m)
{
    pimpl_->postMessage(m);
}

void Cli::stackChanged()
{
    pimpl_->stackChanged();
}

}

