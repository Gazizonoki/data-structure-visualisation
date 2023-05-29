#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace NVisualisator {

template <class... TData>
class Observable;

template <class... TData>
class Observer {
    using Handler = std::function<void(TData...)>;

public:
    class Connection {
        friend class Observable<TData...>;
    public:
        explicit Connection(Observer<TData...>* obs) : obs_(obs) {}

        Connection(const Connection&) = delete;
        Connection(Connection&&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection& operator=(Connection&&) = delete;
        ~Connection() {
            drop_connection();
        }

        void on_notify(TData... data) {
            obs_->on_notify(data...);
        }

        void drop_connection() {
            if (subs_) {
                auto it = std::find(subs_->begin(), subs_->end(), this);
                if (it != subs_->end()) {
                    subs_->erase(it);
                }
                subs_ = nullptr;
            }
        }

    private:
        Observer<TData...>* obs_;
        std::vector<Connection*>* subs_ = nullptr;
    };

    explicit Observer(Handler&& handler) : conn_(this), handler_(handler) {}
    void unsubscribe() {
        conn_.drop_connection();
    }

    Connection* get_connection() {
        return &conn_;
    }

    void on_notify(TData... data) {
        handler_(data...);
    }

private:
    Connection conn_;
    Handler handler_;
};
}  // namespace NVisualisator
