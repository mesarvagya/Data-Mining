
/**
 sp0090_bns0028DecisionTree.cpp
 Sarvagya Pant
 Bidhya Nandan Sharma
 April 3, 2017
 **/

#include "sp0090_bns0028DecisionTree.h"

DecisionTree::DecisionTree()
{
}

DecisionTree::DecisionTree(vvs &table, msvs &tableInfo)
{
    originalTable = table;
    tableInfoMeta = tableInfo;
    attributeNames = table[0];
    classAttribute = attributeNames[table[0].size()-1];
    classVals = tableInfoMeta.find(classAttribute)->second;
    defaultClass = get_most_frequent_class(table);
}

Node* DecisionTree::dt_generator(vvs& table, msvs& tableInfoMap, Node* dt)
{
    if (is_table_empty(table))
    {
        dt->isLeaf = true;
        dt->label = defaultClass;
        return dt;
    }
    if (is_homogeneous(table))
    {
        dt->isLeaf = true;
        dt->label = table[1][table[1].size()-1];
        return dt;
    }
    if (is_homogeneous_records(table))
    {
        dt->isLeaf = true;
        dt->label = get_class_with_maxcount(table);
        return dt;
    }
    if (table[0].size() > 1)
    {
        string splitAttr = get_splitting_attribute(table);
        dt->splitAttribute = splitAttr;
        vs eachAttrMeta = tableInfoMap.find(splitAttr)->second;
        int i;
        for (i=0; i<(int)eachAttrMeta.size(); i++)
        {
            vvs trimmedTable = table_trimmer(table, splitAttr, eachAttrMeta[i]);
            
            Node* newDt = (Node*) new Node;
            newDt->splitAttribute = splitAttr;
            // cout << i << " "<< "NEW label is :" << eachAttrMeta[i] << " name: " << splitAttr << endl;
            newDt->label = eachAttrMeta[i];
            newDt->isLeaf = false;
            dt->childrenValues.push_back(eachAttrMeta[i]);
            
            dt->children.push_back(dt_generator(trimmedTable, tableInfoMap, newDt));
        }
    }
    return dt;
}

bool DecisionTree::is_table_empty(vvs & table)
{
    return (table.size() == 1);
}

bool DecisionTree::is_homogeneous(vvs& table)
{
    int classCol = (int)table[0].size() - 1;
    string classLabel = table[1][classCol];
    
    // print_table(table);
    
    for(int i=1; i<(int)table.size(); i++)
    {
        if (classLabel != table[i][classCol]){
            return false;
        }
    }
    return true;
}

bool DecisionTree::is_homogeneous_records(vvs & table)
{
    for (int i=2; i<(int)table.size(); i++)
    {
        for (int j=0; j<(int)table[0].size()-1; j++)
        {
            if (table[i][j] != table[i-1][j])
            {
                // cout << "REUTRN FALSE" << endl;
                return false;
            }
        }
    }
    return true;
}

double DecisionTree::calculate_gini(vvs &table)
{
    double gini = 1.0;
    
    
    if (!is_table_empty(table))
    {
        vi eachClassCounts = distinct_attribute_values_count(table, table[0][table[0].size() -1]);
        for (int ecc=0; ecc<(int)eachClassCounts.size(); ecc++)
        {
            gini -= pow((double)eachClassCounts[ecc]/(table.size()-1), 2);
        }
    }
    return  gini;
}

string DecisionTree::get_splitting_attribute(vvs & table)
{
    if (table[0].size() == 2)
    {
        return table[0][0];
    }
    double maxGain = 0.0;
    double tableGini = calculate_gini(table) ;
    // cout << "TABLE GINI " << tableGini << endl;
    string splittingAttr;
    for (int col=0; col<(int)table[1].size()-1; col++)
    {
        string attrName = table[0][col];
        vi attrValCounts = distinct_attribute_values_count(table, attrName);
        vs attrVals = tableInfoMeta.find(attrName)->second;
        
        double colGini = 0.0;
        for (int i=0; i<(int)attrVals.size(); i++)
        {
            vvs tempTable = table_trimmer(table, attrName, attrVals[i]);
            if (!is_table_empty(tempTable))
            {
                double gini = calculate_gini(tempTable);
                colGini += gini * attrValCounts[i] / (table.size()-1);
            }
            
        }
        double informationGain = tableGini - colGini;
        
        if (informationGain > maxGain)
        {
            maxGain = informationGain;
            splittingAttr = attrName;
        }
    }
    return splittingAttr;
}

void DecisionTree::write_confusion_matrix(string &outFilePath, vs &givenData, vs &predictions)
{
    double accuracy = 0;
    ofstream dtAccuracyFile;
    dtAccuracyFile.open( outFilePath.c_str());
    dtAccuracyFile << "@ConfusionMatrix" << endl;
    vvi confusionMatrix = confusion_matrix_generator  (givenData, predictions);
    
    vs classes = tableInfoMeta.find(classAttribute)->second;
    int classCount = (int)(classes.size());
    dtAccuracyFile << setw(10) << "\t" <<  setw(20) << "PREDICTED" << endl;
    dtAccuracyFile << setw(10) << "\t" <<  setw(30) << "-----------------------------" << endl;
    dtAccuracyFile << setw(10) << "\t LABELS    ";
    for (int i=0; i<classCount; i++)
    {
        dtAccuracyFile << setw(10) << classes[i];
    }
    dtAccuracyFile << endl;
    dtAccuracyFile << setw(10) << "\t" <<  setw(30) << "-----------------------------" << endl;
    for (int r=0; r<classCount; r++)
    {
        dtAccuracyFile << setw(10) << "TRUE: \t" << classes[r] << " |";
        for (int c=0; c<classCount; c++)
        {
            dtAccuracyFile << setw(10) << confusionMatrix[r][c];
            if (r==c)
            {
                accuracy += confusionMatrix[r][c];
            }
        }
        dtAccuracyFile << endl;
    }
    
    dtAccuracyFile << endl << "Accuracy = " << accuracy << "/" << givenData.size();
    accuracy = accuracy * 100 / givenData.size();
    dtAccuracyFile << " = " << accuracy << "%" << endl;
    dtAccuracyFile.close();
}

string DecisionTree::get_most_frequent_class(vvs &table)
{
    string frequentClass;
    msi trainingClasses;     	 // Stores the classlabels and their frequency
    for (int i = 1; i < (int)table.size(); i++)
    {
        if (trainingClasses.count(table[i][table[0].size()-1]) == 0)
        {
            trainingClasses[table[i][table[0].size()-1]] = 1;
        } else
        {
            trainingClasses[table[i][table[0].size()-1]]++;
        }
    }
    
    msi::iterator mapIter;
    int highestClassCount = 0;
    for (mapIter = trainingClasses.begin(); mapIter != trainingClasses.end(); mapIter++)
    {
        if (mapIter->second >= highestClassCount)
        {
            highestClassCount = mapIter->second;
            frequentClass = mapIter->first;
        }
    }
    return frequentClass;
    
}


string DecisionTree::get_class_with_maxcount(vvs & table)
{
    return get_most_frequent_class(table);
}


void DecisionTree::print_table(vvs &table)
{
    int inner, outer;
    for (outer = 0; outer < (int)table.size(); outer++)
    {
        for (inner = 0; inner < (int)table[outer].size(); inner++)
        {
            cout << table[outer][inner] << "\t";
        }
        cout << endl;
    }
}

void DecisionTree::print_decision_tree(Node* dt, int depth)
{
    if(dt == NULL)
    {
        return;
    }
    if (dt->isLeaf)
    {
        cout << " class = " << dt->label;
    }
    else if (!dt->isLeaf && !dt->children.empty())
    {
        for (int i = 0; i < (int)dt->children.size(); i++)
        {
            cout << endl;
            for (int d=0; d<depth; d++)
            {
                cout << "|\t";
            }
            cout << dt->splitAttribute << " = " ;
            cout << dt->childrenValues[i];
            print_decision_tree(dt->children[i], depth + 1);
        }
    }
}


string DecisionTree::test_data_on_dt(vs &singleRecord, Node* dt)
{
    string prediction;
    while (!dt->isLeaf && !dt->children.empty())
    {
        string nodeAttribute = dt->splitAttribute;
        int index = return_attribute_index(nodeAttribute);
        string value = singleRecord[index];
        int childIndex = return_index_of_vector(dt->childrenValues, value);
        dt = dt->children[childIndex];
        prediction = dt->label;
        if (dt->isLeaf)
        {
            break;
        }
    }
    //    cout << "returning ... " << prediction << endl;
    return prediction;
}


void DecisionTree::write_tree_recursively(Node * dt, std::ofstream & dtTrainingOutfile, int depth)
{
    if(dt == NULL)
    {
        return;
    }
    
    if (dt->isLeaf)
    {
        dtTrainingOutfile << "  " << dt->label;
    }
    else if (!dt->isLeaf && !dt->children.empty())
    {
        for (int i = 0; i < (int)dt->children.size(); i++)
        {
            dtTrainingOutfile << endl;
            for (int d=0; d<depth; d++)
            {
                dtTrainingOutfile << "|\t";
            }
            dtTrainingOutfile << dt->splitAttribute << " = " ;
            dtTrainingOutfile << dt->childrenValues[i];
            write_tree_recursively(dt->children[i], dtTrainingOutfile, depth + 1);
        }
    }
}

void DecisionTree::tree_to_file_writer(Node * dt, string dtTrainingOutfileName)
{
    std::ofstream dtTrainingOutfile;
    dtTrainingOutfile.open(dtTrainingOutfileName.c_str());
    write_tree_recursively(dt, dtTrainingOutfile);
    dtTrainingOutfile << endl;
    dtTrainingOutfile.close();
    cout << "\nGeneration of DecisionTree Completed. Writing in file completed." << endl;
}



int DecisionTree::return_attribute_index(string & attribute)
{
    int i;
    for (i=0; i<(int)attributeNames.size(); i++)
    {
        if (attribute == attributeNames[i])
        {
            return i;
        }
    }
    return -1;
}

int DecisionTree::return_index_of_vector(vs & attributes, string attribute)
{
    int i;
    for (i=0; i<(int)attributes.size(); i++)
    {
        if (attribute == attributes[i])
        {
            return i;
        }
    }
    return -1;
}

vi DecisionTree::distinct_attribute_values_count(vvs &table, string attributeName)
{
    int attrIndex = return_index_of_vector(table[0], attributeName);
    vs attrVals = tableInfoMeta.find(attributeName)->second;
    vi countVals (attrVals.size(), 0);
    for (int i=1; i<(int)table.size(); i++)
    {
        for (int j=0; j<(int)countVals.size(); j++)
        {
            if (table[i][attrIndex] == attrVals[j])
            {
                countVals[j]++;
            }
        }
        
    }
    return countVals;
}

double DecisionTree::print_predictions_and_cal_accuracy (vs &givenData, vs &predictions)
{
    ofstream predictionFile;
    int correct = 0;
    
    cout << setw(3) << "#" << setw (16) << "Given Class" << setw(31) << right << "Predicted Class" << endl;
    cout << "--------------------------------------------------" << endl;
    for (int iii = 0; iii < (int)givenData.size(); iii++) {
        cout << setw(3) << iii+1 << setw(16) << givenData[iii];
        if (givenData[iii] == predictions[iii]) {
            correct++;
            cout << "  ------------  ";
        } else {
            cout << "  xxxxxxxxxxxx  ";
        }
        cout << predictions[iii] << endl;
    }
    cout << "--------------------------------------------------" << endl;
    cout << "Total number of instances in test data = " << givenData.size() << endl;
    cout << "Number of correctly predicted instances = " << correct << endl;
    //    predictionFile.close();
    return (double) correct/50 * 100;
}

vvi DecisionTree::confusion_matrix_generator  (vs &givenData, vs &predictions)
{
    vs classes = tableInfoMeta.find(classAttribute)->second;
    int classCount = (int)(classes.size());
    vi eachRow (classCount, 0);
    vvi confusionMatrix(classCount, eachRow);
    
    for (int i=0; i<(int)givenData.size(); i++)
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


vvs DecisionTree::table_trimmer(vvs &table, string & splitAttr, string attrVal)
{
    
    int i, j;
    vvs trimmedTable;
    int col = -1;
    vs headerRow;
    
    for (i = 0; i < (int)table[0].size(); i++)
    {
        if (table[0][i] == splitAttr)
        {
            col = i;
            break;
        }
    }
    for (i = 0; i < (int)table[0].size(); i++)
    {
        if (i != col)
        {
            headerRow.push_back(table[0][i]);
        }
    }
    
    trimmedTable.push_back(headerRow);
    
    for (i = 1; i < (int)table.size(); i++)
    {
        vs tempRow;
        if (table[i][col] == attrVal)
        {
            for (j = 0; j < (int)table[0].size(); j++)
            {
                if(j != col)
                {
                    tempRow.push_back(table[i][j]);
                }
            }
            trimmedTable.push_back(tempRow);
        }
    }
    return trimmedTable;
}


