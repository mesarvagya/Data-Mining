
#ifndef sp0090_bns0028ARFFParser_hpp
#define sp0090_bns0028ARFFParser_hpp

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <regex>

#include "globals.h"

class ARFFParser{
    
private:
    std::string input_file;
    std::string class_name;
    bool data_begin;
    bool header_begin;
    vs header_data;
    vvs data;
    int attrib_count;
    int skip_attribute_number;
    vs attribute_names;
    std::vector<std::string> attributeVals;
    msvs attributeInfoMap;
    vs classData;
    std::string relationName;
    vvs main_table_data;
    
public:
    ARFFParser(std::string input_file, std::string class_name);
    void print_header();
    void print_data();
    void parse();
    void computeZScoreNormalization();
    vs get_header_data();
    vvs get_row_data();
    vvs get_tabular_data();
    msvs get_info_map(){
        return attributeInfoMap;
    }
    vs get_class_data(){
        return classData;
    }
    std::string get_relation_name(){
        return relationName;
    }
    std::vector<std::string> getAttributeNames() { return attribute_names; }
    std::vector<std::string> getAttributeVals() { return attributeVals; }
    vvs get_data() { return main_table_data; }

    
};

#endif 
