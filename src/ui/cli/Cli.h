#ifndef CLI_H
#define CLI_H

#include <string>
#include <memory>
#include <iostream>
#include "../utilities/UserInterface.h"

namespace pdCalc {

class Cli : public UserInterface
{
    class CliImpl;

public:
    Cli(std::istream&, std::ostream&);
    ~Cli();

    void execute(bool suppressStartUpMessage=false, bool echo=false);

    Cli(const Cli&) = delete;
    Cli(Cli&&) = delete;
    Cli& operator=(const Cli&) = delete;
    Cli& operator=(Cli&&) = delete;

private:
    void postMessage(const std::string& m) override;

    void stackChanged() override;

    std::unique_ptr<CliImpl> pimpl_;
};

}
#endif //CLI_H
