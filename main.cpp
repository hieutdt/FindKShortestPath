#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include "Tokenizer.h"
#include "Lib.h"

#define INF 1000000

using namespace std;

int findIndex(string str) {
	int index = 0;	//Vi tri cua thanh pho xuat phat trong map
	for (int j = 0; j < cities.size(); j++) {
		if (str == cities[j]) {
			index = j;
			break;
		}
	}
	return index;
}

bool existCity(string name) {
	for (int i = 0; i < n; i++)
		if (cities[i] == name)
			return true;
	return false;
}

//CAC HAM NHAP XUAT
void readCityFile(string fileName) {
	ifstream in;
	in.open(fileName);

	if (!in.is_open())
		return;

	int i = 0;
	while (!in.eof()) {
		string tmp;
		getline(in, tmp);
		cities.push_back(tmp);
		i++;
	}
	in.close();
}

void readFlightFile(string fileName) {
	ifstream in;	
	in.open(fileName);

	if (!in.is_open())
		return;

	while (!in.eof()) {
		Flight fl;
		string line, source, destination;

		getline(in, line);
		vector<string> bigTokens = Tokenizer::Parse(line, ",");

		source = bigTokens[0];
		fl.source = findIndex(source);

		vector<string> tokens = Tokenizer::Parse(bigTokens[1], " ");
		fl.duration = stoi(tokens[tokens.size() - 1]);
		fl.cost = stoi(tokens[tokens.size() - 2]);
		fl.flightNumber = stoi(tokens[tokens.size() - 3]);

		for (int i = 1; i < tokens.size() - 3; i++)
			destination	 += tokens[i] + " ";
		destination.erase(destination.end() - 1);
		fl.destination = findIndex(destination);

		//Thanh pho xuat phat do se them mot chuyen bay
		map[fl.source][fl.destination] = fl;
		dMap[fl.source][fl.destination] = fl;
	}
	in.close();
}

void printMap() {
	for (int i = 0; i < n; i++) {
		cout << "=> "<< cities[i] << ": " << endl;
		for (int j = 0; j < n; j++)
			if (map[i][j].cost != INF)			   
				cout << cities[i] << " -> " << cities[j] << ", Cost = " << map[i][j].cost << endl;
		cout << endl;
	}
}

void FloydWarshallAlg() {
	//Gia su duong di ngan nhat giua moi cap dinh la duong truc tiep
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			trace[i][j] = j;
			dTrace[i][j] = j;
		}

	//Thuat toan Floyd-Warshall xay dung duong di ngan nhat giua tat ca cac cap dinh
	for (int k = 0; k < n; k++)
		for (int u = 0; u < n; u++)
			for (int v = 0; v < n; v++) {
				if (map[u][v].cost > map[u][k].cost + map[k][v].cost) {
					map[u][v].cost = map[u][k].cost + map[k][v].cost;
					map[u][v].duration = map[u][k].duration + map[k][v].duration;
					trace[u][v] = trace[u][k];
				}
				if (dMap[u][v].duration > dMap[u][k].duration + dMap[k][v].duration) {
					dMap[u][v].duration = dMap[u][k].duration + dMap[k][v].duration;
					dMap[u][v].cost = dMap[u][v].cost + dMap[k][v].cost;
				}
			}
}

bool checkPath(int x, list<int> &paths) {
	for (list<int>::iterator i = paths.begin(); i != paths.end(); i++) {
		if (*i == x)
			return false;
	}
	return true;
}

void printResultByCost(int s, int f, int** newTrace) {
	int cost = 0;
	int duration = 0;
	int k = newTrace[s][f];	//Diem trung gian
	//In quang duong tu s den diem trung gian k
	int ss = s;
	while (ss != k) {
		cout << cities[ss] << " - ";
		int prev = ss;
		ss = trace[ss][k];
		cost += map[prev][ss].cost;
		duration += map[prev][ss].duration;
	} 
	//In quang duong tu k den dich f
	while (ss != f) {
		cout << cities[s] << " - ";
		int prev = ss;
		ss = trace[ss][f];
		cost += map[prev][ss].cost;
		duration += map[prev][ss].duration;
	}
	cout << cities[f] << endl;

	ss = s;
	while (ss != k) {
		int prev = ss;
		ss = trace[ss][k];
		cout << "Flight #" << map[prev][ss].flightNumber << " from " << cities[prev] << " to " << cities[ss]
			<< " Cost: $" << map[prev][ss].cost << " Duration: " << map[prev][ss].duration << " hours" << endl;
	}
	while (ss != f) {
		int prev = ss;
		ss = trace[ss][f];
		cout << "Flight #" << map[prev][ss].flightNumber << " from " << cities[prev] << " to " << cities[ss]
			<< " Cost: $" << map[prev][ss].cost << " Duration: " << map[prev][ss].duration << " hours" << endl;
	}

	cout << "Total Cost ............ $" << cost << endl;
	cout << "Total Duration ........ " << duration << " hours" << endl << endl;
}

void printResultByCost(int s, int f, int k) {
	//Khoi tao mang trace moi
	int **newTrace = new int*[n];
	for (int i = 0; i < n; i++) {
		newTrace[i] = new int[n];
		for (int j = 0; j < n; j++)
			newTrace[i][j] = trace[i][j];
	}

	//Khoi tao do thi moi
	Flight **newMap = new Flight*[n];
	for (int i = 0; i < n; i++) {
		newMap[i] = new Flight[n];
		for (int j = 0; j < n; j++)
			newMap[i][j] = map[i][j];
	}

	cout << "Route 1: ";
	printResultByCost(s, f, newTrace);

	int prevMin = newMap[s][f].cost;
	list<int> prevPath;
	prevPath.push_front(f);

	for (int i = 1; i < k; i++) {
		//Tim duong di nho nhat lon hon duong di nho nhat hien tai
		cout << "Route " << i + 1 << ": ";
		for (int j = 0; j < n; j++)
			if (j != s && j != f && checkPath(j, prevPath)) {
				if (map[s][j].cost != INF && map[j][f].cost != INF) {
					int cost = map[s][j].cost + map[j][f].cost;
					if (cost >= prevMin) {
						newMap[s][f].cost = cost;
						newMap[s][f].duration = map[s][j].duration + map[j][f].duration;
						newTrace[s][f] = j;
						break;
					}
				}
			}
		for (int j = 0; j < n; j++) {
			if (j != s && j != f && checkPath(j, prevPath)) {
				int cost = map[s][j].cost + map[j][f].cost;
				if (cost <= newMap[s][f].cost && cost >= prevMin) {
					newMap[s][f].cost = cost;
					newMap[s][f].duration = map[s][j].duration + map[j][f].duration;
					newTrace[s][f] = j;
				}
			}
		}
		prevMin = newMap[s][f].cost;
		prevPath.clear();
		int ss = s, ff = f;
		int m = newTrace[s][f];
		while (ss != m) {
			prevPath.push_front(ss);
			ss = trace[ss][m];
		}
		while (ss != ff) {
			prevPath.push_front(ss);
			ss = trace[ss][f];
		}
		prevPath.push_front(ff);

		printResultByCost(s, f, newTrace);
	}
}

void printResultByDuration(int s, int f, int **newTrace) {
	int cost = 0;
	int duration = 0;
	int k = newTrace[s][f];	//Diem trung gian
	//In quang duong tu s den diem trung gian k
	int ss = s;

	while (ss != k) {
		cout << cities[s] << " - ";
		int prev = ss;
		ss = trace[ss][k];
		cost += dMap[prev][ss].cost;
		duration += dMap[prev][ss].duration;
	}
	//In quang duong tu k den dich f
	while (ss != f) {
		cout << cities[ss] << " - ";
		int prev = ss;
		ss = trace[ss][f];
		cost += dMap[prev][ss].cost;
		duration += dMap[prev][ss].duration;
	}
	cout << cities[f] << endl;

	ss = s;
	while (ss != k) {
		int prev = ss;
		ss = trace[ss][k];
		cout << "Flight #" << dMap[prev][ss].flightNumber << " from " << cities[prev] << " to " << cities[ss]
			<< " Cost: $" << dMap[prev][ss].cost << " Duration: " << dMap[prev][ss].duration << " hours" << endl;
	}
	while (ss != f) {
		int prev = ss;
		ss = trace[ss][f];
		cout << "Flight #" << dMap[prev][ss].flightNumber << " from " << cities[prev] << " to " << cities[ss]
			<< " Cost: $" << dMap[prev][ss].cost << " Duration: " << dMap[prev][ss].duration << " hours" << endl;
	}

	cout << "Total Cost ............ $" << cost << endl;
	cout << "Total Duration ........ " << duration << " hours" << endl << endl;
}

void printResultByDuration(int s, int f, int k) {
	//Khoi tao mang trace moi
	int **newTrace = new int*[n];
	for (int i = 0; i < n; i++) {
		newTrace[i] = new int[n];
		for (int j = 0; j < n; j++)
			newTrace[i][j] = trace[i][j];
	}

	//Khoi tao do thi moi
	Flight **newMap = new Flight*[n];
	for (int i = 0; i < n; i++) {
		newMap[i] = new Flight[n];
		for (int j = 0; j < n; j++)
			newMap[i][j] = map[i][j];
	}

	cout << "Route 1: ";
	printResultByDuration(s, f, newTrace);

	int prevMin = newMap[s][f].duration;
	list<int> prevPath;
	prevPath.push_front(f);

	for (int i = 1; i < k; i++) {
		//Tim duong di nho nhat lon hon duong di nho nhat hien tai
		cout << "Route " << i + 1 << ":";
		for (int j = 0; j < n; j++)
			if (j != s && j != f && checkPath(j, prevPath)) {
				if (dMap[s][j].cost != INF && dMap[j][f].cost != INF) {
					int time = dMap[s][j].duration + dMap[j][f].duration;
					if (time >= prevMin) {
						newMap[s][f].cost = dMap[s][j].cost + dMap[j][f].cost;
						newMap[s][f].duration = time;
						newTrace[s][f] = j;
						break;
					}
				}
			}
		for (int j = 0; j < n; j++) {
			if (j != s && j != f && checkPath(j, prevPath)) {
				int time = dMap[s][j].duration + dMap[j][f].duration;
				if (time <= newMap[s][f].duration && time >= prevMin) {
					newMap[s][f].cost = dMap[s][j].cost + dMap[j][f].cost;
					newMap[s][f].duration = time;
					newTrace[s][f] = j;
				}
			}
		}
		prevMin = newMap[s][f].duration;
		prevPath.clear();
		int ss = s, ff = f;
		int m = newTrace[s][f];
		while (ss != m) {
			prevPath.push_front(ss);
			ss = trace[ss][m];
		}
		while (ss != ff) {
			prevPath.push_front(ss);
			ss = trace[ss][f];
		}
		prevPath.push_front(ff);

		printResultByDuration(s, f, newTrace);
	}
}

void readRequests(string fileName) {
	ifstream in;
	in.open(fileName);
	in >> k;
	while (!in.eof()) {
		Request tmp;
		string line;
		getline(in, line);
		vector<string> bigTokens = Tokenizer::Parse(line, ",");
		if (bigTokens.size() == 2) {
			tmp.source = bigTokens[0];
			vector<string> tokens = Tokenizer::Parse(bigTokens[1], " ");
			tmp.type = stoi(tokens[tokens.size() - 1]);
			for (int i = 1; i < tokens.size() - 1; i++)
				tmp.destination += tokens[i] + " ";
			tmp.destination.erase(tmp.destination.end() - 1);

			requests.push_back(tmp);
		}
	}
}

void sendRequest() {
	for (list<Request>::iterator i = requests.begin(); i != requests.end(); i++) {
		int s = findIndex(i->source);
		int f = findIndex(i->destination);
		cout << "------------------------------------------------------------------------------" << endl;
		if (i->type == 2) {
			cout << "Request is to fly from " << cities[s] << " to " << cities[f] << "." << endl;
			if (dMap[s][f].cost == INF) {
				cout << "Sorry. HPAir does not fly from " << cities[s] << " to " << cities[f] << "." << endl;
				return;
			}
			if (!existCity(i->destination)) {
				cout << "Sorry. HPAir does not serve " << i->destination << "." << endl;
				return;
			}
			printResultByDuration(s, f, k);
		}
		else if (i->type == 1) {
			cout << "Request is to fly from " << cities[s] << " to " << cities[f] << "." << endl;
			if (map[s][f].cost == INF) {
				cout << "Sorry. HPAir does not fly from " << cities[s] << " to " << cities[f] << "." << endl;
				return;
			}
			if (!existCity(i->destination)) {
				cout << "Sorry. HPAir does not serve " << i->destination << "." << endl;
				return;
			}
			printResultByCost(s, f, k);
		}
		cout << "------------------------------------------------------------------------------" << endl;
	}
}

void init() {
	map = new Flight*[n];
	for (int i = 0; i < n; i++) {
		map[i] = new Flight[n];
		for (int j = 0; j < n; j++) {
			map[i][j].cost = INF;
			map[i][j].duration = INF;
		}
	}
	dMap = new Flight*[n];
	for (int i = 0; i < n; i++) {
		dMap[i] = new Flight[n];
		for (int j = 0; j < n; j++) {
			dMap[i][j].cost = INF;
			dMap[i][j].duration = INF;
		}
	}

	trace = new int*[n];
	for (int i = 0; i < n; i++)
		trace[i] = new int[n];
	dTrace = new int*[n];
	for (int i = 0; i < n; i++)
		dTrace[i] = new int[n];
}

int main() {
	const string cityFile = "CITY.INP";
	const string flightFile = "FLIGHT.INP";
	const string requestFile = "REQUEST.INP";

	//Khoi tao du lieu do thi
	readCityFile(cityFile);
	n = cities.size();
	init();
	readFlightFile(flightFile);
	
	printMap();

	//Thuc hien thuat toan Floyd doi voi tim kiem theo chi phi va thoi gian
	FloydWarshallAlg();

	readRequests(requestFile);
	sendRequest();

	system("pause");
	return 0;			 
}