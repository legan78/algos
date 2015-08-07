#include "graph_algos.h"

namespace algos {
  namespace graph_algos {

    Node::Node() {
    }

    Node::Node(const Node& n) 
      : vID(n.vID) {
    }

    Node::Node(unsigned int v)
      :vID(v) {
    }

    Node& Node::operator=(const Node& n) {
      vID = n.vID;
    }

    unsigned int Node::get_index() {
      return vID;
    }

    SuperNode::SuperNode() {
    }
    
    SuperNode::SuperNode(const SuperNode& sn)
      : vID(sn.vID)
      , nodes(sn.nodes) {
    }

    SuperNode::SuperNode(unsigned int v)
      : vID(v) {
    }

    SuperNode& SuperNode::operator=(const SuperNode& sn) {
      vID = sn.vID;
      nodes = sn.nodes;
    }

    unsigned int SuperNode::get_index() {
      return vID;
    }

    void SuperNode::push(unsigned int _vID) {
      nodes.insert(_vID);
    }
    
    bool SuperNode::find(unsigned int _vID) {
      std::set<unsinged int>::iteratir it = std::find(nodes.begin(), nodes.end(), _vID);

      if(it != nodes.end()) return true;
      
      return false;
    }



    AdjacencyList::AdjacencyList() {
    
    }

    AdjacencyList::AdjacencyList(const AdjacencyList& list) {
    
    }
    
    AdjacencyList::AdjacencyList(unsigned int nNodes) {
    
    }

    AdjacencyList& AdjacencyList::operator=(const AdjacencyList& list) {
    }

    void AdjacencyList::set_edge(const Edge& _e) {

    }

    unsigned int AdjacencyList::compute_min_cut() {

    }

    const std::vector<unsigned int>& AdjacencyList::get_vertices() const {
    }

    const std::vector<Edge>& AdjacencyList::get_edges()const {
    
    }

  }
}



