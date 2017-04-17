//
//  sp0090_bns0028NaiveBayes.hpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Sarvagya Pant on 4/16/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#ifndef sp0090_bns0028NaiveBayes_hpp
#define sp0090_bns0028NaiveBayes_hpp

#include "globals.hpp"

class NaiveBayesClassifier{
private:
    vvs table;
    msvs table_info;
    int start_index;
    int end_index;
    vs predictions;
    
    vi test_table_indexes;
    vi train_table_indexes;
    std::string class_attribute;
    vs class_values;
    
public:
    NaiveBayesClassifier(vvs&, msvs&, int, int); // table, table_info_map, start_index and end_index of the data
    void set_train_test_indexes();
};

#endif /* sp0090_bns0028NaiveBayes_hpp */
