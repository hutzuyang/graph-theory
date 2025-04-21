#include "Graph.hpp"
#include <bits/stdc++.h>

using namespace std;

void Graph::addNode(const string& id) {
    adj[id]; // 預設插入
}

void Graph::addEdge(const string& from, const string& to) {
    adj[from].insert(to);
    adj[to].insert(from);
}

const unordered_set<string>& Graph::getNeighbors(const string& id) const {
    static unordered_set<string> empty;
    auto it = adj.find(id);
    return it != adj.end() ? it->second : empty;
}

vector<string> Graph::getAllNodes() const {
    vector<string> nodes;
    for (const auto& [node, _] : adj) {
        nodes.push_back(node);
    }
    return nodes;
}

vector<pair<string, string>> Graph::getAllEdges() const {
    vector<pair<string, string>> edges;
    unordered_set<string> seen;
    for (const auto& [u, neighbors] : adj) {
        for (const auto& v : neighbors) {
            string key = u < v ? (u + "-" + v) : (v + "-" + u);
            if (!seen.count(key)) {
                edges.emplace_back(u, v);
                seen.insert(key);
            }
        }
    }
    return edges;
}

void Graph::debugPrint() const {
    cout << "[Graph]" << endl;
    for (const auto& [u, neighbors] : adj) {
        cout << u << " → ";
        for (const auto& v : neighbors) {
            cout << v << " ";
        }
        cout << endl;
    }
}

vector<string> Graph::getDominatingSet() const {
    unordered_set<string> covered;
    unordered_set<string> selected;

    while (covered.size() < adj.size()) {
        // 找最多沒被覆蓋的鄰居的節點
        string bestNode;
        size_t maxCover = 0;

        for (const auto& [u, neighbors] : adj) {
            if (covered.count(u)) continue;

            size_t uncovered = 0;
            for (const auto& v : neighbors) {
                if (!covered.count(v)) uncovered++;
            }
            uncovered += !covered.count(u); // 自己也要算

            if (uncovered > maxCover) {
                bestNode = u;
                maxCover = uncovered;
            }
        }

        selected.insert(bestNode);
        covered.insert(bestNode);
        for (const auto& v : adj.at(bestNode)) {
            covered.insert(v);
        }
    }

    return vector<string>(selected.begin(), selected.end());
}
