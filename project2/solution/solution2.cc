// this is a silly solution
// just to show you how different
// components of this framework work
// please bring your wise to write
// a 'real' solution :)

#include <iostream>
#include <fstream>
#include <string>
#include "dx.h"
#include "kernel.h"
#include<cstring>
int main() {
    /* add your solutions */
    for(int i =1;i<=10;i++)
    {
      if(i==4|| i==6 || i==8|| i == 10)
        continue;
      std::string src = "./cases/case"+to_string(i)+".json";
      ifstream infile(src);
      if(!infile.is_open()){
        continue;
      }
      else
      {
        infile.close();
      }
      
      char* src_char = new char[src.size()+1];
      strcpy(src_char,src.c_str());
      json json_config = parse(src_char);
      std::string result = "./kernels/" +json_config.name+".cc";
      std::vector<string>& grad_to = json_config.grad_to;
      std::vector<string>& in = json_config.ins;
      for(int i = 0;i<in.size();i++)
        std::cout<<in[i]<<std::endl;
      std::vector<string>& out = json_config.outs;
      std::string kernel = json_config.kernel;
      //auto diff
      json_config.kernel = Boost::Internal::dx::te(kernel,in,out,grad_to);

      std::cout<<"ins "<<std::endl;
       for(int i = 0;i<in.size();i++)
        std::cout<<json_config.ins[i]<<std::endl;
       std::cout<<"out "<<std::endl;
         for(int i = 0;i<out.size();i++)
        std::cout<<json_config.outs[i]<<std::endl;
      std::cout<<json_config.kernel<<std::endl;


      //translation
      std::string code = Boost::Internal::compile_project1(json_config);
      std::cout<<code<<std::endl;
      std::string header = "#include \"../run2.h\" \n\n";
      std::ofstream ofile(result, std::ios::out);
      ofile << header;
      ofile << code;
      ofile.close();
    }
    return 0;
}