#pragma once
#include <vector>
class Subject;

class Observer {
public:
    Observer() = default;
    virtual ~Observer() = default;
    virtual void Update(Subject* s) = 0;
};
