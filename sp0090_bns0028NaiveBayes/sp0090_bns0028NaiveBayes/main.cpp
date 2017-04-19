//
//  main.cpp
//  sp0090_bns0028NaiveBayes
//
//  Created by Sarvagya Pant on 4/16/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>
#include "sp0090_bns0028ARFFParser.hpp"
#include "sp0090_bns0028NaiveBayes.hpp"
#include "sp0090_bns0028ARFFWriter.hpp"


vvi generateConfusionMatrix(msvs &tableInfoMeta, vs &givenData, vs &predictions)
{
    vs classes = tableInfoMeta.find("class")->second;
    int classCount = (int)(classes.size());
    vi eachRow (classCount, 0);
    vvi confusionMatrix(classCount, eachRow);
    
    for (size_t i=0; i<givenData.size(); i++)
    {
        int row = 0;
        int col = 0;
        for (int r=0; r<classCount; r++)
        {
            if (givenData[i] == classes[r])
            {
                row = r;
                break;
            }
        }
        for (int c=0; c<classCount; c++)
        {
            if (predictions[i] == classes[c])
            {
                col = c;
                break;
            }
        }
        confusionMatrix[row][col] += 1;
    }
    
    return confusionMatrix;
}

void writeConfusionMatrix(std::string &outFilePath, msvs &tableInfoMeta, vs &givenData, vs &predictions, vf &all_accuracy)
{
    double accuracy = 0;
    std::ofstream nbConfusionFile;
    nbConfusionFile.open( outFilePath.c_str());
    nbConfusionFile << "ConfusionMatrix" << std::endl;
    vvi confusionMatrix = generateConfusionMatrix(tableInfoMeta, givenData, predictions);
    
    vs classes = tableInfoMeta.find("class")->second;
    int classCount = (int)(classes.size());
    nbConfusionFile << std::setw(10) << "\t" <<  std::setw(20) << "PREDICTED" << std::endl;
    nbConfusionFile << std::setw(10) << "\t" <<  std::setw(30) << "-----------------------------" << std::endl;
    nbConfusionFile << std::setw(10) << "\t";
    for (int i=0; i<classCount; i++)
    {
        nbConfusionFile << std::setw(10) << "\t" << classes[i];
    }
    nbConfusionFile << std::endl;
    nbConfusionFile << std::setw(10) << "\t" <<  std::setw(30) << "-----------------------------" << std::endl;
    for (int r=0; r<classCount; r++)
    {
        nbConfusionFile << std::setw(10) << "ACTUAL: \t" << classes[r] << " |";
        for (int c=0; c<classCount; c++)
        {
            nbConfusionFile << std::setw(10) << confusionMatrix[r][c];
            if (r==c)
            {
                accuracy += confusionMatrix[r][c];
            }
        }
        nbConfusionFile << std::endl;
    }
    
    nbConfusionFile << std::endl << "Accuracy = " << accuracy << "/" << givenData.size();
    accuracy = accuracy * 100 / givenData.size();
    nbConfusionFile << " = " << accuracy << "%\n" << std::endl;
    
    for(int i=0; i < all_accuracy.size(); i++){
        nbConfusionFile << "K fold iteration # " << std::to_string(i+1) << " Accuracy is " << std::to_string(all_accuracy[i]*100) << " %" << std::endl;
    }
    
    
    nbConfusionFile.close();
}


int main(int argc, const char * argv[]) {
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -c <class attribute> -K <# of folds>" << std::endl;
        exit(1);
    }
    
    std::string class_name = "undefined_class_name";
    std::string file_name;
    int folds = 10; //default 10
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-K")
            folds = std::stoi(argv[i+1]);
    }
    
    ARFFParser arff(file_name, class_name);
    arff.parse();
    
    vvs data = arff.get_tabular_data();
    msvs tableInfo = arff.get_info_map();
    vs classData = arff.get_class_data();
    
    int num_of_records = (int) data.size() - 1;
    int test_number_record = num_of_records / folds;
    
    vf accuracy_list;
    vs all_predictions;

    
    for(int i=0; i < folds; i++){
        // naive bayes object here
        int start_index = i * test_number_record + i + 1;
        int end_index = (i+1) * test_number_record + i + 1;
        
        if(i == folds - 1)
            end_index = num_of_records;
        
        NaiveBayesClassifier nbc(data, tableInfo,start_index, end_index);
        nbc.set_train_test_indexes();
        nbc.generate_model();
        float accuracy = nbc.get_accuracy();
        accuracy_list.push_back(accuracy);
        vs predictions = nbc.get_predictions();
    
        
        for(int i=0; i< predictions.size(); i++){
            all_predictions.push_back(predictions[i]);
        }
    }
    
    int correct_predictions = 0;
    for(int i=0; i < classData.size(); i++){
        std::string correct_class = classData[i];
        std::string predicted_class = all_predictions[i];
        if(correct_class == predicted_class){
            correct_predictions++;
        }
    }
    
    float overall_accuracy = (float)correct_predictions/(float)num_of_records;
    
    std::string naiveBayesOutputFile = "sp0090_bns0028" + std::to_string(folds) + "FoldClassification" + file_name;
    std::string naiveBayesConfusionMatrix = "sp0090_bns0028" + std::to_string(folds) + "FoldConfusion" + file_name + ".txt";
    
    ArffWriter arffWriter(naiveBayesOutputFile);
    arffWriter.relation_writer(arff.get_relation_name());
    arffWriter.attributes_writter(arff.getAttributeNames(), arff.getAttributeVals());
    string class_line = string("@attribute bayesClass real");
    arffWriter.single_line_writer(class_line);
    arffWriter.data_writer(arff.get_data(), all_predictions, true);
    
    writeConfusionMatrix(naiveBayesConfusionMatrix, tableInfo, classData, all_predictions, accuracy_list);
    
    return 0;
}
