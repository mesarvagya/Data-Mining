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
    
    std::vector<float> accuracy_list;
    vs all_predictions;
    
    for(int i=0; i < folds; i++){
        // naive bayes object here
        int start_index = i * test_number_record + i + 1;
        int end_index = (i+1) * test_number_record + i + 1;
        
        if(i == folds - 1)
            end_index = num_of_records;
        
        std::cout << start_index << " " <<end_index << std::endl;
        
        NaiveBayesClassifier nbc(data, tableInfo,start_index, end_index);
        nbc.set_train_test_indexes();
        nbc.generate_model();
        float accuracy = nbc.get_accuracy();
        accuracy_list.push_back(accuracy);
        vs predictions = nbc.get_predictions();
        
        for(int i=0; i< predictions.size(); i++){
            all_predictions.push_back(predictions[i]);
        }
        
        std::cout << accuracy << std::endl;
    }
    
    int correct_predictions = 0;
    for(int i=0; i < data.size()-1; i++){
        vs row_data = data[i];
        std::string correct_class = row_data[row_data.size()-1];
        std::string predicted_class = all_predictions[i];
        if(correct_class == predicted_class){
            correct_predictions++;
        }
    }
    
    float overall_accuracy = (float)correct_predictions/(float)num_of_records;
    std::cout<<overall_accuracy<<std::endl;
    return 0;
}
