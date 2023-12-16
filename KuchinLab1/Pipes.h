#pragma once
#include "Header.h"

class PipeMap {
public:
	void addElement(const Pipe& value) {
		int newKey = getNextKey();
		myMap[newKey] = value;
	}

	void removeElement(int key, NetworkMap& networks) {
		myMap.erase(key);
		for (auto& pair : networks) {
			for (auto& edges : pair.second.getEdgeMat())
				if (edges.first == key) {
					pair.second.deleteEdge(key);
				}
		}
	}

	void clear() {
		myMap.clear();
		int currentKey = 1;
	}

	void addDirectly(int key, Pipe& pipe) {
		myMap[key] = pipe;
	}

	int getLastId() {
		return currentKey;
	}

	void setLastId(int id) {
		currentKey = id;
	}

	void printOneElement(int id) {
		auto it = myMap.find(id);

		if (it != myMap.end()) {
			Pipe pair = it->second;
			cout << "Pipe id: " << it->first << "\n"
				<< "Pipe name: " << pair.getName() << "\n"
				<< "Pipe length: " << pair.getLength() << "\n"
				<< "Pipe diameter: " << pair.getDiameter() << "\n"
				<< std::boolalpha
				<< "Pipe repairing: " << pair.isRepairing() << "\n"
				<< std::noboolalpha
				<< "----------------\n";
		}
		else {
			std::cerr << "Pipe with id " << id << " not found." << std::endl;
			return;
		}
	}

	void printElements() const {
		cout << "Pipes:\n";
		for (const auto& pair : myMap) {
			cout << "Pipe id: " << pair.first << "\n"
				<< "Pipe name: " << pair.second.getName() << "\n"
				<< "Pipe length: " << pair.second.getLength() << "\n"
				<< "Pipe diameter: " << pair.second.getDiameter() << "\n"
				<< std::boolalpha
				<< "Pipe repairing: " << pair.second.isRepairing() << "\n"
				<< std::noboolalpha
				<< "----------------\n";
		}
	}

	void saveElements(ofstream& fout) const {
		for (const auto& pair : myMap) {
			fout << pair.first << "\n"
				<< pair.second.getName() << "\n"
				<< pair.second.getLength() << "\n"
				<< pair.second.getDiameter() << "\n"
				<< pair.second.isRepairing() << "\n";
		}
	}

	void loadElements(ifstream& fin, PipeMap& pipes) const {
		string str;
		getline(fin, str);
		if (str == "Pipes:")
		{
			getline(fin, str);
			if (str == "CompressStations:")
			{
				return;
			}
			pipes.setLastId(stoi(str));
			while (true)
			{
				getline(fin, str);
				if (str == "CompressStations:")
				{
					break;
				}
				else {
					string name;
					getline(fin, name);
					string length;
					getline(fin, length);
					string diameter;
					getline(fin, diameter);
					string status;
					getline(fin, status);
					Pipe new_pipe;
					new_pipe.setName(name);
					new_pipe.setLength(stod(length));
					new_pipe.setDiameter(stoi(diameter));
					new_pipe.setStatus(stoi(status));
					pipes.addDirectly(stoi(str), new_pipe);
					continue;
				}
			}
		}

	}

	vector<int> searchElements(string filter) {
		vector<int> result;
		if (filter == "name") {
			string name;
			cout << "Type pipe name to search: ";
			getline(cin, name);
			for (auto& pair : myMap) {
				if (pair.second.getName().find(name) != string::npos) {
					result.push_back(pair.first);
				}
			}
		}

		else {

			string status;
			while (true) {
				cout << "Type filter for search (true/false): ";
				getline(cin, status);
				if (status == "true" or status == "false") {
					break;
				}

				else {
					cout << "Invalid input. Please enter 'true' or 'false'." << std::endl;
				}
			}
			bool bstatus;
			if (status == "true") {
				bstatus = true;
			}
			else {
				bstatus = false;
			}
			for (auto& pair : myMap) {
				if (pair.second.isRepairing() == bstatus) {
					result.push_back(pair.first);
				}

			}
		}
		return result;
	}

	int SearchByDiameter(int diameter, NetworkMap& stations) {
		for (auto& pair : myMap) {
			if (pair.second.getDiameter() == diameter and !(stations.searchPipe(pair.first))) {
				return pair.first;
			}
		}
		return 0;
	}

	map<int, Pipe>::iterator find(int key) {
		return myMap.find(key);
	}

	map<int, Pipe>::iterator end() {
		return myMap.end();
	}

private:
	map<int, Pipe> myMap;
	int currentKey = 1;

	int getNextKey() {
		return currentKey++;
	}
};

class Pipe {
public:
	Pipe()
		: name("Pipe"), length(0.0), diameter(0), is_repairing(false) {}

	string getName() const {
		return name;
	}

	double getLength() const {
		return length;
	}

	int getDiameter() const {
		return diameter;
	}

	bool isRepairing() const {
		return is_repairing;
	}

	void setName(string newName) {
		name = newName;
	}

	void setLength(double newLength) {
		length = newLength;
	}

	void setDiameter(int newDiam) {
		diameter = newDiam;
	}

	void setStatus(bool value) {
		is_repairing = value;
	}


private:
	string name;
	double length;
	int diameter;
	bool is_repairing;
};
