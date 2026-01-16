#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<cstdint>
#include<fcntl.h>
/*
FILE *fp=fopen("dump (19).bin","rb");
FILE *wp=fopen("output.bin","wb");
*/

#define fp stdin
#define wp stdout

using namespace std;

typedef unsigned __int128 u128;
typedef uint64_t u64;

u64 p,q;
u64 p_inv,q_inv;
u64 R2_mod_p,R2_mod_q;

u64 calc_inv(u64 n){
    u64 x=1;
    for(int i=0;i<6;++i) x=x*((u64)2-n*x);
    return -x;
}

inline u64 mont_mul(u64 a, u64 b) {
    u128 T=(u128)a*b;
    u64 m=(u64)T*p_inv;
    u128 mp=(u128)m*p;
    u128 R=T+mp;
    u64 t=(u64)(R>>64);
    if(R<T) return t-p;
    if(t>=p) return t-p;
    return t;
}
inline u64 mont_mul_q(u64 a, u64 b) {
    u128 T=(u128)a*b;
    u64 m=(u64)T*q_inv;
    u128 mq=(u128)m*q;
    u128 R=T+mq;
    u64 t=(u64)(R>>64);
    if(R<T) return t-q;
    if(t>=q) return t-q;
    return t;
}

void init_mont(){
    u64 R_mod_p=1;
    for(int i=0;i<64;++i){
        bool overflow=(R_mod_p>>63)&1;
        R_mod_p<<=1;
        if(overflow){
            R_mod_p+=(0-p);
        }
		if(R_mod_p>=p) R_mod_p-=p;
    }
    R2_mod_p=R_mod_p;
    for(int i=0;i<64;++i){
        bool overflow=(R2_mod_p>>63)&1;
        R2_mod_p<<=1;
        if(overflow){
            R2_mod_p+=(0-p);
        }
		if(R2_mod_p>=p) R2_mod_p-=p;
	}
}
void init_mont_q(){
    u64 R_mod_q=1;
    for(int i=0;i<64;++i){
        bool overflow=(R_mod_q>>63)&1;
        R_mod_q<<=1;
        if(overflow){
            R_mod_q+=(0-q);
        }
		if(R_mod_q>=q) R_mod_q-=q;
    }
    R2_mod_q=R_mod_q;
    for(int i=0;i<64;++i){
        bool overflow=(R2_mod_q>>63)&1;
        R2_mod_q<<=1;
        if(overflow){
            R2_mod_q+=(0-q);
        }
		if(R2_mod_q>=q) R2_mod_q-=q;
	}
}

u64 from_bytes_le(const uint8_t* buf){
    u64 res=0;
    for(int i=0;i<8;++i){
        res|=((u64)buf[i]<<(i<<3));
    }
    return res;
}

u64 mont_pow(u64 a, u64 b) {
    u64 base = mont_mul(a, R2_mod_p);
    u64 res = mont_mul(1, R2_mod_p);
    while(b) {
        if(b & 1) res = mont_mul(res, base);
        base = mont_mul(base, base);
        b >>= 1;
    }
    return mont_mul(res, 1);
}
u64 mont_pow_q(u64 a, u64 b) {
    u64 base = mont_mul_q(a, R2_mod_q);
    u64 res = mont_mul_q(1, R2_mod_q);
    while(b) {
        if(b & 1) res = mont_mul_q(res, base);
        base = mont_mul_q(base, base);
        b >>= 1;
    }
    return mont_mul_q(res, 1);
}
int main(){
	#ifdef _WIN32
    _setmode(_fileno(stdin),_O_BINARY);
    _setmode(_fileno(stdout),_O_BINARY);
	#endif
    uint8_t r_buf[8];
    uint8_t g_buf[8];
	uint8_t h_buf[8];
    uint8_t x0_buf[8];
    uint32_t data_len;
    fread(r_buf,1,8,fp);
    fread(g_buf,1,8,fp);
    fread(h_buf,1,8,fp);
    fread(x0_buf,1,8,fp);
    fread(&data_len,4,1,fp);
    u64 r=from_bytes_le(r_buf);
    u64 g=from_bytes_le(g_buf);
    u64 h=from_bytes_le(h_buf);
    u64 x0=from_bytes_le(x0_buf);
    u64 x=x0;
    q=2*r+1;
    p=2*q+1;
    p_inv=calc_inv(p);
    q_inv=calc_inv(q);
    init_mont();
    init_mont_q();
    uint32_t tot_cnt1=0,cnt_l[9]={0},rr=0;
    uint8_t cnt1[data_len/8+2]={0};
    int last=-1,tot_l=0;
    for(uint32_t i=0;i<data_len;++i){
        uint8_t byte_out=0;
        for(int bit=0;bit<8;++bit){
			x=mont_pow_q(h,x);
			x=mont_pow(g,x);
			x%=r;
            if(x&1){
            	tot_cnt1++;
            	byte_out|=(1<<bit);
            	cnt1[i/8]++;
            	if(last==1){
            		rr++;
				}
            	else{
            		if(rr<=8) cnt_l[rr]++;
            		rr=1;
            		tot_l++;
				}
				last=1;
			}
			else{
				if(last==0){
					rr++;
				}
				else{
					if(rr<=8) cnt_l[rr]++;
					rr=1;
					tot_l++;
				}
				last=0;
			}
        }
        fwrite(&byte_out,1,1,wp);
    }
    if(x&1){
    	if(last==1) cnt_l[rr]++;
    	else cnt_l[1]++;
	}
	else{
		if(last==0) cnt_l[rr]++;
		else cnt_l[1]++;
	}
    fwrite(&tot_cnt1,4,1,wp);
    for(uint32_t i=0;i<data_len/8;++i) fwrite(&cnt1[i],1,1,wp);
    fwrite(&tot_l,4,1,wp);
    for(int i=1;i<=8;++i) fwrite(&cnt_l[i],4,1,wp);
    return 0;
}

