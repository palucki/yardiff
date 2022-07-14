#pragma once

#include <fstream>

#include "dataprovider.h"

/*!
 * The FileDataProvider class provides data from file.
 */
class FileDataProvider : public DataProvider
{
public:
    FileDataProvider(std::ifstream& file_handle, int buffer_size);
    bool readData() override;

    virtual std::vector<unsigned char>& data() override;

    bool finished() override;

private:
    std::ifstream& m_file_handle;
    std::vector<unsigned char> m_buffer;
};
