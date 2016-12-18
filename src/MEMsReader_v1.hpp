//=================================
// include guard
#ifndef MEMsReader
#define MEMsReader

//=================================
// forward declared dependencies
class MemsList;

//=================================
// included dependencies
#include <string>
#include <forward_list>
#include <vector>
#include <iostream>
#include <fstream>

#include "MEMsList.hpp"

class MemsReader {
private:
    std::ifstream memsFile;
    std::forward_list<std::pair<std::string, MemsList> > patterns;
    std::vector<int> extractMEM(std::string line);
public:
  MemsReader(const std::string& fpath);
    void addPattern(const std::string& pattern_id, const int& pattern_length, std::forward_list<std::vector<int> > MEMs);
    void readMEMsFile(const int& max);
    void print();
};

#endif
