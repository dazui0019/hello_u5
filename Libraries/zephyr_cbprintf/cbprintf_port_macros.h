/* GNU preprocessor helpers used by cbprintf; maximum 16 entries including fmt. */
#ifndef CBPRINTF_PORT_MACROS_H
#define CBPRINTF_PORT_MACROS_H
#define CBP_CAT_(a,b) a##b
#define CBP_CAT(a,b) CBP_CAT_(a,b)
#define CBP_UNWRAP(...) __VA_ARGS__
#define CBP_ENABLED_1 CBP_UNUSED,
#define CBP_ZERO_0 CBP_UNUSED,
#define CBP_ENABLED_GET(_ignored,value,...) value
#define CBP_ENABLED_EXPAND(args) CBP_ENABLED_GET(args 1, 0)
#define IS_ENABLED(value) CBP_ENABLED_EXPAND(CBP_CAT(CBP_ENABLED_,value))
#define CBP_CHOOSE_1(yes,no) CBP_UNWRAP yes
#define CBP_CHOOSE_0(yes,no) CBP_UNWRAP no
#define COND_CODE_1(flag,yes,no) CBP_CAT(CBP_CHOOSE_,IS_ENABLED(flag))(yes,no)
#define CBP_IS_ZERO(value) CBP_ENABLED_EXPAND(CBP_CAT(CBP_ZERO_,value))
#define COND_CODE_0(flag,yes,no) CBP_CAT(CBP_CHOOSE_,CBP_IS_ZERO(flag))(yes,no)
#define IF_ENABLED(flag,code) COND_CODE_1(flag,code,())
#define CBP_COUNT_(_ignored,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,n,...) n
#define CBP_COUNT(...) CBP_COUNT_(_, ##__VA_ARGS__,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define NUM_VA_ARGS_LESS_1(...) CBP_CAT(CBP_DEC_,CBP_COUNT(__VA_ARGS__))
#define GET_ARG_N(n,...) CBP_CAT(CBP_GET_,n)(__VA_ARGS__)
#define CBP_GET_1(first,...) first
#define GET_ARGS_LESS_N(n,...) CBP_CAT(CBP_DROP_,n)(__VA_ARGS__)
#define CBP_DROP_1(first,...) __VA_ARGS__
#define CBP_DEC_0 0
#define CBP_DEC_1 0
#define CBP_DEC_2 1
#define CBP_DEC_3 2
#define CBP_DEC_4 3
#define CBP_DEC_5 4
#define CBP_DEC_6 5
#define CBP_DEC_7 6
#define CBP_DEC_8 7
#define CBP_DEC_9 8
#define CBP_DEC_10 9
#define CBP_DEC_11 10
#define CBP_DEC_12 11
#define CBP_DEC_13 12
#define CBP_DEC_14 13
#define CBP_DEC_15 14
#define CBP_DEC_16 15
#define CBP_EACH_0(f,sep,...) 
#define CBP_EACH_1(f,sep,a0) f(a0)
#define CBP_EACH_2(f,sep,a0,a1) f(a0) CBP_UNWRAP sep f(a1)
#define CBP_EACH_3(f,sep,a0,a1,a2) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2)
#define CBP_EACH_4(f,sep,a0,a1,a2,a3) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3)
#define CBP_EACH_5(f,sep,a0,a1,a2,a3,a4) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4)
#define CBP_EACH_6(f,sep,a0,a1,a2,a3,a4,a5) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5)
#define CBP_EACH_7(f,sep,a0,a1,a2,a3,a4,a5,a6) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6)
#define CBP_EACH_8(f,sep,a0,a1,a2,a3,a4,a5,a6,a7) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7)
#define CBP_EACH_9(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8)
#define CBP_EACH_10(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9)
#define CBP_EACH_11(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10)
#define CBP_EACH_12(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10) CBP_UNWRAP sep f(a11)
#define CBP_EACH_13(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10) CBP_UNWRAP sep f(a11) CBP_UNWRAP sep f(a12)
#define CBP_EACH_14(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10) CBP_UNWRAP sep f(a11) CBP_UNWRAP sep f(a12) CBP_UNWRAP sep f(a13)
#define CBP_EACH_15(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10) CBP_UNWRAP sep f(a11) CBP_UNWRAP sep f(a12) CBP_UNWRAP sep f(a13) CBP_UNWRAP sep f(a14)
#define CBP_EACH_16(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) f(a0) CBP_UNWRAP sep f(a1) CBP_UNWRAP sep f(a2) CBP_UNWRAP sep f(a3) CBP_UNWRAP sep f(a4) CBP_UNWRAP sep f(a5) CBP_UNWRAP sep f(a6) CBP_UNWRAP sep f(a7) CBP_UNWRAP sep f(a8) CBP_UNWRAP sep f(a9) CBP_UNWRAP sep f(a10) CBP_UNWRAP sep f(a11) CBP_UNWRAP sep f(a12) CBP_UNWRAP sep f(a13) CBP_UNWRAP sep f(a14) CBP_UNWRAP sep f(a15)
#define CBP_IDX_0(f,sep,...) 
#define CBP_IDX_1(f,sep,a0) f(0,a0)
#define CBP_IDX_2(f,sep,a0,a1) f(0,a0) CBP_UNWRAP sep f(1,a1)
#define CBP_IDX_3(f,sep,a0,a1,a2) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2)
#define CBP_IDX_4(f,sep,a0,a1,a2,a3) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3)
#define CBP_IDX_5(f,sep,a0,a1,a2,a3,a4) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4)
#define CBP_IDX_6(f,sep,a0,a1,a2,a3,a4,a5) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5)
#define CBP_IDX_7(f,sep,a0,a1,a2,a3,a4,a5,a6) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6)
#define CBP_IDX_8(f,sep,a0,a1,a2,a3,a4,a5,a6,a7) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7)
#define CBP_IDX_9(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8)
#define CBP_IDX_10(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9)
#define CBP_IDX_11(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10)
#define CBP_IDX_12(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10) CBP_UNWRAP sep f(11,a11)
#define CBP_IDX_13(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10) CBP_UNWRAP sep f(11,a11) CBP_UNWRAP sep f(12,a12)
#define CBP_IDX_14(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10) CBP_UNWRAP sep f(11,a11) CBP_UNWRAP sep f(12,a12) CBP_UNWRAP sep f(13,a13)
#define CBP_IDX_15(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10) CBP_UNWRAP sep f(11,a11) CBP_UNWRAP sep f(12,a12) CBP_UNWRAP sep f(13,a13) CBP_UNWRAP sep f(14,a14)
#define CBP_IDX_16(f,sep,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) f(0,a0) CBP_UNWRAP sep f(1,a1) CBP_UNWRAP sep f(2,a2) CBP_UNWRAP sep f(3,a3) CBP_UNWRAP sep f(4,a4) CBP_UNWRAP sep f(5,a5) CBP_UNWRAP sep f(6,a6) CBP_UNWRAP sep f(7,a7) CBP_UNWRAP sep f(8,a8) CBP_UNWRAP sep f(9,a9) CBP_UNWRAP sep f(10,a10) CBP_UNWRAP sep f(11,a11) CBP_UNWRAP sep f(12,a12) CBP_UNWRAP sep f(13,a13) CBP_UNWRAP sep f(14,a14) CBP_UNWRAP sep f(15,a15)
#define CBP_FIXED_0(f,sep,fixed,...) 
#define CBP_FIXED_1(f,sep,fixed,a0) f(0,a0,fixed)
#define CBP_FIXED_2(f,sep,fixed,a0,a1) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed)
#define CBP_FIXED_3(f,sep,fixed,a0,a1,a2) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed)
#define CBP_FIXED_4(f,sep,fixed,a0,a1,a2,a3) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed)
#define CBP_FIXED_5(f,sep,fixed,a0,a1,a2,a3,a4) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed)
#define CBP_FIXED_6(f,sep,fixed,a0,a1,a2,a3,a4,a5) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed)
#define CBP_FIXED_7(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed)
#define CBP_FIXED_8(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed)
#define CBP_FIXED_9(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed)
#define CBP_FIXED_10(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed)
#define CBP_FIXED_11(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed)
#define CBP_FIXED_12(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed) CBP_UNWRAP sep f(11,a11,fixed)
#define CBP_FIXED_13(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed) CBP_UNWRAP sep f(11,a11,fixed) CBP_UNWRAP sep f(12,a12,fixed)
#define CBP_FIXED_14(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed) CBP_UNWRAP sep f(11,a11,fixed) CBP_UNWRAP sep f(12,a12,fixed) CBP_UNWRAP sep f(13,a13,fixed)
#define CBP_FIXED_15(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed) CBP_UNWRAP sep f(11,a11,fixed) CBP_UNWRAP sep f(12,a12,fixed) CBP_UNWRAP sep f(13,a13,fixed) CBP_UNWRAP sep f(14,a14,fixed)
#define CBP_FIXED_16(f,sep,fixed,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) f(0,a0,fixed) CBP_UNWRAP sep f(1,a1,fixed) CBP_UNWRAP sep f(2,a2,fixed) CBP_UNWRAP sep f(3,a3,fixed) CBP_UNWRAP sep f(4,a4,fixed) CBP_UNWRAP sep f(5,a5,fixed) CBP_UNWRAP sep f(6,a6,fixed) CBP_UNWRAP sep f(7,a7,fixed) CBP_UNWRAP sep f(8,a8,fixed) CBP_UNWRAP sep f(9,a9,fixed) CBP_UNWRAP sep f(10,a10,fixed) CBP_UNWRAP sep f(11,a11,fixed) CBP_UNWRAP sep f(12,a12,fixed) CBP_UNWRAP sep f(13,a13,fixed) CBP_UNWRAP sep f(14,a14,fixed) CBP_UNWRAP sep f(15,a15,fixed)
#define FOR_EACH(f,sep,...) CBP_CAT(CBP_EACH_,CBP_COUNT(__VA_ARGS__))(f,sep, ##__VA_ARGS__)
#define FOR_EACH_IDX(f,sep,...) CBP_CAT(CBP_IDX_,CBP_COUNT(__VA_ARGS__))(f,sep, ##__VA_ARGS__)
#define FOR_EACH_IDX_FIXED_ARG(f,sep,fixed,...) CBP_CAT(CBP_FIXED_,CBP_COUNT(__VA_ARGS__))(f,sep,fixed, ##__VA_ARGS__)
#endif
