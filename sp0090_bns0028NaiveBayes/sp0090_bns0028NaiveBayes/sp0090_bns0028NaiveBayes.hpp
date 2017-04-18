//
//  sp0090_bns0028NaiveBayes.hpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Bidhya N Sharma on 4/16/17.
//  Copyright © 2017 Bidhya. All rights reserved.
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
    float accuracy;
    
public:
    NaiveBayesClassifier(vvs&, msvs&, int, int); // table, table_info_map, start_index and end_index of the data
    void set_train_test_indexes();
    void generate_model();
    float get_accuracy();
    vs get_predictions();
};

#endif /* sp0090_bns0028NaiveBayes_hpp */
