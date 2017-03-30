//
//  sp0090_bns0028DecisionTree.cpp
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include "sp0090_bns0028DecisionTree.h"

DecisionTree::DecisionTree(vvs table, Node* root){
    this->table = table;
    this->root = root;
}

Node* DecisionTree::buildDecisionTree(vvs &table, Node* nodePtr, vvs &tableInfo){
    if(isTableEmpty(table)){
        return NULL;
    }
    if(isDataHomogenous(table)){
        nodePtr->isLeaf = true;
        nodePtr->label = table[1][table[1].size()-1];
        return nodePtr;
    }
    else{
        // decide good splitting condition here.
        return NULL;
    }
}

bool DecisionTree::isTableEmpty(vvs &table){
    return (table.size() == 1);
}

bool DecisionTree::isDataHomogenous(vvs &table){
   	int iii;
    int lastCol = table[0].size() - 1;
    std::string firstValue = table[1][lastCol];
    for (iii = 1; iii < table.size(); iii++) {
        if (firstValue != table[iii][lastCol]) {
            return false;
        }
    }
    return true;
}

vvs DecisionTree::getTableInfo(){
    vvs tableInfo;
    for (int iii = 0; iii < table[0].size(); iii++) {
    vs tempInfo;
    msi tempMap;
    for (int jjj = 0; jjj < table.size(); jjj++) {
        if (tempMap.count(table[jjj][iii]) == 0) {
            tempMap[table[jjj][iii]] = 1;
            tempInfo.push_back(table[jjj][iii]);
                } else	{
                    tempMap[table[jjj][iii]]++;
                }
            }
            tableInfo.push_back(tempInfo);
        }
    return tableInfo;
}
