/*ETIENNE (づ｡◕‿‿◕｡)づ PENAULT */

#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <ctime>
#include <algorithm>
#include <mutex>
#include <memory>

#include <glm/glm.hpp>

/*UNCOMMENT FOR HAVING THE MAXIMUM OF CONNEXIONS THAT A NODE COULD HAVE FOR EACH NODE (UNDER A FIXED LIMIT)*/
//#define MAX_CONNEXIONS
/*CHANGE THIS VALUE TO MODIFY LIMIT OF CONNEXION OF A NODE*/
#define CONNEXION_LIMIT 4

typedef int node;
typedef std::list<node> adjacency_list;
typedef glm::vec3 point;

class Graphe{
	public:

		Graphe(int nb);

		void AddEdges(uint start, uint final);

		void Display();

		void RandomFill();

		int  GetSize();

		void Lock(int node);

		void Unlock(int node);

		int  GetNodeState(int index);

		void SetNodeState(int index, int state);

		int  SelectingSubNode(int node);



		void GeneratePoints();

        void GenerateLines();

        std::vector<std::vector<float>> GetEdges();

        std::vector<point> GetPoints();

	private:
    	std::vector<adjacency_list> nodes_; //Our graph nodes
    	std::vector<std::mutex*> busy_nodes_; //For each nodes, there is it mutex at the nodes index
    	std::vector<int> states_; //Vector of nodes state

    	std::vector<point> p_; //point coordinates for every nodesw
        std::vector<std::vector<float>> edges_graph_; //edges coordinates
};