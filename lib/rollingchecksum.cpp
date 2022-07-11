#include "rollingchecksum.h"

RollingChecksum::RollingChecksum(const std::vector<unsigned char> &data, const int factor)
    : m_block_size(data.size()),
      m_factor(factor)
{
    for(int i = 0; i < data.size(); ++i)
    {
        m_r1 += data[i];
        m_r2 += (m_block_size - i) * data[i];
    }

    m_r1 = m_r1 % m_factor;
    m_r2 = m_r2 % m_factor;
    m_r = m_r1 + m_factor * m_r2;
}

long long RollingChecksum::roll(const unsigned char outgoing, const unsigned char incoming)
{
    m_r1 = (m_r1 - outgoing + incoming) % m_factor;
    m_r2 = (m_r2 - m_block_size * outgoing + m_r1) % m_factor;
    m_r = m_r1 + m_factor * m_r2;

    return m_r;
}

long long RollingChecksum::value()
{
    return m_r;
}


