#pragma once

#include <fstream>

#include "dataprovider.h"

/*!
 * The FileDataProvider class provides data from file.
 * It reads chunks of data not larger than given buffer size.
 */
class FileDataProvider : public DataProvider
{
public:
    FileDataProvider(std::ifstream& file_handle, int block_size);
    bool readData() override;

    void setReadPosition(long position_from_start) override;

    std::vector<unsigned char>& data() override;

    bool finished() override;

private:
    std::ifstream& m_file_handle;
    std::vector<unsigned char> m_buffer;
};
