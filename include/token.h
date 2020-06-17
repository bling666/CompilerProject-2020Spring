#ifndef TOKEN_H
#define TOKEN_H
#include<string>
class Token
{
public:
    int type;
    std::string stringValue;
    int intValue;
    float floatValue;
    std::string getname()
    {
    	return stringValue;
    }
    int getvalue()
    {
    	return intValue;
    }

};


#endif