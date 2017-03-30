//
//  sp0090_bns0028DecisionTree.h
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#ifndef sp0090_bns0028DecisionTree_h
#define sp0090_bns0028DecisionTree_h
#include "globals.h"

class DecisionTree {
private:
    vvs table;
    Node* root;
    vvs tableInfo;
    
public:
    DecisionTree(vvs table, Node* root);
    vvs getTableInfo();
    Node* buildDecisionTree(vvs&, Node*, vvs&);
    bool isTableEmpty(vvs&);
    bool isDataHomogenous(vvs&);
    
};

#endif /* sp0090_bns0028DecisionTree_h */
