#pragma once

#include <string>
#include <vector>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

/*
    Serializable plain data structure that contains file's name, extension, binary data.
*/
struct file_pds
{
	std::string name;
	std::string extension;
    std::vector<char> bytes;
	
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & name & extension & bytes;
    }
};