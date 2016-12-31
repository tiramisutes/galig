#include "ReferenceGraph.hpp"

std::vector<int> ReferenceGraph::extractEdge(std::string line) {
    std::vector<int> edge (2,0);
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while((pos = line.find(delimiter)) != std::string::npos) {
	token = line.substr(0, pos);
	edge[0] = stoi(token);
	line.erase(0, pos + delimiter.length());
    }
    edge[1] = stoi(line);
    
    return edge;
}

std::vector<int> ReferenceGraph::extractExonsLengths(const std::string& fpath) {
    std::vector<int> e_lens;
    std::string line;
    std::ifstream memsFile(fpath);
    if (memsFile.is_open()) {
	while(getline(memsFile,line)) {
	    e_lens.push_back(stoi(line));
	}
	memsFile.close();
    }
    else {
	std::cout << "Unable to open exons file" << std::endl;
    }   
    return e_lens;
}

void ReferenceGraph::setupEdges(const std::string& fpath, int nex) {
    std::string line;
    edges.resize(nex+1);
    for(int i = 0; i <= nex; i++) {
	edges[i] = std::vector< int>(nex, 0);
    }
    std::ifstream edgesFile(fpath);
    if (edgesFile.is_open()) {
	while(getline(edgesFile,line)) {
	    std::vector<int> edge = extractEdge(line);
	    edges[edge[0]][edge[1]] = 1;
	}
	edgesFile.close();
    }
    else {
	std::cout << "Unable to open edges file" << std::endl;
    }
}

int ReferenceGraph::setupBitVector(const std::string& fpath) {
    std::vector<int> e_lens = extractExonsLengths(fpath);
    int tot_L = 1;
    for(int l:e_lens) {
	tot_L += l+1;
    }
    sdsl::bit_vector BV (tot_L, 0);
    int i = 0;
    BV[i] = 1;
    for(int l:e_lens) {
	i += l+1;
	BV[i] = 1;
    }
    bitVector = sdsl::rrr_vector<>(BV);
    select_BV = sdsl::rrr_vector<>::select_1_type(&bitVector);
    rank_BV = sdsl::rrr_vector<>::rank_1_type(&bitVector);
    return e_lens.size();
}

int ReferenceGraph::rank(const int& i) {
    return rank_BV(i);
}

int ReferenceGraph::select(const int& i) {
    return select_BV(i);
}

bool ReferenceGraph::contain(std::vector<int> edge) {
    if(edges[edge[0]][edge[1]] == 1) {
	return true;
    } else {
	return false;
    }
}

ReferenceGraph::ReferenceGraph(const std::string& exons_file_path, const std::string& edges_file_path) {
    int nex = setupBitVector(exons_file_path);
    setupEdges(edges_file_path, nex);
}
