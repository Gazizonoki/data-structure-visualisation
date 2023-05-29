#pragma once

#include "mainwindow.h"

namespace NVisualisator {

class Controller {
    using Handler = std::function<void(AvlTree::Event, int64_t)>;
    using Connection = Observable<AvlTree::Event, int64_t>::Connection;

public:
    Controller(AvlTree* tree);
    void update_ui_handler(AvlTree::Event event, int64_t key = 0);
    Connection* get_connection();

private:
    Handler get_handler();

    AvlTree* tree_;
    Observer<AvlTree::Event, int64_t> viewer_;
};

}  // namespace NVisualisator
