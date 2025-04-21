#pragma once
#include <bits/stdc++.h>

using namespace std;

class Graph {
public:
    void addNode(const string& id);
    void addEdge(const string& from, const string& to);

    const unordered_set<string>& getNeighbors(const string& id) const;
    vector<string> getAllNodes() const;
    vector<pair<string, string>> getAllEdges() const;
    void debugPrint() const;
    vector<string> getDominatingSet() const;
private:
    unordered_map<string, unordered_set<string>> adj;
};
