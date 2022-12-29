#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>

#define UINT32_SIZE 4
#define UINT8_SIZE 1

int check_params(int argc, char** argv) {
    if (argc != 6) {
        std::cout << "Wrong number of parameters" << std::endl;
        return -1;
    }

    int mode = 0;
    std::string param = argv[1];
    if (param == "-s") {
        mode = 1;
    } else if (param == "-d") {
        mode = 2;
    } else {
        std::cout << "Wrong first flag. Must be -s or -d" << std::endl;
        return -1;
    }

    param = argv[2];
    if (param != "-i") {
        std::cout << "Wrong second flag. Must be -i" << std::endl;
        return -1;
    }

    param = argv[4];
    if (param != "-o") {
        std::cout << "Wrong third flag. Must be -o" << std::endl;
        return 1;
    }
    return mode;
}

void encode(std::string input, std::string output) {
    std::map<uint32_t, uint32_t> edge_cnt;

    std::ifstream infile(input, std::ios::in);

    std::map<uint32_t, std::map<uint32_t, uint8_t>> weights;

    uint32_t a = 0, b = 0;
    uint32_t tmp_c = 0;
    uint8_t c = 0;


    while (infile >> a >> b >> tmp_c)
    {   
        c = (uint8_t)tmp_c; 
        
        ++edge_cnt[a];

        weights[a][b] = c;

    }

    infile.close();

    std::ofstream outfile(output, std::ios::binary);

    for (auto& node_cnt : edge_cnt) {

        outfile.write(reinterpret_cast<const char*>(&node_cnt.first), UINT32_SIZE);
        outfile.write(reinterpret_cast<const char*>(&node_cnt.second), UINT32_SIZE);

        for (auto& adj_weight : weights[node_cnt.first]) {
            outfile.write(reinterpret_cast<const char*>(&adj_weight.first), UINT32_SIZE);
            outfile.write(reinterpret_cast<char*>(&adj_weight.second), UINT8_SIZE);
        }
    }

    outfile.close();
}



void decode(std::string input, std::string output) {
    std::ifstream infile(input, std::ios::binary);
    std::ofstream outfile(output, std::ios::out);    
    uint32_t a = 0, b = 0, cnt = 0;
    uint8_t c = 0;

    if (infile) {
        infile.read(reinterpret_cast<char*>(&a), UINT32_SIZE);
        infile.read(reinterpret_cast<char*>(&cnt), UINT32_SIZE);
        infile.read(reinterpret_cast<char*>(&b), UINT32_SIZE);
        infile.read(reinterpret_cast<char*>(&c), UINT8_SIZE);
        outfile << a << "\t" << b << "\t" << (int)c;
    }
    
    --cnt;

    while (infile) {
        for (int i = 0; i < cnt; ++i) {

            infile.read(reinterpret_cast<char*>(&b), UINT32_SIZE);
            infile.read(reinterpret_cast<char*>(&c), UINT8_SIZE);

            outfile << std::endl << a << "\t" << b << "\t" << (int)c;
        }
        cnt = 0;
        a = 0;
        infile.read(reinterpret_cast<char*>(&a), UINT32_SIZE);
        infile.read(reinterpret_cast<char*>(&cnt), UINT32_SIZE);
    }

    infile.close();
    outfile.close();
}

int main(int argc, char** argv) {

    int mode = 0;
    mode = check_params(argc, argv);
    if (mode == -1) {
        return 0;
    }

    std::string input = argv[3];
    std::string output = argv[5];

    if (mode == 1) {
        encode(input, output);
    } else if (mode == 2) {
        decode(input, output);
    }

}