//
//  rp0037DecisionTree.hpp
//  cs641
//
//  Created by Ritesh Pradhan on 2/26/16.
//  Copyright © 2016 Ritesh Pradhan. All rights reserved.
//

#ifndef rp0037DecisionTree_hpp
#define rp0037DecisionTree_hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <fstream>
//#include <sstream>
#include <iomanip>

//#include "rp0037ArffParser.cpp"


using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<vi> vvi;
typedef vector<vs> vvs;
typedef map<string, int> msi;
typedef map<string, vs> msvs;

typedef struct Node // struct node defines the structure of a node of the decision tree
{
    string splitAttribute;       // Stores which attribute to split on at a particular node
    string label;			// Stores the class label for leaf nodes. For nodes that are not leaf nodes, it stores the value of the attribute of the parent's' split
    bool isLeaf;			// boolean flag for leaf nodes
    vector<string> childrenValues;			// Stores the values of the childrens' attributes
    vector<Node*> children;			// Stores pointers to the children of a node
} Node;

class DecisionTree
{
private:
    vvs originalTable;
    msvs tableInfoMeta;
    vs attributeNames;
    string classAttribute;
    vs classVals;
    string defaultClass;
    ofstream dtFile;
    
public:
    /** Constructor **/
    DecisionTree();
    DecisionTree(vvs&, msvs&); //{}
    
    /** Destructor **/
    ~DecisionTree() {}
    
    /**
     *   Parse the given arff file
     *   @param  string filename - input filepath/filename
     */
    
    
    /*
     *   Write to arff file
     *   @param  string filename - output filepath/filename
     */
    Node* generateDecisionTree(vvs&, msvs&, Node*);					// Builds the decision tree based on the table it is passed
    
    bool isTableEmpty(vvs&);    //Returns true if a subtable is empty
    
    bool isHomogeneous(vvs&);   //Returns true if all instances in a subtable at a node have the same class label
    
    bool isHomogeneousRecords(vvs&); //Returns true if all instances table has same records values.
    
    double calculateGini(vvs&table);    //Calculate gini of any table based of class
    
    string getSplittingAttribute(vvs&); //Returns the attribute on which to split on. Decision of column is based on GINI
    
    vvs trimTable(vvs&, string&, string);   // Prunes a table based on a column/attribute's name and the value of that attribute. Removes that column and all instances that have that value for that column
    
    void printTable(vvs&);								// For debugging purposes only. Prints a data table
    
    
    int countDistinct(vvs&, int);								// Returns a vector of integers containing the counts of all the various values of an attribute/column
    
    vi countDistinctAttributeValues(vvs&, string);								// Returns a vector of integers containing the counts of all the various values of an attribute/column
    
    int returnAttributeIndex(string&);						// Returns the index of a column in a subtable
    
    int returnIndexOfVector(vs&, string);						// Returns the index of a string in a vector of strings
    
    void printDecisionTree(Node*, int depth=0);								// For degubbing purposes only. Recursively prints decision tree
    
    string getMostFrequentClass(vvs &); // Returns the most frequent class from the training data. This class is used as the default class during the testing phase
    
    string testDataOnDecisionTree(vs&, Node*);  //Test a single record
    
    double printPredictionsAndCalculateAccuracy(vs&, vs&);  //Print and test accuracy for testing purpose
    
    string getClassWithMaxCount(vvs &);     // Returns class with max counts for labeling the leaf with homogenous recoreds.
    
    void writeTreeToFile(Node *, string);       // Write generated decisiont tree to file in depth first approach.
    
    void writeTreeRecursively(Node* , std::ofstream &, int depth=0);    // Write tree recursively to a file
    
    vvi generateConfusionMatrix(vs&, vs&);      // generates and returns confusion matrix.
    
    void writeConfusionMatrix(string&, vs&, vs&);   // Write confision matrix and accuracy to outfile.
    
};

#endif /* rp0037DecisionTree_hpp */
