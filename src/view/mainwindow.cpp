#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace NVisualisator {

MainWindow::MainWindow()
    : QMainWindow(nullptr), viewer_(get_handler()), ui_(new Ui::MainWindow), window_(this) {
    ui_->setupUi(this);
    connect(ui_->add_button, &QPushButton::clicked, this, &MainWindow::add_button_clicked);
    connect(ui_->remove_button, &QPushButton::clicked, this, &MainWindow::remove_button_clicked);
    connect(ui_->find_button, &QPushButton::clicked, this, &MainWindow::find_button_clicked);
    connect(ui_->clear_button, &QPushButton::clicked, this, &MainWindow::clear_button_clicked);
    setPalette(QPalette(QPalette::Window, Qt::white));
    show();
}

Observable<AvlTree::Node*, AvlTree::Event>::Connection* MainWindow::get_connection() {
    return viewer_.get_connection();
}

void MainWindow::sleep_for(std::chrono::milliseconds m_secs) {
    QEventLoop loop;
    timer_.connect(&timer_, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer_.start(m_secs);
    loop.exec();
}

void MainWindow::tree_update_handler(AvlTree::Node* root, AvlTree::Event event) {
    if (event == AvlTree::Event::Add) {
        ui_->info_window->setText("Added value");
    } else if (event == AvlTree::Event::Remove) {
        ui_->info_window->setText("Removed value");
    } else if (event == AvlTree::Event::Find) {
        ui_->info_window->setText("Value found!");
    } else if (event == AvlTree::Event::NotFind) {
        ui_->info_window->setText("Value not found!");
    } else if (event == AvlTree::Event::Clear) {
        ui_->info_window->setText("Tree cleared");
    }
    window_.draw_tree(root);
    sleep_for(kDelay);
}

void MainWindow::set_all_buttons(bool value) {
    ui_->add_button->setEnabled(value);
    ui_->remove_button->setEnabled(value);
    ui_->find_button->setEnabled(value);
    ui_->clear_button->setEnabled(value);
}

void MainWindow::add_button_clicked() {
    ui_->info_window->setText("");
    bool ok = true;
    int64_t key = ui_->value_field->displayText().toLongLong(&ok);
    if (!ok) {
        ui_->info_window->setText("ERROR: invalid input");
        return;
    }
    set_all_buttons(false);
    state_.notify(AvlTree::Event::Add, key);
    set_all_buttons(true);
}

void MainWindow::remove_button_clicked() {
    ui_->info_window->setText("");
    bool ok = true;
    int64_t key = ui_->value_field->displayText().toLongLong(&ok);
    if (!ok) {
        ui_->info_window->setText("ERROR: invalid input");
        return;
    }
    set_all_buttons(false);
    state_.notify(AvlTree::Event::Remove, key);
    set_all_buttons(true);
}

void MainWindow::find_button_clicked() {
    ui_->info_window->setText("");
    bool ok = true;
    int64_t key = ui_->value_field->displayText().toLongLong(&ok);
    if (!ok) {
        ui_->info_window->setText("ERROR: invalid input");
        return;
    }
    set_all_buttons(false);
    state_.notify(AvlTree::Event::Find, key);
    set_all_buttons(true);
}

void MainWindow::clear_button_clicked() {
    ui_->info_window->setText("");
    set_all_buttons(false);
    state_.notify(AvlTree::Event::Clear, 0);
    set_all_buttons(true);
}

MainWindow::Handler MainWindow::get_handler() {
    return [this](AvlTree::Node* root, AvlTree::Event event) -> void {
        tree_update_handler(root, event);
    };
}

void MainWindow::subscribe(MainWindow::Connection* conn) {
    state_.subscribe(conn);
}

MainWindow::~MainWindow() {
    delete ui_;
}
}  // namespace NVisualisator
