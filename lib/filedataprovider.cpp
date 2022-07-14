#include "include/filedataprovider.h"

#include <iostream>

FileDataProvider::FileDataProvider(std::ifstream &file_handle, int buffer_size)
    : m_file_handle(file_handle)
{
    m_buffer.resize(buffer_size);
    m_file_handle.clear();
    m_file_handle.seekg(0, std::ios::beg);
}

bool FileDataProvider::readData()
{
    m_file_handle.read((char*)m_buffer.data(), m_buffer.size());

    std::streamsize succesfully_read_bytes = m_file_handle.gcount();
    std::cout << "succesfully_read_bytes " << succesfully_read_bytes << "\n";

    if(succesfully_read_bytes == 0)
    {
        std::cout << "END OF THE GAME\n";
        return false;
    }
    else if(succesfully_read_bytes < m_buffer.size())
    {
        std::cout << "last chunk, resizing buffer\n";
        m_buffer.resize(succesfully_read_bytes);
    }

    return true;
}

std::vector<unsigned char> &FileDataProvider::data()
{
    return m_buffer;
}

bool FileDataProvider::finished()
{
    return m_file_handle.eof();
}
