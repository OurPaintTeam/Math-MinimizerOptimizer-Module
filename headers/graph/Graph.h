#ifndef MINIMIZEROPTIMIZER_HEADERS_GRAPH_GRAPH_H_
#define MINIMIZEROPTIMIZER_HEADERS_GRAPH_GRAPH_H_

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>

#include "Politicians.h"
#include "GraphObjects.h"

#define GRAPH_TEMPLATE_PARAMS \
    typename VertexType, \
    typename WeightType, \
    typename DirectedPolicyType, \
    typename WeightedPolicyType


template <typename VertexType,
          typename WeightType = double,
          typename DirectedPolicyType = UndirectedPolicy,
          typename WeightedPolicyType = UnweightedPolicy>
class Graph :
        private DirectedPolicyType,
        private WeightedPolicyType {

    using EdgeType = Edge<VertexType, WeightType>;

public:
    Graph() = default;
    virtual ~Graph() = default;

    template <typename... Args>
    void addVertex(const Args&... vertex);

    bool removeVertex(const VertexType& v);

    bool addEdge(const VertexType& from, const VertexType& to, const WeightType& weight = WeightType());

    bool removeEdge(const VertexType& from, const VertexType& to);

    // TODO test and Imp
    bool setEdgeWeight(const VertexType& from, const VertexType& to, const WeightType& weight) {
        if (_vertices.find(from) != _vertices.end() && _vertices.find(to) != _vertices.end()) {
            if constexpr (!WeightedPolicyType::isWeighted) {
                throw std::invalid_argument("Cannot set weight on an unweighted graph.");
            }
            auto itFrom = _adjacencyList.find(from);

            bool found = false;
            for (auto& edge : itFrom->second) {
                if (edge.to == to) {
                    edge.weight = weight;
                    found = true;
                    break;
                }
            }
            if constexpr (!DirectedPolicyType::isDirected) {
                auto itTo = _adjacencyList.find(to);
                for (auto& edge : itTo->second) {
                    if (edge.to == from) {
                        edge.weight = weight;
                        break;
                    }
                }
            }
            return found;
        }
        return false;
    }

    // TODO test and Imp
    WeightType getEdgeWeight(const VertexType& from, const VertexType& to) const {
        if (_vertices.find(from) != _vertices.end() && _vertices.find(to) != _vertices.end()) {
            if constexpr (!WeightedPolicyType::isWeighted) {
                throw std::invalid_argument("Cannot get weight on an unweighted graph.");
            }
            auto it = _adjacencyList.find(from);
            if (it != _adjacencyList.end()) {
                for (const auto& edge : it->second) {
                    if (edge.to == to) {
                        return edge.weight;
                    }
                }
            }
            throw std::invalid_argument("EdgeType does not exist.");
        }
        throw std::invalid_argument("One or both vertices do not exist.");
    }

    // TODO test and Imp
    std::vector<EdgeType> getAllEdges() const {
        std::unordered_set<EdgeType> targetEdges;
        for (const auto& [vertex, edges] : _adjacencyList) {
            for (const auto& edge : edges) {
                targetEdges.emplace(edge);
            }
        }
        return std::vector<EdgeType>(targetEdges.begin(), targetEdges.end());
    }

    bool isDirected() const;

    bool isWeighted() const;

    bool hasVertex(const VertexType& v) const {
        return _vertices.find(v) != _vertices.end();
    }

    template <typename... Args>
    bool hasVertices(const Args&... vertex) {
        return ((_vertices.find(vertex) != _vertices.end()) && ...);
    }

    bool hasEdge(const VertexType& from, const VertexType& to) const {
        auto it = _adjacencyList.find(from);
        if (it != _adjacencyList.end()) {
            return std::any_of(it->second.begin(), it->second.end(),
                               [&](const EdgeType& e) { return e.to == to; });
        }
        return false;
    }

    std::vector<VertexType> getVertices() const {
        return std::vector<VertexType>(_vertices.begin(), _vertices.end());
    };

    // TODO test and Imp
    std::unordered_map<VertexType, std::vector<EdgeType>> getAdjacencyList() const {
        return _adjacencyList;
    }

    // TODO test and Imp
    std::vector<EdgeType> getVertexEdges(const VertexType& v) const {
        auto it = _adjacencyList.find(v);
        if (it != _adjacencyList.end()) {
            return it->second;
        }
        throw std::runtime_error("Vertex not found");
    }

    // TODO test and Imp
    void printGraph(std::ostream& os) const {
        for (const auto& [vertex, edges] : _adjacencyList) {
            os << vertex << " -> ";
            for (const auto& edge : edges) {
                os << edge.from << edge.to;
                if (WeightedPolicyType::isWeighted) {
                    os << "(" << edge.weight << ")";
                }
                os << ", ";
            }
            os << std::endl;
        }
    }

    // TODO test and Imp
    std::string printGraph() const {
        std::string str;
        for (const auto& [vertex, edges] : _adjacencyList) {
            str += vertex + " -> ";
            for (const auto& edge : edges) {
                str += edge.from + edge.to + " (" + edge.weight + ") ";
            }
            str += '\n';
        }
        return str;
    }

    std::vector<VertexType> findConnectedComponent(const VertexType& start) const {
        std::vector<VertexType> component;
        if (hasVertex(start)) {
            std::unordered_set<VertexType> visited;
            DFS(start, visited, component);
        }
        return component;
    }

    // TODO test and Imp
    Representation getRepresentation() const {}

    size_t vertexCount() const {
        return _vertices.size();
    }

    size_t edgeCount() const {
        std::unordered_set<EdgeType> targetEdges;
        for (const auto& [vertex, edges] : _adjacencyList) {
            for (const auto& edge : edges) {
                targetEdges.emplace(edge);
            }
        }
        if constexpr (DirectedPolicyType::isDirected) {
            return targetEdges.size();
        } else {
            return targetEdges.size() / 2;
        }
    }

    // TODO test and Imp
    std::vector<VertexType> traverse(const VertexType& start, SearchType type) const {}

    // TODO test and Imp
    // Является ли граф связным (для ориентированных)
    // и
    // сильно связным (для неориентированных)
    bool isConnected() const {}

    // TODO test and Imp
    // Выделить все компоненты связности (или сильной связности)
    std::vector<std::vector<VertexType>> connectedComponents() const {}

    // TODO test and Imp
    // Проверка графа на ацикличность (для ориентированных - DAG)
    bool isAcyclic() const {}

    // TODO test and Imp
    // Топологическая сортировка (актуально для ориентированного ацикличного графа)
    std::vector<VertexType> topologicalSort() const;

    // TODO test and Imp
    // желательно использовать using
    PathResult<VertexType, WeightType> dijkstra(const VertexType& start) const {}

    // TODO test and Imp
    PathResult<VertexType, WeightType> bellmanFord(const VertexType& start) const {}

    // TODO test and Imp
    // Алгоритм Флойда — Уоршелла: возвращает матрицу кратчайших путей между всеми парами вершин
    std::unordered_map<VertexType, std::unordered_map<VertexType, WeightType>> floydWarshall() const {}

    // TODO test and Imp
    std::vector<WeightType> kruskalMST() const {}

    // TODO test and Imp
    std::vector<WeightType> primMST(const VertexType& start) const {}

    // TODO test and Imp
    // Существует ли Эйлеров цикл / путь (неориентированный, ориентированный)
    bool hasEulerianPath() const {}

    // TODO test and Imp
    // Построение Эйлерова пути или цикла, если он существует
    std::vector<VertexType> eulerianPath() const {}

    // TODO test and Imp
    // Поиск гамильтонова пути (или цикла)
    // это NP полная задача
    std::vector<VertexType> hamiltonianPath() const {}

    // TODO test and Imp
    // Алгоритм Форда-Фулкерсона (Edmonds-Karp) для вычисления максимального потока
    WeightType maxFlow(const VertexType& source, const VertexType& sink) {}

    // TODO test and Imp
    // Поиск максимального паросочетания в двудольном графе (алгоритм Хопкрофта–Карпа)
    WeightType bipartiteMatching() const {}

    // TODO test and Imp
    // Транспонирования графа (актально для ориентированных графов)
    Graph transpose() const {}

    // TODO test and Imp
    // Построение дополнительного графа (для неориентированного, невзвешенного)
    Graph complement() const {}

    // TODO test and Imp
    // Выделение подграфа по множеству вершин
    Graph subGraph(std::vector<VertexType>& vertices) const {}

    // Maybe next: graph iterators

private:
    std::unordered_set<VertexType> _vertices;

    std::unordered_map<VertexType, std::vector<EdgeType>> _adjacencyList;

protected:
    void DFS(const VertexType& v, std::unordered_set<VertexType>& visited, std::vector<VertexType>& component) const {
        visited.insert(v);
        component.push_back(v);
        if (_adjacencyList.find(v) != _adjacencyList.end()) {
            for (const auto& e : _adjacencyList.at(v)) {
                if (visited.find(e.to) == visited.end()) {
                    DFS(e.to, visited, component);
                }
            }
        }
    }
};



template <GRAPH_TEMPLATE_PARAMS>
template <typename... Args>
void Graph<VertexType,WeightType,DirectedPolicyType,WeightedPolicyType>::addVertex(const Args&... vertex)
{
    (void)std::initializer_list<int>{
        (
            _vertices.insert(vertex),
            _adjacencyList.emplace(vertex, std::vector<EdgeType>{}),
            0
            )...
    };
}

template <GRAPH_TEMPLATE_PARAMS>
bool Graph<VertexType, WeightType, DirectedPolicyType, WeightedPolicyType>::removeVertex(const VertexType& v) {
    // Find the vertex in the set of vertices
    auto it = _vertices.find(v);
    if (it == _vertices.end()) {
        return false; // Vertex not found
    }

    // Remove the vertex from the set
    _vertices.erase(it);

    if (DirectedPolicyType::isDirected) {
        // For a directed graph, remove outgoing edges
        _adjacencyList.erase(v); // Remove outgoing edges

        // Remove incoming edges by iterating over all vertices
        for (auto& [vertex, edges] : _adjacencyList) {
            edges.erase(
                std::remove_if(edges.begin(), edges.end(), [&](const EdgeType& edge) { return edge.to == v; }),
                edges.end()
            );
        }
    } else {
        // For an undirected graph, remove both outgoing and incoming edges
        auto adjIt = _adjacencyList.find(v);
        if (adjIt != _adjacencyList.end()) {
            auto& vec = adjIt->second;

            // For each vertex that has an edge to 'v', remove the edge to 'v'
            for (const auto& elem : vec) {
                VertexType to = elem.to;

                auto toIt = _adjacencyList.find(to);
                if (toIt != _adjacencyList.end()) {
                    auto& vec2 = toIt->second;
                    vec2.erase(std::remove_if(
                        vec2.begin(), vec2.end(), [&](const EdgeType& edge) { return edge.to == v; }),
                        vec2.end());
                }
            }
        }

        // Remove the vertex from the adjacency list
        _adjacencyList.erase(v);
    }

    return true; // Vertex successfully removed
}

template <GRAPH_TEMPLATE_PARAMS>
bool Graph<VertexType, WeightType, DirectedPolicyType, WeightedPolicyType>::addEdge(const VertexType& from, const VertexType& to, const WeightType& weight) {
    if (_vertices.find(from) == _vertices.end() || _vertices.find(to) == _vertices.end()) {
        return false;
    }

    if constexpr (WeightedPolicyType::isWeighted) {
        if (weight == WeightType()) {
            throw std::invalid_argument("should be use weight arg in weighted graph.");
        }
    } else {
        if (weight != WeightType()) {
            throw std::invalid_argument("do not use weight arg in unweighted graph.");
        }
    }

    if constexpr (DirectedPolicyType::isDirected) {
        if (hasEdge(from, to)) {
            return false;
        }
    } else {
        if (hasEdge(from, to) || hasEdge(to, from)) {
            return false;
        }
    }

    _adjacencyList[from].emplace_back(from, to, weight);
    if constexpr (DirectedPolicyType::isDirected == false) {
        _adjacencyList[to].emplace_back(to, from, weight);
    }
    return true;
}

template <GRAPH_TEMPLATE_PARAMS>
bool Graph<VertexType, WeightType, DirectedPolicyType, WeightedPolicyType>::removeEdge(const VertexType& from, const VertexType& to) {
    if (_vertices.find(from) == _vertices.end() || _vertices.find(to) == _vertices.end()) {
        return false;
    }

    bool removed = false;

    // Remove edge from "from" to "to"
    auto& fromEdges = _adjacencyList[from];
    size_t originalSize = fromEdges.size();
    fromEdges.erase(
        std::remove_if(fromEdges.begin(), fromEdges.end(), [&](const EdgeType& edge) { return edge.to == to; }),
        fromEdges.end()
    );
    if (fromEdges.size() != originalSize) {
        removed = true;
    }

    // If the graph is undirected, also remove the edge from "to" to "from"
    if constexpr (!DirectedPolicyType::isDirected) {
        auto& toEdges = _adjacencyList[to];
        originalSize = toEdges.size();
        toEdges.erase(
            std::remove_if(toEdges.begin(), toEdges.end(), [&](const EdgeType& edge) { return edge.to == from; }),
            toEdges.end()
        );
        if (toEdges.size() != originalSize) {
            removed = true;
        }
    }

    return removed;
}

template <GRAPH_TEMPLATE_PARAMS>
bool Graph<VertexType, WeightType, DirectedPolicyType, WeightedPolicyType>::isWeighted() const {
    return WeightedPolicyType::isWeighted;
}

template <GRAPH_TEMPLATE_PARAMS>
bool Graph<VertexType, WeightType, DirectedPolicyType, WeightedPolicyType>::isDirected() const {
    return DirectedPolicyType::isDirected;
}

#endif // ! MINIMIZEROPTIMIZER_HEADERS_GRAPH_GRAPH_H_