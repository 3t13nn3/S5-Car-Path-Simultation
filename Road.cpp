/*ETIENNE (づ｡◕‿‿◕｡)づ PENAULT */

#include "Road.hpp"

/*INITIALIZE OUR ROAD*/
Road::Road(int graph_size, int nb_car) : g_(graph_size){
	//initializing our graph
	g_.GeneratePoints();
	g_.RandomFill();
	g_.GenerateLines();

	g_.Display();

	t_.reserve(nb_car);
	next_nodes_.reserve(nb_car);
    starts_.reserve(nb_car);
    to_wait_.reserve(nb_car);		

    //Fill a vector of value to after pick up a vlaue randomly
	std::vector<int> v;
	for(size_t i = 0; i< g_.GetSize(); ++i){
		v.emplace_back(i);
	}

	for(size_t i = 0; i < nb_car; ++i){
		//setting a car at a random node and mark it state as one
		int index = rand()%v.size();
		car_nodes_.emplace_back(v[index]);
		g_.SetNodeState(v[index],1);
		v.erase(v.begin()+index,v.begin()+index+1);

		//Initialize value
		next_nodes_.emplace_back(-1);
		potential_next_nodes_.emplace_back(-1);
	  	starts_.emplace_back(std::chrono::high_resolution_clock::now());
	    to_wait_.emplace_back(0);
	}

	//Launching a thread by car
	for(size_t i = 0; i < nb_car; ++i){
		t_.emplace_back(&Road::Loop,this,i);
	}
}

/*JOINING THREADS*/
void Road::Join(){
	for(size_t i = 0; i < t_.size(); ++i)
		t_[i].join();
}

/*MAIN LOOP FOR EACH CARS*/
void Road::Loop(int actual_car){
	//At the first iteration we have to define a pentential futur node to go
	int i = 1;
	potential_next_nodes_[actual_car] = g_.SelectingSubNode(car_nodes_[actual_car]);
	bool node_state = true;

	for(ever){
		//randomly get a time to wait between [1:5] secs
		int to_wait = (rand()%5 + 1) * 1000;
		
		
		//STATE 1 --> First try (BLUE)
		//STATE [2:3] --> Second try (RED)
		//STATE 4 --> Reassign a targetted node
		if(g_.GetNodeState(car_nodes_[actual_car]) > 4){

			//setting the state as 4, we need to reassign the targeted node 
			g_.Lock(car_nodes_[actual_car]);
			g_.SetNodeState(car_nodes_[actual_car],4);
			g_.Unlock(car_nodes_[actual_car]);
			potential_next_nodes_[actual_car] = g_.SelectingSubNode(car_nodes_[actual_car]);
			i = 2;
		}
		

		node_state = true;

		//Checking if the node isn't already taked by an other car
		g_.Lock(potential_next_nodes_[actual_car]);
		if(g_.GetNodeState(potential_next_nodes_[actual_car])){
			node_state = 0;
			g_.Unlock(potential_next_nodes_[actual_car]);
		}

		//if the node isn't already taked by an other car
		if(node_state){
			//Settings our values for the graphics like times, time to wait, targeted node
			auto start = std::chrono::high_resolution_clock::now();
			next_nodes_[actual_car] = potential_next_nodes_[actual_car];
			starts_[actual_car] = start;
			to_wait_[actual_car] = to_wait;
	
			//make the curent node free by modifying it state
			g_.Lock(car_nodes_[actual_car]);
			g_.SetNodeState(car_nodes_[actual_car],0);
			g_.Unlock(car_nodes_[actual_car]);
			//std::cout <<  actual_car << " Init : "<< car_nodes_[actual_car]<< " next: " << next_nodes_[actual_car] << "  state: " << g_.GetNodeState(car_nodes_[actual_car]) << std::endl;

			//make the new node occupied by modifying it state
			g_.SetNodeState(potential_next_nodes_[actual_car],1);
			g_.Unlock(potential_next_nodes_[actual_car]);

			/*ASSIGN A NEW TARGETED POINT TO OUR CAR*/
			potential_next_nodes_[actual_car] = g_.SelectingSubNode(potential_next_nodes_[actual_car]);
			i = 1;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(to_wait));
		if(node_state){	
			car_nodes_[actual_car] = next_nodes_[actual_car];
		} else {
			//std::cout << "La voiture " << actual_car <<  " n'a pas pu aller au noeud "<< potential_next_nodes_[actual_car] << std::endl;

			//The car haven't move, we need to actualize the state for knowing if we need to reassign the targetted node by using i
			i++;
			g_.Lock(car_nodes_[actual_car]);
			g_.SetNodeState(car_nodes_[actual_car],i);
			g_.Unlock(car_nodes_[actual_car]);
			
		}
	}
}

/*GETTER/SETTER*/

std::vector<std::vector<float>> Road::GetEdges(){
	return g_.GetEdges();
}

std::vector<point> Road::GetPoints(){
	return g_.GetPoints();
}

std::vector<int> Road::GetCarNodes(){
	return car_nodes_;
}
std::vector<int> Road::GetNextNodes(){
	return next_nodes_;
}

std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> Road::GetStarts(){
	return starts_;
}

std::vector<int> Road::GetWaitTimes(){
	return to_wait_;
}

int Road::GetCarState(int car){
	return g_.GetNodeState(car_nodes_[car]);
}
