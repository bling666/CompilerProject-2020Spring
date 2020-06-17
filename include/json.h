//
//  json.h
//  
//
//  Created by ji luyang on 2020/5/8.
//
#ifndef JOSN_H
#define JSON_H

#include <string>
#include <vector>
using namespace std;
class json{
    public:
    string name;
    vector<string> ins;
    vector<string> outs;
    string data_type;
    string kernel;
};
json parse(char *filename);
/* json_h */

#endif