#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n, m; std::cin >> n >> m;
  graph G(n);
  edge_adder adder(G);
  int source = boost::add_vertex(G);
  int sink = boost::add_vertex(G);
  int total = 0;
  
  for (int i = 0; i < n; i++) {
    int balance; std::cin >> balance;
    if (balance > 0) {
      total += balance;
      adder.add_edge(source, i, balance);
    } else {
      adder.add_edge(i, sink, -balance);
    }
  }
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    int debt; std::cin >> debt;
    adder.add_edge(u, v, debt);
  }
  
  int mincutCost = boost::push_relabel_max_flow(G, source, sink);
  
  if (total - mincutCost > 0) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
