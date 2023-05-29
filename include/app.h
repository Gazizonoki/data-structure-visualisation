#pragma once

#include "controller.h"

#include <QApplication>

namespace NVisualisator {
class Application {
public:
    Application(int argc, char* argv[]);
    int exec();

private:
    QApplication q_app_;
    AvlTree tree_;
    Controller controller_;
    MainWindow window_;
};
}  // namespace NVisualisation
