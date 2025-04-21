#pragma once
#include <QWidget>
#include <vector>
#include <QString>
#include <QPoint>

#include "../graph/Graph.hpp"

struct Node {
    int id;
    QPoint position;
};

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    std::vector<Node> m_nodes;
    std::vector<std::pair<int, int>> m_edges;  // <節點id1, 節點id2>
    int m_nextId = 0;
    int m_dragStartNode = -1;

    Graph m_graph;  // ← 圖資料同步對象
    std::unordered_set<int> m_dominatingSet;  // MDS 結果

    void loadFromJson(const std::string& filename);
    int findNodeAtPosition(const QPoint& pos); // 傳回 node id 或 -1
};
