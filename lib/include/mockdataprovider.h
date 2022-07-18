#pragma once

#include <iostream>

#include "dataprovider.h"

/*!
 * The MockDataProvider class provides data for unit testing purposes
 */
class MockDataProvider : public DataProvider
{
public:
    MockDataProvider(const std::vector<unsigned char>& input, const int block_size)
       : m_input_data(input)
    {
        m_buffer.resize(block_size, 0);
    }

    bool readData() override
    {
        if(m_buffer.size() == 0)
        {
            return false;
        }

        for(int i = 0; i < m_buffer.size(); ++i)
        {
            if(m_read_position < m_input_data.size())
            {
                //read block of data from prepared input
                m_buffer[i] = m_input_data[m_read_position];
            }
            else
            {
                m_buffer.resize(i);

                if(m_buffer.size() == 0)
                {
                    return false;
                }

                break;
            }

            ++m_read_position;
        }

        return true;
    }

    void setReadPosition(long position_from_start) override
    {
        m_read_position = position_from_start;
    }

    std::vector<unsigned char>& data() override
    {
        return m_buffer;
    }

    std::vector<unsigned char> m_input_data;
    std::vector<unsigned char> m_buffer;
    int m_read_position = 0;
};
