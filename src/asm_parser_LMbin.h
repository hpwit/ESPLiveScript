#pragma once
#ifndef __ASM_LM
#define __ASM_LM
#include <stdlib.h>
#include "asm_struct_enum.h"

operandeType op_rsr[2] = {operandeType::registers, operandeType::l0_255};

uint32_t bin_rsr(uint32_t *values)
{
    return 0x0 + ((values[0] << 4) & 0xF0) + ((values[1] << 8) & 0xFF00) +0x030000;
}

uint32_t bin_wsr(uint32_t *values)
{
    return 0x0 + ((values[0] << 4) & 0xF0) + ((values[1] << 8) & 0xFF00) +0x130000;
}

uint32_t bin_mov_n(uint32_t *values)
{
    return 0xd + ((values[0] << 4) & 0xF0) + ((values[1] << 8) & 0xF00);
}

uint32_t bin_sll(uint32_t *values)
{
    return ((values[0] << 12) & 0xF000) + ((values[1] << 8) & 0xF00) + 0xa10000;
}
operandeType op_ssl[1]={operandeType::registers};
uint32_t bin_ssl(uint32_t *values)
{
    return  ((values[0] << 8) & 0xF00) +0x401000;
}

uint32_t bin_srl(uint32_t *values)
{
    return ((values[0] << 12) & 0xF000) + ((values[1] << 4) & 0xF0) + 0x910000;
}
uint32_t bin_mov(uint32_t *values)
{
    return ((values[0] << 12) & 0xF000) + ((values[1] << 8) & 0xF00) + ((values[1] << 4) & 0xF0) + 0x200000;
}
uint32_t bin_abs(uint32_t *values)
{
    return ((values[0] << 12) & 0xF000) + ((values[1] << 4) & 0xF0) + 0x600100;
}

uint32_t bin_neg(uint32_t *values)
{
    return ((values[0] << 12) & 0xF000) + ((values[1] << 4) & 0xF0)  + 0x600000;
}

uint32_t bin_l32i(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x2000 + ((((values[2] / 4) << 16) & 0xFF0000));
}


uint32_t bin_l8ui(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + ((((values[2]) << 16) & 0xFF0000));
}


uint32_t bin_s8i(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x4000 + ((((values[2]) << 16) & 0xFF0000));
}


uint32_t bin_l16si(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x9000 + ((((values[2] / 2) << 16) & 0xFF0000));
}

uint32_t bin_l16ui(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x1000 + ((((values[2] / 2) << 16) & 0xFF0000));
}


uint32_t bin_s16i(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x5000 + ((((values[2] / 2) << 16) & 0xFF0000));
}


uint32_t bin_l32i_n(uint32_t *values)
{
    return 0x8 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + ((((values[2] / 4) << 12) & 0xF000));
}


uint32_t bin_add(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0] << 12) & 0xF000)) + 0x800000;
}


uint32_t bin_quou(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0] << 12) & 0xF000)) + 0xC20000;
}


uint32_t bin_sub(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0] << 12) & 0xF000)) + 0xc00000;
}


uint32_t bin_mull(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0] << 12) & 0xF000)) + 0x820000;
}

uint32_t bin_add_n(uint32_t *values)
{
    return 10 + (((values[2] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0] << 12) & 0xF000));
}
operandeType op_addi[3] = {operandeType::registers, operandeType::registers, operandeType::lm128_127};
uint32_t bin_addi(uint32_t *values)
{
    return 2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0xc000 + (((values[2] << 16) & 0xFF0000));
}
operandeType op_addi_n[3] = {operandeType::registers, operandeType::registers, operandeType::lm1_15};
;
uint32_t bin_addi_n(uint32_t *values)
{
    int v = values[2];
    if (values[2] == -1)
        v = 0;
    return 0xb + (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + ((v << 4) & 0xF0);
}


uint32_t bin_s32i(uint32_t *values)
{
    return 0x2 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x6000 + ((((values[2] / 4) << 16) & 0xFF0000));
}



uint32_t bin_s32i_n(uint32_t *values)
{
    return 0x9 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + ((((values[2] / 4) << 12) & 0xF000));
}

operandeType op_bnez[3] = {operandeType::registers, operandeType::label};
;
uint32_t bin_bnez(uint32_t *values)
{

    return 0x56 + (((values[0] << 8) & 0xF00));
    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t bin_beqz(uint32_t *values)
{

    return 0x16 + (((values[0] << 8) & 0xF00));
    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_bnez(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;
    return value + ((dif << 12) & 0xFFF000);
}

operandeType op_blt[3] = {operandeType::registers, operandeType::registers, operandeType::label};
uint32_t bin_blt(uint32_t *values)
{

    return 0x7 + (((values[1] << 4) & 0xF0)) + (((values[0] << 8) & 0xF00)) + 0x2000;

    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_blt(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;

    return value + ((dif << 16) & 0xFF0000);
}

operandeType op_bge[3] = {operandeType::registers, operandeType::registers, operandeType::label};
uint32_t bin_bge(uint32_t *values)
{

    return 0x7 + (((values[1] << 4) & 0xF0)) + (((values[0] << 8) & 0xF00)) + 0xa000;

    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_bge(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;

    return value + ((dif << 16) & 0xFF0000);
}

operandeType *op_bne= op_bge;   // [3] = {operandeType::registers, operandeType::registers, operandeType::label};
uint32_t bin_bne(uint32_t *values)
{

    return 0x7 + (((values[1] << 4) & 0xF0)) + (((values[0] << 8) & 0xF00)) + 0x9000;

    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_bne(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;

    return value + ((dif << 16) & 0xFF0000);
}

operandeType *op_beq=op_bge;    //[3] = {operandeType::registers, operandeType::registers, operandeType::label};
uint32_t bin_beq(uint32_t *values)
{

    return 0x7 + (((values[1] << 4) & 0xF0)) + (((values[0] << 8) & 0xF00)) + 0x1000;

    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_beq(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;

    return value + ((dif << 16) & 0xFF0000);
}
operandeType op_j[3] = {operandeType::label};
uint32_t bin_j(uint32_t *values)
{

    return 0x6;

    // (( ((32-values[2]) <<4) & 0xF0 )) + (( (values[1] <<8) & 0xF00 )) + (( (values[0]) <<12) & 0x0F000)  + 0x10000 + (( ((32-values[2]) <<16) & 0x100000 ));
}
uint32_t jump_j(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    uint32_t dif = destination_address - current_address - 4;

    return value + ((dif << 6) & 0xFFFFC0);
}

operandeType op_slli[3] = {operandeType::registers, operandeType::registers, operandeType::l1_31};
;
uint32_t bin_slli(uint32_t *values)
{
    return ((((32 - values[2]) << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + (((values[0]) << 12) & 0x0F000) + 0x10000 + ((((32 - values[2]) << 16) & 0x100000));
}

operandeType op_movi[3] = {operandeType::registers, operandeType::lm2048_2047};
uint32_t bin_movi(uint32_t *values)
{
    return 2 + (((values[0] << 4) & 0xF0)) + ((values[1]) & 0xF00) + (0xa << 12) + ((values[1] << 16) & 0xFF0000);

    // return  0xc + (( (values[0] <<8) & 0xF00 )) +   ((values[1]<<12) &0xF000 ) +  ((values[1]) & 0x70 ) ;
}
operandeType op_movi_n[2] = {operandeType::registers, operandeType::lm32_95};
uint32_t bin_movi_n(uint32_t *values)
{
    return 0xc + (((values[0] << 8) & 0xF00)) + ((values[1] << 12) & 0xF000) + ((values[1]) & 0x70);
}

operandeType *op_and=op_add;  // [3] = {operandeType::registers, operandeType::registers, operandeType::registers};
uint32_t bin_and(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + ((values[1] << 8) & 0xF00) + ((values[0] << 12) & 0x0F000) + 0x100000;
}

operandeType *op_remu=op_add; //[3] = {operandeType::registers, operandeType::registers, operandeType::registers};
uint32_t bin_remu(uint32_t *values)
{
    return (((values[2] << 4) & 0xF0)) + ((values[1] << 8) & 0xF00) + ((values[0] << 12) & 0x0F000) + 0xe20000;
}

operandeType op_entry[2] = {operandeType::registers, operandeType::l0_32760};
uint32_t bin_entry(uint32_t *values)
{
    return 0x36 + (((values[0] << 8) & 0xF00)) + (((values[1] / 8) << 12) & 0xFF000);
}
operandeType op_word[2] = {operandeType::l0_FFFFFFFF};
uint32_t bin_word(uint32_t *values)
{
    return values[0];
}

operandeType op_extui[4] = {operandeType::registers, operandeType::registers, operandeType::l0_31, operandeType::l0_15};
uint32_t bin_extui(uint32_t *values)
{
    return (values[1] << 4) + (values[0] << 12) + (2 << 17) + ((values[3] - 1) << 20) + ((values[2] & 0xF) << 8) + ((values[2] & 0x10) << 12);
}
uint32_t bin_nop_n(uint32_t *values)
{
    return 0xf03d;
    ;
}
uint32_t bin_nop(uint32_t *values)
{
    return 0x0020f0;
}
operandeType op_l32r[2] = {operandeType::registers, operandeType::label};
uint32_t bin_l32r(uint32_t *values)
{
    return 1 + ((values[0] << 4) & 0xF0);
}

/******float********/


uint32_t bin_lsi(uint32_t *values)
{
    return 0x3 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x0000 + ((((values[2] / 4) << 16) & 0xFF0000));
}


uint32_t bin_ssi(uint32_t *values)
{
    return 0x3 + (((values[0] << 4) & 0xF0)) + (((values[1] << 8) & 0xF00)) + 0x4000 + ((((values[2] / 4) << 16) & 0xFF0000));
}

uint32_t bin_rfr(uint32_t *values)
{
    return 0x0 + 0x40+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + 0xa0000+0xf00000;
}

uint32_t bin_wfr(uint32_t *values)
{
    return 0x0 + 0x50+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + 0xa0000+0xf00000;
}

uint32_t bin_adds(uint32_t *values)
{
    return 0x0 + 0xA0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}
uint32_t bin_subs(uint32_t *values)
{
    return 0x0 + 0x1A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}
operandeType op_floats[3] = {operandeType::floatregisters, operandeType::registers,  operandeType::l0_15};
uint32_t bin_floats(uint32_t *values)
{
    return 0x0 + 0xCA0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}

operandeType op_floors[3] = {operandeType::registers, operandeType::floatregisters,  operandeType::l0_15};
uint32_t bin_floors(uint32_t *values)
{
    return 0x0 + 0xAA0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +  (((values[2] << 4) & 0xF0)) ;
}
uint32_t bin_rounds(uint32_t *values)
{
    return 0x0 + 0x8A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +  (((values[2] << 4) & 0xF0)) ;
}
operandeType *op_truncs=op_floors;     //[3] = {operandeType::registers, operandeType::floatregisters,  operandeType::l0_15};
uint32_t bin_truncs(uint32_t *values)
{
    return 0x0 + 0x9A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}

//operandeType op_floors[3] = {operandeType::registers, operandeType::floatregisters,  operandeType::l0_15};
uint32_t bin_muls(uint32_t *values)
{
    return 0x0 + 0x2A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}
operandeType op_div0s[2] = {operandeType::floatregisters, operandeType::floatregisters};
uint32_t bin_div0s(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0x70+0xfa0000 ;
}
operandeType *op_divns =op_adds;    //[3] = {operandeType::floatregisters, operandeType::floatregisters, operandeType::floatregisters};
uint32_t bin_divns(uint32_t *values)
{
    return 0x0 + 0x7A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}

operandeType *op_maddns=op_divns;
uint32_t bin_maddns(uint32_t *values)
{
    return 0x0 + 0x6A0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) + + (((values[2] << 4) & 0xF0)) ;
}

uint32_t bin_nexp01s(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0xB0+0xfa0000 ;
}

  //[2] = {operandeType::floatregisters, operandeType::floatregisters};
uint32_t bin_negs(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0x60+0xfa0000 ;
}

operandeType *op_addexpms=op_negs;
uint32_t bin_addexpms(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0xF0+0xfa0000 ;
}
operandeType *op_addexps=op_negs;
uint32_t bin_addexps(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0xE0+0xfa0000 ;
}
operandeType *op_mkdadjs=op_negs;
uint32_t bin_mkdadjs(uint32_t *values)
{
    return (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00)) +0xD0+0xfa0000 ;
}
operandeType op_consts[2] = {operandeType::floatregisters, operandeType::l0_15};
uint32_t bin_consts(uint32_t *values)
{
    return 0x30 + 0xFA0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00))  ;
}

operandeType *op_movs=op_nexp01s;  //[2] = {operandeType::floatregisters, operandeType::floatregisters};
uint32_t bin_movs(uint32_t *values)
{
    return 0x00 + 0xFA0000+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00))  ;
}
uint32_t bin_abss(uint32_t *values)
{
    return 0xFA0010+ (((values[0] << 12) & 0xF000)) + (((values[1] << 8) & 0xF00))  ;
}


operandeType op_floatco[3]={operandeType::boolregisters,operandeType::floatregisters,operandeType::floatregisters};
uint32_t bin_olts(uint32_t *values)
{
    return 0x00 + (((values[2] << 4) & 0x00F0)) +(((values[1] << 8) & 0x00F00))+(((values[0] << 12) & 0x00F000))+0x4B0000 ;
}
uint32_t bin_oles(uint32_t *values)
{
    return 0x00 + (((values[2] << 4) & 0x00F0)) +(((values[1] << 8) & 0x00F00))+(((values[0] << 12) & 0x00F000))+0x6B0000 ;
}
uint32_t bin_oeqs(uint32_t *values)
{
    return 0x00 + (((values[2] << 4) & 0x00F0)) +(((values[1] << 8) & 0x00F00))+(((values[0] << 12) & 0x00F000))+0x2B0000 ;
}
operandeType op_jumpfloat[2]={operandeType::boolregisters,operandeType::label};
uint32_t bin_bt(uint32_t *values)
{
    return 0x1076+(((values[0] << 8) & 0x00F00));
}
uint32_t bin_bf(uint32_t *values)
{
    return 0x0076+(((values[0] << 8) & 0x00F00));
}

operandeType op_call8[2] = {operandeType::label};
uint32_t bin_call8(uint32_t *values)
{
    return 0x25;
}
uint32_t bin_call12(uint32_t *values)
{
    return 0x35;
}
uint32_t jump_call8(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    if ((((destination_address) / 4) * 4) != destination_address)
    {
        printf("impossible de calculer le jump\n");
        return 0;
    }
    else
    {
        uint32_t dif = ((destination_address - (current_address & 0xFFFFFFFC)) - 4) & 0xFFFFFFC;
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
        return value + ((dif << 4) & 0xFFFFC0);
    }
}

uint32_t jump_l32r(uint32_t value, uint32_t current_address, uint32_t destination_address)
{
    if ((((destination_address) / 4) * 4) != destination_address)
    {
        printf("impossible de calculer le jump\n");
        return 0;
    }
    else
    {
        uint32_t dif = ((destination_address - current_address) / 4) & 0xFFFF;
        uint32_t recalculate = (((current_address & 0xFFFF) + dif * 4) + 3) & 0xFFFc;
        if (recalculate != (destination_address & 0xFFFF))
        {

            recalculate = ((current_address & 0xFFFF) + 3 + 4) & 0xFFFFFc;
            if (recalculate == (destination_address & 0xFFFF))
            {
                dif++;
            }
            else
            {
                printf("not it\n");
            }
        }
        return value + ((dif << 8) & 0xFFFF00);
    }
}

operandeType op_callx8[1] = {operandeType::registers};
uint32_t bin_callx8(uint32_t *values)
{
    return 0xe0 + ((values[0] << 8) & 0xF00);
}

uint32_t bin_retw_n(uint32_t *values)
{
    return 0xf01d;
}

operandeType op_global[1] = {operandeType::label};
operandeType op_movExt[2] = {operandeType::registers, operandeType::label};
uint32_t bin_movExt(uint32_t *values)
{

    return values[0];
}






#endif