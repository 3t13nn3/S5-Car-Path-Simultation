/*ETIENNE (づ｡◕‿‿◕｡)づ PENAULT */

#include "Graphe.hpp"

/*INITIALIZING OUR GRAPH AND IT COMPONENTS*/
Graphe::Graphe(int nb){
	//reserving memory
	nodes_.resize(nb);
	busy_nodes_.resize(nb);
	states_.resize(nb);

	//filling our vectors of mutexs and states*/
	for (int i = 0; i < nb; ++i){
		std::mutex* emtpy_mutex;
		emtpy_mutex = new std::mutex();
		busy_nodes_[i] = emtpy_mutex;	
		states_[i] = 0;	
	}
	
}

/*ADDING AN EDGE TO THE GRAPH*/
void Graphe::AddEdges(uint start, uint final){
	auto i=nodes_[start].begin();

	/*Test if we could add our edge*/
	if(start> nodes_.size()-1 || final> nodes_.size()-1 || start == final){
		std::cout<< start << " --- " << final <<std::endl;
		std::cout<<"Node out of the graph."<<std::endl;
		return;
	}
	while(i != nodes_[start].end()){
		if(*i == node(final)){
			std::cout<<"Node " << node(start) << ": Edge " << node(start) << "--" << node(final) << " already exist, didn't adding it."<<std::endl;
			return;
		}
		i++;
	}

	/*Adding nodes to form an edge*/
	nodes_[start].emplace_back(node(final));
	nodes_[final].emplace_back(node(start));
}

/*DISPLAYING GRAPH NODE BY NODE*/
void Graphe::Display(){
	for(size_t i=0; i < nodes_.size(); i++){
		if(nodes_[i].size() > 0){
			std::cout<<"Node "<< i <<": ";
			for(auto j = nodes_[i].begin(); j != nodes_[i].end(); ++j){
				std::cout<<"| "<<i<<"->"<<(*j) << " ";
			}
			std::cout<<"|\n";
		}
	}
}

/*FILLING OUR GRAPH RANDOMLY*/
void Graphe::RandomFill(){
	/*WE GOT A CONNECT AND DISCONNECT VECTOR*/
	std::vector<int> connect;
   std::vector<int> disconnect;
	//Adding to our connect vector a random node
	int start = rand()%nodes_.size();
	connect.emplace_back(start);

	//then adding others nodes to disconnect
	for(size_t i = 0; i< nodes_.size(); ++i)
		if(i != start)
			disconnect.emplace_back(i);

	//While all nodes are not connected to others
	while(!disconnect.empty()){

		int c, d;	

		//We pick a conected node and a disconnected one
		c = connect[rand()%(connect.size())];
		d = disconnect[rand()%(disconnect.size())];

		
		//Checking btw if or connection limit by nodes is not exceeded
		if(nodes_[c].size() < CONNEXION_LIMIT){
			//Then we connect our disconnected node (d) to our already connected node (c)
			this->AddEdges(c,d);
			//Deleting d of our disconnect vector because we will connect this node to others
			std::vector<int>::iterator it = std::find(disconnect.begin(), disconnect.end(), d);
			disconnect.erase(it);
			//adding d to connected nodes
			connect.emplace_back(d);
		}
	}

/*FOR HAVING THE MAXIMUM OF CONNEXIONS THAT A NODE COULD HAVE FOR EACH NODE (UNDER CONNEXION_LIMIT)*/
#ifdef MAX_CONNEXIONS
	//For every nodes
	for(size_t i = 0; i< nodes_.size(); ++i){
		//browse into each sub-nodes (still under the limit)
		while(nodes_[i].size() < CONNEXION_LIMIT-1){
			//filling the vector of every sub-nodes
			std::vector<int> existing_node;
			for(auto j = nodes_[i].begin(); j != nodes_[i].end(); ++j){
				existing_node.emplace_back(*j);
			}

			/*Selecting randomly a sub-node which is not already a sub-nodes of the current nodes, and not even the current node :)*/
			int a = rand()%nodes_.size();
			while(((std::find(existing_node.begin(), existing_node.end(), a)) != existing_node.end()) || a == i ){
				a = rand()%nodes_.size();
			}
			
			//Adding our new selected node as a sub-node of the current node
			if(nodes_[a].size()< CONNEXION_LIMIT)
				this->AddEdges(i,a);
		}
	}
#endif
	}

/*GENERATE RANDOMLY SOME POINTS COORDINATES FOR DISPLAYING NODES*/
void Graphe::GeneratePoints(){
	for(size_t i = 0; i< nodes_.size()	; ++i){
		point tmp = glm::vec3(rand()%(1000-(-1000) + 1000)-1000,rand()%(1000-(-1000) + 1000)-1000,rand()%(1000-(-1000) + 1000)-1000);
		tmp.x/=1000;
		tmp.y/=1000;
		tmp.z/=1000;
		p_.emplace_back(tmp);
	}
}

/*GENERATE LINES AS COORDINATES FOR DISPLAYING EDGES*/
void Graphe::GenerateLines(){
	//for every sub-nodes
	for(size_t i=0; i < nodes_.size(); i++){
		for(auto j = nodes_[i].begin(); j != nodes_[i].end(); ++j){


			bool in = false;

			//if the current sub-node is greater than the parent, it mean we have already create the line, then we don't need to recreate the line
			if((*j) > i)
				in = true;

			//Creating the line object by the points we generate just before
			if(!in){
				std::vector<float> tmp;
				tmp.emplace_back(p_[i].x);
				tmp.emplace_back(p_[i].y);
				tmp.emplace_back(p_[i].z);
				tmp.emplace_back(p_[(*j)].x);
				tmp.emplace_back(p_[(*j)].y);
				tmp.emplace_back(p_[(*j)].z);

				edges_graph_.emplace_back(tmp);
			}
		}
	}
}

//Return randomly a sub-node of a node :)
int Graphe::SelectingSubNode(int node){
	int r = rand()%nodes_[node].size();
	int cpt = 0;
	for(auto i = nodes_[node].begin(); i != nodes_[node].end(); ++i){
		if(cpt == r)
			return (*i);
		cpt++;
	}
	return 0;
}

/*Lock or Unlock the current node by modifying it mutex into busy_nodes_ vector*/
void Graphe::Lock(int node){
	return busy_nodes_[node]->lock();
}

void Graphe::Unlock(int node){
	busy_nodes_[node]->unlock();
}

/*CLASSIC GETTER/SETTER*/

int Graphe::GetSize(){
	return nodes_.size();
}

std::vector<std::vector<float>> Graphe::GetEdges(){
	return edges_graph_;
}

std::vector<point> Graphe::GetPoints(){
	return p_;
}
int Graphe::GetNodeState(int index){
	return states_[index];
}

void Graphe::SetNodeState(int index, int state){
	states_[index] = state;
}
