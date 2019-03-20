#pragma once

#include <string>
#include <vector>
#include <list>

using namespace std;

struct Flight {
	int source; //vi tri trong mang du lieu
	int destination; //vi tri trong mang du lieu
	int flightNumber;
	int cost;
	int duration;
};

struct Request {
	string source;
	string destination;
	int type;
};

//BIEN TOAN CUC
vector<string> cities;
Flight **map;
Flight **dMap;
int **trace;
int **dTrace;
int n, k;
list<Request> requests;