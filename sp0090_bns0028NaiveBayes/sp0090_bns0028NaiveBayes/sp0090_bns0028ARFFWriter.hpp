//
//  sp0090_bns0028ARFFWriter.hpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Sarvagya Pant on 4/16/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#ifndef sp0090_bns0028ARFFWriter_hpp
#define sp0090_bns0028ARFFWriter_hpp

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

using namespace std;

class ArffWriter
{
private:
    std::ofstream outFile;
public:
    ArffWriter(std::string);
    
    ~ArffWriter() {};
    
    void single_line_writer(std::string);
    
    void relation_writer(std::string);
    
    void attributes_writter(std::vector<std::string>, std::vector<std::string>);
    
    void data_writer(std::vector<std::vector<string> >, std::vector<string>, bool includeClass = false, std::string delimeter = " " );
    
};

#endif /* sp0090_bns0028ARFFWriter_hpp */
