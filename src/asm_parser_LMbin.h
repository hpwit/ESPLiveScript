#pragma once
#ifndef __ASM_LM
#define __ASM_LM
#include <stdlib.h>
#include "asm_struct_enum.h"


operandeType op_mov_n[2]={operandeType::registers,operandeType::registers};

uint32_t bin_mov_n(uint32_t *values)
{
  return 0xd + ((values[0] <<4) & 0xF0 ) + ( (values[1] <<8) & 0xF00) ;
}

operandeType op_mov[3]={operandeType::registers,operandeType::registers};
uint32_t bin_mov(uint32_t *values)
{
  return  ((values[0] <<12) & 0xF000 ) + ( (values[1] <<8) & 0xF00) +( (values[1] <<4) & 0xF0 ) +0x200000;
}

operandeType op_l32i[3]={operandeType::registers,operandeType::registers,operandeType::l0_1020};
uint32_t bin_l32i(uint32_t *values)
{
 return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +0x2000+ (( ((values[2]/4) <<16) & 0xFF0000 ));
}


operandeType op_l8ui[3]={operandeType::registers,operandeType::registers,operandeType::l0_255};
uint32_t bin_l8ui(uint32_t *values)
{
 return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( ((values[2]) <<16) & 0xFF0000 ));
}

operandeType op_s8i[3]={operandeType::registers,operandeType::registers,operandeType::l0_255};
uint32_t bin_s8i(uint32_t *values)
{
 return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + 0x4000+ (( ((values[2]) <<16) & 0xFF0000 ));
}


operandeType op_l16si[3]={operandeType::registers,operandeType::registers,operandeType::l0_510};
uint32_t bin_l16si(uint32_t *values)
{
 return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + 0x9000+(( ((values[2]/2) <<16) & 0xFF0000 ));
}

operandeType op_s16i[3]={operandeType::registers,operandeType::registers,operandeType::l0_510};
uint32_t bin_s16i(uint32_t *values)
{
 return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + 0x5000+ (( ((values[2]/2) <<16) & 0xFF0000 ));
}




operandeType op_l32i_n[3]={operandeType::registers,operandeType::registers,operandeType::l0_60};
uint32_t bin_l32i_n(uint32_t *values)
{
 return 0x8+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( ((values[2]/4) <<12) & 0xF000 ));
}


operandeType *op_add=op_mov;
uint32_t bin_add(uint32_t *values)
{
    return (( (values[2] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +(( (values[0] <<12) & 0xF000 )) +0x800000;
}

operandeType *op_quou=op_mov;
uint32_t bin_quou(uint32_t *values)
{
    return (( (values[2] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +(( (values[0] <<12) & 0xF000 )) +0x820000;
}

operandeType *op_sub=op_mov;
uint32_t bin_sub(uint32_t *values)
{
    return (( (values[2] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +(( (values[0] <<12) & 0xF000 )) +0xc00000;
}

operandeType *op_mull=op_mov;
uint32_t bin_mull(uint32_t *values)
{
    return (( (values[2] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +(( (values[0] <<12) & 0xF000 )) +0x820000;
}

uint32_t bin_add_n(uint32_t *values)
{
    return 10+ (( (values[2] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +(( (values[0] <<12) & 0xF000 )) ;
}
operandeType op_addi[3]={operandeType::registers,operandeType::registers,operandeType::lm128_127};
uint32_t bin_addi(uint32_t *values)
{
    return 2+ (( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + 0xc000 + (( (values[2] <<16) & 0xFF0000 )) ;
}
operandeType op_addi_n[3]={operandeType::registers,operandeType::registers,operandeType::lm1_15};;
uint32_t bin_addi_n(uint32_t *values)
{
    int v=values[2];
    if (values[2]==-1)
        v=0;
    return 0xb+ (( (values[0] <<12) & 0xF000 )) + (( (values[1] <<8) & 0xF00 ))  + ((v<<4) & 0xF0 ) ;
}
operandeType op_s32i[3]={operandeType::registers,operandeType::registers,operandeType::l0_1020};;
uint32_t bin_s32i(uint32_t *values)
{
    return 0x2+(( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) +0x6000+ (( ((values[2]/4) <<16) & 0xFF0000 ));
}


operandeType op_s32i_n[3]={operandeType::registers,operandeType::registers,operandeType::l0_60};;
uint32_t bin_s32i_n(uint32_t *values)
{
    return 0x9 + (( (values[0] <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( ((values[2]/4) <<12) & 0xF000 ));
}

operandeType op_bnez[3]={operandeType::registers,operandeType::label};;
uint32_t bin_bnez(uint32_t *values)
{
   
    return 0x56 +  (( (values[0] <<8) & 0xF00 ));
   // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_bnez(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
            uint32_t dif=destination_address-current_address-4;
            return value+ ((dif<<12) & 0xFFF000);
          
}



operandeType op_blt[3]={operandeType::registers,operandeType::registers,operandeType::label};
uint32_t bin_blt(uint32_t *values)
{
    
     return 0x7 +  (( (values[1] <<4) & 0xF0 ))+ (( (values[0] <<8) & 0xF00 ))+0x2000;
    
   // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_blt(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
            uint32_t dif=destination_address-current_address-4;
          
          return value+ ((dif<<16) & 0xFF0000);
        
}


operandeType op_bge[3]={operandeType::registers,operandeType::registers,operandeType::label};
uint32_t bin_bge(uint32_t *values)
{
    
     return 0x7 +  (( (values[1] <<4) & 0xF0 ))+ (( (values[0] <<8) & 0xF00 ))+0xa000;
    
   // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_bge(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
            uint32_t dif=destination_address-current_address-4;
          
          return value+ ((dif<<16) & 0xFF0000);
        
}

operandeType op_j[3]={operandeType::label};
uint32_t bin_j(uint32_t *values)
{
    
     return 0x6;
    
   // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_j(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
            uint32_t dif=destination_address-current_address-4;
          
          return value+ ((dif<<6) & 0xFFFFC0);
        
}

operandeType op_slli[3]={operandeType::registers,operandeType::registers,operandeType::l1_31};;
uint32_t bin_slli(uint32_t *values)
{
    return (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}

operandeType op_movi[3]={operandeType::registers,operandeType::lm2048_2047};
uint32_t bin_movi(uint32_t *values)
{
     return  2 + (( (values[0] <<4) & 0xF0 )) +   ((values[1]) &0xF00 )  + (0xa << 12)  +   ((values[1]<<16) &0xFF0000 ) ;   

   // return  0xc + (( (values[0] <<8) & 0xF00 )) +   ((values[1]<<12) &0xF000 ) +  ((values[1]) & 0x70 ) ;   
}
operandeType op_movi_n[3]={operandeType::registers,operandeType::lm32_95};
uint32_t bin_movi_n(uint32_t *values)
{
    return  0xc + (( (values[0] <<8) & 0xF00 )) +   ((values[1]<<12) &0xF000 ) +  ((values[1]) & 0x70 ) ;   
}

operandeType op_and[3]={operandeType::registers,operandeType::registers,operandeType::registers};
uint32_t bin_and(uint32_t *values)
{
    return  (( (values[2] <<4) & 0xF0 )) +   ((values[1]<<8) &0xF00 )  +  ((values[0]<<12) &0x0F000 ) +0x100000 ;   
}

operandeType op_remu[3]={operandeType::registers,operandeType::registers,operandeType::registers};
uint32_t bin_remu(uint32_t *values)
{
    return  (( (values[2] <<4) & 0xF0 )) +   ((values[1]<<8) &0xF00 )  +  ((values[0]<<12) &0x0F000 ) +0xe20000;   
}

operandeType op_entry[2]={operandeType::registers,operandeType::l0_32760};
uint32_t bin_entry(uint32_t *values)
{
    return  0x36 +  (( (values[0] <<8) & 0xF00 )) +    (((values[1]/8)<<12) &0xFF000 )  ;   
}
operandeType op_word[2]={operandeType::l0_FFFFFFFF};
uint32_t bin_word(uint32_t *values)
{
    return values[0]  ;   
}

uint32_t bin_nop_n(uint32_t *values)
{
    return  0xf03d;   ;   
}
uint32_t bin_nop(uint32_t *values)
{
    return  0x0020f0;      
}
operandeType op_l32r[2]={operandeType::registers,operandeType::label};
uint32_t bin_l32r(uint32_t *values)
{
    return  1+ ((values[0]<<4)&0xF0);   
}


operandeType op_call8[2]={operandeType::label};
uint32_t bin_call8(uint32_t *values)
{
    return  0x25;   
}
uint32_t jump_call8(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
    if((((destination_address)/4)*4)!=destination_address)
    {
        printf("impossible de calculer le jump\n");
        return 0;
    }
    else 
    {
        uint32_t dif=((destination_address-(current_address&0xFFFFFFFC))-4) & 0xFFFFFFC;
        /*
                uint32_t recalculate=(((current_address & 0xFFFF)+dif*4)+3)& 0xFFFc ;
        if(recalculate!=(destination_address & 0xFFFF))
        {
        
            recalculate=((current_address & 0xFFFF)+3+4)& 0xFFFFFc ;
            if(recalculate==(destination_address & 0xFFFF))
                {
                        dif++;
                }
                else
                {
                    printf("not it\n");
                }
        }ffd
        */
        return  value+((dif<<4) & 0xFFFFC0);
    }    
}

uint32_t jump_l32r(uint32_t value,uint32_t current_address,uint32_t destination_address)
{
    if((((destination_address)/4)*4)!=destination_address)
    {
        printf("impossible de calculer le jump\n");
        return 0;
    }
    else 
    {
        uint32_t dif=((destination_address-current_address)/4) & 0xFFFF;
        uint32_t recalculate=(((current_address & 0xFFFF)+dif*4)+3)& 0xFFFc ;
        if(recalculate!=(destination_address & 0xFFFF))
        {
        
            recalculate=((current_address & 0xFFFF)+3+4)& 0xFFFFFc ;
            if(recalculate==(destination_address & 0xFFFF))
                {
                        dif++;
                }
                else
                {
                    printf("not it\n");
                }
        }
        return  value+((dif<<8) & 0xFFFF00);
    }    
}

operandeType op_callx8[2]={operandeType::registers};
uint32_t bin_callx8(uint32_t *values)
{
    return 0xe0 + ((values[0]<<8 ) & 0xF00);
}

uint32_t bin_retw_n(uint32_t *values)
{
    return 0xf01d;
}

operandeType op_global[2]={operandeType::label};
operandeType op_movExt[2]={operandeType::registers ,operandeType::label};
uint32_t bin_movExt(uint32_t *values)
{

    return values[0];
}
#endif