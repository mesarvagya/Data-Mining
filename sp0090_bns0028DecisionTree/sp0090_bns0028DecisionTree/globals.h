//
//  globals.h
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <iomanip>

typedef std::vector<std::string> vs;
typedef std::vector<int> vi;
typedef std::vector<std::vector<std::string>> vvs;
typedef std::map<std::string, int> msi;
typedef std::vector<double> vd;
typedef std::map<std::string, std::vector<std::string>> msvs;

/*
typedef struct node {
    std::string splitOn;
    std::string label;
    bool isLeaf;
    vs childrenValues;
    std::vector<node*> children;
} Node;
 */
struct node {
    std::string splitOn;
    std::string label;
    bool isLeaf;
    vs childrenValues;
    std::vector<node*> children;
};
#endif /* globals_h */
