#include "signaturecalculator.h"

#include <iostream>

#include "rollingchecksum.h"
#include "strongchecksum.h"

SignatureCalculator::SignatureCalculator(DataProvider &data_provider) :
    m_data_provider(data_provider)
{
}

const Signature& SignatureCalculator::calculate()
{
    auto block_index = 0;
    while(m_data_provider.readData())
    {
        RollingChecksum r;
        const auto fast_signature = r.calculate(m_data_provider.data());
        const auto strong_signature = StrongChecksum::calculate(m_data_provider.data());

        const auto signatures_map_entry_it = m_signature.find(fast_signature);
        if(signatures_map_entry_it != m_signature.end())
        {
            if(signatures_map_entry_it->second.m_strong_signature == strong_signature)
            {
                ++block_index;
                continue;
            }

            std::cerr << "Strong signature does not match\n";
        }

        m_signature[fast_signature] = {strong_signature, block_index};
        ++block_index;
    }

    return m_signature;
}
