#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <chrono>
#include <fstream>


template<typename _Tp, typename _Up = char>
class vertex_ {
public:
  _Tp key;
  _Up label;
  int parent;
  int idx;

  vertex_(int _idx=-1, int p =-1, _Tp _key = std::numeric_limits<_Tp>::max())
  :idx(_idx)
  ,parent(p)
  ,key(_key) 
  {  }

  vertex_(const vertex_& n) {
    key = n.key;
    parent = n.parent;
    idx = n.idx;
  }

  vertex_& operator=(const vertex_& n) {
    key = n.key;
    parent = n.parent;
    idx = n.idx;

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
typedef vertex_<double> vertex;
typedef vertex* vertex_ptr;

struct vertex_dptr{
  vertex_dptr(const vertex_ptr& ptr) {
    key = ptr->key;
  }
  double key;
};


bool operator<(const vertex& n1, const vertex& n2) {
  return n1.idx < n2.idx;
}

bool operator>(const vertex_dptr& n1, const vertex_dptr& n2) {
  return n1.key > n2.key;
}

std::ostream& operator<<(std::ostream& os, const vertex& vx) {
  os << "Index: " << vx.idx
     << " parent: " << vx.parent
     << " key: " << vx.key
     << std::endl;

  return os;
}


typedef std::map<int, std::vector<edge> > edge_adjacency_list;
typedef std::map<int, std::vector<int> > vertex_adjacency_list;
typedef std::vector<vertex> vertex_list;

typedef std::priority_queue<vertex_ptr, 
          std::vector<vertex_ptr>, 
            std::greater<vertex_dptr> > min_priority_queue;


void fillAdjList(const char* file, edge_adjacency_list& eadj, vertex_adjacency_list& vAdj);




template<typename _Tp>
class BinaryHeap {
public:
  BinaryHeap();


protected:
  
};




int main(int argc, char** argv) {

  edge_adjacency_list eAdj;
  vertex_adjacency_list nAdj;

  fillAdjList(argv[1],eAdj, nAdj);

  min_priority_queue Q;
  std::vector<bool> isInQueue(nAdj.size(), true);
  vertex_list vertices(nAdj.size());

  vertices[5].key = 0;
  char c = 'a';

  for(size_t i=0; i<vertices.size(); i++) {
    vertices[i].idx = i;
    vertices[i].label = c++;
    Q.push(&vertices[i]);
  }

  std::cout <<"Bulding minimum spanning tree" << std::endl;

  while(Q.size()) {
    vertex_ptr u = Q.top();
    Q.pop();

    isInQueue[u->idx] = false;

    std::vector<int> adjList = nAdj[u->idx];
    std::vector<edge> incident = eAdj[u->idx];

    for(size_t i=0; i< adjList.size(); i++) {
      double w = std::numeric_limits<double>::max();
      int v = adjList[i];

      for(size_t j=0; j<incident.size(); j++)
        if(incident[j].head == v) w = incident[j].weight;

      std::cout << u->idx 
               << " with " << v  
               << " weight " << w 
               << " key of v " << vertices[v].key
               << std::endl;
      std::cout << "Is in Q " << isInQueue[v] 
                << " weight less? " << (w < vertices[v].key) 
                << std::endl;
  
      getchar();

      if(isInQueue[v] && w < vertices[v].key) {
        vertices[v].parent = u->idx;
        vertices[v].key = w;
      }
    }

    while(Q.size()) Q.pop();

    for(size_t i=0; i<vertices.size(); i++)
      if(isInQueue[i])
        Q.push(&vertices[i]);
  }

  for(size_t i=0; i< vertices.size(); i++) {
    int pLabel = vertices[i].parent;
    std::cout << vertices[i].label 
              << " with parent " 
              << ((pLabel>=0)?char(vertices[pLabel].label):' ') << std::endl;
  }



  return EXIT_SUCCESS;
}

void fillAdjList(const char* fileName,
                 edge_adjacency_list& eAdj, 
                 vertex_adjacency_list& vAdj) { 

  std::ifstream file(fileName);

  size_t nVertex=0, nEdges=0;
  int tailIdx=0, headIdx=0;
  double weight = 0.0;
  file >> nVertex >> nEdges;

  std::cout << "G = (V,E) with " <<nVertex << " and " << nEdges << std::endl;

  for(size_t i=0; i<nEdges; i++) {
    file >> tailIdx >> headIdx >> weight;
    eAdj[tailIdx].push_back(edge(tailIdx, headIdx, weight));
    eAdj[headIdx].push_back(edge(headIdx, tailIdx, weight));

    vAdj[tailIdx].push_back(headIdx);
    vAdj[headIdx].push_back(tailIdx);

    std::cout << "Tail " << tailIdx << " head: " << headIdx << " weight: " << weight <<std::endl;
  }


  file.close();
}

