#include "controller.h"

namespace NVisualisator {

Controller::Controller(AvlTree* tree) : tree_(tree), viewer_(get_handler()) {
}

void Controller::update_ui_handler(AvlTree::Event event, int64_t key) {
    if (event == AvlTree::Event::Add) {
        tree_->add(key);
    } else if (event == AvlTree::Event::Remove) {
        tree_->remove(key);
    } else if (event == AvlTree::Event::Find) {
        tree_->is_exists(key);
    } else if (event == AvlTree::Event::Clear) {
        tree_->clear();
    }
}

Controller::Handler Controller::get_handler() {
    return [this](AvlTree::Event event, int64_t key) -> void { update_ui_handler(event, key); };
}

Controller::Connection* Controller::get_connection() {
    return viewer_.get_connection();
}

}  // namespace NVisualisator
