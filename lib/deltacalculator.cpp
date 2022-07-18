#include "deltacalculator.h"

#include "strongchecksum.h"
#include "rollingchecksum.h"

DeltaCalculator::DeltaCalculator(DataProvider &data_provider, const Signature &signature) :
    m_data_provider(data_provider),
    m_signature(signature)
{

}

#include <iostream>

Delta DeltaCalculator::calculate()
{
    Delta delta;

    if(m_signature.empty())
    {
        while(m_data_provider.readData())
        {
            for(const auto c : m_data_provider.data())
            {
                delta += c;
            }
        }
        return delta;
    }

    RollingChecksum r;
    bool roll_fast = false;
    long block_start_pos = 0;
    unsigned char rolled_out_byte = 0;

    const int BLOCK_SIZE = m_data_provider.data().size();

    std::cout << "START computing delta. Block size " << BLOCK_SIZE << "\n";
    while(m_data_provider.readData())
    {
        std::cout << "Reading block of bytes starting from " << block_start_pos << '\n';

        const auto& buffer = m_data_provider.data();

        if(buffer.size() < BLOCK_SIZE)
        {
            roll_fast = false;
        }

        long long fast_signature;
        if(roll_fast)
        {
            std::cout << "Roll out: " << rolled_out_byte
                      << " roll in: " << buffer[buffer.size() -1] << '\n';
            fast_signature = r.roll(rolled_out_byte, buffer[buffer.size() -1]);

            if(fast_signature != r.calculate(buffer))
            {
                std::cerr << "CRITCAL ERROR IN ALGORITHM\n";
                break;
            }
        }
        else
        {
            std::cout << "recompute fast checksum\n";
            fast_signature = r.calculate(buffer);
        }

        std::cout << "Fast signature for block " << fast_signature << '\n';

        const auto known_block_it = m_signature.find(fast_signature);
        if(known_block_it != m_signature.end())
        {
            std::cout << "Match in fast signature " << fast_signature << '\n';
            const auto strong_signature = StrongChecksum::calculate(buffer);
            std::cout << "Strong signature " << strong_signature << '\n';
            if(known_block_it->second.m_strong_signature == strong_signature)
            {
                std::cout << "Strong signature also matches. Taking block index "
                          << known_block_it->second.m_block_index << '\n';
                block_start_pos += buffer.size();
                delta += "{BLOCK_" + std::to_string(known_block_it->second.m_block_index) + "}";
                roll_fast = false;
            }
            else
            {
                ++block_start_pos;
                delta += buffer[0];
                rolled_out_byte = buffer[0];
                roll_fast = true;
            }
        }
        else
        {
            ++block_start_pos;
            delta += buffer[0];
            rolled_out_byte = buffer[0];
            roll_fast = true;
        }

        std::cout << "Updating file pos to " << block_start_pos << '\n';
        m_data_provider.setReadPosition(block_start_pos);
    }

    return delta;
}
