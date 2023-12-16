#pragma once
#include "Header.h"

class OilNetwork {
public:

	void addEdge(int v1, int v2, int edge) {
		adjMatrix[v1].push_back(v2);
		adjMatrix[v2];
		edgeAdjMatrix[edge].push_back(v1);
		edgeAdjMatrix[edge].push_back(v2);
	}

	void addVertex(int v) {
		adjMatrix[v];
	}

	auto getAdjMat() {
		return adjMatrix;
	}

	auto getEdgeMat() {
		return edgeAdjMatrix;
	}

	void deleteEdge(int id) {
		auto it = edgeAdjMatrix.find(id);

		if (it != edgeAdjMatrix.end()) {
			int v1 = edgeAdjMatrix[id][0];
			int v2 = edgeAdjMatrix[id][1];
			vector<int>& con = adjMatrix[v1];
			auto i = find(begin(con), end(con), v2);
			con.erase(i);
			edgeAdjMatrix.erase(id);
			for (auto& pair : adjMatrix) {
				for (auto& con : edgeAdjMatrix) {
					if (pair.first == con.second[0] or pair.first == con.second[1]) {
						return;
					}
				}
				adjMatrix.erase(pair.first);
				return;
			}

		}
		else {
			std::cerr << "Pipe with id " << id << " not found." << std::endl;
			return;
		}
	}

	bool ifEdgeExists(int edge) {
		auto it = edgeAdjMatrix.find(edge);

		if (it == edgeAdjMatrix.end()) {
			return false;
		}
		return true;
	}

	int ifVertexExists(int v) {
		for (auto& row : edgeAdjMatrix) {

			if (row.second[0] == v) {
				return row.second[0];
			}

			if (row.second[1] == v) {
				return row.second[1];
			}
		}

		return 0;
	}

	void printTable() {
		for (auto& pair : adjMatrix) {
			cout << pair.first << "  ->  ";

			for (size_t ind = 0; ind < pair.second.size(); ++ind) {
				cout << pair.second[ind];

				if (ind < pair.second.size() - 1) {
					cout << " -> ";
				}
			}

			cout << "\n";
		}

		cout << "\n-----------\n";
		for (auto& pair : edgeAdjMatrix) {
			cout << pair.first << "    ";
			cout << pair.second[0] << " -> " << pair.second[1] << "\n\n";
		}
	}

	void saveEdge(ofstream& fout) {
		for (auto& pair : edgeAdjMatrix) {
			fout << pair.first << "\n"
				<< pair.second[0] << "\n"
				<< pair.second[1] << "\n";
		}
	}

	void loadEdge(ifstream& fin) {
		string edge_num;
		getline(fin, edge_num);

		for (int s = 0; s < stoi(edge_num); s++) {
			string id, v1, v2;
			getline(fin, id);
			getline(fin, v1);
			getline(fin, v2);
			addEdge(stoi(v1), stoi(v2), stoi(id));
		}
	}

	void topologicalSortUtil(int v, map<int, bool>& visited, stack<int>& stack) {
		visited[v] = true;
		for (int neighbor : adjMatrix[v]) {
			if (!visited[neighbor]) {
				topologicalSortUtil(neighbor, visited, stack);
			}
		}
		stack.push(v);
	}

	void topologicalSort() {

		if (isCyclic()) {
			cout << "Network contains a cycle. Topological sorting is not possible.\n\n";
			return;
		}
		stack<int> stack;
		map<int, bool> visited;
		for (auto& pair : adjMatrix) {
			visited[pair.first] = false;
		}

		for (auto& pair : adjMatrix) {
			if (!visited[pair.first]) {
				topologicalSortUtil(pair.first, visited, stack);
			}
		}

		cout << "Topological sorted network:\n";
		while (!stack.empty()) {
			cout << stack.top() << " ";
			stack.pop();
		}

		cout << "\n\n";
	}

	bool isCyclicUtil(int v, unordered_set<int>& visited, unordered_set<int>& recStack) {
		visited.insert(v);
		recStack.insert(v);

		for (const auto& neighbor : adjMatrix[v]) {
			if (visited.find(neighbor) == visited.end()) {
				if (isCyclicUtil(neighbor, visited, recStack)) {
					return true;
				}
			}
			else if (recStack.find(neighbor) != recStack.end()) {
				return true;
			}
		}

		recStack.erase(v);
		return false;
	}

	bool isCyclic() {
		unordered_set<int> visited;
		unordered_set<int> recStack;

		for (const auto& pair : adjMatrix) {
			int v = pair.first;
			if (visited.find(v) == visited.end()) {
				if (isCyclicUtil(v, visited, recStack)) {
					return true;
				}
			}
		}

		return false;
	}

private:
	unordered_map<int, vector<int>> adjMatrix;
	unordered_map<int, vector<int>> edgeAdjMatrix;
};

class NetworkMap {
public:
	void addElement(const OilNetwork& value) {
		int newKey = getNextKey();
		myMap[newKey] = value;
	}

	void removeElement(int key) {
		myMap.erase(key);
	}

	OilNetwork returnElem(int id) {
		return myMap[id];
	}

	void clear() {
		myMap.clear();
		currentKey = 1;
	}

	void addDirectly(int key, OilNetwork& on) {
		myMap[key] = on;
	}

	int getLastId() {
		currentKey += 1;
		return currentKey - 1;
	}

	void setLastId(int id) {
		currentKey = id;
	}

	bool searchPipe(int id) {
		for (auto& pair : myMap) {
			if (pair.second.ifEdgeExists(id)) {
				return true;
			}
		}
		return false;
	}

	void printElems() {
		cout << "Oil Networks:\n\n";
		for (auto& pair : myMap) {
			cout << "Oil Network id: " << pair.first << "\n\n";
			pair.second.printTable();
			cout << "\n----------------\n";
		}
	}

	int searchVertex(int id) {
		for (auto& pair : myMap) {
			if (pair.second.ifVertexExists(id) != 0) {
				return pair.first;
			}
		}
		return 0;
	}

	void addToMap(int id, int v1, int v2, int edge) {
		int id2 = searchVertex(v2);
		if (id2 == 0) {
			myMap[id].addEdge(v1, v2, edge);
		}
		else if (id != id2) {
			combineNets(id, id2);
			myMap[id].addEdge(v1, v2, edge);
		}

		else {
			myMap[id].addEdge(v1, v2, edge);
		}
	}

	void combineNets(int id1, int id2) {
		for (auto& rows : myMap[id2].getEdgeMat()) {
			myMap[id1].addEdge(rows.second[0], rows.second[1], rows.first);
		}
		myMap.erase(id2);
		currentKey -= 1;
	}

	void saveMap(ofstream& fout) {
		for (auto& pair : myMap) {
			fout << pair.first << "\n"
				<< pair.second.getEdgeMat().size() << "\n";
			pair.second.saveEdge(fout);
		}

	}

	void loadMap(ifstream& fin, NetworkMap& networks) {
		string str;
		getline(fin, str);
		if (str == "") {
			return;
		}

		else {
			networks.setLastId(stoi(str));
			for (int i = 1; i < stoi(str); i++) {
				string id;
				getline(fin, id);
				myMap[stoi(id)].loadEdge(fin);
			}
		}
	}

	void topSort(int id) {
		myMap[id].topologicalSort();
	}

	void delEdge(int n_id, int p_id) {
		myMap[n_id].deleteEdge(p_id);
	}

	map<int, OilNetwork>::iterator find(int key) {
		return myMap.find(key);
	}

	map<int, OilNetwork>::iterator end() {
		return myMap.end();
	}

	map<int, OilNetwork>::iterator begin() {
		return myMap.begin();
	}

private:
	map<int, OilNetwork> myMap;
	int currentKey = 1;

	int getNextKey() {
		return currentKey++;
	}
};
