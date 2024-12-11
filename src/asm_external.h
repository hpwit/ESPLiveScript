#pragma once
#ifndef __ASM_EXTERNAL
#define __ASM_EXTERNAL
#include <stdio.h>
//#include <vector>
//#include <string>

#include "asm_struct_enum.h"

vector<asm_external> external_links;

uint32_t external_links_array[60];

uint32_t * createExternalLinks()
{
      for(int i=0;i<external_links.size();i++)
  {
    #ifndef __TEST_DEBUG
     external_links_array[i]=(uint32_t) external_links[i].ptr;
     #endif
    }
    return external_links_array;

}

void addExternal(string name,externalType type, void * ptr)
{
    asm_external asmex;
    asmex.name=name;
    asmex.type=type;
    if(ptr!=NULL)
         asmex.ptr=ptr;
    external_links.push_back(asmex);
}

void replaceExternal(string name, void *ptr)
{
   //  vector<asm_external>::iterator it=external_links.begin();
     for(vector<asm_external>::iterator it=external_links.begin();it<external_links.end();it++)
  {
    if(it->name.compare(name)==0)
    {
        it->ptr=ptr;
        return;
    }
  }        
}

int findLink(string label, externalType op)
{
    if(label.find_first_of("(")!=string::npos)
    {
         label=label.substr(0,label.find_first_of("("));
    }
  for(int i=0;i<external_links.size();i++)
  {
        if( (external_links[i].name.compare(label)==0) )//&&  (external_links[i].type==op))
        {
            return i;
        }
  }
 return -1;
}

/*
error_message_struct link()
{
    vector<result_parse_line>::iterator it=asmParsed.begin();
  for(it;it<asmParsed.end();it++)
  {
    if(it->opCodeType==opCodeType::external_var || it->opCodeType==opCodeType::external_call)
    {
        int index=findLink(it->name,it->opCodeType);
    }
  }
}
*/


#endif