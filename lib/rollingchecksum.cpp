#include "rollingchecksum.h"


RollingChecksum::RollingChecksum(const std::vector<unsigned char> &data, const int block_size, const int factor)
    : m_input(data),
      m_block_size(block_size),
      m_factor(factor)
{
    if(block_size > m_input.size())
    {
        return;
    }

    for(int i = 0; i < block_size; ++i)
    {
        m_r1 += m_input[i];
        m_r2 += (m_block_size - i) * m_input[i];
    }

    m_r1 = m_r1 % m_factor;
    m_r2 = m_r2 % m_factor;
    m_r = m_r1 + m_factor * m_r2;
}

void RollingChecksum::roll()
{
    const int end_index = m_current_pos + m_block_size;
    if(end_index >= m_input.size())
    {
        return;
    }

    const auto outgoing_element = m_input[m_current_pos];
    const auto incoming_element = m_input[m_current_pos + m_block_size];

    ++m_current_pos;

    m_r1 = (m_r1 - outgoing_element + incoming_element) % m_factor;
    m_r2 = (m_r2 - m_block_size * outgoing_element + m_r1) % m_factor;
    m_r = m_r1 + m_factor * m_r2;

}

long long RollingChecksum::currentValue()
{
    return m_r;
}


