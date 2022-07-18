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

    //edge case
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

    const int BLOCK_SIZE = m_data_provider.data().size();
    bool roll_fast_signature = false;
    long next_block_start_pos = 0;
    unsigned char rolled_out_byte = 0;
    RollingChecksum r;

    const auto take_single_byte = [&roll_fast_signature, &next_block_start_pos,
                                   &rolled_out_byte, &delta](unsigned char byte)
    {
        delta += byte;
        rolled_out_byte = byte;
        ++next_block_start_pos;
        roll_fast_signature = true;
    };

    while(m_data_provider.readData())
    {
        const auto& buffer = m_data_provider.data();
        if(buffer.size() < BLOCK_SIZE)
        {
            roll_fast_signature = false;
        }

        long long fast_signature;
        if(roll_fast_signature)
        {
            fast_signature = r.roll(rolled_out_byte, buffer[buffer.size() -1]);
        }
        else
        {
            fast_signature = r.calculate(buffer);
        }

        const auto known_block_it = m_signature.find(fast_signature);
        if(known_block_it != m_signature.end())
        {
            const auto strong_signature = StrongChecksum::calculate(buffer);
            if(known_block_it->second.m_strong_signature == strong_signature)
            {
                //we've got a match
                next_block_start_pos += buffer.size();
                delta += "{BLOCK_" + std::to_string(known_block_it->second.m_block_index) + "}";
                roll_fast_signature = false;
            }
            else
            {
                //no match, take single byte
                take_single_byte(buffer[0]);
            }
        }
        else
        {
            //no match, take single byte
            take_single_byte(buffer[0]);
        }

        m_data_provider.setReadPosition(next_block_start_pos);
    }

    return delta;
}
