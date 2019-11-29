/*ETIENNE (づ｡◕‿‿◕｡)づ PENAULT */

#pragma once 
#include "Graphe.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <algorithm>
#include <glm/glm.hpp>

#define ever ;;

typedef glm::vec3 point;

class Road{
	public:

		Road(int graph_size, int nb_car);

		void Loop(int actual_car);

		void Join();

		std::vector<int> GetCarNodes();

		int GetCarState(int);

		std::vector<std::vector<float>> GetEdges();

		std::vector<point> GetPoints();

		

		std::vector<int> GetNextNodes();

		std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> GetStarts();

		std::vector<int> GetWaitTimes();

		
		

	private:
    	Graphe g_; //Our graph
    	std::vector<std::thread> t_; //Our thread for each cars
    	std::vector<int> car_nodes_; //Storing the actual node for each car
    	std::vector<int> potential_next_nodes_; //Storing targeted nodes for every cars

    	std::vector<int> next_nodes_; //next nodes for graphic display
    	std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> starts_; //times for graphic display
    	std::vector<int> to_wait_; //wait time for graphic display
};