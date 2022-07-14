#pragma once

#include <vector>

/*!
 * The DataProvider class defines an interface for providing data as a std::vector of bytes (unsigned char).
 */

class DataProvider
{
public:
    virtual ~DataProvider() = default;

    //! reads data, returns true if succeded, false otherwise
    virtual bool readData() = 0;

    //! returns data container
    virtual std::vector<unsigned char>& data() = 0;

    virtual bool finished() = 0;
};
