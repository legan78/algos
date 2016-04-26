#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <chrono>


template<typename _Tp>
class node_ {
public:
  _Tp key;
  int parent;
  int nodeIdx;

  node_(int idx=-1, int p =-1, _Tp _key = std::numeric_limits<_Tp>::max())
  :nodeIdx(idx)
  ,parent(p)
  ,key(_key) 
  {  }

  node_(const node_& n) {
    key = n.key;
    parent = n.parent;
    nodeIdx = n.nodeIdx;
  }

  node_& operator=(const node_& n) {
    key = n.key;
    parent = n.parent;
    nodeIdx = n.nodeIdx;

    return *this;
  }
};

template<typename _Tp=int, typename _Up=double>
class edge_ {
public:
  _Tp head;
  _Tp tail;
  _Up weight;

  edge_(_Tp t = -1, _Tp h = -1, _Up w = 0)
  :head(h), tail(t), weight(w) 
  { }

  edge_(const edge_& e)
  :head(e.head), tail(e.tail), weight(e.weight)
  { }

  edge_& operator=(const edge_& e) {
    head=e.head;
    tail=e.tail;
    weight = e.weight;
    return *this;
  }
};

typedef edge_<int, double> edge;
typedef node_<double> node;

bool operator<(const node& n1, const node& n2) {
  return n1.nodeIdx < n2.nodeIdx;
}

bool operator>(const node& n1, const node& n2) {
  return n1.key > n2.key;
}


typedef std::map<int, std::vector<edge> > edge_adjacency_list;
typedef std::map<int, std::vector<node> > vertex_adjavency_list;
typedef std::vector<node> node_list;

typedef std::priority_queue<node, 
          std::vector<node>, 
            std::greater<node> > min_priority_queue;


int main(int argc, char** argv) {

  edge_adjacency_list eAdj;
  vertex_adjavency_list nAdj;
  vertex_list nodes;

  fillAdjList(eAdj);



  return EXIT_SUCCESS;
}

void fillAdjList(edge_adjacency_list& eadj) {
  std::vector<edge> adj0, adj1, adj2, adj3, adj4, adj5, adj6, adj7;

  adj0.push_back(edge(0,1,4));
  adj0.push_back(edge(0,7,8));
  adj1.push_back(edge(1,2,8));
  adj1.push_back(edge(1,7,11));
  adj2.push_back(edge(2,3,7));
  adj2.push_back(edge(2,5,4));
  adj2.push_back(edge(2,8,2));
  adj3.push_back(edge(3,4,9));
  adj3.push_back(edge(3,5,14));
  adj4.push_back(edge(4,5,10));
  adj5.push_back(edge(5,6,2));
  adj6.push_back(edge(6,8,6));
  adj6.push_back(edge(6,7,1));
  adj7.push_back(edge(7,8,7));

  eadj[0] = adj0;
  eadj[1] = adj1;
  eadj[2] = adj2;
  eadj[3] = adj3;
  eadj[4] = adj4;
  eadj[5] = adj5;
  eadj[6] = adj6;
  eadj[7] = adj7;

}

