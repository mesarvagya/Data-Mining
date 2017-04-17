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
public:
    NaiveBayesClassifier(vvs&, msvs&, int, int); // table, table_info_map, start_index and end_index of the data
};

#endif /* sp0090_bns0028NaiveBayes_hpp */
