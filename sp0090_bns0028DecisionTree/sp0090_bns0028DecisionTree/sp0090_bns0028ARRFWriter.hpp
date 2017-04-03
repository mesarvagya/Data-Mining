

/**
 sp0090_bns0028ARRFWriter.hpp
 Sarvagya Pant
 Bidhya Nandan Sharma
 April 3, 2017
 **/

#ifndef sp0090_bns0028ArffWriter_hpp
#define sp0090_bns0028ArffWriter_hpp

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

#endif 
