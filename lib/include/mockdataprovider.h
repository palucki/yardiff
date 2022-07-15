#pragma once

#include "dataprovider.h"

/*!
 * The MockDataProvider class provides data for unit testing purposes
 */
class MockDataProvider : public DataProvider
{
public:
    bool readData() override
    {
        return m_ok;
    }

    std::vector<unsigned char>& data() override
    {
        return m_buffer;
    }

    bool finished() override
    {
        return m_finished;
    }

    std::vector<unsigned char> m_buffer;
    bool m_ok = true;
    bool m_finished = false;
};
