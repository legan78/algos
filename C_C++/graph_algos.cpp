#include "graph_algos.h"
#include <fstream>
#include <sstream>
#include <chrono>

//#define DEBUG_VERSION


std::ostream& operator << ( std::ostream& _os, const algos::graph_algos::Edge& e ) {
  _os << "("
      << e.get_tail()
      << ","
      << e.get_head()
      << ")";

  return _os;
}


bool operator==(const algos::graph_algos::Edge& e1, const algos::graph_algos::Edge& e2) {
  if( (e1.get_head() == e2.get_head() &&
       e1.get_tail() == e2.get_tail()) ||
      (e1.get_head() == e2.get_tail() &&
       e1.get_tail() == e2.get_head()) ) return true;

  return false;
}


namespace algos {
  namespace graph_algos {



    Node::Node() {
    }
    Node::Node(const Node& n) 
      : vID(n.vID)
      , incidentEdges(n.incidentEdges) {
    }

    Node::Node(unsigned int v)
      :vID(v) {
    }

    Node& Node::operator=(const Node& n) {
      vID = n.vID;
      incidentEdges = n.incidentEdges;

      return *this;
    }

    bool operator<(const Node& n1, const Node& n2) {
      return (n1.get_index() < n2.get_index());
    }

    unsigned int Node::get_index() const {
      return vID;
    }

    void Node::set_incident_edge(unsigned int it) {
      incidentEdges.insert(it);
      /*	  std::cout << "Insident edge for node " 
		    << get_index() 
		    << ": " 
		    << it
		    << "  size: "
		    << incidentEdges.size()
		    << std::endl;*/
    }


    const std::set<unsigned int>& Node::get_incident_edges() const {
      return incidentEdges;
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
      
      return *this;
    }

    bool operator<(const SuperNode& sn1, const SuperNode& sn2) {
      return (sn1.get_index() < sn2.get_index());
    }

    unsigned int SuperNode::get_index()const {
      return vID;
    }

    void SuperNode::push(unsigned int _vID) {
      nodes.insert(Node(_vID));
    }

    void SuperNode::push(const std::set<Node>& nodeSet) {
      nodes.insert( nodeSet.begin(), nodeSet.end());
    }
    
    bool SuperNode::find(unsigned int _vID) {

      std::set<Node>::iterator it = nodes.begin();

      for (; it!= nodes.end(); it++)
	if (it->get_index() == _vID) return true;

      return false;
    }

    unsigned int SuperNode::find(const Edge& e) {
      unsigned int headF = (find(e.get_head()))?1:0;
      unsigned int tailF = (find(e.get_tail()))?1:0;

      return headF + tailF;
    }


    const std::set<Node>& SuperNode::get_node_set()const {
      return nodes;
    }


    unsigned int SuperNode::internal_size()const {
      return nodes.size();
    }


    void SuperNode::push(const Node& n) {
      nodes.insert(n);
    }

    Edge::Edge() {
    }

    Edge::Edge(const Edge& e)
      : head(e.head)
      , tail(e.tail)
      , id(e.id)
      , weight(e.weight) {
    }

    Edge::Edge(unsigned int _head, unsigned int _tail, unsigned int _id, unsigned int _weight) 
      : head(_head)
      , tail(_tail)
      , id(_id)
      , weight(_weight) {
    }

    Edge& Edge::operator=(const Edge& other) {
      head = other.head;
      tail = other.tail;
      id = other.id;
      weight = other.weight;

      return *this;
    }

    unsigned int Edge::get_head() const {
      return head;
    }

    unsigned int Edge::get_tail() const {
      return tail;
    }

    unsigned int Edge::get_index() const {
      return id;
    }

    unsigned int Edge::get_weight() const {
      return weight;
    }

    AdjacencyList::AdjacencyList() 
    { }

    AdjacencyList::AdjacencyList(const AdjacencyList& list)
      : vertices(list.vertices)
      , edges(list.edges) {
    }
    
    AdjacencyList::AdjacencyList(unsigned int nNodes) {
      for(unsigned int i = 0; i< nNodes; i++)
	vertices.insert(Node(i));
    }

    AdjacencyList& AdjacencyList::operator=(const AdjacencyList& list) {
      vertices = list.vertices;
      edges = list.edges;

      return *this;
    }

    AdjacencyList AdjacencyList::load_from_file(const char* fileName) {
      std::ifstream inputFile(fileName);
      std::string line;
      unsigned int nodeId, linkedId, edgeCounter = 0;

      AdjacencyList G;

      while (std::getline(inputFile, line)) {
	std::stringstream ss;
	ss.str(line.c_str());

	ss >> nodeId;
	G.vertices.insert(nodeId);

	while ( ss >> linkedId ) {
	  Edge e = Edge(nodeId, linkedId, edgeCounter++);

	  if(!G.find_edge(e)) {
	    G.vertices.insert( Node(linkedId) );
	    G.set_edge(e);
	  }
	}
      }

#ifdef DEBUG_VERSION
      std::cout << "AdjacencyList with " 
		<< G.vertices.size()
		<< " vertices and "
		<< G.edges.size()
		<< " edges."
		<< std::endl;

      std::cout << "[";
      for (unsigned int i = 0; i < G.edges.size(); i++) {
	std::cout << G.edges[i]
		  << ", ";
      }
      std::cout << "]"
		<< std::endl;

#endif

      return G;
    }



    AdjacencyList AdjacencyList::load_weighted_graph(const char* fileName, bool _offset) {

      std::ifstream inputFile(fileName);
      std::string line, chunk;
      unsigned int nodeId, linkedId, edgeCounter = 0, weight;

      unsigned int offset = (_offset)?1:0;

      AdjacencyList G;

      while (std::getline(inputFile, line)) {
	std::stringstream ss;
	ss.str(line.c_str());

	ss >> nodeId;
	G.vertices.insert(nodeId+offset);

	while ( ss >> chunk ) {
	  sscanf(chunk.c_str(), "%d,%d", &linkedId, &weight);
	  Edge e = Edge(linkedId+offset, nodeId+offset, edgeCounter++, weight);

	  //if(!G.find_edge(e)) {
	    std::cout << e << " " << weight << std::endl;
	    G.vertices.insert( Node(linkedId+offset) );
	    G.set_edge(e);
	    //}
	}
      }

      return G;
    }

    void AdjacencyList::set_edge(const Edge& _e) {
      edges.push_back(_e);

      for ( std::set<Node>::iterator it = vertices.begin(); it!=vertices.end(); it++) {

	if (_e.get_head() == it->get_index() ||
	    _e.get_tail() == it->get_index() ) { 
	  const Node& tmp = *it;
	  Node& tmp2 = const_cast<Node&>(tmp);
	  tmp2.set_incident_edge(edges.back().get_index());

	}
      }
    }


    bool AdjacencyList::find_edge(const Edge& _e) {
      for(unsigned int i = 0; i<edges.size(); i++)
	if(_e == edges[i]) return true;

      return false;
    }


    std::set<Node>::iterator AdjacencyList::find_node(unsigned int nodeId)const {
      std::set<Node>::iterator it = vertices.begin();

      for (; it!=vertices.end(); it++)
	if (nodeId == it->get_index())
	  return it;

      return it;
    }





    SuperNode AdjacencyList::merge_super_nodes(const std::vector<std::list<SuperNode>::iterator>& ptrs) {

      SuperNode sp;

      for (unsigned int i = 0; i<ptrs.size(); i++) {
	sp.push(ptrs[i]->get_node_set());
      }

      return sp;
    }


    AdjacencyList::MinCutTraits AdjacencyList::compute_min_cut(unsigned int _times) {

      MinCutTraits outPut, it;

      outPut.first = 1E100;

      for (unsigned int i=0; i<_times; i++) {
	it = compute_random_contraction();
	if ( it.first < outPut.first) {
	  outPut.first = it.first;
	  outPut.second = it.second;
	}
      }

      return outPut;
    }

    AdjacencyList::MinCutTraits AdjacencyList::compute_random_contraction() {

      std::list<SuperNode> auxVertices;
      std::vector<bool> deletedEdges(edges.size(), false);
      std::vector<unsigned int> randSamp = sampling_no_replace(edges.size(), edges.size());

      MinCutTraits outPut;

#ifdef DEBUG_VERSION
      std::cout << randSamp << std::endl;
#endif

      unsigned int delEdgesCount = 0;

      for(unsigned int i=1; i<=vertices.size(); i++) {
	SuperNode sn(i);
	sn.push(i);
	auxVertices.push_back(sn);
      }

      unsigned int eIndex = 0, f;
      std::vector<std::list<SuperNode>::iterator > ptrs;

      while (auxVertices.size() > 2) {
	std::list<SuperNode>::iterator it = auxVertices.begin();
	ptrs.clear();

	for(; it!=auxVertices.end(); it++) {
	  f = it->find(edges[randSamp[eIndex]]);

	  std::vector<Node> _tmp = std::vector<Node>(it->get_node_set().begin(), it->get_node_set().end());
#ifdef DEBUG_VERSION
	  if (f==0) {
	    std::cout << "Nodes of edge "
		      << edges[randSamp[eIndex]]
		      << " not found in set "
		      << _tmp
		      << std::endl;
	  } else {
	    std::cout << "Nodes of edge "
		      << edges[randSamp[eIndex]]
		      << " found in set "
		      << _tmp
		      << std::endl;
	  }
#endif

	  if (f==1) {
	    std::list<SuperNode>::iterator _it = it;
	    ptrs.push_back(_it);
	    delEdgesCount += (!deletedEdges[randSamp[eIndex]])?1:0;
	    deletedEdges[randSamp[eIndex]] = true;
	  } else if(f==2){
	    delEdgesCount += (!deletedEdges[randSamp[eIndex]])?1:0;
	    deletedEdges[randSamp[eIndex]] = true;
	  }
	}

	if (ptrs.size()) {
	  SuperNode sn = merge_super_nodes(ptrs);

	  for(unsigned int i = 0; i<ptrs.size(); i++)
	    auxVertices.erase(ptrs[i]);

	  auxVertices.push_back(sn);
	}

#ifdef DEBUG_VERSION
	getchar();
#endif
	eIndex++;
      }


#ifdef DEBUG_VERSION

      std::cout << "The super nodes are: ";
      for (std::list<SuperNode>::iterator it = auxVertices.begin(); it!=auxVertices.end(); it++) {
	std::cout << std::vector<unsigned int>(it->get_node_set().begin(), it->get_node_set().end())
		  << " ";
      }
      std::cout << std::endl;

#endif

      for (unsigned int i = 0; i < deletedEdges.size(); i++) {
	for (std::list<SuperNode>::iterator it = auxVertices.begin(); it!=auxVertices.end(); it++) {
	  if (!deletedEdges[i] && it->find(edges[i]) == 2) {
	    deletedEdges[i] =  true;
	    delEdgesCount++;
	    break;
	  }
	}
      }

      outPut.first = edges.size() - delEdgesCount;

      for (unsigned int i=0; i<deletedEdges.size(); i++)
	if(!deletedEdges[i])
	  outPut.second.push_back(edges[i]);

      return outPut;
    }

    const std::set<Node>& AdjacencyList::get_vertices() const {
      return vertices;
    }

    const std::vector<Edge>& AdjacencyList::get_edges()const {
      return edges;
    }


    std::vector<unsigned int> sampling_no_replace(int N, int n) {
      // Generator of random numbers
      std::default_random_engine generator
        ( std::chrono::system_clock::now().time_since_epoch().count() );
      // Distribution to generate probabilities
      std::uniform_real_distribution<double> unif(0,1);
      // Indicator vector
      std::vector<unsigned int> indicator(N, 1);
      std::vector<unsigned int> index;
        
      // Loop to select points in the sample
      for(int i=0; i<n; i++) {
	// Generate probability and weight selection
	double tau = unif(generator);
	double p = 1.0/(double(N-i));

	// Cumulative weight
	double cummP = 0.0;
	// Move to select data from population
	for(int j=0; j<N; j++) {
	  cummP += double(indicator[j])*p;
	  // Select data if the cummulative is greater
	  if(cummP > tau) {
	    indicator[j]=0;
	    index.push_back(j);
	    break;
	  }
	}
      }

      return index;
    } 

    unsigned int AdjacencyList::dijkstra_shortest_path( unsigned int source ) {

      std::vector<unsigned int> dijkstraPath;
      std::vector<unsigned int> shortDist(vertices.size(), 0);
      unsigned int select = 0, nodeHeadIndex;

      SuperNode X; // Set the node

      X.push(*find_node(source));
	
      while(X.internal_size() != vertices.size()) {
	unsigned int minCriteria = 1E100;

	const std::set<Node>& xNodes = X.get_node_set();
	std::set<Node>::iterator itn = xNodes.begin();

	nodeHeadIndex = 0;

	for(; itn!=xNodes.end(); itn++) {
	  
	  const std::set<unsigned int>& es = itn->get_incident_edges();

	  /*	  std::cout << "Edges for node : "
		    << itn->get_index()
		    << " " 
		    << std::vector<unsigned int>(es.begin(), es.end()) 
		    << std::endl;*/

	  //getchar();

	  std::set<unsigned int>::iterator ite = es.begin();
	  for (; ite!=es.end(); ite++ ) {

	    if ( edges[*ite].get_head() == itn->get_index() ||
		 X.find(edges[*ite].get_head()) ) continue;

	    if (shortDist[itn->get_index()-1] + edges[*ite].get_weight() < minCriteria) {
	      minCriteria = shortDist[itn->get_index()-1] + edges[*ite].get_weight();
	      select = *ite;
	      nodeHeadIndex = edges[*ite].get_head();

	      if(nodeHeadIndex == 0) {
		std::cout << "Zeroooo [" << *ite << "] : " << edges[*ite] << std::endl;
	      }
	    }

	  }
	}

        
	if (nodeHeadIndex != 0) {
	  X.push(*find_node(nodeHeadIndex));
	  shortDist[nodeHeadIndex-1] = minCriteria;
	  dijkstraPath.push_back(select);
	
	  std::cout << "Selecting node: " 
		    << nodeHeadIndex 
		    << " with length: "
		    << shortDist[nodeHeadIndex-1]
		    << std::endl;
	} else 
	  std::cout << "Node index: " << nodeHeadIndex << std::endl;


      }// while

      int indices[] = {6,36,58,81,98,114,132,164,187,196};

      for (unsigned int i=0; i<sizeof(indices)/sizeof(int); i++) {
	std::cout << shortDist[indices[i]] <<"," ;
      }

      std::cout << std::endl;
      return shortDist[shortDist.size()-1];
    }



  }
}



