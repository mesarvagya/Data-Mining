//
//  main.cpp
//  DataMiningAssignment_1
//
//  Created by Sarvagya Pant on 1/27/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#import <cmath>

class ARFFParser{
    
private:
    std::string input_file;
    std::string output_file;
    std::string class_name;
    bool data_begin;
    bool header_begin;
    std::vector<std::string> header_data;
    std::vector<std::vector<float>> data;
    int attrib_count;
    std::vector<float> compute_mean_std(std::vector<float>data);
    int skip_attribute_number;
    std::vector<float> mean_vector;
    std::vector<float> std_vector;
    
public:
    ARFFParser(std::string input_file, std::string output_file, std::string class_name);
    void print_header();
    void print_data();
    void parse();
    void computeZScoreNormalization();
    
};

ARFFParser::ARFFParser(std::string input_file, std::string output_file, std::string class_name){
    this->input_file = input_file;
    this->output_file = output_file;
    this->class_name  = class_name;
    data_begin = false;
    header_begin = true;
}


void ARFFParser::parse(){
    std::string line;
    std::ifstream filestream(input_file);
    attrib_count = 0;
    skip_attribute_number = 0;
    std::string attribute_class = "@attribute " + std::string(class_name);
    while (std::getline(filestream, line)){
        if(line.empty())
            continue;
        if(line[0] == '%'){
            continue;
        }
        std::size_t pos = line.find("@relation");
        if(pos != std::string::npos ){
            header_data.push_back(line);
            header_begin = true;
            continue;
        }
        pos = line.find("@data");
        if(pos != std::string::npos){
            header_data.push_back(line);
            data_begin = true;
            header_begin = false;
            continue;
        }
        if(header_begin){
            std::size_t pos = line.find("@attribute");
            std::size_t pos2 = line.find(attribute_class);
            
            if(pos != std::string::npos && pos2 != std::string::npos){
                skip_attribute_number++;
                attrib_count++;
            }
            else if (pos != std::string::npos){
                skip_attribute_number++;
                attrib_count++;
            }
            header_data.push_back(line);
        }
        
        if(data_begin){
            std::istringstream ss(line);
            std::vector<float> row_data;
            do{
                std::string sub;
                ss >> sub;
                if(!sub.empty())
                    row_data.push_back(std::stof(sub));
            }while(ss);
            data.push_back(row_data);
        }
    }
    
}

void ARFFParser::print_header(){
    for(auto it = header_data.begin(); it != header_data.end(); ++it){
        std::cout << *it << std::endl;
    }
}
void ARFFParser::print_data(){
    for(int i(0); i < attrib_count; i++){
        for(auto row: data){
            std::cout << row.at(i) << std::endl;
        }
    }
}

void ARFFParser::computeZScoreNormalization(){
    //std::cout << "Attrib count " << attrib_count << std::endl;
    std::vector<std::vector<float>> computed_values;
    for(int i(0); i < attrib_count; i++){
        if(i == skip_attribute_number - 1){
            std::vector<float> temp_data;
            for(auto row: data){
                temp_data.push_back(row.at(i));
            }
            computed_values.push_back(temp_data);
        }
        else{
            std::vector<float> temp_data;
            for(auto row: data){
                temp_data.push_back(row.at(i));
            }
            computed_values.push_back(compute_mean_std(temp_data));
        }
    }
    
    std::ofstream off(output_file);
    std::ofstream off_mean_std("meanstd.arff");
    
    for(auto row: header_data){
        off << row;
    }
    
    for(auto row: header_data){
        off_mean_std << row;
    }
    //std::cout << "Writing normalized data" << std::endl;
    for(int j(0); j < computed_values[0].size(); j++){
        for(int i(0); i < attrib_count; i++){
            off << computed_values.at(i).at(j) << " ";
        }
        off << "\n";
    }
    
    for(auto item: mean_vector){
        off_mean_std << item << " ";
    }
    off_mean_std << "\n";
    for(auto item: std_vector){
        off_mean_std << item << " ";
    }
    off_mean_std << "\n";
    off.close();
    off_mean_std.close();
}

std::vector<float> ARFFParser::compute_mean_std(std::vector<float>data){
    float sum1(0), sum2(0), variance, sigma;
    for(auto it: data){
        sum1 += it;
    }
    int size = data.size();
    float mean = sum1/size;
    mean_vector.push_back(mean);
    for(auto it: data){
        sum2 += pow((it-mean),2);
    }
    variance = sum2/(size);
    sigma = sqrt(variance);
    std_vector.push_back(sigma);
    std::vector<float> temp;
    for(auto it: data){
        temp.push_back((it-mean)/sigma);
    }
    return temp;
}
int main(int argc, const char * argv[]) {
    
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -c <class_name>" << std::endl;
        exit(1);
    }
    std::string class_name, file_name;
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
    }
    
    ARFFParser a = ARFFParser(file_name, "output.arff", class_name);
    a.parse();
    a.computeZScoreNormalization();
    return 0;
}
