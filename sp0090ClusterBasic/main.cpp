//
//  main.cpp
//  sp0090ClusterBasic
//
//  Created by Sarvagya Pant on 2/22/17.
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
    std::string class_name;
    bool normalize;
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
    ARFFParser(std::string input_file, std::string class_name, bool normalize);
    void print_header();
    void print_data();
    void parse();
    void computeZScoreNormalization();
    std::vector<std::string> get_header_data();
    std::vector<std::vector<float>> get_row_data();
    std::vector<std::vector<float>> get_row_data_normalized();
    
};

ARFFParser::ARFFParser(std::string input_file, std::string class_name, bool normalize){
    this->input_file = input_file;
    this->class_name  = class_name;
    this->normalize = normalize;
    data_begin = false;
    header_begin = true;
}


void ARFFParser::parse(){
    std::string line;
    std::ifstream filestream(input_file);
    attrib_count = 0;
    skip_attribute_number = 0;
    std::string attribute_class = "@attribute " + std::string(class_name);
    bool class_column_found = false;
    
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
            if(pos2 != std::string::npos){
                class_column_found = true;
                attrib_count++;
            }
            else if(pos != std::string::npos){
                if(!class_column_found){
                    attrib_count++;
                    skip_attribute_number++;
                }
                else
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
    std::vector<std::vector<float>> computed_values;
    for(int i(0); i < attrib_count; i++){
        if(i == skip_attribute_number){
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
    
    /*for(auto row: header_data){
        off << row;
    }
    */
    
    for(int j(0); j < computed_values[0].size(); j++){
        for(int i(0); i < attrib_count; i++){
            std::cout << computed_values.at(i).at(j) << " ";
        }
    }

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

std::vector<std::string> ARFFParser::get_header_data(){
    return header_data;
}

std::vector<std::vector<float>> ARFFParser::get_row_data(){
    std::vector<std::vector<float>> all_row_data;
    return all_row_data;
}

std::vector<std::vector<float>> ARFFParser::get_row_data_normalized(){
    std::vector<std::vector<float>> all_row_data;
    if(normalize){
        std::vector<std::vector<float>> computed_values;
        for(int i(0); i < attrib_count; i++){
            if(i == skip_attribute_number){
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
        
        for(int j(0); j < computed_values[0].size(); j++){
            std::vector<float> row_data;
            for(int i(0); i < attrib_count; i++){
                row_data.push_back(computed_values.at(i).at(j));
            }
            all_row_data.push_back(row_data);
            }

    }
    return all_row_data;
}

class KMeansBasic {
    
private:
    int clusters;
    std::string input_file;
    bool normalize;
    std::string class_name;
    std::vector<std::vector<float> > row_data;
    
public:
    KMeansBasic(std::string input_file, int clusters, bool normalize, std::string class_name);
    void setRowData(std::vector<std::vector<float> > row_data);
    void parseARFF();
};

KMeansBasic::KMeansBasic(std::string input_file, int clusters, bool normalize, std::string class_name){
    this->clusters = clusters;
    this->input_file = input_file;
    this->normalize = normalize;
    this->class_name = class_name;
}

void KMeansBasic::parseARFF(){
    ARFFParser arff(input_file,class_name, normalize);
    arff.parse();
    arff.get_row_data_normalized();
    
}

int main(int argc, const char * argv[]) {
    /*if(argc < 5){
        std::cout << "please run program as program -i <input_file> -k <Number_of_cluster> and other optional arguments (-normalize, -c)" << std::endl;
        exit(1);
    }
     */
    std::string class_name = "undefined_class_name";
    std::string file_name = "/Users/sarvagya/Downloads/kmtest.arff";
    bool normalize = true;
    int cluster = 4;
    /*for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-k")
            cluster = std::stoi(argv[i+1]);
        else if(std::string(argv[i]) == "-normalize")
            normalize = true;
    }*/
    std::cout << class_name << file_name << normalize << cluster << std::endl;
    KMeansBasic kmeans(file_name, cluster, normalize, class_name);
    kmeans.parseARFF();
    
    
    // ARFFParser a = ARFFParser(file_name, "sp0090Normalize" + file_name, class_name);
    //a.parse();
    //a.computeZScoreNormalization();
    return 0;
}
