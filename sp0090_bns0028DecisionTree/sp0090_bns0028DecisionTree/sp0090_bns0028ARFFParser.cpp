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
            relationName = line;
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
            if(pos != std::string::npos){
                std::string attrLine = line.substr(line.find_first_of(" \t") + 1);
                std::string attrName = attrLine.substr(0, attrLine.find_first_of(" \t"));
                attribute_names.push_back(attrName);
                std::string attrVal = attrLine.substr(attrName.size());
                attributeVals.push_back(attrVal);
                
                std::cout << "Attribute is : " << attrName << "\t" << attrVal  << "\t" << std::endl;
                // size_t final_index = attrVal.find_first_of("}");
                // size_t read_index = 0L;
                // std::cout << "Val start end is : " << read_index << final_index << std::endl;
                
                std::stringstream ss(attrVal);
                std::string i;
                vs attrValV;
                attrVal = attrVal.substr(attrVal.find_first_of(" ") + 1);
                // std::cout << "here " << attrVal << std::endl;
                while (attrVal.length() > 2)
                {
                    attrVal = attrVal.substr(attrVal.find_first_not_of("{, "));
                    std::string val = attrVal.substr(0, attrVal.find_first_of(",}"));
                    // std::cout << "Val is : " << val << std::endl;
                    attrVal = attrVal.substr(attrVal.find_first_of(",}")+1);
                    attrValV.push_back(val);
                }
                attributeInfoMap[attrName] = attrValV;
                
            }
            
            
            /*
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
            }*/
            
        }
        
        if(data_begin){
            int class_col = attribute_names.size() - 1;
            std::istringstream ss(line);
            vs row_data;
            int counter = 0;
            do{
                std::string sub;
                ss >> sub;
                
                if(!sub.empty()){
                    row_data.push_back(sub);
                    if(counter == class_col){
                        classData.push_back(sub);
                    }
                }
                counter++;
                
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
    main_table_data = table_data;
    return table_data;
}

