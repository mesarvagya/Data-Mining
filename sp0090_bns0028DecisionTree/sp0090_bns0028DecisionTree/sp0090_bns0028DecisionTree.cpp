//
//  rp0037DecisionTree.cpp
//  cs641
//
//  Created by Ritesh Pradhan on 2/26/16.
//  Copyright © 2016 Ritesh Pradhan. All rights reserved.
//

#include "sp0090_bns0028DecisionTree.h"

/* Default constructor */
DecisionTree::DecisionTree()
{
    
}

/* Constructor with table and tableInfo for creating decision tree from */
DecisionTree::DecisionTree(vvs &table, msvs &tableInfo)
{
    originalTable = table;
    tableInfoMeta = tableInfo;
    attributeNames = table[0];
    classAttribute = attributeNames[table[0].size()-1];
    classVals = tableInfoMeta.find(classAttribute)->second;
    defaultClass = getMostFrequentClass(table);
}

/*
 *   Generate actual decision tree by recursion
 *   Check for Empty table, Homogenous class and homogenous records
 *   Uses information gain for creating new Node of decision tree.
 *   Multiway split
 *   Remove attributes after used once.
 *   @params table, tableInfoMap, dt
 */
Node* DecisionTree::generateDecisionTree(vvs& table, msvs& tableInfoMap, Node* dt)
{
    if (isTableEmpty(table))
    {
        // cout << "Assigning class  to Empty table ********* " << endl;
        dt->isLeaf = true;
        dt->label = defaultClass;
        return dt;
    }
    if (isHomogeneous(table))
    {
        // cout << "Found homogenous class here ********* " << endl;
        dt->isLeaf = true;
        dt->label = table[1][table[1].size()-1];
        return dt;
    }
    if (isHomogeneousRecords(table))
    {
        // cout << "Found homogenous records " << endl;
        dt->isLeaf = true;
        // find count of each classes and assign class -> class with max(classes_counts)
        dt->label = getClassWithMaxCount(table);
        return dt;
    }
    if (table[0].size() > 1)
    {
        string splitAttr = getSplittingAttribute(table);
        // cout << "generateDecisionTree; Split attr is : " << splitAttr << endl;
        dt->splitAttribute = splitAttr;
        vs eachAttrMeta = tableInfoMap.find(splitAttr)->second;
        int i;
        for (i=0; i<(int)eachAttrMeta.size(); i++)
        {
            vvs trimmedTable = trimTable(table, splitAttr, eachAttrMeta[i]);
            
            Node* newDt = (Node*) new Node;
            newDt->splitAttribute = splitAttr;
            // cout << i << " "<< "NEW label is :" << eachAttrMeta[i] << " name: " << splitAttr << endl;
            newDt->label = eachAttrMeta[i];
            newDt->isLeaf = false;
            dt->childrenValues.push_back(eachAttrMeta[i]);
            
            dt->children.push_back(generateDecisionTree(trimmedTable, tableInfoMap, newDt));
        }
    }
    return dt;
}

/*
 * Returns true if the table is empty
 * returns false otherwise
 */
bool DecisionTree::isTableEmpty(vvs & table)
{
    return (table.size() == 1);
}

/*
 * Return true if all recoreds are of same class
 * returns false otherwise
 */
bool DecisionTree::isHomogeneous(vvs& table)
{
    int classCol = (int)table[0].size() - 1;
    string classLabel = table[1][classCol];
    
    // printTable(table);
    
    for(int i=1; i<(int)table.size(); i++)
    {
        //        cout << "test HOMO : " << classLabel << " ::: " << table[1][classCol] << endl;
        if (classLabel != table[i][classCol]){
            return false;
        }
    }
    return true;
}

/*
 *  Returns true if all records in table have same records even if the class
 *  label is different.
 */
bool DecisionTree::isHomogeneousRecords(vvs & table)
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

/*
 *  Returns the gini of any table
 */
double DecisionTree::calculateGini(vvs &table)
{
    double gini = 1.0;
    
    
    if (!isTableEmpty(table))
    {
        vi eachClassCounts = countDistinctAttributeValues(table, table[0][table[0].size() -1]);
        // cout << "2s and 4s : " << eachClassCounts[0] << " " << eachClassCounts[1] << endl;
        for (int ecc=0; ecc<(int)eachClassCounts.size(); ecc++)
        {
            gini -= pow((double)eachClassCounts[ecc]/(table.size()-1), 2);
        }
    }
    return  gini;
}


/*
 * Decides which column to split on
 * based on Information gain using GINI.
 * Returns the column with the max Information Gain.
 */
string DecisionTree::getSplittingAttribute(vvs & table)
{
    if (table[0].size() == 2)
    {
        return table[0][0];
    }
    double maxGain = 0.0;
    double tableGini = calculateGini(table) ;
    // cout << "TABLE GINI " << tableGini << endl;
    string splittingAttr;
    for (int col=0; col<(int)table[1].size()-1; col++)
    {
        string attrName = table[0][col];
        vi attrValCounts = countDistinctAttributeValues(table, attrName);
        vs attrVals = tableInfoMeta.find(attrName)->second;
        
        double colGini = 0.0;
        for (int i=0; i<(int)attrVals.size(); i++)
        {
            vvs tempTable = trimTable(table, attrName, attrVals[i]);
            if (!isTableEmpty(tempTable))
            {
                double gini = calculateGini(tempTable);
                colGini += gini * attrValCounts[i] / (table.size()-1);
            }
            
        }
        double informationGain = tableGini - colGini;
        // cout << "ATTR: " << attrName << " GINI: " << colGini <<  " IG: " << informationGain << endl;
        
        if (informationGain > maxGain)
        {
            maxGain = informationGain;
            splittingAttr = attrName;
        }
    }
    // cout << "INFORMATION GAIN MAX " << maxGain << endl;
    return splittingAttr;
}

/*
 * Prunes a table based on a column/attribute's name
 * and value of that attribute. Removes that column
 * and all rows that have that value for that column.
 */
vvs DecisionTree::trimTable(vvs &table, string & splitAttr, string attrVal)
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

/*
 * Returns the most frequent class from the given table data
 * This class will be used as the default class label
 */

string DecisionTree::getMostFrequentClass(vvs &table)
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


string DecisionTree::getClassWithMaxCount(vvs & table)
{
    return getMostFrequentClass(table);
}

/*
 * Prints a vector of vector of strings
 * For debugging purposes only.
 */

void DecisionTree::printTable(vvs &table)
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

/*
 * Recursively prints the decision tree
 * For debugging purposes only
 */
void DecisionTree::printDecisionTree(Node* dt, int depth)
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
            printDecisionTree(dt->children[i], depth + 1);
        }
    }
}

void DecisionTree::writeTreeRecursively(Node * dt, std::ofstream & dtTrainingOutfile, int depth)
{
    if(dt == NULL)
    {
        return;
    }
    if (dt->isLeaf)
    {
        dtTrainingOutfile << " class = " << dt->label;
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
            writeTreeRecursively(dt->children[i], dtTrainingOutfile, depth + 1);
        }
    }
}

void DecisionTree::writeTreeToFile(Node * dt, string dtTrainingOutfileName)
{
    std::ofstream dtTrainingOutfile;
    dtTrainingOutfile.open(dtTrainingOutfileName.c_str());
    writeTreeRecursively(dt, dtTrainingOutfile);
    dtTrainingOutfile << endl;
    dtTrainingOutfile.close();
    cout << "\nGeneration of DecisionTree Completed. Writing in file completed." << endl;
}


/*
 * Takes a row and traverses that row through
 * the decision tree to find out the
 * predicted class label. If none is found
 * returns the default class label which is
 * the class label with the highest frequency.
 */
string DecisionTree::testDataOnDecisionTree(vs &singleRecord, Node* dt)
{
    string prediction;
    while (!dt->isLeaf && !dt->children.empty())
    {
        string nodeAttribute = dt->splitAttribute;
        int index = returnAttributeIndex(nodeAttribute);
        string value = singleRecord[index];
        int childIndex = returnIndexOfVector(dt->childrenValues, value);
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

/*
 *  Return index of string from the attributeNames vector
 *  Used to find the index of the attribute in the table
 */

int DecisionTree::returnAttributeIndex(string & attribute)
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

/*
 *   return index of any value in vector.
 */
int DecisionTree::returnIndexOfVector(vs & attributes, string attribute)
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

/*
 *   Returns a vector of integers containing the counts of all the various values of an attribute/column
 */
vi DecisionTree::countDistinctAttributeValues(vvs &table, string attributeName)
{
    int attrIndex = returnIndexOfVector(table[0], attributeName);
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


/*
 * Outputs the predictions to file
 * and returns the accuracy of the classification
 */

double DecisionTree::printPredictionsAndCalculateAccuracy(vs &givenData, vs &predictions)
{
    ofstream predictionFile;
    //    predictionFile.open("./rp0037DTAccuracy.txt");
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

/*
 *   Generates confusion matrix and returns
 *   n*n confusion matrix based on prediction of classes.
 */
vvi DecisionTree::generateConfusionMatrix(vs &givenData, vs &predictions)
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

/*
 *   Write the generated confusion matrix and accuracy to file.
 */
void DecisionTree::writeConfusionMatrix(string &outFilePath, vs &givenData, vs &predictions)
{
    double accuracy = 0;
    ofstream dtAccuracyFile;
    dtAccuracyFile.open( outFilePath.c_str());
    dtAccuracyFile << "@ConfusionMatrix" << endl;
    vvi confusionMatrix = generateConfusionMatrix(givenData, predictions);
    
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
