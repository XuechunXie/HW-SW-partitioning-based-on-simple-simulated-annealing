// SimulatedAnnealing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

double cost();
void initialize();
void storeEdges();
void simulatedAnnealing(int TI, int TL, double alpha);
void display();
struct node {
	int nth_node;
	double W2_N;
	double W1_N;
	//each edge and correspoinding ci will display as edgeij|ci
	//ex.1.2|0.5is edge for node 1 and 2 with ci=0.5
	//vector<string> edgesAndCIs;
};
struct edge {
	int nth_edge;
	int node1;
	int node2;
	int CI;
};
node *currentSolution;
edge *currentEdges;
vector<int> hw_nodes;
vector<int> sw_nodes;
vector<int> hw_nodes_temp;
vector<int> sw_nodes_temp;
vector<int> hw_nodes_best;
vector<int> sw_nodes_best;
vector<string> edgesAndCIs;
int nNodes;
int totalEdge;


int main(){
	int TI, TL;
	double alpha;
	initialize();			//initialize the current solution
	
	cout << "please input the initial temperature TI:" << endl;
	cin >> TI;
	cout << "please input the temperature length TL:" << endl;
	cin >> TL;

	cout << "please input the cooling ration alpha:" << endl;
	cin >> alpha;
	simulatedAnnealing(TI, TL, alpha);   //start partioning the nodes 
	display();
}




void initialize() {
	ifstream nodesInfo;
	string line,line_1st;
	int M_cl = 5, M_u = 1, M_p = 1, M_so = 1;
	double K_cl, K_u, K_p, K_so;
	//double CL_sw, CL_hw, total_CL;
	double CL, total_CL;
	int Nr_op, Nr_kind_op, L_path, sum_w_op;
	double W1_N_temp, W2_N_temp;
	
	nodesInfo.open("nodesInfo.txt");
	if (!nodesInfo) {
		cout << "ERROR:can not open nodesInfo.txt ";
		return;
	}
	getline(nodesInfo, line_1st);
	istringstream iss(line_1st);
	string sub_1st;
	iss >> sub_1st;
	nNodes = stoi(sub_1st);       //catch the total nodes which indicated in the first line of nodesInfo.txt
	currentSolution = new node[nNodes];
	iss >> sub_1st;
	total_CL = stoi(sub_1st);		//catch the total CL in the firstline
	for (int i = 0; i < nNodes; i++) {
		getline(nodesInfo, line);
		istringstream is(line);
		string filed;
		is >> filed;
		currentSolution[i].nth_node = stoi(filed)-1;
		sw_nodes.push_back(stoi(filed));
		is >> filed;
		CL= stoi(filed);
		is >> filed;
		Nr_op = stoi(filed);
		is >> filed;
		Nr_kind_op = stoi(filed);
		is >> filed;
		L_path = stoi(filed);
		is >> filed;
		sum_w_op = stoi(filed);

		K_u = Nr_op / Nr_kind_op;
		if (L_path != 0)
			K_p = Nr_op / L_path;
		else
			K_p = 0;
		K_so = sum_w_op / Nr_op;
		W1_N_temp = CL;				//Computer W1_N for each node
		K_cl = W1_N_temp / total_CL;

		W2_N_temp = M_cl * K_cl + M_u * K_u + M_p * K_p - M_so * K_so;			//Computer W2_N for each node
		currentSolution[i].W1_N = W1_N_temp;		//store W1_N for each node
		currentSolution[i].W2_N = W2_N_temp;		//store W2_N for each node

	}
	storeEdges();			//store edges
}

void storeEdges() {
	string line_1st,word, edgeLine;
	//int totalEdge=0;
	ifstream edgeInfo;
	edgeInfo.open("edges.txt");
	if (!edgeInfo) {
		cout << "ERROR: No edges.txt found." << endl;
		return;
	}
	getline(edgeInfo, line_1st);
	totalEdge = stoi(line_1st);
	currentEdges = new edge[totalEdge];
	for (int i = 0; i < totalEdge; i++) {
		getline(edgeInfo, edgeLine);
		istringstream iss(edgeLine);
		iss >> word;
		currentEdges[i].nth_edge=stoi(word);
		iss >> word;
		currentEdges[i].node1 = stoi(word);
		iss >> word;
		currentEdges[i].node2 = stoi(word);
		iss >> word;
		currentEdges[i].CI = stoi(word);
	}
	return;
}

double cost() {
	double W1_E;
	int wd=8;					//Assume all the channel width is 8bits
	double Q1 = 1, Q2 =1, Q3 = 1;
	int nSwNodes, nHwNodes;
	double total_W1_E = 0, total_W2_E = 0, total_W2_N_sw = 0, total_W2_N_hw = 0, total_parallel = 0;
	int node1, node2;
	bool isSW_node1=false,isSW_node2=false;


	//add the edge of hw and sw to the cut
	nSwNodes = sw_nodes.size();
	nHwNodes = hw_nodes.size();
	vector<int>::iterator it_hw;

	for (int i = 0; i < totalEdge; i++) {			//iterate every edge 
		node1 = currentEdges[i].node1;
		node2 = currentEdges[i].node2;
		vector<int>::iterator iter_node1 = find(sw_nodes.begin(), sw_nodes.end(), node1);	
		vector<int>::iterator iter_node2 = find(sw_nodes.begin(), sw_nodes.end(), node1);
		if (iter_node1 != sw_nodes.end())		//if node1 is found in sw_nodes
			isSW_node1 = true;					// node1 is a sw node
		if (iter_node2 != sw_nodes.end())		//if node2 is found in sw_nodes
			isSW_node2 = true;					//node2 is a sw node

		if ((isSW_node1 && !isSW_node2) || (!isSW_node1 && isSW_node2))		//if node1 and node2 are not both sw or both hw
			total_W1_E += currentEdges[i].CI*wd;				//accumulate W1_E
	}
	
	for (int j = 0; j < nHwNodes; j++) {
		node currentNode= currentSolution[hw_nodes[j]];
		//total_parallel += total_W2_E / currentNode.W1_N;
		total_W2_N_hw += currentNode.W2_N;	
		for (int i = 0; i < totalEdge; i++) {
			node1 = currentEdges[i].node1;
			node2 = currentEdges[i].node2;
			if (node1 == hw_nodes[j]|| node2 == hw_nodes[j]) {
				total_W2_E+= currentEdges[i].CI;				
			}
		}
		total_parallel += total_W2_E / currentNode.W1_N;
	}
	for (int i = 0; i < nSwNodes; i++) {
		node currentNode = currentSolution[sw_nodes[i]];
		total_W2_N_sw += currentNode.W2_N;
		//sw_nodes.push_back();
	}
	if (nHwNodes != 0) {
		total_W2_N_hw = total_W2_N_hw / nHwNodes;
		total_parallel = total_parallel / nHwNodes;
	}
	else {
		total_W2_N_hw = 0;
		total_parallel = 0;
	}

	if (nSwNodes != 0) 
		total_W2_N_sw = total_W2_N_sw / nSwNodes;
	else
		total_W2_N_sw = 0;

	double cost = Q1 * total_W1_E + Q2 * total_parallel - Q3 * (total_W2_N_hw - total_W2_N_sw);
	return cost;
}


void simulatedAnnealing(int TI,int TL,double alpha) {
	int random,T,TM,stopCount=0;
	double q;
	double curCost, newCost,diffCost;
	double bestCost;
	curCost = cost();
	bestCost = curCost;
	srand((unsigned)time(NULL));
	T = TI;
	TM = TI - TL;					//terminate temperature is inital temperature-temperature length
	while (true) {				//if current temperature is larger than terminate temperature, go into loop
		for (int t = 1; t < TL; t++) {   //don't know why we should have this loop, just implement as algorithm mentioned
			random =1+rand() % nNodes;
			//cout << random << endl;
			//store the original 1solution to a temp vector
			sw_nodes_temp.clear();
			hw_nodes_temp.clear();
			for (vector<int>::iterator it = sw_nodes.begin(); it != sw_nodes.end(); it++) {
				sw_nodes_temp.push_back(*it);
			}
			for (vector<int>::iterator it = hw_nodes.begin(); it != hw_nodes.end(); it++) {
				hw_nodes_temp.push_back(*it);
			}
			vector<int>::iterator iter_sw = find(sw_nodes.begin(), sw_nodes.end(), random);
			if (iter_sw != sw_nodes.end()) {			//if node random is found in the set of sw_nodes
				hw_nodes.push_back(random);			//move it to the set of hw_nodes 
				sw_nodes.erase(iter_sw);				//remove node random from the set of sw_nodes 
			}
			else {
				sw_nodes.push_back(random);			//move it to the set of hw_nodes 
				vector<int>::iterator iter_hw= find(hw_nodes.begin(), hw_nodes.end(), random);
				hw_nodes.erase(iter_hw);				//remove node random from the set of hw_nodes 
			}
			newCost = cost();
			diffCost = newCost - curCost;
			if (newCost < bestCost) {
				bestCost = newCost;
				cout << "bestCost=" << bestCost<<endl;
				hw_nodes_best.clear();
				sw_nodes_best.clear();
				for (vector<int>::iterator it = sw_nodes.begin(); it != sw_nodes.end(); it++) {
					sw_nodes_best.push_back(*it);
				}
				for (vector<int>::iterator it = hw_nodes.begin(); it != hw_nodes.end(); it++) {
					hw_nodes_best.push_back(*it);
				}
			}
			if (diffCost <= 0) {
				curCost = newCost;  //current solution change to the new one
				stopCount = 0;
			}else {	
				q = rand() / double(RAND_MAX);
				if (q < exp(-diffCost / T)) {		//if q less than the exp
					curCost = newCost;				//the new solution is accepted as the current solution
					stopCount = 0;
				}else {								//restore the original solution
					stopCount++;
					sw_nodes.clear();
					hw_nodes.clear();
					for (vector<int>::iterator it = sw_nodes_temp.begin(); it != sw_nodes_temp.end(); it++) {
						sw_nodes.push_back(*it);
					}
					for (vector<int>::iterator it = hw_nodes_temp.begin(); it != hw_nodes_temp.end(); it++) {
						hw_nodes.push_back(*it);
					}
				}
					
			}
			if (stopCount > 2)
				return ;
		}
		T = alpha * T;
	}	
	return;	
}

void display() {
	cout << "The min cost solution contains nodes in hw set are:" << endl;
	for (vector<int>::iterator it = hw_nodes_best.begin(); it != hw_nodes_best.end(); it++) {
		cout << *it << endl;
	}
	cout << "The min cost solution contains nodes in sw set are:" << endl;
	for (vector<int>::iterator it = sw_nodes_best.begin(); it != sw_nodes_best.end(); it++) {
		cout << *it << endl;
	}
}