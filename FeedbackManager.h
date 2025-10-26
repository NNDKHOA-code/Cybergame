#ifndef FEEDBACK_MANAGER_H
#define FEEDBACK_MANAGER_H

#include "ManagerBase.h"
#include "../model/Feedback.h"

using namespace std;

static string getId_Feedback(const Feedback& x) {
    return x.getID();
}

class FeedbackManager : public ManagerBase<Feedback> {
public:
    FeedbackManager(const string& path)
        : ManagerBase<Feedback>(path) {}

    Vec<string> toRow(const Feedback& f) const override {
        return f.to_csv();
    }

    Feedback fromRow(const Vec<string>& r) const override {
        return Feedback::from_csv(r);
    }

    string IDof(const Feedback& f) const override {
        return f.getID();
    }

    string (*IDgetter() const)(const Feedback&) override {
        return getId_Feedback;
    }
};

#endif
