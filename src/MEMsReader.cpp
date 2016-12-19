#include "MEMsReader.hpp"
#include "MEMsList.hpp"

MemsReader::MemsReader(const std::string& fpath) {
    memsFile.open(fpath);
}

void MemsReader::addPattern(const std::string& pattern_id, const int& pattern_length, std::forward_list<std::vector<int> > MEMs) {
    MemsList ml (pattern_length);
    /**
    while(!MEMs.empty()) {
	std::vector<int> m = MEMs.front();	
	MEMs.pop_front();
	ml.addMem(m[0], m[1], m[2]);
    }
    **/
    for(std::forward_list<std::vector<int> >::iterator it=MEMs.begin(); it != MEMs.end(); ++it) {
	std::vector<int> m = (*it);
	ml.addMem(m[0], m[1], m[2]);
    }
    std::pair<std::string, MemsList&> p (pattern_id, ml);
    patterns.push_front(p);
}

bool MemsReader::hasPattern() {
    return !patterns.empty();
}
std::pair<std::string, MemsList> MemsReader::popPattern() {
    std::pair<std::string, MemsList> p = patterns.front();
    patterns.pop_front();
    return p;
}

std::vector<int> MemsReader::extractMEM(std::string line) {
    std::vector<int> m (3);
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = line.find(delimiter)) != std::string::npos) {
	token = line.substr(0, pos);
	m[i] = stoi(token);
	i++;
	line.erase(0, pos + delimiter.length());
    }
    m[i] = stoi(line);
    return m;
}

int MemsReader::extractLength(std::string line) {
    int l = 0;
    std::string delimiter = "=";
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) {
	line.erase(0, pos + delimiter.length());
    }
    l = stoi(line);
    return l;
}

void MemsReader::readMEMsFile() {
    std::cout << "Reading patterns..." << std::endl;
    patterns.clear();
    const int& max = 1000000;
    bool flag = false;
    std::string line;
    std::string pattern_id;
    int pattern_length;
    std::forward_list<std::vector<int> > MEMs;
    int i = 0;
    if (memsFile.is_open()) {
	while(i<max && getline(memsFile,line)) {
	    if(line.length() != 0) {
		if(line.at(0) == '#') {
		    if(flag) {
			if(MEMs.empty()) {
			    MEMs.clear();
			    flag = false;
			}
			else {
			    //std::cout << "3" << std::endl;
			    addPattern(pattern_id, pattern_length, MEMs);
			    i++;
			    std::cout << "Pattern " << i << std::endl;
			    MEMs.clear();
			    flag = false;
			}
		    }
		    else {
			if(line.substr(0,7).compare("# P.len") == 0) {
			    //std::cout << "1" << std::endl;
			    pattern_length = extractLength(line);
			}
		    }
		}
		if(flag) {
		    std::vector<int> mem = extractMEM(line);
		    MEMs.push_front(mem);
		}
		if(line.at(0) == '>') {
		    //std::cout << "2" << std::endl;
		    pattern_id = line.substr(2, line.length());
		    flag = true;
		}
	    }
	}
	if(memsFile.eof()) {
	    std::cout << "All patterns read" << std::endl;
	    memsFile.close();
	}
    }
    else {
	std::cout << "Unable to open MEMs file" << std::endl;
    }
}

void MemsReader::print() {
    std::cout << "Printing patterns..." << std::endl;
    for(std::forward_list<std::pair<std::string, MemsList> >::iterator it=patterns.begin(); it != patterns.end(); ++it) {
	std::pair<std::string, MemsList> pattern = (*it);
	std::cout << pattern.first << std::endl;
	MemsList MEMs = pattern.second;
	int i = 0;
	while(i<MEMs.getLength()) {
	    std::forward_list<Mem> l = MEMs.getMems(i);
	    std::cout << i << std::endl;
	    for(std::forward_list<Mem>::iterator it1=l.begin(); it1 != l.end(); ++it1) {
		Mem m = (*it1);
		std::cout << m.toStr() << std::endl;
	    }
	    i++;
	}
    }
}
