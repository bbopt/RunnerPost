#ifndef __UTILS__
#define __UTILS__

#include "runner_defines.hpp"

#include "runnerpost_platform.hpp"
#include "runnerpost_nsbegin.hpp"
    
std::pair<std::string,std::vector<std::string>> extract_from_bracket(std::string &s, const char * keyValSep =" ");

std::vector<std::string> extract_words(std::string &s);

void toUpperCase(std::string& str);


struct StatOutputType
{
    
    /// Available output types
    enum Type
    {
        OBJ,        ///< Objective value
        CST,         ///< Constraint value: <=0 is feas >0 is infeas
        SOL,         ///<  Variable value
        TIME,        ///< Time elapsed since start
        CNT_EVAL,   ///< Evaluation count
        INFEAS,        ///< Infeasibility flag: 0 is infeas, 1 is feas
        UNDEFINED ///< Output ignored
    };

    Type _type;
    
    
    // Implicit constructor
    StatOutputType()
    {
        _type = Type::UNDEFINED;
    }
    
    /**
     Convert a string (ex "OBJ", "CST", ...)
     to a StatsOutputType.
     */
    StatOutputType(const std::string & s) ;
    
    /**
     Make a StatOutputType from its type.
     */
    StatOutputType(const Type & t)
    {
        _type = t;
    }
    
    // Assignement operator
    StatOutputType(const StatOutputType & b)
    {
        _type = b._type;
    }
    
    /// Verify if the BBOutputType defines a constraint
    bool isConstraint() const
    {
        return (_type == Type::CST);
    }
    
    // TODO make a template
    bool isOfType(const StatOutputType & type) const
    {
        return (_type == type);
    }
    
    bool isObjective() const
    {
        return (_type == Type::OBJ);
    }

    bool isUndefined() const
    {
        return (_type == Type::UNDEFINED);
    }
    
    std::string display() const;
    
    bool operator==(const StatOutputType& so) const
    {
        return (this->_type == so._type);

    };
    bool operator!=(const StatOutputType& so) const
    {
        return this->_type != so._type;

    };
    

    bool operator==(const Type& type) const
    {
        return (this->_type == type);

    };
    bool operator!=(const Type& type) const
    {
        return this->_type != type;

    };
    
};



/// Definition for the list of blackbox output types
typedef std::vector<StatOutputType> StatOutputTypeList;

typedef StatOutputTypeList::const_iterator StatOutputTypeListIt;


/// Utility for StatOutputType
/**
 Convert a string containing multiple StatOutputTypes (ex "OBJ CST")
 to a StatOutputTypeList.
 */
StatOutputTypeList stringToStatOutputTypeList(const std::string &s);

/// Utility for StatOutputType
/**
 Convert a StatOutputTypeList into a string
 */
std::string StatOutputTypeListToString ( const StatOutputTypeList & sotList );

/// Count the number of constraints
size_t getNbConstraints(const StatOutputTypeList& sotList);

/// Count the number of objectives
size_t getNbObj(const StatOutputTypeList& sotList);

size_t getNbOfType(const StatOutputTypeList& sotList, const StatOutputType & type);


/// Displau StatOutputType
inline std::ostream& operator<<(std::ostream& os, const StatOutputType &sot)
{
    os << sot.display();

    return os;
}

/// Display StatOutputTypeList
inline std::ostream& operator<<(std::ostream& out, const StatOutputTypeList &sotList)
{
    StatOutputTypeListIt it;
    bool first = true;
    for (it = sotList.begin(); it != sotList.end(); ++it)
    {
        if (!first)
        {
            out << " ";
        }
        out << *it;
        first = false;
    }
    return out;
}

std::istream& operator>>(std::istream& is, StatOutputTypeList& sotList);



#include "runnerpost_nsend.hpp"

#endif


