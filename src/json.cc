//
//  parse_json.cpp
//  
//
//  Created by ji luyang on 2020/5/8.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "json.h"
using namespace std;

json parse(char *filename){
    ifstream infile(filename);
    json tmp;
    if(!infile.is_open()){
        cout<<"error opening!\n";
        return tmp;
    }
        char buf[256];
        infile.getline(buf,256);
        //read name
        infile.getline(buf,256);
        string tmp_name;
        int nindex=-1,nbegin=-1,cnt=0;
        for(int i=0;i<256;i++){
            if(buf[i]=='n'&&buf[i+1]=='a'&&buf[i+2]=='m'&&buf[i+3]=='e'){
                nindex=i;
                break;
            }
        }
        for(int i=nindex;i<256;i++){
            if(buf[i]=='"'){
                cnt++;
            }
            if(cnt==2){
                nbegin=i+1;
                break;
            }
        }
        for(int i=nbegin;buf[i]!='"';i++){
            tmp_name.push_back(buf[i]);
        }
        tmp.name=tmp_name;
        //read ins
        infile.getline(buf,256);
        int ibegin=-1;
        for(int i=0;i<256;i++){
            if(buf[i]=='['){
                ibegin=i+1;
                break;
            }
        }
        int i=ibegin;
        while(1){
            string tmpin;
            while(1){
                if(buf[i]==','||buf[i]==']') break;
                tmpin.push_back(buf[i++]);
            }
            if(tmpin.size()>2) tmp.ins.push_back(tmpin.substr(1,tmpin.size()-2));
            if(buf[i]==']') break;
            else i+=2;
            
        }
        //read outs
        infile.getline(buf,256);
        int obegin=-1;
        for(int i=0;i<256;i++){
            if(buf[i]=='['){
                obegin=i+1;
                break;
            }
        }
        i=obegin;
        while(1){
            string tmpout;
            while(1){
                if(buf[i]==','||buf[i]==']') break;
                tmpout.push_back(buf[i++]);
            }
            if(tmpout.size()>2) tmp.outs.push_back(tmpout.substr(1,tmpout.size()-2));
            if(buf[i]==']') break;
            else i+=2;
        }
      //read datatype
    infile.getline(buf,256);
    string tmp_datatype;
    int dindex=-1,dbegin=-1;
    cnt=0;
    for(int i=0;i<256;i++){
        if(buf[i]=='t'&&buf[i+1]=='y'&&buf[i+2]=='p'&&buf[i+3]=='e'){
            dindex=i;
            break;
        }
    }
    for(int i=dindex;i<256;i++){
        if(buf[i]=='"'){
            cnt++;
        }
        if(cnt==2){
            dbegin=i+1;
            break;
        }
    }
    for(int i=dbegin;buf[i]!='"';i++){
        tmp_datatype.push_back(buf[i]);
    }
    tmp.data_type=tmp_datatype;
    //read kernel
    infile.getline(buf,256);
    string tmp_kernel;
    int kindex=-1,kbegin=-1;
    cnt=0;
    for(int i=0;i<256;i++){
        if(buf[i]=='k'&&buf[i+1]=='e'&&buf[i+2]=='r'&&buf[i+3]=='n'){
            kindex=i;
            break;
        }
    }
    for(int i=kindex;i<256;i++){
        if(buf[i]=='"'){
            cnt++;
        }
        if(cnt==2){
            kbegin=i+1;
            break;
        }
    }
    for(int i=kbegin;buf[i]!='"';i++){
        tmp_kernel.push_back(buf[i]);
    }
    tmp.kernel=tmp_kernel;
    cout<<"parse success\n";
    
    infile.getline(buf,256);
        obegin=-1;
        for(int i=0;i<256;i++){
            if(buf[i]=='['){
                obegin=i+1;
                break;
            }
        }
        i=obegin;
        while(1){
            string tmpout;
            while(1){
                if(buf[i]==','||buf[i]==']') break;
                tmpout.push_back(buf[i++]);
            }
            if(tmpout.size()>2) tmp.grad_to.push_back(tmpout.substr(1,tmpout.size()-2));
            if(buf[i]==']') break;
            else i+=2;
        }
        return tmp;
}
/*int main(){
    json res=parse("/Users/jiluyang/Desktop/CompilerProject-2020Spring-Part1/project1/cases/case10.json");
    for(int i=0;i<res.ins.size();i++) cout<<res.ins[i]<<" ";
    cout<<endl;
    for(int i=0;i<res.outs.size();i++) cout<<res.outs[i]<<" ";
    cout<<endl;
    cout<<res.kernel<<endl;
    return 0;
}*/
