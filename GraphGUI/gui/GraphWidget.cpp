#include "GraphWidget.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr int NODE_RADIUS = 20;

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus(); 
}

void GraphWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 畫邊
    painter.setPen(QPen(Qt::black, 2));
    for (auto &edge : m_edges)
    {
        QPoint p1 = m_nodes[edge.first].position;
        QPoint p2 = m_nodes[edge.second].position;
        painter.drawLine(p1, p2);
    }

    // 畫節點
    for (const Node& node : m_nodes) {
        // 顏色根據是否在支配集合
        if (m_dominatingSet.count(node.id)) {
            painter.setBrush(Qt::red); // 支配節點
        } else {
            painter.setBrush(Qt::cyan); // 普通節點
        }
        painter.setPen(Qt::black);
        painter.drawEllipse(node.position, NODE_RADIUS, NODE_RADIUS);
        painter.drawText(node.position + QPoint(-5, 5), QString::number(node.id));
    }
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int nodeId = findNodeAtPosition(event->pos());
        if (nodeId == -1)
        {
            // 點空白區新增節點
            Node newNode{m_nextId++, event->pos()};
            m_nodes.push_back(newNode);

            // 同步到 Graph 結構
            m_graph.addNode(std::to_string(newNode.id));
            update();
        }
        else
        {
            // 點到節點 ➜ 開始拖曳建立邊
            m_dragStartNode = nodeId;
        }
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragStartNode != -1)
    {
        int endNode = findNodeAtPosition(event->pos());
        if (endNode != -1 && endNode != m_dragStartNode)
        {
            // 檢查是否已有邊
            auto e1 = std::make_pair(m_dragStartNode, endNode);
            auto e2 = std::make_pair(endNode, m_dragStartNode);
            if (std::find(m_edges.begin(), m_edges.end(), e1) == m_edges.end() &&
                std::find(m_edges.begin(), m_edges.end(), e2) == m_edges.end())
            {
                m_edges.push_back(e1);

                // 同步到 Graph 結構
                m_graph.addEdge(std::to_string(m_dragStartNode),
                                std::to_string(endNode));
            }
        }
        m_dragStartNode = -1;
        update();
    }
}

void GraphWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_D) {
        m_graph.debugPrint();
    } else if (event->key() == Qt::Key_M) {
        auto result = m_graph.getDominatingSet();
        m_dominatingSet.clear();
        for (const auto& id : result) {
            m_dominatingSet.insert(std::stoi(id));
        }
        update();
    }
    else if (event->key() == Qt::Key_E) {
        json j;
    
        // 儲存節點含位置
        json nodeArray = json::array();
        for (const auto& node : m_nodes) {
            nodeArray.push_back({
                {"id", node.id},
                {"x", node.position.x()},
                {"y", node.position.y()}
            });
        }
        j["nodes"] = nodeArray;
    
        // 儲存邊
        json edgeArray = json::array();
        for (const auto& [u, v] : m_edges) {
            edgeArray.push_back({u, v});
        }
        j["edges"] = edgeArray;
    
        // 儲存支配集合
        j["mds"] = std::vector<int>(m_dominatingSet.begin(), m_dominatingSet.end());
    
        // 寫入檔案（壓縮格式）
        std::ofstream out("graph_export.json");
        out << j.dump();  // 無縮排
        out.close();
    
        qDebug() << "Graph exported with positions.";
    }
    else if (event->key() == Qt::Key_L) {
        loadFromJson("graph_export.json");
    }
}

void GraphWidget::loadFromJson(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        qDebug() << "Cannot open file:" << QString::fromStdString(filename);
        return;
    }

    json j;
    in >> j;
    in.close();

    m_nodes.clear();
    m_edges.clear();
    m_dominatingSet.clear();
    m_graph = Graph();  // 清空圖
    m_nextId = 0;

    // 讀取節點（含位置）
    for (const auto& nodeJson : j["nodes"]) {
        Node n;
        n.id = nodeJson["id"];
        n.position = QPoint(nodeJson["x"], nodeJson["y"]);

        m_nodes.push_back(n);
        m_graph.addNode(std::to_string(n.id));
        m_nextId = std::max(m_nextId, n.id + 1);
    }

    // 讀取邊
    for (const auto& edge : j["edges"]) {
        int u = edge[0], v = edge[1];
        m_edges.emplace_back(u, v);
        m_graph.addEdge(std::to_string(u), std::to_string(v));
    }

    // 讀取支配集合
    for (int id : j["mds"]) {
        m_dominatingSet.insert(id);
    }

    update();
}

int GraphWidget::findNodeAtPosition(const QPoint &pos)
{
    for (const Node &node : m_nodes)
    {
        int dx = pos.x() - node.position.x();
        int dy = pos.y() - node.position.y();
        if (dx * dx + dy * dy <= NODE_RADIUS * NODE_RADIUS)
        {
            return node.id;
        }
    }
    return -1;
}
