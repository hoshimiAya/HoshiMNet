#ifndef HOSHIMNET_BASE_OBSERVER_H_
#define HOSHIMNET_BASE_OBSERVER_H_

#include <list>

#include "Singleton.h"

namespace hoshiMNet
{
namespace base
{

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

class Observer : public Singleton<Observer>
{
friend class Singleton<Observer>;

public:
    void attach(IObserver* observer)
    {
        observers_.push_back(observer);
    }

    void detach(IObserver* observer)
    {
        observers_.remove(observer);
    }

    void notify()
    {
        for (auto& observer : observers_)
        {
            observer->update();
        }
    }

private:
    Observer() {}
    ~Observer() {}

private:
    std::list<IObserver*> observers_;
};

/* test
class IObserverTest : public IObserver
{
public:
    IObserverTest() { Observer::instance().attach(this); }
    ~IObserverTest() { Observer::instance().detach(this); }

    virtual void update() override {}
};
*/

} // namespace base
} // namespace hoshiMNet

#endif // HOSHIMNET_BASE_OBSERVER_H_