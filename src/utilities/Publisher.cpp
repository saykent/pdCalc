
#include <sstream>
#include <unordered_map>
#include <set>

#include "Publisher.h"
#include "Observer.h"
#include "Exception.h"

using std::string;
using std::vector;
using std::set;
using std::ostringstream;
using std::unique_ptr;
using std::shared_ptr;

namespace pdCalc {

class Publisher::PublisherImpl
{
    using ObserverList = std::unordered_map<string, unique_ptr<Observer>>;
    using Events = std::unordered_map<string, ObserverList>;

public:
    PublisherImpl();
    ~PublisherImpl();

    void attach(const std::string& eventName, unique_ptr<Observer> observer);
    unique_ptr<Observer> detach(const string& eventName, const string& observerName);
    void notify(const string& eventName, shared_ptr<EventData> d) const;
    void registerEvent(const string& eventName);
    void registerEvents(const vector<string>& eventNames);
    set<string> listEvents() const;
    set<string> listEventObservers(const string& eventName) const;

    Events::const_iterator findCheckedEvent(const string& eventName) const;
    Events::iterator findCheckedEvent(const string& eventName);

private:
    Events events_;
};

Publisher::PublisherImpl::PublisherImpl()
{
}

Publisher::PublisherImpl::~PublisherImpl()
{
}

void Publisher::PublisherImpl::attach(const string &eventName, unique_ptr<Observer> observer)
{
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;

    auto obs = obsList.find(observer->name());
    if (obs != obsList.end())
        throw Exception("Observer already attached to publisher");

    obsList.insert(std::make_pair((observer->name()), std::move(observer)));
}

unique_ptr<Observer> Publisher::PublisherImpl::detach(const string &eventName, const string &observerName)
{
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;

    auto obs = obsList.find(observerName);
    if (obs == obsList.end())
        throw Exception("Cannot detach observer because observer not found");

    auto tmp = std::move(obs->second);
    obsList.erase(obs);
    return tmp;
}

void Publisher::PublisherImpl::notify(const string &eventName, shared_ptr<EventData> d) const
{
    auto ev = findCheckedEvent(eventName);
    const auto& obsList = ev->second;

    for (const auto& obs : obsList)
        obs.second->notify(d);
}

void Publisher::PublisherImpl::registerEvent(const string &eventName)
{
    auto i = events_.find(eventName);
    if (i != events_.end())
        throw Exception("Event already registered");

    events_[eventName] = ObserverList{};
};

void Publisher::PublisherImpl::registerEvents(const vector<string> &eventNames)
{
    for (const auto& name : eventNames)
        registerEvent(name);
}

set<string> Publisher::PublisherImpl::listEvents() const
{
    set<string> tmp;
    for (const auto& ev : events_)
        tmp.insert(ev.first);

    return tmp;
}

set<string> Publisher::PublisherImpl::listEventObservers(const string &eventName) const
{
    auto ev = findCheckedEvent(eventName);

    set<string> tmp;
    for (const auto& obs : ev->second)
        tmp.insert(obs.first);

    return tmp;
}

Publisher::PublisherImpl::Events::const_iterator Publisher::PublisherImpl::findCheckedEvent(const string &eventName) const
{
    auto ev = events_.find(eventName);
    if (ev == events_.end())
    {
        ostringstream oss;
        oss << "Publisher does not support event '" << eventName << "'";
        throw Exception{ oss.str() };
    }

    return ev;
}

Publisher::PublisherImpl::Events::iterator Publisher::PublisherImpl::findCheckedEvent(const string &eventName)
{
    auto ev = events_.find(eventName);
    if (ev == events_.end())
    {
        ostringstream oss;
        oss << "Publisher does not support event '" << eventName << "'";
        throw Exception(oss.str());
    }

    return ev;
}

Publisher::Publisher()
{
    publisherImpl_ = std::make_unique<PublisherImpl>();
}

void Publisher::attach(const string &eventName, std::unique_ptr<Observer> observer)
{
    publisherImpl_->attach(eventName, std::move(observer));
}

std::unique_ptr<Observer> Publisher::detach(const string &eventName, const string &observerName)
{
    return publisherImpl_->detach(eventName, observerName);
}

std::set<std::string> Publisher::listEvents() const
{
    return publisherImpl_->listEvents();
}

std::set<std::string> Publisher::listEventObservers(const string &eventName) const
{
    return publisherImpl_->listEventObservers(eventName);
}

Publisher::~Publisher()
{

}

void Publisher::raise(const string &eventName, std::shared_ptr<EventData> d) const
{
    publisherImpl_->notify(eventName, d);
}

void Publisher::registerEvent(const string &eventName)
{
    publisherImpl_->registerEvent(eventName);
}

void Publisher::registerEvents(const vector<std::string> &eventNames)
{
    publisherImpl_->registerEvents(eventNames);
}

EventData::~EventData()
{}

}