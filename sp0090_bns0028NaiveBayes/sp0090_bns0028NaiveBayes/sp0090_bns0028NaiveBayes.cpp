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
}
