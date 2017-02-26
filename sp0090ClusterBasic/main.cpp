//
//  main.cpp
//  sp0090ClusterBasic
//
//  Created by Sarvagya Pant on 2/22/17.
//  Copyright © 2017 sarvagya. All rights reserved.
//

#include <iostream>

#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <map>

class ARFFParser{
    
private:
    std::string input_file;
    std::string class_name;
    bool normalize;
    bool data_begin;
    bool header_begin;
    std::vector<std::string> header_data;
    std::vector<std::vector<float>> data;
    int attrib_count;
    std::vector<float> compute_mean_std(std::vector<float>data);
    int skip_attribute_number;
    std::vector<float> mean_vector;
    std::vector<float> std_vector;
    
public:
    ARFFParser(std::string input_file, std::string class_name, bool normalize);
    void print_header();
    void print_data();
    void parse();
    void computeZScoreNormalization();
    std::vector<std::string> get_header_data();
    std::vector<std::vector<float>> get_row_data();
    std::vector<std::vector<float>> get_row_data_normalized();
    
    std::vector<float> get_mean_data(){
        return mean_vector;
    }
    
    std::vector<float> get_std_data(){
        return std_vector;
    }

    
};

ARFFParser::ARFFParser(std::string input_file, std::string class_name, bool normalize){
    this->input_file = input_file;
    this->class_name  = class_name;
    this->normalize = normalize;
    data_begin = false;
    header_begin = true;
}


void ARFFParser::parse(){
    std::string line;
    std::ifstream filestream(input_file);
    attrib_count = 0;
    skip_attribute_number = 0;
    std::string attribute_class = "@attribute " + std::string(class_name);
    bool class_column_found = false;
    
    while (std::getline(filestream, line)){
        if(line.empty())
            continue;
        if(line[0] == '%'){
            continue;
        }
        std::size_t pos = line.find("@relation");
        if(pos != std::string::npos ){
            header_data.push_back(line);
            header_begin = true;
            continue;
        }
        pos = line.find("@data");
        if(pos != std::string::npos){
           // header_data.push_back(line);
            data_begin = true;
            header_begin = false;
            continue;
        }
        if(header_begin){
            std::size_t pos = line.find("@attribute");
            std::size_t pos2 = line.find(attribute_class);
            if(pos2 != std::string::npos){
                class_column_found = true;
                attrib_count++;
            }
            else if(pos != std::string::npos){
                if(!class_column_found){
                    attrib_count++;
                    skip_attribute_number++;
                }
                else
                    attrib_count++;
                
            }
            header_data.push_back(line);
        }
        
        if(data_begin){
            std::istringstream ss(line);
            std::vector<float> row_data;
            do{
                std::string sub;
                ss >> sub;
                if(!sub.empty())
                    row_data.push_back(std::stof(sub));
            }while(ss);
            data.push_back(row_data);
        }
    }
    
}

void ARFFParser::print_header(){
    for(auto it = header_data.begin(); it != header_data.end(); ++it){
        std::cout << *it << std::endl;
    }
}
void ARFFParser::print_data(){
    for(int i(0); i < attrib_count; i++){
        for(auto row: data){
            std::cout << row.at(i) << std::endl;
        }
    }
}

void ARFFParser::computeZScoreNormalization(){
    std::vector<std::vector<float>> computed_values;
    for(int i(0); i < attrib_count; i++){
        if(i == skip_attribute_number){
            std::vector<float> temp_data;
            for(auto row: data){
                temp_data.push_back(row.at(i));
            }
            computed_values.push_back(temp_data);
        }
        else{
            std::vector<float> temp_data;
            for(auto row: data){
                temp_data.push_back(row.at(i));
            }
            computed_values.push_back(compute_mean_std(temp_data));
        }
    }
    
    /*for(auto row: header_data){
        off << row;
    }
    */
    
    for(int j(0); j < computed_values[0].size(); j++){
        for(int i(0); i < attrib_count; i++){
            std::cout << computed_values.at(i).at(j) << " ";
        }
    }

}

std::vector<float> ARFFParser::compute_mean_std(std::vector<float>data){
    float sum1(0), sum2(0), variance, sigma;
    for(auto it: data){
        sum1 += it;
    }
    int size = data.size();
    float mean = sum1/size;
    mean_vector.push_back(mean);
    for(auto it: data){
        sum2 += pow((it-mean),2);
    }
    variance = sum2/(size);
    sigma = sqrt(variance);
    std_vector.push_back(sigma);
    std::vector<float> temp;
    for(auto it: data){
        temp.push_back((it-mean)/sigma);
    }
    return temp;
}

std::vector<std::string> ARFFParser::get_header_data(){
    return header_data;
}

std::vector<std::vector<float>> ARFFParser::get_row_data(){
    std::vector<std::vector<float>> all_row_data;
    return all_row_data;
}

std::vector<std::vector<float>> ARFFParser::get_row_data_normalized(){
    std::vector<std::vector<float>> all_row_data;
    if(normalize){
        std::vector<std::vector<float>> computed_values;
        for(int i(0); i < attrib_count; i++){
            if(i == skip_attribute_number){
                std::vector<float> temp_data;
                for(auto row: data){
                    temp_data.push_back(row.at(i));
                }
                computed_values.push_back(temp_data);
            }
            else{
                std::vector<float> temp_data;
                for(auto row: data){
                    temp_data.push_back(row.at(i));
                }
                computed_values.push_back(compute_mean_std(temp_data));
            }
        }
        
        for(int j(0); j < computed_values[0].size(); j++){
            std::vector<float> row_data;
            for(int i(0); i < attrib_count; i++){
                row_data.push_back(computed_values.at(i).at(j));
            }
            all_row_data.push_back(row_data);
            }

    }
    else{
        all_row_data = data;
    }
    return all_row_data;
}

class KMeansBasic {
    
private:
    int clusters;
    std::string input_file;
    bool normalize;
    std::string class_name;
    std::vector<std::vector<float> > row_data;
    std::vector<std::string> header_data;
    std::map<int, std::vector<std::vector<float>>> clusters_points_final;
    std::vector<int> cluster_index_for_file_final;
    std::vector<std::vector<float>> centroids_final;
    std::vector<float> mean_data;
    std::vector<float> std_data;
    std::map<int, std::vector<float>> map_std_mean;
    
public:
    KMeansBasic(std::string input_file, int clusters, bool normalize, std::string class_name);
    void parseARFFandSetData();
    void startClustering();
    bool checkIndexInArray(int index, std::vector<int>arr);
    float getDistance(std::vector<float> x, std::vector<float> y);
    std::vector<int> getRandomIndexes(int cluster, int data_size);
    bool convergenceCondition(std::vector<std::vector<float>> x,std::vector<std::vector<float>> y );
    void KMeans(std::vector<std::vector<float>>input_data, int total_clusters);
    std::vector<std::vector<float>> get_whole_data(){
        return row_data;
    }
    void write_ouput_to_file();
    
};

KMeansBasic::KMeansBasic(std::string input_file, int clusters, bool normalize, std::string class_name){
    this->clusters = clusters;
    this->input_file = input_file;
    this->normalize = normalize;
    this->class_name = class_name;
}

void KMeansBasic::parseARFFandSetData(){
    ARFFParser arff(input_file,class_name, normalize);
    arff.parse();
    row_data = arff.get_row_data_normalized();
    header_data = arff.get_header_data();
    if(normalize){
        mean_data = arff.get_mean_data();
        std_data = arff.get_std_data();
        for(int i=0; i < mean_data.size(); i++){
            std::vector<float> data;
            data.push_back(mean_data[i]);
            data.push_back(std_data[i]);
            map_std_mean[i] = data;
        }
    }
}

std::vector<int> KMeansBasic::getRandomIndexes(int cluster, int data_size){
    srand(time(NULL));
    std::vector<int> random_indexes;
    for(int i=0; i < cluster; ++i){
        int random;
        do{
            random = rand() % data_size;
        }while(checkIndexInArray(random, random_indexes));
        random_indexes.push_back(random);
    }
    return random_indexes;
}

bool KMeansBasic::convergenceCondition(std::vector<std::vector<float>> x,std::vector<std::vector<float>> y ){
    bool flag = true;
    int vector_size = x.size();
    for(int i=0; i<vector_size; i++){
        std::vector<float> x_point = x.at(i);
        std::vector<float> y_point = y.at(i);
        int point_size = x_point.size();
        for(int j=0; j<point_size; j++){
            if(x_point.at(j) != y_point.at(j)){
                return false;
            }
        }
    }
    return flag;
}

void KMeansBasic::KMeans(std::vector<std::vector<float>>input_data, int total_clusters){
    std::vector<int> random_indexes = getRandomIndexes(total_clusters, input_data.size());
    std::vector<std::vector<float>> centroids;
    bool convergence = false;
    for(auto it:random_indexes)
        centroids.push_back(input_data.at(it));
    std::map<int, std::vector<std::vector<float>>> clusters_points;
    std::vector<int> cluster_index_for_file;
    do
    {
        for(int i=0; i < clusters; i++){
            clusters_points[i] = {};
        }
        cluster_index_for_file = {};
        for(auto it1: input_data){
            float min_distance = 99999999999;
            float min_index = -1;
            for(int i=0; i<centroids.size(); ++i){
                float temp = getDistance(it1, centroids.at(i));
                if(temp < min_distance){
                    min_distance = temp;
                    min_index = i;
                }
            }
            //assign it1 to index of it2
            clusters_points[min_index].push_back(it1);
            cluster_index_for_file.push_back(min_index);
        }
        //recalculate k means
        std::vector<std::vector<float>> all_temp_centroids;
        
        for(int i=0; i< clusters; i++){
            
            std::vector<std::vector<float>> points = clusters_points[i];
            int arity = points[0].size();
            int num_points = points.size();
            
            std::vector<float> single_centroid;
            for(int i=0; i<arity; i++){
                float temp_sum = 0;
                for(int j=0; j<num_points; j++){
                    temp_sum += points.at(j).at(i);
                }
                single_centroid.push_back(temp_sum/num_points);
            }
            all_temp_centroids.push_back(single_centroid);
        }
        convergence = convergenceCondition(centroids, all_temp_centroids);
        centroids = all_temp_centroids;
        
    }
    while(!convergence);//continue if k means are not the same as previous
    
    std::cout << ""<<std::endl;
    cluster_index_for_file_final = cluster_index_for_file;
    clusters_points_final = clusters_points;
    centroids_final = centroids;
}

bool KMeansBasic::checkIndexInArray(int index, std::vector<int> arr)
{
    bool flag = false;
    for(auto it: arr){
        if(it == index)
            flag = true;
    }
    return flag;
}

float KMeansBasic::getDistance(std::vector<float> x, std::vector<float> y){
    int len = x.size();
    float dist = 0;
    for(int i=0; i < len; i++){
        dist += pow((x.at(i) - y.at(i)), 2);
    }
    dist = pow(dist, 0.5);
    return dist;
}

void KMeansBasic::write_ouput_to_file(){
    if(normalize){
        //un normalize the data and write it into files.
        std::string clusters_center_basic_normalized = "sp0090ClusterCenterNormalizedBasic" + std::to_string(clusters) + input_file;
        std::string clusters_points_basic_normalized = "sp0090ClusteringNormalizedBasic" + std::to_string(clusters) + input_file;
        std::string clusters_center_basic_unnormalized = "sp0090ClusterCenterUnnormalizedBasic" + std::to_string(clusters) + input_file;
        std::string clusters_points_basic_unnormalized = "sp0090ClusteringUnnormalizedBasic" + std::to_string(clusters) + input_file;
        
        
        std::ofstream off(clusters_center_basic_normalized);
        for(auto it: header_data){
            off << it;
        }
        off << "@attribute cluster real\n";
        off << "@data\n";
        int cluster = 0;
        for(auto key: centroids_final){
            for(auto item: key){
                off << std::to_string(item) << " ";
            }
            off << std::to_string(cluster) << " \n";
            cluster++;
        }
        off.close();
        
        
        std::ofstream off_points(clusters_points_basic_normalized);
        for(auto it: header_data){
            off_points << it;
        }
        off_points << "@attribute cluster real\n";
        off_points << "@data\n";
        for(int i=0; i < cluster_index_for_file_final.size(); i++){
            int cluster_num = cluster_index_for_file_final[i];
            for(auto data: row_data.at(i)){
                off_points << std::to_string(data) << " ";
            }
            off_points << std::to_string(cluster_num) << " \n";
        }
        off_points.close();
        
        std::ofstream off_unnorm(clusters_center_basic_unnormalized);
        for(auto it: header_data){
            off_unnorm << it;
        }
        off_unnorm << "@attribute cluster real\n";
        off_unnorm << "@data\n";
        cluster = 0;
        for(auto key: centroids_final){
            int column = 0;
            for(auto item: key){
                item = item * map_std_mean[column][1] + map_std_mean[column][0];
                off_unnorm << item << " ";
                column++;
            }
            off_unnorm << std::to_string(cluster) << " \n";
            cluster++;
        }
        off_unnorm.close();
        
        std::ofstream off_points_unnorm(clusters_points_basic_unnormalized);
        for(auto it: header_data){
            off_points_unnorm << it;
        }
        off_points_unnorm << "@attribute cluster real\n";
        off_points_unnorm << "@data\n";
        for(int i=0; i < cluster_index_for_file_final.size(); i++){
            int cluster_num = cluster_index_for_file_final[i];
            int column = 0;
            for(auto data: row_data.at(i)){
                data = data * map_std_mean[column][1] + map_std_mean[column][0];
                off_points_unnorm << data << " ";
                column ++;
            }
            off_points_unnorm << cluster_num << " \n";
        }
        off_points_unnorm.close();


        
    }
    else{
        // just write data
        
        std::string clusters_center_basic = "sp0090ClusterCenterBasic" + std::to_string(clusters) + input_file;
        std::string clusters_points_basic = "sp0090ClusteringBasic" + std::to_string(clusters) + input_file;

        std::ofstream off(clusters_center_basic);
        for(auto it: header_data){
            off << it;
        }
        off << "@attribute cluster real\n";
        off << "@data\n";
        int cluster = 0;
        for(auto key: centroids_final){
            for(auto item: key){
                off << std::to_string(item) << " ";
            }
            off << std::to_string(cluster) << " \n";
            cluster++;
        }
        off.close();
        
        
        std::ofstream off_points(clusters_points_basic);
        for(auto it: header_data){
            off_points << it;
        }
        off_points << "@attribute cluster real\n";
        off_points << "@data\n";
        for(int i=0; i < cluster_index_for_file_final.size(); i++){
            int cluster_num = cluster_index_for_file_final[i];
            for(auto data: row_data.at(i)){
                off_points << std::to_string(data) << " ";
            }
            off_points << std::to_string(cluster_num) << " \n";
        }
        off_points.close();
    }
}
                
int main(int argc, const char * argv[]) {
    if(argc < 5){
        std::cout << "please run program as program -i <input_file> -k <Number_of_cluster> and other optional arguments (-normalize, -c)" << std::endl;
        exit(1);
    }
    
    std::string class_name = "undefined_class_name";
    std::string file_name;
    bool normalize = false;
    int cluster;
    for(int i=1; i < argc; i++){
        if(std::string(argv[i]) == "-c")
            class_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-i")
            file_name = std::string(argv[i+1]);
        else if(std::string(argv[i]) == "-k")
            cluster = std::stoi(argv[i+1]);
        else if(std::string(argv[i]) == "-normalize")
            normalize = true;
    }
    std::cout << class_name << file_name << normalize << cluster << std::endl;
    KMeansBasic kmeans(file_name, cluster, normalize, class_name);
    kmeans.parseARFFandSetData();
    auto data = kmeans.get_whole_data();
    kmeans.KMeans(data, cluster);
    kmeans.write_ouput_to_file();
    
    // ARFFParser a = ARFFParser(file_name, "sp0090Normalize" + file_name, class_name);
    //a.parse();
    //a.computeZScoreNormalization();
    return 0;
}
