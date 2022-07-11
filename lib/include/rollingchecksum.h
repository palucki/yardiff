#pragma once

#include <vector>

/*!
 * \brief The FastSignature class implements the hash function based on Adler32 checksum.
 * See https://www.samba.org/~tridge/phd_thesis.pdf and https://rsync.samba.org/tech_report/node3.html
 */

class RollingChecksum
{
public:
    RollingChecksum(const std::vector<unsigned char>& data, const int factor);

    long long roll(const unsigned char outgoing, const unsigned char incoming);

    long long value();
private:
    const int m_block_size;
    const int m_factor;

    long long m_r = 0;
    long long m_r1 = 0;
    long long m_r2 = 0;
};
