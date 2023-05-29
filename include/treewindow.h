#pragma once

#include "avl_tree.h"

#include <QPen>
#include <QWidget>

namespace NVisualisator {

class TreeWindow : public QWidget {
    Q_OBJECT

public:
    explicit TreeWindow(QWidget* parent = nullptr);
    void draw_tree(AvlTree::Node* root);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    static constexpr qreal kWidth = 640;
    static constexpr qreal kHeight = 575;
    static constexpr qreal kRadius = 13;
    static constexpr qreal kDistY = 44;
    static constexpr qreal kRootY = 20;

    static QPointF get_point(qreal left, qreal right, qreal height);

    void tree_bypass(QPainter& painter, AvlTree::Node* vertex, qreal left = 0, qreal right = kWidth,
                     qreal height = kRootY);
    void draw_vertex(QPainter& painter, AvlTree::Node* vertex, const QPointF& point);

    QPen pen_;
    AvlTree::Node* root_ = nullptr;
};

}  // namespace NVisualisator
