#include <algorithm>
#include "Subject.h"
#include "Observer.h"

Subject::~Subject() {
	observers_.clear();
}

void Subject::Attach(Observer* o) {
    observers_.push_back(o);
}

void Subject::Detach(Observer* o) {
    observers_.erase(std::find(observers_.begin(), observers_.end(), o));
}

void Subject::Notify() {
    for (Observer* o : observers_) {
        o->Update(this);
    }
}
