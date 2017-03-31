//
//  main.cpp
//  sp0090_bns0028DecisionTree
//
//  Created by Sarvagya Pant on 3/30/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>
#include "sp0090_bns0028ARFFParser.hpp"
#include "sp0090_bns0028DecisionTree.h"
#include "sp0090_bns0028ARRFWriter.hpp"

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
    msvs tableInfo = arrf.get_info_map();
    vs classData = arrf.get_class_data();
    
    Node *dt = new Node;
    DecisionTree decisionTree(table_data, tableInfo);
    std::cout << "Generating Decision Tree ..." << std::endl;
    dt = decisionTree.generateDecisionTree(table_data, tableInfo, dt);
    
    decisionTree.writeTreeToFile(dt, "/Users/sarvagya/Desktop/aalu.dt");
    
    vs predictions;
    for(int i=1; i<(int)table_data.size(); i++)
    {
        predictions.push_back(decisionTree.testDataOnDecisionTree(table_data[i], dt));
    }
    
    
    /* Write accuracy of the predictions from dt*/
    std::cout  << "Writing confusionMatrix and accuracy to file : " << "aalu" << "..."  << std::endl;
    std::string temp = "/Users/sarvagya/Desktop/kalu.dt";
    decisionTree.writeConfusionMatrix(temp, classData, predictions);
    
    
    /* Write to outfile all data along with predicted class as dtClass attribute */
    std::string dtApplicationOutFile = "/Users/sarvagya/Desktop/asdf.arff";
    std::cout  << "Writing predictions to file : " << dtApplicationOutFile  << "..." << std::endl;
    ArffWriter dtApplicationPredictedArff(dtApplicationOutFile);
    dtApplicationPredictedArff.writeRelation(arrf.get_relation_name());
    dtApplicationPredictedArff.writeAttributes(arrf.getAttributeNames(), arrf.getAttributeVals());
    string singleLine = string("@attribute dtClass real");
    dtApplicationPredictedArff.writeSingleLine(singleLine);
    dtApplicationPredictedArff.writeData(arrf.get_data(), predictions, true);


    
    
    return 0;
}
