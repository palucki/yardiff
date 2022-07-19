#include "filedataprovider.h"

FileDataProvider::FileDataProvider(std::ifstream &file_handle, int block_size)
    : m_file_handle(file_handle)
{
    m_buffer.resize(block_size);
    m_file_handle.clear();
    m_file_handle.seekg(0, std::ios::beg);
}

bool FileDataProvider::readData()
{
    m_file_handle.read((char*)m_buffer.data(), m_buffer.size());

    std::streamsize succesfully_read_bytes = m_file_handle.gcount();

    if(succesfully_read_bytes == 0)
    {
        //no more data in buffer
        return false;
    }
    else if(succesfully_read_bytes < m_buffer.size())
    {
        //last chunk, smaller than block size
        m_buffer.resize(succesfully_read_bytes);
    }

    return true;
}

void FileDataProvider::setReadPosition(long position_from_start)
{
    m_file_handle.clear();
    m_file_handle.seekg(position_from_start, std::ios::beg);
}

std::vector<unsigned char> &FileDataProvider::data()
{
    return m_buffer;
}
