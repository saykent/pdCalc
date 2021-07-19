//
// Created by Ken You on 17/7/21.
//

#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <memory>

namespace pdCalc {

class EventData;

class Observer {
public:
    explicit Observer(const std::string& name);
    virtual ~Observer();

    void notify(std::shared_ptr<EventData>);

    const std::string name() const { return observerName_;}

private:
    virtual void notifyImpl(std::shared_ptr<EventData>) = 0;

    std::string observerName_;
};

}

#endif //OBSERVER_H
