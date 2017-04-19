//
//  sp0090_bns0028NaiveBayes.cpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Bidhya N Sharma on 4/16/17.
//  Copyright © 2017 Bidhya. All rights reserved.
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


float NaiveBayesClassifier::get_accuracy(){
    return this->accuracy;
}

vs NaiveBayesClassifier::get_predictions(){
    return this->predictions;
}

vs NaiveBayesClassifier::get_correctlabels(){
    return this->correct_labels;
}

void NaiveBayesClassifier::generate_model(){
    
    smsmsi attr_class_count;
    
    //initialize tha all_model_map;
    vs class_types = this->table_info.find(this->class_attribute)->second;
    
    for(auto const &attr_name : this->table_info) {
        // ent1.first is the first key
        std::string attr_string = attr_name.first;
        vs attr_types = attr_name.second;
        
        if(attr_string == this->class_attribute){
            continue;
        }
       
        for(int i=0; i< attr_types.size(); i++){
            std::string a_type = attr_types[i];
            
            for(int j=0; j<class_types.size(); j++){
                std::string c_type = class_types[j];
                attr_class_count[attr_string][a_type][c_type] = 0;
            }
            
        }
        
    }
    
    msi class_count_map;
    //class count map intialize all to zero
    for(int i=0 ; i<class_values.size(); i++){
        class_count_map[class_values[i]] = 0;
    }
    
    //training process (couting the number of class values for each attrbute type
    vs attr_names = table[0];
    for(int i=0; i<train_table_indexes.size(); i++){
        
        int data_index = train_table_indexes[i];
        vs row_data = table[data_index];
        std::string c_type = row_data[row_data.size()-1];
        class_count_map[c_type]++;
        
        for(int j=0; j<row_data.size()-1; j++){
            std::string attr_string = attr_names[j];
            std::string a_type = row_data[j];
            attr_class_count[attr_string][a_type][c_type]++;
        }
    }
    
    int train_data_count = static_cast<int>(train_table_indexes.size());
    int test_data_count = static_cast<int>(test_table_indexes.size());
    int correct_prediction = 0;
    
    //Testing
    for(int i=0; i<test_table_indexes.size(); i++){
        
        int data_index = test_table_indexes[i];
        vs test_row_data = table[data_index];
        std::string correct_label = test_row_data[test_row_data.size()-1];
        this->correct_labels.push_back(correct_label);
        
        float predicted_probab = 0;
        std::string predicted_class;
        
        for(int j=0; j<class_values.size(); j++){
            
            std::string c_type = class_values[j];
            int temp_class_count = class_count_map[class_values[j]];
            float class_proabs = (float)temp_class_count/(float)train_data_count;
            float attr_probabs = 1;
            
            for(int k=0; k<test_row_data.size()-1; k++){
                std::string attr_string = attr_names[k];
                std::string a_type = test_row_data[k];
                int temp_count = attr_class_count[attr_string][a_type][c_type];
                attr_probabs *= ((float)temp_count/(float)temp_class_count);
                
            }//attribute iteration ends
            
            float total_probabs = attr_probabs * class_proabs;
            
            if(total_probabs > predicted_probab){
                predicted_probab = total_probabs;
                predicted_class = c_type;
            }
            
        }//class iteration ends
        
        this->predictions.push_back(predicted_class);
        if(correct_label == predicted_class){
            correct_prediction++;
        }
        
    }//testing loop ends
    
    this->accuracy = (float)correct_prediction/(float)test_data_count;
}
