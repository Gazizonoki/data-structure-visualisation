#include "treewindow.h"

#include <QPainter>
#include <qwt_symbol.h>
#include <qwt_text.h>

namespace NVisualisator {

TreeWindow::TreeWindow(QWidget* parent) : QWidget(parent) {
    setGeometry(0, 0, kWidth, kHeight);
    pen_.setColor(Qt::black);
    pen_.setWidth(2);
}

void TreeWindow::draw_tree(AvlTree::Node* root) {
    root_ = root;
    update();
}

void TreeWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    tree_bypass(painter, root_);
}

QPointF TreeWindow::get_point(qreal left, qreal right, qreal height) {
    return QPoint((left + right) / 2.0, height);
}

void TreeWindow::tree_bypass(QPainter& painter, AvlTree::Node* vertex, qreal left, qreal right,
                             qreal height) {
    if (!vertex) {
        return;
    }

    QPointF cur_point = get_point(left, right, height);
    if (vertex->get_left_son()) {
        painter.drawLine(cur_point,
                         get_point(left, (left + right) / 2.0 - kRadius, height + kDistY));
    }

    if (vertex->get_right_son()) {
        painter.drawLine(cur_point,
                         get_point((left + right) / 2.0 + kRadius, right, height + kDistY));
    }

    draw_vertex(painter, vertex, cur_point);
    tree_bypass(painter, vertex->get_left_son(), left, (left + right) / 2.0 - kRadius,
                height + kDistY);
    tree_bypass(painter, vertex->get_right_son(), (left + right) / 2.0 + kRadius, right,
                height + kDistY);
}

void TreeWindow::draw_vertex(QPainter& painter, AvlTree::Node* node, const QPointF& point) {
    QwtSymbol graphic_node(QwtSymbol::Ellipse);
    graphic_node.setPen(pen_);
    graphic_node.setSize(2 * kRadius);
    if (node->get_state() == AvlTree::Node::State::Normal) {
        graphic_node.setColor(Qt::lightGray);
    } else if (node->get_state() == AvlTree::Node::State::Current) {
        graphic_node.setColor(Qt::yellow);
    } else if (node->get_state() == AvlTree::Node::State::Selected) {
        graphic_node.setColor(Qt::green);
    } else if (node->get_state() == AvlTree::Node::State::Deleted) {
        graphic_node.setColor(Qt::red);
    }
    graphic_node.drawSymbol(&painter, point);
    QwtText text(QString::number(node->get_key()));
    text.draw(&painter,
              QRectF(point - QPointF(kRadius, kRadius), QSizeF(2 * kRadius, 2 * kRadius)));
}

}  // namespace NVisualisator
