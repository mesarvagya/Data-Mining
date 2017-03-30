//
//  main.cpp
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>
#include "sp0090_bns0028ARFFParser.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -c <class attribute> -T <% of sample to take as training>" << std::endl;
        exit(1);
    }
    
    std::string class_name = "undefined_class_name";
    std::string file_name;
    float percentage = 0;
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-T")
            percentage = std::stof((argv[i+1]));
    }
    
    ARFFParser arrf(file_name, class_name);
    arrf.parse();
    vvs table_data = arrf.get_tabular_data();
    return 0;
}
