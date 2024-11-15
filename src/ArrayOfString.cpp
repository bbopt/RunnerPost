/**
 \file   ArrayOfString.cpp
 \brief  Custom class for array of strings (implementation). Adapted from NOMAD 4 ArrayOfString
 \author Viviane Rochon Montplaisir
 \date   February 2018
 \see    ArrayOfString.hpp
 */
#include <algorithm>
#include <sstream>

#include "ArrayOfString.hpp"


std::ostream& RUNNERPOST::operator<<(std::ostream& out, const RUNNERPOST::ArrayOfString& arrayOfString)
{
    out << arrayOfString.display();

    return out;
}


/*-----------------------------------------------------------*/
/*                        Constructors                       */
/*-----------------------------------------------------------*/
RUNNERPOST::ArrayOfString::ArrayOfString(size_t n, const std::string& initString)
  : _array()
{
    for (size_t i = 0; i < n; i++)
    {
        _array.push_back(initString);
    }
}


RUNNERPOST::ArrayOfString::ArrayOfString(const std::string & inputString,
                                    const std::string & separators)
    : _array()
{
    _array = splitString(inputString, separators);
}


/*-----------------------------------------------*/
/*                    destructor                 */
/*-----------------------------------------------*/
RUNNERPOST::ArrayOfString::~ArrayOfString()
{
}


/*-----------------------------------------------------------*/
/*                       '[]' operators                      */
/*-----------------------------------------------------------*/
const std::string& RUNNERPOST::ArrayOfString::operator[](size_t index) const
{
    if ( index >= _array.size())
    {
        std::ostringstream oss;
        oss << "ArrayOfString: index = " << index << " is out of bounds [0, " << _array.size()-1 << "]";
        
        std::cout << oss.str();
        // TODO throw Exception(__FILE__, __LINE__, oss.str());
    }

    return _array[index];
}


/*------------------------------------------------*/
/* Replace string s at position index.            */
/* Throw an error if the index is too large.      */
/* Note: operator[] returns const only, so to     */
/* modify an array, replace has to be used.       */
/*------------------------------------------------*/
void RUNNERPOST::ArrayOfString::replace(const size_t index, const std::string& s)
{
    if ( index >= _array.size())
    {
        std::ostringstream oss;
        oss << "ArrayOfString: index = " << index << " is out of bounds [0, " << _array.size()-1 << "]";
        std::cout << oss.str();
        // TODO throw Exception(__FILE__, __LINE__, oss.str());throw RUNNERPOST::Exception(__FILE__, __LINE__, oss.str());
    }
    _array[index] = s;
}


/*------------------------------------------------*/
/*  Erase element at position index               */
/*  Return true if string was removed from array. */
/*------------------------------------------------*/
bool RUNNERPOST::ArrayOfString::erase(const size_t index)
{
    size_t k = 0;
    std::vector<std::string>::const_iterator it;

    for (it = _array.begin(); it != _array.end(); ++it)
    {
        if (index == k)
        {
            _array.erase(it);
            return true;
        }
        k++;
    }

    return false;
}


// Return first index of string s in array.
// Return -1 if s not found.
int RUNNERPOST::ArrayOfString::find(const std::string& s) const
{
    int index = -1;

    for (size_t i = 0; i < _array.size(); i++)
    {
        if (s == _array[i])
        {
            index = (int)i;
            break;
        }
    }

    return index;
}


/*------------*/
/* Comparison */
/*------------*/
bool RUNNERPOST::ArrayOfString::operator== (const RUNNERPOST::ArrayOfString &array) const
{
    return (_array == array._array);
}


/*---------------*/
/*    Display    */
/*---------------*/
std::string RUNNERPOST::ArrayOfString::display() const
{
    std::string s;

    if ( size() == 0 )
        s += " - ";

    for (size_t i = 0; i < size(); i++)
    {
        if (i > 0)
        {
            s += " ";
        }
        s += _array[i];
    }

    return s;
}


/*-----------------------------------*/
/* Split                             */
/* - Used as helper for constructor. */
/*-----------------------------------*/

std::vector<std::string> const
RUNNERPOST::ArrayOfString::splitString(const std::string & inputString,
                                       const std::string & separators)
{
    std::vector<std::string> array;

    if (inputString.size() == 0)
    {
        return array;
    }

    size_t splitIndex = 0, index1 = 0, length;

    // Fill array with strings found in inputString.
    // inputString is a list of std::strings, separated by any char in separators.
    // Sample inputString:
    // OBJ PB EB

    while (splitIndex != std::string::npos)
    {
        // Find index of first non-separator.
        index1 = inputString.find_first_not_of(separators, index1);
        if (index1 == std::string::npos)
        {
            break;
        }
        splitIndex = inputString.find_first_of(separators, index1+1);
        if (splitIndex == std::string::npos)
        {
            length = inputString.size() - index1;
        }
        else
        {
            length = splitIndex - index1;
        }

        std::string si = inputString.substr(index1, length);
        array.push_back(si);

        index1 = splitIndex + 1;    // Start looking after separator
    }

    return array;
}


RUNNERPOST::ArrayOfString RUNNERPOST::ArrayOfString::combineAndAddPadding(const RUNNERPOST::ArrayOfString & s1, const RUNNERPOST::ArrayOfString & s2)
{
    // Add a padding on the first string of each pair of string. The padding must assure that the second strings are all aligned.
    // Also add a return at the end of the second string.

    size_t sizeS1 = s1.size();
    if (sizeS1 != s2.size() )
    {
        std::cout << "s1 and s2 must have the same of number of elements." << std::endl;
        // TODO throw RUNNERPOST::Exception(__FILE__, __LINE__, "s1 and s2 must have the same of number of elements.");
    }

    // Max length of first+second elements in s
    size_t maxL = 0;
    for (size_t e = 0 ; e < sizeS1 ; e++)
    {
        maxL = std::max(maxL,s1[e].length()+s2[e].length());
    }

    // Pad the first element to get the same overall length for all + combine first and second + add return
    RUNNERPOST::ArrayOfString paddedString("\n");
    for (size_t e = 0 ; e < sizeS1 ; e++)
    {
        size_t padL = 1 + maxL - s1[e].length() - s2[e].length(); // Add one ' ' at least (for space after colon)
        std::string padS = s1[e];
        padS.insert(s1[e].length(), padL,' ');
        padS += s2[e]+'\n';
        paddedString.add(padS);
    }

    return paddedString;
}





