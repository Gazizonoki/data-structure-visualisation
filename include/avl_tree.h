#pragma once

#include "observer/observable.h"

#include <cstdint>
#include <memory>

namespace NVisualisator {
class AvlTree {
public:
    enum class Event { Add, Remove, Find, NotFind, Clear, Unspecified };

    class Node {
    public:
        enum class State { Normal, Current, Selected, Deleted };

        Node(int64_t key) : key_(key) {
        }

        Node* get_left_son() const;
        Node* get_right_son() const;
        Node* get_parent() const;
        int64_t get_key() const;
        int64_t get_diff() const;
        State get_state() const;

        void swap_keys(Node* other);
        void swap_states(Node* other);
        std::unique_ptr<Node> swap_left_son(std::unique_ptr<Node>&& son);
        std::unique_ptr<Node> swap_right_son(std::unique_ptr<Node>&& son);

        void rotate_left();
        void rotate_right();
        void big_rotate_left();
        void big_rotate_right();
        void update_height();

        void mark_current(AvlTree& tree);
        void mark_normal(AvlTree& tree);

        void mark_selected(AvlTree& tree, bool return_to_normal = true);
        void mark_deleted(AvlTree& tree, bool return_to_normal = true);

    private:
        int64_t key_ = 0;
        int64_t height_ = 1;
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
        Node* parent_ = nullptr;
        State state_ = State::Normal;
    };
    AvlTree() = default;

    AvlTree(const AvlTree&) = delete;
    AvlTree(AvlTree&&) = delete;

    AvlTree& operator=(const AvlTree&) = delete;
    AvlTree& operator=(AvlTree&&) = delete;

    ~AvlTree() = default;

    void add(int64_t key);
    void remove(int64_t key);
    bool is_exists(int64_t key);
    size_t size() const;
    void clear();
    void subscribe(Observable<Node*, Event>::Connection* conn);
    void notify(Event event);

private:
    void fix_tree(Node* vertex);
    Node* remove_vertex(Node* vertex);

    std::unique_ptr<Node> root_;
    size_t size_ = 0;
    Observable<Node*, Event> state_;
};
}  // namespace NVisualisator
