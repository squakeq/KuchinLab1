#pragma once
#include "Header.h"

class Compress_station {
public:
	Compress_station()
		: name("CS"), shops_num(0), busy_shops_num(0), efficiency(0) {}

	std::string getName() const {
		return name;
	}

	int getShops_num() const {
		return shops_num;
	}

	int getBusy_shops_num() const {
		return busy_shops_num;
	}

	int getEfficiency() const {
		return efficiency;
	}

	double getBusy() const {
		return  float(busy_shops_num) / float(shops_num) * 100;
	}

	void setName(string newName) {
		name = newName;
	}

	void setShopsNum(int newNum) {
		shops_num = newNum;
	}

	void setBusyShopNum(int newBusy) {
		busy_shops_num = newBusy;
	}

	void setEfficiency(int newEfficiency) {
		efficiency = newEfficiency;
	}

	void EditShops(string action, int shops) {
		if (action == "activate") {
			if (shops <= (shops_num - busy_shops_num)) {
				busy_shops_num += shops;
			}
			else {
				cout << "Invalid shops amount, try again.\n";
			}
		}

		else {
			if (shops <= busy_shops_num) {
				busy_shops_num -= shops;
			}
			else {
				cout << "Invalid shops amount, try again.\n";
			}
		}
	}

private:
	string name;
	int shops_num;
	int busy_shops_num;
	int efficiency;
};

class CSMap {
public:
	void addElement(const Compress_station& value) {
		int newKey = getNextKey();
		myMap[newKey] = value;
	}

	void removeElement(int key) {
		myMap.erase(key);
	}

	void clear() {
		myMap.clear();
		currentKey = 1;
	}

	void addDirectly(int key, Compress_station& cs) {
		myMap[key] = cs;
	}

	int getLastId() {
		return currentKey;
	}

	void setLastId(int id) {
		currentKey = id;
	}

	vector<int> searchElements(string filter) {
		vector<int> result;
		if (filter == "name") {
			string name;
			cout << "Type cs name to search: ";
			getline(cin, name);
			for (auto& pair : myMap) {
				if (pair.second.getName().find(name) != string::npos) {
					result.push_back(pair.first);
				}
			}
		}

		else {
			string percent;
			cout << "Type percent (integer) for search: ";
			getline(cin, percent);
			while (true) {
				if (is_digit(percent) && stoi(percent) >= 0) {
					break;
				}
				else {
					cout << "Invalid input. Please enter a positive integer." << endl;
				}
			}

			string parameter;
			while (true) {
				cout << "Type parameter to search (lower/higher): ";
				getline(cin, parameter);
				if (parameter == "lower" or parameter == "higher") {
					break;
				}

				else {
					cout << "Invalid input. Please enter 'higher' or 'lower'." << endl;
				}
			}

			for (auto& pair : myMap)
			{
				if (parameter == "lower") {
					if (pair.second.getBusy() < stoi(percent)) {
						result.push_back(pair.first);
					}
				}
				else {
					if (pair.second.getBusy() > stoi(percent)) {
						result.push_back(pair.first);
					}
				}
			}

		}

		return result;
	}

	void printElements() const {
		cout << "Compress Stations:\n";
		for (const auto& pair : myMap) {
			cout << "CS id: " << pair.first << "\n"
				<< "CS name: " << pair.second.getName() << "\n"
				<< "CS shops amount: " << pair.second.getShops_num() << "\n"
				<< "CS busy shops amount: " << pair.second.getBusy_shops_num() << "\n"
				<< "CS efficiency: " << pair.second.getEfficiency() << "\n"
				<< "----------------\n";
		}
	}

	void printOneElement(int id) {
		auto it = myMap.find(id);

		if (it != myMap.end()) {
			Compress_station pair = it->second;
			cout << "CS id: " << it->first << "\n"
				<< "CS name: " << pair.getName() << "\n"
				<< "CS shops amount: " << pair.getShops_num() << "\n"
				<< "CS busy shops amount: " << pair.getBusy_shops_num() << "\n"
				<< "CS effciency: " << pair.getEfficiency() << "\n"
				<< "----------------\n";
		}
		else {
			std::cerr << "CS with id " << id << " not found." << std::endl;
			return;
		}
	}

	void saveElements(ofstream& fout) const {
		for (const auto& pair : myMap) {
			fout << pair.first << "\n"
				<< pair.second.getName() << "\n"
				<< pair.second.getShops_num() << "\n"
				<< pair.second.getBusy_shops_num() << "\n"
				<< pair.second.getEfficiency() << "\n";
		}
	}

	void loadElements(ifstream& fin, CSMap& stations) const {
		string str;
		getline(fin, str);
		if (str != "OilNetworks:")
		{
			stations.setLastId(stoi(str));
			while (true)
			{
				getline(fin, str);
				if (str == "OilNetworks:") {
					break;
				}
				Compress_station new_cs;
				string name;
				getline(fin, name);
				new_cs.setName(name);
				string shops_num;
				getline(fin, shops_num);
				new_cs.setShopsNum(stoi(shops_num));
				string busy_shops_num;
				getline(fin, busy_shops_num);
				new_cs.setBusyShopNum(stoi(busy_shops_num));
				string efficiency;
				getline(fin, efficiency);
				new_cs.setEfficiency(stoi(efficiency));
				stations.addDirectly(stoi(str), new_cs);
			}
		}

	}

	map<int, Compress_station>::iterator find(int key) {
		return myMap.find(key);
	}

	map<int, Compress_station>::iterator end() {
		return myMap.end();
	}

private:
	map<int, Compress_station> myMap;
	int currentKey = 1;

	int getNextKey() {
		return currentKey++;
	}
};