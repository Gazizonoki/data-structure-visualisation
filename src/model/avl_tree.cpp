#include "avl_tree.h"

#include <algorithm>
#include <iostream>

namespace NVisualisator {

AvlTree::Node* AvlTree::Node::get_left_son() const {
    return left_.get();
}

AvlTree::Node* AvlTree::Node::get_right_son() const {
    return right_.get();
}

AvlTree::Node* AvlTree::Node::get_parent() const {
    return parent_;
}

int64_t AvlTree::Node::get_key() const {
    return key_;
}

int64_t AvlTree::Node::get_diff() const {
    int64_t diff = 0;
    if (left_) {
        diff += left_->height_;
    }
    if (right_) {
        diff -= right_->height_;
    }
    return diff;
}

void AvlTree::Node::swap_keys(Node* other) {
    std::swap(key_, other->key_);
}

void AvlTree::Node::swap_states(Node* other) {
    std::swap(state_, other->state_);
}

void AvlTree::Node::update_height() {
    height_ = 1;
    if (left_) {
        height_ = left_->height_ + 1;
    }
    if (right_) {
        height_ = std::max(height_, right_->height_ + 1);
    }
}

std::unique_ptr<AvlTree::Node> AvlTree::Node::swap_left_son(std::unique_ptr<Node>&& son) {
    auto left_son = std::move(left_);
    if (son) {
        son->parent_ = this;
    }
    left_ = std::move(son);
    update_height();
    if (left_son) {
        left_son->parent_ = nullptr;
    }
    return left_son;
}

std::unique_ptr<AvlTree::Node> AvlTree::Node::swap_right_son(std::unique_ptr<Node>&& son) {
    auto right_son = std::move(right_);
    if (son) {
        son->parent_ = this;
    }
    right_ = std::move(son);
    update_height();
    if (right_son) {
        right_son->parent_ = nullptr;
    }
    return right_son;
}

void AvlTree::Node::rotate_left() {
    if (!right_) {
        std::terminate();
    }
    swap_keys(right_.get());
    auto left_grand_son = right_->swap_left_son(nullptr);
    auto right_grand_son = right_->swap_right_son(nullptr);
    auto left_son = swap_left_son(nullptr);
    auto right_son = swap_right_son(nullptr);
    swap_left_son(std::move(right_son));
    left_->swap_left_son(std::move(left_son));
    left_->swap_right_son(std::move(left_grand_son));
    swap_right_son(std::move(right_grand_son));
}

void AvlTree::Node::rotate_right() {
    if (!left_) {
        std::terminate();
    }
    swap_keys(left_.get());
    auto left_grand_son = left_->swap_left_son(nullptr);
    auto right_grand_son = left_->swap_right_son(nullptr);
    auto left_son = swap_left_son(nullptr);
    auto right_son = swap_right_son(nullptr);
    swap_right_son(std::move(left_son));
    right_->swap_right_son(std::move(right_son));
    right_->swap_left_son(std::move(right_grand_son));
    swap_left_son(std::move(left_grand_son));
}

void AvlTree::Node::big_rotate_left() {
    right_->rotate_right();
    rotate_left();
}

void AvlTree::Node::big_rotate_right() {
    left_->rotate_left();
    rotate_right();
}

void AvlTree::Node::mark_current(AvlTree& tree) {
    state_ = Node::State::Current;
    tree.notify(Event::Unspecified);
    state_ = Node::State::Normal;
}

void AvlTree::Node::mark_normal(AvlTree& tree) {
    state_ = Node::State::Normal;
    tree.notify(Event::Unspecified);
}

void AvlTree::Node::mark_selected(AvlTree& tree, bool return_to_normal) {
    state_ = Node::State::Selected;
    tree.notify(Event::Unspecified);
    if (return_to_normal) {
        state_ = Node::State::Normal;
    }
}

void AvlTree::Node::mark_deleted(AvlTree& tree, bool return_to_normal) {
    state_ = Node::State::Deleted;
    tree.notify(Event::Unspecified);
    if (return_to_normal) {
        state_ = Node::State::Normal;
    }
}

AvlTree::Node::State AvlTree::Node::get_state() const {
    return state_;
}

void AvlTree::add(int64_t key) {
    if (!root_) {
        ++size_;
        root_ = std::make_unique<Node>(key);
        root_->mark_selected(*this);
        notify(Event::Add);
        return;
    }
    Node* vertex = root_.get();
    Node* prev;
    while (vertex) {
        prev = vertex;
        vertex->mark_current(*this);
        if (key == vertex->get_key()) {
            vertex->mark_selected(*this);
            notify(Event::Unspecified);
            return;
        }
        if (vertex->get_key() < key) {
            vertex = vertex->get_right_son();
        } else {
            vertex = vertex->get_left_son();
        }
    }
    ++size_;
    if (prev->get_key() < key) {
        prev->swap_right_son(std::make_unique<Node>(key));
        vertex = prev->get_right_son();
    } else {
        prev->swap_left_son(std::make_unique<Node>(key));
        vertex = prev->get_left_son();
    }
    vertex->mark_selected(*this);
    fix_tree(vertex->get_parent());
    notify(Event::Add);
}

void AvlTree::remove(int64_t key) {
    Node* vertex = root_.get();
    while (vertex && vertex->get_key() != key) {
        vertex->mark_current(*this);
        if (vertex->get_key() < key) {
            vertex = vertex->get_right_son();
        } else {
            vertex = vertex->get_left_son();
        }
    }
    if (!vertex) {
        notify(Event::Unspecified);
        return;
    }
    --size_;
    vertex->mark_current(*this);
    vertex->mark_deleted(*this, false);
    vertex = remove_vertex(vertex);
    fix_tree(vertex);
    notify(Event::Remove);
}

AvlTree::Node* AvlTree::remove_vertex(Node* vertex) {
    Node* parent = vertex->get_parent();
    if (!vertex->get_left_son() && !vertex->get_right_son()) {
        if (!parent) {
            root_ = nullptr;
            return nullptr;
        }
        if (parent->get_left_son() == vertex) {
            parent->swap_left_son(nullptr);
        } else {
            parent->swap_right_son(nullptr);
        }
        vertex = parent;
    } else if (!vertex->get_left_son() || !vertex->get_right_son()) {
        std::unique_ptr<Node> son;
        if (vertex->get_left_son()) {
            son = vertex->swap_left_son(nullptr);
        } else {
            son = vertex->swap_right_son(nullptr);
        }
        if (!parent) {
            root_ = std::move(son);
            return root_.get();
        }
        if (parent->get_left_son() == vertex) {
            vertex = son.get();
            parent->swap_left_son(std::move(son));
        } else {
            vertex = son.get();
            parent->swap_right_son(std::move(son));
        }
    } else {
        Node* next = vertex->get_right_son();
        next->mark_current(*this);
        while (next->get_left_son()) {
            next = next->get_left_son();
            next->mark_current(*this);
        }
        next->mark_selected(*this, false);
        vertex->swap_keys(next);
        vertex->swap_states(next);
        notify(Event::Unspecified);
        Node* old_vertex = vertex;
        vertex = remove_vertex(next);
        old_vertex->mark_normal(*this);
    }
    return vertex;
}

void AvlTree::fix_tree(Node* vertex) {
    while (vertex) {
        vertex->mark_current(*this);
        vertex->update_height();
        if (vertex->get_diff() == 0) {
            vertex = vertex->get_parent();
            break;
        }

        if (vertex->get_diff() == 1 || vertex->get_diff() == -1) {
            vertex = vertex->get_parent();
            continue;
        }

        if (vertex->get_diff() == -2) {
            if (vertex->get_right_son()->get_diff() <= 0) {
                vertex->rotate_left();
            } else {
                vertex->big_rotate_left();
            }
        } else {
            if (vertex->get_left_son()->get_diff() >= 0) {
                vertex->rotate_right();
            } else {
                vertex->big_rotate_right();
            }
        }
        vertex = vertex->get_parent();
    }

    while (vertex) {
        vertex->mark_current(*this);
        vertex->update_height();
        vertex = vertex->get_parent();
    }
}

bool AvlTree::is_exists(int64_t key) {
    Node* vertex = root_.get();
    while (vertex && vertex->get_key() != key) {
        vertex->mark_current(*this);
        if (vertex->get_key() < key) {
            vertex = vertex->get_right_son();
        } else {
            vertex = vertex->get_left_son();
        }
    }

    if (vertex) {
        vertex->mark_current(*this);
        vertex->mark_selected(*this);
        notify(Event::Find);
    } else {
        notify(Event::NotFind);
    }
    return vertex;
}

size_t AvlTree::size() const {
    return size_;
}

void AvlTree::clear() {
    root_ = nullptr;
    notify(Event::Clear);
}

void AvlTree::subscribe(Observable<Node*, Event>::Connection* conn) {
    state_.subscribe(conn);
}

void AvlTree::notify(Event event) {
    state_.notify(root_.get(), event);
}

}  // namespace NVisualisator
