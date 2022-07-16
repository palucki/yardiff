#include "rollingchecksum.h"
#include "params.h"

RollingChecksum::RollingChecksum()
{
}

long long RollingChecksum::calculate(const std::vector<unsigned char> &data)
{
    m_r = 0;
    m_r1 = 0;
    m_r2 = 0;
    m_block_size = data.size();

    for(int i = 0; i < data.size(); ++i)
    {
        m_r1 += data[i];
        m_r2 += (m_block_size - i) * data[i];
    }

    m_r1 = m_r1 % params::MODULO_FACTOR;
    m_r2 = m_r2 % params::MODULO_FACTOR;
    m_r = m_r1 + params::MODULO_FACTOR * m_r2;

    return m_r;
}

long long RollingChecksum::roll(const unsigned char outgoing, const unsigned char incoming)
{
    m_r1 = (m_r1 - outgoing + incoming) % params::MODULO_FACTOR;
    m_r2 = (m_r2 - m_block_size * outgoing + m_r1) % params::MODULO_FACTOR;
    m_r = m_r1 + params::MODULO_FACTOR * m_r2;

    return m_r;
}

