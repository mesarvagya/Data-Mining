//
//  sp0090_bns0028ARFFParser.cpp
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include "sp0090_bns0028ARFFParser.hpp"


ARFFParser::ARFFParser(std::string input_file, std::string class_name){
    this->input_file = input_file;
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
    bool class_column_found = false;
    std::regex regex("\\@attribute\\s+(\\w+)\\s+\\{");
    std::smatch match;
    
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
            // header_data.push_back(line);
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
                if (std::regex_search(line, match, regex) && match.size() > 1) {
                    attribute_names.push_back(match.str(1));
                } else {
                    //result = std::string("");
                }
            }
            else if(pos != std::string::npos){
                if(!class_column_found){
                    attrib_count++;
                    skip_attribute_number++;
                    header_data.push_back(line);
                    if (std::regex_search(line, match, regex) && match.size() > 1) {
                        attribute_names.push_back(match.str(1));
                    } else {
                        //result = std::string("");
                    }
                }
                else
                    attrib_count++;
            }
        }
        
        if(data_begin){
            std::istringstream ss(line);
            vs row_data;
            do{
                std::string sub;
                ss >> sub;
                if(!sub.empty()){
                    row_data.push_back(sub);
                }
                
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


std::vector<std::string> ARFFParser::get_header_data(){
    return header_data;
}

vvs ARFFParser::get_row_data(){
    return data;
}

vvs ARFFParser::get_tabular_data(){
    vvs table_data;
    table_data.push_back(attribute_names);
    for(auto row: data){
        table_data.push_back(row);
    }
    return table_data;
}