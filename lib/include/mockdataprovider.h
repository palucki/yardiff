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
            m_finished = true;
            return false;
        }

        for(int i = 0; i < m_buffer.size(); ++i)
        {
            std::cout << "Mock provider i = " << i << " block position " << m_read_position << '\n';

            if(m_read_position < m_input_data.size())
            {
                std::cout << "read ok\n";
                //read block of data from prepared input
                m_buffer[i] = m_input_data[m_read_position];
            }
            else
            {
                std::cout << "resizing to " << i << "\n";
                m_buffer.resize(i);
                m_finished = true;

                if(m_buffer.size() == 0)
                {
                    return false;
                }

                break;
            }

            ++m_read_position;
        }

        if(m_buffer.size() == m_input_data.size())
        {
            m_finished = true;
        }

        return true;
    }

    std::vector<unsigned char>& data() override
    {
        return m_buffer;
    }

    bool finished() override
    {
        return m_finished;
    }

    std::vector<unsigned char> m_input_data;
    std::vector<unsigned char> m_buffer;
    int m_read_position = 0;
    bool m_finished = false;
};
