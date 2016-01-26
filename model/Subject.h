#pragma once
#include <vector>
class Observer;

class Subject {
public:
    Subject() = default;
    virtual ~Subject();
    void Attach(Observer* o);
    void Detach(Observer* o);
    void Notify();

private:
    std::vector<Observer*> observers_;
};
