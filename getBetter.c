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
void getBetter(const char* input_file = "result.csv", const char* output_file = "result_batch.csv"){
    std::ifstream file(input_file);
    std::string str;
    std::ofstream outfile(output_file);
    std::getline(file, str);
    outfile << "Difference Abs" << "," << str << endl;
    int column = str.find("Fiber (%)");
    int i = 0;
    int counts = 0;
    while ((i + 1) < column){
        i = str.find(",", i + 1);
        counts++;
    }
    //cout << counts << endl;
    std::string strN;
    std::string strW;
    std::string subN;
    std::string subW;
    
    while (!file.eof()){
        std::getline(file, strN);
        if (strN == ""){break;}
        int start = -1;
        for (int j = 1; j <= counts; j++){
            start = strN.find(',', start + 1);
        }
        int nlen = strN.find(',', start + 1) - start;
        subN = strN.substr(start + 1, nlen-1);
        double percentN = std::stod(subN);

        
        std::getline(file, strW);
        start = 0;
        for (int k = 1; k <= counts; k++){
            start = strW.find(",", start + 1);
        }
        nlen = strW.find(",", start + 1) - start;
        subW = strW.substr(start + 1, nlen-1);
        double percentW = std::stod(subW);
        double diff = abs(percentW - percentN);
        if (percentW >= percentN){
            outfile << diff << "," << strW << endl;
        }
        else {
            outfile << diff << "," << strN << endl;
        }
    }
    file.close();
    outfile.close();
}
