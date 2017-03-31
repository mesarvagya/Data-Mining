//
//  rp0037ArffWriter.hpp
//  cs641
// Ritesh Pradhan
//  Created by Ritesh Pradhan on 1/27/16.
//  Copyright © 2016 Ritesh Pradhan. All rights reserved.
//

#ifndef rp0037ArffWriter_hpp
#define rp0037ArffWriter_hpp

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

using namespace std;
/**
 * FileWriter writes the contents to the arff file format
 */

class ArffWriter
{
private:
    std::ofstream outFile;     //Output File Stream
public:
    
    //Default Constructor
    ArffWriter(std::string);
    
    //Destructor
    ~ArffWriter() {};
    
    /**
     *   Write relationName to output arff file
     *   @param  string  - input relationName to write
     */
    void writeRelation(std::string);
    
    /**
     *   Write relationName to output arff file
     *   @param  vector<string>  - vector of attributes name
     *   @param  vector<string>  - vector of attributes values type
     */
    void writeAttributes(std::vector<std::string>, std::vector<std::string>);
    
    /**
     *   Write relationName to output arff file
     *   @param  vector<vector<double> >  - 2D vector of double (@data) to write to arff file
     *   @param  vector<double>  - class Attributes to add to each sample data
     *   @param  string  delimeter - Delimeter to separate each data elements
     *   @param  bool includeClass  - boolean value to suggest weather to include class in output file
     */
    void writeData(std::vector<std::vector<string> >, std::vector<string>, bool includeClass = false, std::string delimeter = " " );
    
    void writeSingleLine(std::string);
    
};

#endif /* rp0037ArffWriter_hpp */
