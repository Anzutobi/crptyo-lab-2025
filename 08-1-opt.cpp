#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<cstdint>
#include<fcntl.h>
FILE *fp=fopen("0.bin","rb");
FILE *wp=fopen("output.bin","wb");

/*
#define fp stdin
#define wp stdout
*/
using namespace std;

typedef unsigned __int128 u128;
typedef uint64_t u64;

u128 p;
u128 p_inv;
u128 R2_mod_p;

u128 calc_inv(u128 n){
    u128 x=1;
    for(int i=0;i<7;++i) x=x*((u128)2-n*x);
    return -x;
}

inline u128 mont_mul(u128 a, u128 b) {
    u64 a_lo=(u64)a;
    u64 a_hi=(u64)(a>>64);
    u64 b_lo=(u64)b;
    u64 b_hi=(u64)(b>>64);
    u128 p0=(u128)a_lo*b_lo;
    u128 p1=(u128)a_lo*b_hi;
    u128 p2=(u128)a_hi*b_lo;
    u128 p3=(u128)a_hi*b_hi;
    u128 T_lo=p0;
    u128 carry_T=0;
    u128 t1=p1<<64;
    T_lo+=t1;
    if(T_lo<t1) carry_T++;
    u128 t2=p2<<64;
    T_lo+=t2;
    if(T_lo<t2) carry_T++;
    u128 T_hi=p3+(p1>>64)+(p2>>64)+carry_T;
    u128 m=T_lo*p_inv;
    u64 m_lo=(u64)m;
    u64 m_hi=(u64)(m>>64);
    u64 p_lo=(u64)p;
    u64 p_hi=(u64)(p>>64);
    u128 mp0=(u128)m_lo*p_lo;
    u128 mp1=(u128)m_lo*p_hi;
    u128 mp2=(u128)m_hi*p_lo;
    u128 mp3=(u128)m_hi*p_hi;
    u128 mp_lo=mp0;
    u128 carry_mp=0;
    u128 mt1=mp1<<64;
    mp_lo+=mt1;
    if(mp_lo<mt1) carry_mp++;
    u128 mt2=mp2<<64;
    mp_lo+=mt2;
    if(mp_lo<mt2) carry_mp++;
    u128 mp_hi=mp3+(mp1>>64)+(mp2>>64)+carry_mp;
    u128 r_lo=T_lo+mp_lo;
    u128 carry_final=0;
    if(r_lo<T_lo) carry_final=1;
    u128 res=T_hi+mp_hi+carry_final;
    bool overflow=false;
    if(res<T_hi) overflow=true;
    if(!overflow && res<mp_hi) overflow=true;
    u128 sum_tmp=T_hi+mp_hi;
    if(sum_tmp<T_hi) overflow=true;
    res=sum_tmp+carry_final;
    if(res<sum_tmp) overflow=true;
    if(overflow){
        return res-p;
    }
	else{
        if(res>=p) return res-p;
        return res;
    }
}

void init_mont(){
    u128 R_mod_p=1;
    for(int i=0;i<128;++i){
        bool overflow=(R_mod_p>>127)&1;
        R_mod_p<<=1;
        if(overflow){
            R_mod_p+=(0-p);
        }
		if(R_mod_p>=p) R_mod_p-=p;
    }
    R2_mod_p=R_mod_p;
    for(int i=0;i<128;++i){
        bool overflow=(R2_mod_p>>127)&1;
        R2_mod_p<<=1;
        if(overflow){
            R2_mod_p+=(0-p);
        }
		if(R2_mod_p>=p) R2_mod_p-=p;
	}
}

u128 from_bytes_le(const uint8_t* buf){
    u128 res=0;
    for(int i=0;i<16;++i){
        res|=((u128)buf[i]<<(i<<3));
    }
    return res;
}

const int W_WINDOW=5;
struct MontPrecomp{
    u128 table[1<<W_WINDOW];
};

MontPrecomp build_mont_precomp(u128 a){
    MontPrecomp mp;
	u128 one_mont=mont_mul(1,R2_mod_p);
    u128 a_mont=mont_mul(a,R2_mod_p);
    mp.table[0]=one_mont;
    mp.table[1]=a_mont;
    for(int i=2;i<(1<<W_WINDOW);++i){
        mp.table[i]=mont_mul(mp.table[i-1], a_mont);
    }
    return mp;
}

u128 mont_pow_with_table(const MontPrecomp& mp, u128 b) {
    u128 ans=mp.table[0];
    int bit_len;
    u64 hi=(u64)(b>>64);
    if(hi){
        bit_len=128-__builtin_clzll(hi);
    }
	else{
        u64 lo=(u64)b;
        if(lo) bit_len=64-__builtin_clzll(lo);
        else bit_len=0;
    }
    int i=bit_len-1;
    while(i>=0){
        if(((b>>i)&1)==0){
            ans=mont_mul(ans,ans);
            i--;
        }
		else{
            int s=max(0,i-W_WINDOW+1);
            while(s<i && (((b>>s)&1)==0)) s++;
            int len=i-s+1;
            for(int j=0;j<len;++j) ans=mont_mul(ans,ans);
            u128 val=(b>>s) & (((u128)1<<len)-1);
            ans=mont_mul(ans,mp.table[(int)val]);
            i=s-1;
        }
    }
    return mont_mul(ans,1);
}
int main(){
	#ifdef _WIN32
    _setmode(_fileno(stdin),_O_BINARY);
    _setmode(_fileno(stdout),_O_BINARY);
	#endif
    uint8_t p_buf[16];
    uint8_t g_buf[16];
    uint8_t s0_buf[16];
    uint32_t data_len;
    fread(p_buf,1,16,fp);
    fread(g_buf,1,16,fp);
    fread(s0_buf,1,16,fp);
    fread(&data_len,4,1,fp);
    p=from_bytes_le(p_buf);
    u128 g=from_bytes_le(g_buf);
    u128 s=from_bytes_le(s0_buf);
    p_inv=calc_inv(p);
    init_mont();
    u128 p_half=p>>1;
    uint32_t cnt0=0;
    uint32_t cnt1=0;
    MontPrecomp mp_g=build_mont_precomp(g);
    for(uint32_t i=0;i<data_len;++i){
        uint8_t byte_out=0;
        for(int bit=0;bit<8;++bit){
			s=mont_pow_with_table(mp_g,s);
            int z=0;
            if(s<=p_half){
                z=0;
                cnt0++;
            }
			else{
                z=1;
                cnt1++;
            }
            if(z) byte_out|=(1<<bit);
        }
        fwrite(&byte_out,1,1,wp);
    }
    fwrite(&cnt0,4,1,wp);
    fwrite(&cnt1,4,1,wp);
    return 0;
}
