
#ifndef sp0090_bns0028DecisionTree_hpp
#define sp0090_bns0028DecisionTree_hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <fstream>
#include <iomanip>

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<vi> vvi;
typedef vector<vs> vvs;
typedef map<string, int> msi;
typedef map<string, vs> msvs;

typedef struct Node
{
    string splitAttribute;      
    string label;			
    bool isLeaf;			
    vector<string> childrenValues;			
    vector<Node*> children;			
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
    DecisionTree();
    DecisionTree(vvs&, msvs&); //{}

    ~DecisionTree() {}

    Node* dt_generator(vvs&, msvs&, Node*);
    
    bool is_table_empty(vvs&);   
    
    bool is_homogeneous(vvs&);   
    
    bool is_homogeneous_records(vvs&); 
    
    double calculate_gini(vvs&table);    
    
    string get_splitting_attribute(vvs&); 
    
    vvs table_trimmer(vvs&, string&, string);   
    
    void print_table(vvs&);								
    
    int countDistinct(vvs&, int);								
    
    vi distinct_attribute_values_count(vvs&, string);								
    
    int return_attribute_index(string&);						 
    
    double print_predictions_and_cal_accuracy (vs&, vs&); 
    
    string get_class_with_maxcount(vvs &);   
    
    void tree_to_file_writer(Node *, string); 

    int return_index_of_vector(vs&, string);                        
    
    void print_decision_tree(Node*, int depth=0);                               
    
    string get_most_frequent_class(vvs &); 
    
    string test_data_on_dt(vs&, Node*); 
    
    void write_tree_recursively(Node* , std::ofstream &, int depth=0);    
    
    vvi confusion_matrix_generator  (vs&, vs&);     
    
    void write_confusion_matrix(string&, vs&, vs&);   
};

#endif 
