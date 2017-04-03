
#include <iostream>
#include "sp0090_bns0028ARFFParser.hpp"
#include "sp0090_bns0028DecisionTree.h"
#include "sp0090_bns0028ARRFWriter.hpp"

int main(int argc, const char * argv[]) {
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -c <class attribute> -T <% of sample to take as training>" << std::endl;
        exit(1);
    }
    
    std::string class_name = "undefined_class_name";
    std::string file_name;
    float percentage = 0;
    std::string M = "";
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-T")
            M = argv[i+1];
    }
    
    percentage = std::stof(M);

    
    ARFFParser arrf(file_name, class_name);
    arrf.parse();
    vvs table_data = arrf.get_tabular_data();
    msvs tableInfo = arrf.get_info_map();
    vs classData = arrf.get_class_data();
    
    int trainingRecordsCount = (int)(percentage * (table_data.size() - 1) / 100);
    vvs trainingDataset(table_data.begin(), table_data.end() - (table_data.size() - trainingRecordsCount));
    
    Node *dt = new Node;
    DecisionTree decisionTree(trainingDataset, tableInfo);
    std::cout << "Generating Decision Tree ..." << std::endl;
    
    file_name.pop_back();
    file_name.pop_back();
    file_name.pop_back();
    file_name.pop_back();
    file_name.pop_back();
    
    dt = decisionTree.dt_generator(trainingDataset, tableInfo, dt);
    std::string dt_name = "sp0090_bns0028DTTrain" + M + file_name + ".dt";
    decisionTree.tree_to_file_writer(dt, dt_name);
    
    vs predictions;
    for(int i=1; i<(int)table_data.size(); i++)
    {
        predictions.push_back(decisionTree.test_data_on_dt(table_data[i], dt));
    }

    std::string temp = "sp0090_bns0028DTAccuracy" + M + file_name + ".dt";
    std::cout  << "Writing confusionMatrix and accuracy to file : "<<temp<< std::endl;
    decisionTree.write_confusion_matrix(temp, classData, predictions);

    std::string dtApplicationOutFile = "sp0090_bns0028DTApply" + M + file_name + ".arff";
    std::cout  << "Writing predictions to file : " << dtApplicationOutFile  << "..." << std::endl;
    ArffWriter dtApplicationPredictedArff(dtApplicationOutFile);
    dtApplicationPredictedArff.relation_writer(arrf.get_relation_name());
    dtApplicationPredictedArff.attributes_writter(arrf.getAttributeNames(), arrf.getAttributeVals());
    string singleLine = string("@attribute dtClass real");
    dtApplicationPredictedArff.single_line_writer(singleLine);
    dtApplicationPredictedArff.data_writer(arrf.get_data(), predictions, true);

    return 0;
}
