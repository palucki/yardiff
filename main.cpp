#include <iostream>
#include <fstream>
#include <vector>

#include "strongchecksum.h"
#include "rollingchecksum.h"
#include "utils.h"

int main()
{
    const unsigned int block_size = 10;
    const std::string base_file_name = "base.txt";
    const std::string updated_file_name = "updated.txt";

    std::cout << "yardiff - yet another rdiff implementation\n"
              << "using block size: " << block_size << " bytes\n";

    std::ifstream base(base_file_name, std::ios::binary), updated(updated_file_name, std::ios::binary);
    if(!base)
    {
        std::cerr << "Cannot open the file : "<< base_file_name << '\n';
        return -1;
    }

    if(!updated)
    {
        std::cerr << "Cannot open the file : "<< updated_file_name << '\n';
        return -1;
    }

    std::vector<unsigned char> buffer(block_size, 0);
    int block_index = 0;
    while(!base.eof())
    {
        base.read((char*)buffer.data(), buffer.size());
        std::streamsize data_size = base.gcount();

        const auto strong = StrongChecksum::calculate(buffer);
        RollingChecksum r(buffer, 1 << 16);

        std::cout << "block index: " << block_index++ << " - " << data_size << " bytes\n";
        std::cout << "strong checksum (MD4): " << utils::bytesToHex(strong) << '\n';
        std::cout << "rolling checksum: " << r.value() << '\n';
    }

    //generate signature for file
    //process file_2 using signature
    //generate delta

    return 0;
}
