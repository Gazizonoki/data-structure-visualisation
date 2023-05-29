#pragma once

#include "avl_tree.h"
#include "observer/observer.h"
#include "treewindow.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

namespace NVisualisator {
class MainWindow : public QMainWindow {
    Q_OBJECT

    using Handler = std::function<void(AvlTree::Node*, AvlTree::Event)>;
    using Connection = Observable<AvlTree::Event, int64_t>::Connection;

public:
    MainWindow();
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    virtual ~MainWindow() override;
    Observer<AvlTree::Node*, AvlTree::Event>::Connection* get_connection();
    void subscribe(Connection* conn);
    void tree_update_handler(AvlTree::Node* root, AvlTree::Event event);

private slots:
    void add_button_clicked();
    void remove_button_clicked();
    void find_button_clicked();
    void clear_button_clicked();

private:
    static constexpr auto kDelay = std::chrono::milliseconds(800);

    void sleep_for(std::chrono::milliseconds m_secs);
    Handler get_handler();
    void set_all_buttons(bool value);

    Observer<AvlTree::Node*, AvlTree::Event> viewer_;
    Observable<AvlTree::Event, int64_t> state_;
    Ui::MainWindow* ui_;
    TreeWindow window_;
    QTimer timer_;
};
}  // namespace NVisualisator
