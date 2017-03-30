//
//  sp0090_bns0028ARFFParser.hpp
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

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
    
public:
    ARFFParser(std::string input_file, std::string class_name);
    void print_header();
    void print_data();
    void parse();
    void computeZScoreNormalization();
    vs get_header_data();
    vvs get_row_data();
    vvs get_tabular_data();
    
};

#endif /* sp0090_bns0028ARFFParser_hpp */
