#pragma once

#include <vector>

/*!
 * \brief The RollingChecksum class implements the hash function based on Adler32 checksum.
 * See https://www.samba.org/~tridge/phd_thesis.pdf and https://rsync.samba.org/tech_report/node3.html
 * The checksum is 32-bit long. It is used as a filter for more expensive operations.
 */

class RollingChecksum
{
public:
    RollingChecksum(const std::vector<unsigned char>& data, const int factor);

    //! Recomputes the checksum discarding outgoing and including incoming bytes, returns computed checksum
    long long roll(const unsigned char outgoing, const unsigned char incoming);

    //! Getter for current checksum value
    long long value();
private:
    const int m_block_size;
    const int m_factor;

    long long m_r = 0;
    long long m_r1 = 0;
    long long m_r2 = 0;
};
