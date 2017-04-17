//
//  main.cpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Sarvagya Pant on 4/16/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>
#include "sp0090_bns0028ARFFParser.hpp"
#include "sp0090_bns0028NaiveBayes.hpp"

int main(int argc, const char * argv[]) {
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -c <class attribute> -K <# of folds>" << std::endl;
        exit(1);
    }
    
    std::string class_name = "undefined_class_name";
    std::string file_name;
    int folds = 10; //default 10
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-K")
            folds = std::stoi(argv[i+1]);
    }
    
    ARFFParser arff(file_name, class_name);
    arff.parse();
    
    vvs data = arff.get_tabular_data();
    msvs tableInfo = arff.get_info_map();
    vs classData = arff.get_class_data();
    
    int num_of_records = (int) data.size() - 1;
    int test_number_record = num_of_records / folds;
    
    for(int i=0; i < folds; i++){
        // naive bayes object here
        int start_index = i * test_number_record;
        int end_index = (i+1) * test_number_record;
        NaiveBayesClassifier nbc(data, tableInfo, start_index, end_index);
        nbc.set_train_test_indexes();
        
        
        
    }
    
    return 0;
}
