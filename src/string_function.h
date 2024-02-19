#pragma once
#ifndef __ASM_STRING
#define __ASM_STRING
#include <stdio.h>
using namespace std; 
#include <string>
#include <stdlib.h>
#include <memory>
#include <vector>
//#include <iostream>
//#include <functional>
#include <Arduino.h>

vector<string> split(string s, string delimiter)
 {
        vector <string> res;
        int pos = 0
        ;
         pos = s.find(delimiter);
         while(pos >=0 ){
        pos = s.find(delimiter);
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+delimiter.size()); 
         pos = s.find(delimiter);
    }
    if(s.size()>0)
        {
             res.push_back(s);
        }
    return res;
 }



template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )

{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside

}

std::string trim(std::string s)

{
  if(s.find("//")!=-1)
  {
      s=s.substr(0,s.find("//")).c_str();
  }
    s.erase(s.find_last_not_of(" \n\r\t")+1);
  
  return s.replace(0,s.find_first_not_of(" \n\r\t"),"");
}

#endif
