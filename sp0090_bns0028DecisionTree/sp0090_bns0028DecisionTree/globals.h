//
//  globals.h
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include <vector>
#include <string>
#include <map>

typedef std::vector<std::string> vs;
typedef std::vector<int> vi;
typedef std::vector<std::vector<std::string>> vvs;
typedef std::map<std::string, int> msi;
typedef std::vector<double> vd;

typedef struct node {
    std::string splitOn;
    std::string label;
    bool isLeaf;
    vs childrenValues;
    std::vector<node*> children;
} Node;
#endif /* globals_h */
