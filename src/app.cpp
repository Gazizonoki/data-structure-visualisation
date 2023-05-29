#include "app.h"

namespace NVisualisator {

Application::Application(int argc, char* argv[]) : q_app_(argc, argv), controller_(&tree_) {
    tree_.subscribe(window_.get_connection());
    window_.subscribe(controller_.get_connection());
}

int Application::exec() {
    return q_app_.exec();
}

}  // namespace NVisualisator
