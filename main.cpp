#include <iostream>
#include <fstream>

#include "filedataprovider.h"
#include "signaturecalculator.h"
#include "deltacalculator.h"

/*!
 * This application demonstrates an implementation of rsync algorithm for rolling hash based diff tool.
 * This algorithm make use of 2 checksums (aka signatures) to be as fast, efficient and accurate as possible.
 * The operation of generating a diff is described in the following papers:
 * https://rsync.samba.org/tech_report
 * https://www.samba.org/~tridge/phd_thesis.pdf
 */

int main(int argc, char** argv)
{
    std::cout << "yardiff - yet another rdiff implementation\n";

    if(argc != 4)
    {
        std::cout << "Usage:\n"
                  << "    yardiff BASE_FILE UPDATED_FILE BLOCK_SIZE\n";

        return -1;
    }

    const std::string base_file_name = argv[1];
    const std::string updated_file_name = argv[2];
    const unsigned int BLOCK_SIZE = std::stoi(argv[3]);

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

    FileDataProvider base_data_provider(base_file_handle, BLOCK_SIZE);
    SignatureCalculator signature_calculator{base_data_provider};
    const auto& signature = signature_calculator.calculate();

    std::cout << "Signature: " << signature.size() << " blocks\n";

    FileDataProvider updated_data_provider(updated_file_handle, BLOCK_SIZE);
    DeltaCalculator delta_calculator{updated_data_provider, signature};

    Delta delta = delta_calculator.calculate();

    std::cout << "Delta: " << delta << '\n';

    return 0;
}
