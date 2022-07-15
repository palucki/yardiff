#include "signaturecalculator.h"

#include <iostream>

#include "rollingchecksum.h"
#include "strongchecksum.h"

SignatureCalculator::SignatureCalculator(DataProvider &data_provider, const int modulo_factor) :
    m_data_provider(data_provider),
    m_modulo_factor(modulo_factor)
{
}


const Signature& SignatureCalculator::calculate()
{
    auto block_index = 0;
    while(!m_data_provider.finished())
    {
        if(!m_data_provider.readData())
        {
            std::cout << "No more data in buffer data\n";
            continue;
        }

        RollingChecksum r{m_modulo_factor};
        const auto strong_signature = StrongChecksum::calculate(m_data_provider.data());
        const auto fast_signature = r.calculate(m_data_provider.data());

        std::cout << "strong checksum (MD4): " << strong_signature << '\n';
        std::cout << "rolling checksum: " << fast_signature << '\n';

        const auto signatures_map_entry_it = m_signature.find(fast_signature);
        if(signatures_map_entry_it != m_signature.end())
        {
            std::cout << "fast signature hash colission for " << fast_signature << '\n';
            std::cout << "Checking also strong signature: " << strong_signature << "\n";

            if(signatures_map_entry_it->second.m_strong_signature == strong_signature)
            {
                std::cout << "match, skipping\n";
                ++block_index;
                continue;
            }

            std::cerr << "Strong signature does not match :O\n";
        }

        std::cout << "Adding at key " << fast_signature << " vals {" <<strong_signature << "," << block_index << "}\n";
        m_signature[fast_signature] = {strong_signature, block_index};
        ++block_index;
    }

    return m_signature;
}
