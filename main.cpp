#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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

    std::ifstream base_file_handle(base_file_name, std::ios::binary);
    std::ifstream updated_file_handle(updated_file_name, std::ios::binary);
    if(!base_file_handle)
    {
        std::cerr << "Cannot open the file : "<< base_file_name << '\n';
        return -1;
    }
    struct BlockData
    {
        std::string m_strong_signature;
        int m_block_index;
    };

    std::unordered_map<unsigned int, BlockData> fast_strong_signature_map;

    std::vector<unsigned char> buffer(block_size, 0);

    auto block_index = 0;
    while(!base_file_handle.eof())
    {
        base_file_handle.read((char*)buffer.data(), buffer.size());

        if(buffer.empty())
        {
            std::cout << "Empty buffer\n";
            continue;
        }

        std::streamsize succesfully_read_bytes = base_file_handle.gcount();

        if(succesfully_read_bytes < buffer.size())
        {
            std::cout << "last chunk, resizing buffer\n";
            buffer.resize(succesfully_read_bytes);
        }

        const auto strong_signature = StrongChecksum::calculate(buffer);
        RollingChecksum r(buffer, 1 << 16);
        const auto fast_signature = r.value();

        std::cout << "block index: " << block_index++ << " - " << succesfully_read_bytes << " bytes\n";
        std::cout << "strong checksum (MD4): " << utils::bytesToHex(strong_signature) << '\n';
        std::cout << "rolling checksum: " << fast_signature << '\n';

        if(fast_strong_signature_map.find(fast_signature) != fast_strong_signature_map.end())
        {
            std::cerr << "fast signature hash colission for " << fast_signature << '\n';
            continue;
        }

        fast_strong_signature_map[fast_signature] = {utils::bytesToHex(strong_signature), block_index};
    }

    std::cout << "=============== UPDATED FILE =================\n";

    if(!updated_file_handle)
    {
        std::cerr << "Cannot open the file : "<< updated_file_name << '\n';
        return -1;
    }

    buffer.resize(block_size, 0);
    updated_file_handle.read((char*)buffer.data(), buffer.size());
    RollingChecksum r(buffer, 1 << 16);
    auto fast_signature = r.value();

    std::cout << "Fast signature for first block " << fast_signature << '\n';

    if(buffer.empty())
    {
        std::cerr << "Updated file buffer is empty";
        //whole file needs to be deleted -> delta is empty
        return -1;
    }

    while(!updated_file_handle.eof())
    {
        const auto outgoing_byte = buffer[0];
        const auto incoming_byte = updated_file_handle.get();
        fast_signature = r.roll(outgoing_byte, incoming_byte);

        std::cout << "rolling checksum: " << r.value() << '\n';
    }

    //generate signature for file
    //process file_2 using signature
    //generate delta

    return 0;
}
