#pragma once

#include <vector>

/*!
 * The DataProvider class defines an interface for providing data as a std::vector of bytes (unsigned char).
 */

class DataProvider
{
public:
    virtual ~DataProvider() = default;

    //! reads data, returns true as long as can read data, false otherwise
    virtual bool readData() = 0;

    //! returns data container
    virtual std::vector<unsigned char>& data() = 0;

    //! sets the read position to the given position_from_start from the beginning of the data stream.
    virtual void setReadPosition(long position_from_start) = 0;
};
