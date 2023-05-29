#pragma once

#include "observer.h"

namespace NVisualisator {

template <class... TData>
class Observable {
public:
    using Connection = typename Observer<TData...>::Connection;

    Observable() = default;
    Observable(const Observable&) = delete;
    Observable(Observable&&) = delete;
    Observable& operator=(const Observable&) = delete;
    Observable& operator=(Observable&&) = delete;
    ~Observable() {
        for (auto sub : subs_) {
            sub->subs_ = nullptr;
        }
    }

    void subscribe(Connection* conn) {
        conn->drop_connection();
        conn->subs_ = &subs_;
        subs_.push_back(conn);
    }

    void notify(TData... data) {
        for (auto sub : subs_) {
            sub->on_notify(data...);
        }
    }

private:
    std::vector<Connection*> subs_;
};
}  // namespace NVisualisator
