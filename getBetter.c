//
//  getBetter.c
//  
//
//  Created by Cher Wang on 2/21/19.
//
//

#include "getBetter.h"
#include <istream>
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
void getBetter(const char* input_folder = "test_pictures", const char* input_file = "Feb_20_Analysis.csv", const char* output_folder = "test_pictures", const char* output_file = "result_Batch.csv"){
    std::ifstream file(Form("%s/%s",input_folder, input_file));
    std::string str;
    std::ofstream outfile(Form("%s/%s", output_folder, output_file));
    std::getline(file, str);
    outfile << "Difference Abs" << "," << str;
    int column = str.find("Fiber (%)");
    int i = 0;
    int counts = 0;
    while ((i + 1) < column){
        i = str.find(",", i + 1);
        counts++;
    }
    
    std::string strN;
    std::string strW;
    
    while (!file.eof()){
        std::getline(file, strN);
        int start = 0;
        for (int j = 1; j <= counts; j++){
            start = strN.find(',', start + 1);
        }
        int ed = strN.find(',', start + 1);
        double percentN = stod(strN.substr(start + 1, ed - 1));
        
        std::getline(file, strW);
        start = 0;
        for (int k = 1; k <= counts; k++){
            start = strW.find(",", start + 1);
        }
        ed = strW.find(",", start + 1);
        double percentW = stod(strW.substr(start + 1, ed - 1));
        double diff = abs(percentW - percentN);
        if (percentW >= percentN){
            outfile << diff << "," << strW;
        }
        else {
            outfile << diff << "," << strN;
        }
    }
}
