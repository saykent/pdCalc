//
// Created by Ken You on 17/7/21.
//

#include "Observer.h"
#include "Publisher.h"
//#include <iostream>

using std::shared_ptr;

namespace pdCalc {
Observer::Observer(const std::string &name)
: observerName_{name}
{ }

Observer::~Observer() {}

void Observer::notify(std::shared_ptr<EventData> d)
{
    notifyImpl(d);
}

}
