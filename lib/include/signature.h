#pragma once

#include <string>
#include <unordered_map>

//! The Signature type holds the signature map.
//! The key in the map is the fast signature (a.k.a. rolling checksum).
//! The value in the map consist of strong signature and block index.

struct BlockData
{
    std::string m_strong_signature;
    int m_block_index;

    bool operator==(const BlockData& other) const
    {
        return this->m_strong_signature == other.m_strong_signature &&
               this->m_block_index == other.m_block_index;
    }
};

using Signature = std::unordered_map<unsigned int, BlockData>;
