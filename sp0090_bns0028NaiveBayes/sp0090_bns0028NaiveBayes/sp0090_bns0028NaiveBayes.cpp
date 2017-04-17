//
//  sp0090_bns0028NaiveBayes.cpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Sarvagya Pant on 4/16/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include "sp0090_bns0028NaiveBayes.hpp"

NaiveBayesClassifier::NaiveBayesClassifier(vvs& table, msvs& table_info, int start_index, int end_index){
    this->table = table;
    this->table_info = table_info;
    this->start_index = start_index;
    this->end_index = end_index;
    class_attribute = table[0][table[0].size()-1];
    class_values = table_info.find(class_attribute)->second;
}

void NaiveBayesClassifier::set_train_test_indexes(){
    for(int i=start_index; i <= end_index; i++){
        test_table_indexes.push_back(i);
    }
    for(int i=0; i < table.size(); i++){
        if(i<start_index || i>end_index){
            train_table_indexes.push_back(i);
        }
    }
}

