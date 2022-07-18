#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "params.h"
#include "strongchecksum.h"
#include "rollingchecksum.h"
#include "filedataprovider.h"
#include "signaturecalculator.h"
#include "deltacalculator.h"

int main()
{
    //convert to input parameters
    const unsigned int BLOCK_SIZE = 5;
    const std::string base_file_name = "base.txt";
    const std::string updated_file_name = "updated.txt";

    std::cout << "yardiff - yet another rdiff implementation\n"
              << "using block size: " << BLOCK_SIZE << " bytes\n";

    if(BLOCK_SIZE <= 0)
    {
        std::cerr << "Incorrect block size used! Must be greater than 0!\n";
        return -1;
    }

    std::ifstream base_file_handle(base_file_name, std::ios::binary);
    std::ifstream updated_file_handle(updated_file_name, std::ios::binary);
    if(!base_file_handle)
    {
        std::cerr << "Cannot open the file : "<< base_file_name << '\n';
        return -1;
    }

    if(!updated_file_handle)
    {
        std::cerr << "Cannot open the file : "<< updated_file_name << '\n';
        return -1;
    }

    FileDataProvider base_data_provider{base_file_handle, BLOCK_SIZE};
    SignatureCalculator signature_calculator{base_data_provider};
    const auto& signature = signature_calculator.calculate();

    std::cout << "Signature: " << signature.size() << " blocks\n";

    FileDataProvider updated_data_provider{updated_file_handle, BLOCK_SIZE};
    DeltaCalculator delta_calculator{updated_data_provider, signature};

    Delta delta = delta_calculator.calculate();

    std::cout << "Delta: " << delta << '\n';

    return 0;
}
