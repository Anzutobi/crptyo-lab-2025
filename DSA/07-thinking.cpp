#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<cstdint>
#include<fcntl.h>
#include<wmmintrin.h>
#include<immintrin.h>
#include<emmintrin.h> 
using namespace std;

FILE *fp=fopen("dump (14).bin","rb");
FILE *fp2=fopen("dump (15).bin","rb");
FILE *fp3=fopen("dump (16).bin","rb");
FILE *fp4=fopen("dump (17).bin","rb");
FILE *wp=fopen("output.bin","wb");
/*
#define fp stdin
#define wp stdout
*/
const uint32_t h[9]={0,0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                       0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
uint32_t _hash[10]={0};
__m128i state0,state1,msg,msgtmp0,msgtmp1,msgtmp2,msgtmp3,msgtmp4,abef,cdgh;
__m128i shuf_mask=_mm_set_epi32(
    0x0c0d0e0f,
    0x08090a0b,
    0x04050607,
    0x00010203
);
void sha_256(uint8_t tmp[],uint32_t _hash[],int start){
	if(!start){
		state0=_mm_set_epi64x(0x6a09e667bb67ae85,0x510e527f9b05688c);
		state1=_mm_set_epi64x(0x3c6ef372a54ff53a,0x1f83d9ab5be0cd19);
	}
	abef=state0;
	cdgh=state1;
	msg=_mm_loadu_si128((__m128i*)(tmp+0));
	msgtmp0=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0xe9b5dba5b5c0fbcf,0x71374491428a2f98));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msg=_mm_loadu_si128((__m128i*)(tmp+16));
	msgtmp1=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0xab1c5ed5923f82a4,0x59f111f13956c25b));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
	msg=_mm_loadu_si128((__m128i*)(tmp+32));
	msgtmp2=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0x550c7dc3243185be,0x12835b01d807aa98));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
	msg=_mm_loadu_si128((__m128i*)(tmp+48));
	msgtmp3=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0xc19bf1749bdc06a7,0x80deb1fe72be5d74));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
    msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x240ca1cc0fc19dc6,0xefbe4786e49b69c1));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x76f988da5cb0a9dc,0x4a7484aa2de92c6f));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
    msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
    msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0xbf597fc7b00327c8,0xa831c66d983e5152));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
    msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
    msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0x1429296706ca6351,0xd5a79147c6e00bf3));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
	msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x53380d134d2c6dfc,0x2e1b213827b70a85));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x92722c8581c2c92e,0x766a0abb650a7354));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
    msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
    msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0xc76c51a3c24b8b70,0xa81a664ba2bfe8a1));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
    msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
    msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0x106aa070f40e3585,0xd6990624d192e819));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
    msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x34b0bcb52748774c,0x1e376c0819a4c116));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x682e6ff35b9cca4f,0x4ed8aa4a391c0cb3));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
	msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
	msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0x8cc7020884c87814,0x78a5636f748f82ee));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
	msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
	msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0xc67178f2bef9a3f7,0xa4506ceb90befffa));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	state0=_mm_add_epi32(state0,abef);
    state1=_mm_add_epi32(state1,cdgh);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    _hash[0]=(uint32_t)(state0[1]>>32);
    _hash[1]=(uint32_t)(state0[1]);
    _hash[4]=(uint32_t)(state0[0]>>32);
    _hash[5]=(uint32_t)(state0[0]);
    _hash[2]=(uint32_t)(state1[1]>>32);
    _hash[3]=(uint32_t)(state1[1]);
    _hash[6]=(uint32_t)(state1[0]>>32);
    _hash[7]=(uint32_t)(state1[0]);
    //for(int i=0;i<=7;++i) cout<<hex<<(int)_hash[i]<<" ";
    //cout<<endl;
}
void H(uint8_t data[],uint16_t len){
    uint8_t tmp[64];
    int start=0;
    uint16_t rem=len;
    int offset=0;
    while(rem>=64){
        memcpy(tmp,data+offset,64);
        sha_256(tmp,_hash,start);
        start=1;
        rem-=64;
        offset+=64;
    }
    memcpy(tmp,data+offset,rem);
    tmp[rem]=0x80;
    if(rem<=55) memset(tmp+rem+1,0,55-rem);
    else{
        memset(tmp+rem+1,0,63-rem);
        sha_256(tmp,_hash,start);
        start=1;
        memset(tmp,0,56);
    }
    uint64_t tot_bits=(uint64_t)len*8;
    for(int i=0;i<8;++i) tmp[63-i]=(tot_bits>>(i*8))&0xFF;
    sha_256(tmp,_hash,start);
}
const int MAXN=130;
const uint64_t BASE10_19=10000000000000000000ull;
inline uint8_t addcarry_u64(uint8_t c_in,uint64_t a,uint64_t b,uint64_t *out){
    unsigned __int128 res=(unsigned __int128)a+b+c_in;
    *out=(uint64_t)res;
    return (uint8_t)(res>>64);
}
inline uint8_t subborrow_u64(uint8_t b_in,uint64_t a,uint64_t b,uint64_t *out) {
    unsigned __int128 res=(unsigned __int128)a-b-b_in;
    *out=(uint64_t)res;
    return (uint8_t)(res>>64)&1;
}

inline uint64_t mulx_u64(uint64_t a,uint64_t b,uint64_t *hi){
    unsigned __int128 res=(unsigned __int128)a*b;
    *hi=(uint64_t)(res>>64);
    return (uint64_t)res;
}

struct bign{
    uint64_t d[MAXN];
    int len;
    bool sign;
    bign(){
        memset(d,0,sizeof(d));
        len=1;
        sign=false;
    }
    bign(int v){
        memset(d,0,sizeof(d));
        if(v<0){
            sign=true;
            v=-v;
        }
		else sign=false;
        d[0]=(uint64_t)v;
        len=1;
    }
    bign(long long v){
        memset(d,0,sizeof(d));
        if(v<0){
            sign=true;
            v=-v;
        }
		else sign=false;
        d[0]=(uint64_t)v;
        len=1;
    }
    bign(unsigned long long v){
        memset(d,0,sizeof(d));
        d[0]=v;
        len=1;
        sign=false;
    }
    void clean(){
        while(len>1 && d[len-1]==0) len--;
        if(len==1 && d[0]==0) sign=false;
    }
    int abs_cmp(const bign& b) const {
        if(len!=b.len) return len<b.len?-1:1;
        for(int i=len-1;i>= 0;--i){
            if(d[i]!=b.d[i]) return d[i]<b.d[i]?-1:1;
        }
        return 0;
	}
    bool operator<(const bign& b) const {
        if(sign!=b.sign) return sign;
        if(sign) return abs_cmp(b)>0;
        return abs_cmp(b)<0;
    }
    bool operator>(const bign& b) const { return b < *this; }
    bool operator<=(const bign& b) const { return !(*this > b); }
    bool operator>=(const bign& b) const { return !(*this < b); }
    bool operator==(const bign& b) const { return sign==b.sign && abs_cmp(b)==0; }
    bool operator!=(const bign& b) const { return !(*this == b); }
    
    static bign abs_add(const bign& a,const bign& b){
        bign res;
        res.len=max(a.len,b.len);
        uint8_t carry=0;
        for(int i=0; i<res.len;++i){
            uint64_t ai=(i<a.len)?a.d[i]:0ull;
            uint64_t bi=(i<b.len)?b.d[i]:0ull;
            carry=addcarry_u64(carry,ai,bi,&res.d[i]);
        }
        if(carry){
            if(res.len<MAXN) res.d[res.len++]=carry;
        }
        return res;
    }

    static bign abs_sub(const bign& a,const bign& b){
        bign res;
        res.len=a.len;
        uint8_t borrow=0;
        for (int i=0;i<res.len;++i){
            uint64_t ai=(i<a.len)?a.d[i]:0ull;
            uint64_t bi=(i<b.len)?b.d[i]:0ull;
            borrow=subborrow_u64(borrow,ai,bi,&res.d[i]);
        }
        res.clean();
        return res;
    }

    void shift_words(int k) {
        if(len==1 && d[0]==0) return;
        if(k<=0) return;
        for(int i=len-1;i>=0;--i) if(i+k<MAXN) d[i+k]=d[i];
        for(int i=0;i<k && i<MAXN;++i) d[i]=0;
        len+=k;
        if(len>MAXN) len=MAXN;
        while(len>1 && d[len-1]==0) len--;
    }
    void div_2(){
        uint64_t rem=0;
        for(int i=len-1;i>=0;--i){
            uint64_t cur=d[i];
            d[i]=(cur>>1) | (rem<<63);
            rem=cur&1;
        }
        clean();
    }
    void div_3(){
        uint64_t rem=0;
        for(int i=len-1;i>=0;--i){
            unsigned __int128 cur=((unsigned __int128)rem<<64)|d[i];
            d[i]=(uint64_t)(cur/3);
            rem=(uint64_t)(cur%3);
        }
        clean();
    }

    static bign karatsuba_mul(const bign& a, const bign& b){
        bign a_abs=a;
		a_abs.sign=false;
        bign b_abs=b;
		b_abs.sign=false;
        int n=max(a_abs.len,b_abs.len);
        int m=(n+1)/2;
        bign a0,a1,b0,b1;
        a0.len=min(a_abs.len, m);
        for(int i=0;i<a0.len;++i) a0.d[i]=a_abs.d[i];
        a0.clean();
        if(a_abs.len>m){
            a1.len=a_abs.len-m;
            for(int i=0;i<a1.len;++i) a1.d[i]=a_abs.d[i+m];
            a1.clean();
        }
        b0.len=min(b_abs.len,m);
        for(int i=0;i<b0.len;++i) b0.d[i]=b_abs.d[i];
        b0.clean();
        if(b_abs.len>m){
            b1.len=b_abs.len-m;
            for(int i=0;i<b1.len;++i) b1.d[i]=b_abs.d[i+m];
            b1.clean();
        }
        bign z0=mul(a0,b0);
        bign z2=mul(a1,b1);
        bign z1=mul(a0+a1,b0+b1)-z0-z2;
        z2.shift_words(2*m);
        z1.shift_words(m);
        bign res=z2+z1+z0;
        res.sign=a.sign^b.sign;
        return res;
    }

    static bign mul(const bign& a,const bign& b){
        if((a.len==1 && a.d[0]==0) || (b.len==1 && b.d[0]==0)) return bign(0);
        //int max_len=max(a.len, b.len);
        //int min_len=min(a.len, b.len);
        //if(max_len*64>240 && min_len>=4) return toom3_mul(a,b);
        //if(max_len*64>80 && min_len>=2) return karatsuba_mul(a,b);
        bign res;
        res.len=a.len+b.len;
        if(res.len>MAXN) res.len=MAXN;
        memset(res.d,0,sizeof(uint64_t)*res.len);
        for(int i=0;i<a.len;++i){
            uint64_t r_c=0,hi,lo,v;
            for(int j=0;j<b.len;++j){
                if(i+j>=MAXN) break;
                lo=mulx_u64(a.d[i],b.d[j],&hi);
				v=res.d[i+j];
                uint8_t k1=addcarry_u64(0,v,lo,&v);
                uint8_t k2=addcarry_u64(0,v,r_c,&res.d[i+j]);
                r_c=hi+k1+k2;
            }
            if(i+b.len<MAXN) res.d[i+b.len]=r_c;
        }
        res.clean();
        res.sign=a.sign^b.sign;
        return res;
    }

    static pair<bign, bign> div_mod(const bign& u, const bign& v){
        if(v.len==1 && v.d[0]==0) return {bign(0),bign(0)};
        if(u.abs_cmp(v)<0) return {bign(0),u};
        if(v.len==1){
            bign q;
            q.len=u.len;
            uint64_t rem=0;
            for(int i=u.len-1;i>=0;--i){
                unsigned __int128 cur=((unsigned __int128)rem<<64)|u.d[i];
                q.d[i]=(uint64_t)(cur/v.d[0]);
                rem=(uint64_t)(cur%v.d[0]);
            }
            q.clean();
            q.sign=u.sign^v.sign;
            bign r((unsigned long long)rem);
            r.sign=u.sign;
            return {q,r};
        }
        int s=__builtin_clzll(v.d[v.len-1]);
        bign vn=v,un=u;
        if(s>0){
            for(int i=vn.len-1;i>0;--i) vn.d[i]=(vn.d[i]<<s)|(vn.d[i-1]>>(64-s));
            vn.d[0]<<=s;
            if(un.len<MAXN) un.d[un.len]=0;
            for(int i=un.len;i>0;--i) if(i<MAXN) un.d[i]=(un.d[i]<<s)|(un.d[i-1]>>(64-s));
            un.d[0]<<=s;
            if(un.len<MAXN && un.d[un.len]) un.len++;
        }
        if(un.len==u.len && un.len<MAXN) un.d[un.len++]=0;
        bign q;
        q.len=un.len-vn.len;
        for(int j=un.len-vn.len-1;j>=0;--j){
            unsigned __int128 num=((unsigned __int128)un.d[j+vn.len]<<64)|un.d[j+vn.len-1];
            unsigned __int128 q_=num/vn.d[vn.len-1];
            unsigned __int128 r_=num%vn.d[vn.len-1];
            while(1){
                if(q_>=((unsigned __int128)1<<64)||(q_*vn.d[vn.len-2]>((r_<<64)|un.d[j+vn.len-2]))){
                    q_--;
                    r_+=vn.d[vn.len-1];
                    if(r_>=((unsigned __int128)1<<64)) break;
                }
				else break;
            }
            unsigned __int128 k=0;
            for(int i=0;i<vn.len;++i){
                unsigned __int128 p=(unsigned __int128)q_*vn.d[i]+k;
                uint64_t sub=(uint64_t)p;
                k=p>>64;
                if(un.d[i+j]<sub) k++;
                un.d[i+j]-=sub;
            }
            if(un.d[j+vn.len]<k){
                un.d[j+vn.len]-=k;
                q_--;
                uint8_t c=0;
                for(int i=0;i<vn.len;++i) c=addcarry_u64(c,un.d[i+j],vn.d[i],&un.d[i+j]);
                un.d[j+vn.len]+=c;
            }
			else un.d[j+vn.len]-=k;
            q.d[j]=(uint64_t)q_;
        }
        q.clean();
        q.sign=u.sign^v.sign;
        if(s>0) for(int i=0;i<vn.len;++i) un.d[i]=(un.d[i]>>s)|(un.d[i+1]<<(64-s));
        un.len=vn.len;
        un.clean();
        un.sign=u.sign;
        return {q,un};
    }

    bign operator+(const bign& b) const {
        if(sign==b.sign) {
            bign res=abs_add(*this,b);
            res.sign=sign;
            return res;
        }
		else{
            if(abs_cmp(b)>=0){
                bign res=abs_sub(*this,b);
                res.sign=sign;
                return res;
            }
			else{
                bign res=abs_sub(b,*this);
                res.sign=b.sign;
                return res;
            }
        }
    }
    bign operator-(const bign& b) const{
        bign t=b;
        t.sign=!t.sign;
        return *this+t;
    }
    bign operator*(const bign& b) const{ return mul(*this, b); }
    bign operator/(const bign& b) const{ return div_mod(*this, b).first; }
    bign operator%(const bign& b) const{
        bign res=div_mod(*this,b).second;
        if(res.sign) res=res+b;
        return res;
    }
    bign operator+=(const bign& b) { *this = *this + b; return *this; }
    bign operator-=(const bign& b) { *this = *this - b; return *this; }
    bign operator*=(const bign& b) { *this = *this * b; return *this; }
    bign operator/=(const bign& b) { *this = *this / b; return *this; }
    bign operator%=(const bign& b) { *this = *this % b; return *this; }
    
    static bign from_string(const uint8_t* str){
        bign res(0);
        int i=0;
        bool neg=false;
        if(str[0]=='-'){ neg = true; i++; }
        for(;str[i]; ++i){
            bign ten(10);
            bign digit(str[i]-'0');
            res=res*ten+digit;
        }
        res.sign=neg;
        return res;
    }
    void print(){
        if(len==1 && d[0]==0){
            putchar('0');
            putchar('\n');
            return;
        }
        if(sign) putchar('-');
        bign tmp=*this;
        tmp.sign=false;
        bign base((unsigned long long)BASE10_19);
        vector<uint64_t> c;
        while(tmp.len>1 || tmp.d[0]>=BASE10_19){
            pair<bign,bign> qr=div_mod(tmp,base);
            c.push_back(qr.second.d[0]);
            tmp=qr.first;
        }
        c.push_back(tmp.d[0]);
        
        uint64_t val = c.back();
        if (val == 0) putchar('0');
        else {
            char buf[32];
            int idx = 0;
            while(val) {
                buf[idx++] = val % 10 + '0';
                val /= 10;
            }
            while(idx--) putchar(buf[idx]);
        }

        for(int i=c.size()-2;i>=0;--i) {
            val = c[i];
            char buf[32];
            for(int j=0; j<19; ++j) {
                buf[j] = val % 10 + '0';
                val /= 10;
            }
            for(int j=18; j>=0; --j) putchar(buf[j]);
        }
        putchar('\n');
    }
    static bign from_bytes(const uint8_t* buf,int len){
        bign res=0;
        for(int i=0;i<len;++i) res=res*256+buf[i];
        return res;
    }
    void to_bytes(uint8_t* buf,int len)const{
        bign temp=*this;
        for(int i=len-1;i>=0;--i){
			buf[i]=(uint8_t)((temp%256).d[0]);
			temp=temp/256;
		}
    }
};
void mont_mul(const bign& a, const bign& b, const bign& n, uint64_t n_prime, bign& res){
    uint64_t t[2*MAXN+2]={0}; 
    int k=n.len;
    for(int i=0;i<k;++i){
        uint64_t ai=(i<a.len)?a.d[i]:0;
        uint64_t carry=0;
        for(int j=0;j<k;++j){
            uint64_t bj=(j<b.len)?b.d[j]:0;
            uint64_t hi,lo;
            lo=mulx_u64(ai,bj,&hi);
            uint64_t old_t=t[i+j];
            uint8_t c1=addcarry_u64(0,old_t,lo,&t[i+j]);
            uint8_t c2=addcarry_u64(0,t[i+j],carry,&t[i+j]);
            carry=hi+c1+c2;
        }
        uint64_t old_t=t[i+k];
        uint8_t c1=addcarry_u64(0,old_t,carry,&t[i+k]);
        t[i+k+1]+=c1;
        uint64_t m=t[i]*n_prime;
        carry=0;
        for(int j=0;j<k;++j){
            uint64_t nj=n.d[j];
            uint64_t hi,lo;
            lo=mulx_u64(m,nj,&hi);
            old_t=t[i+j];
            c1=addcarry_u64(0,old_t,lo,&t[i+j]);
            uint8_t c2=addcarry_u64(0,t[i+j],carry,&t[i+j]);
            carry=hi+c1+c2;
        }
        old_t=t[i+k];
        c1=addcarry_u64(0,old_t,carry,&t[i+k]);
        t[i+k+1]+=c1;
    }
    res.len=k;
    for(int i=0;i<k;++i) res.d[i]=t[i+k];
    bool fl=(t[2*k]!=0);
    if(!fl){
        for(int i=k-1;i>=0;--i){
            if(res.d[i]>n.d[i]) { fl = true; break; }
            if(res.d[i]<n.d[i]) { break; }
            if(i==0) fl=true;
        }
    }
    if(fl){
        uint8_t borrow=0;
        for(int i=0;i<k;++i){
            uint64_t ni=n.d[i];
            borrow=subborrow_u64(borrow,res.d[i],ni,&res.d[i]);
        }
    }
    res.clean();
}

bign mont_mul(const bign& a, const bign& b, const bign& n, uint64_t n_prime){
    bign res;
    mont_mul(a,b,n,n_prime,res);
    return res;
}
bign zero(0),one(1);
bign mont_pow(bign a,bign b,bign n){
    if(n.len==1 && n.d[0]==1) return zero;
    if(b.len==1 && b.d[0]==0) return one;
    uint64_t x=1;
    uint64_t n0=n.d[0];
    for(int i=0;i<6;++i) x=x*(2-n0*x);
    uint64_t n_prime=-x;
    bign R;
    R.len=n.len+1;
    R.d[n.len]=1;
    bign R_mod_n=R%n;
    bign R2_mod_n=(R_mod_n*R_mod_n)%n;
    bign a_mont=mont_mul(a,R2_mod_n,n,n_prime);
    bign ans=R_mod_n;
    int bit_len=b.len*64-__builtin_clzll(b.d[b.len-1]);
    for(int i=bit_len-1;i>=0;--i){
        ans=mont_mul(ans,ans,n,n_prime);
        if((b.d[i/64]>>(i%64))&1) ans=mont_mul(ans,a_mont,n,n_prime);
    }
    bign result=mont_mul(ans,one,n,n_prime);
    return result;
}
void ex_gcd(bign a,bign b,bign& x,bign& y){
    bign r0=a,r1=b;
    bign s0(1),s1(0),t0(0),t1(1);
    while(r1.len>1 || r1.d[0]!=0){
        pair<bign,bign> qr=bign::div_mod(r0, r1);
        bign q=qr.first,r2=qr.second;
        bign s2=s0-q*s1;
        bign t2=t0-q*t1;
        r0=r1;
		r1=r2;
        s0=s1;
		s1=s2;
        t0=t1;
		t1=t2;
    }
    x=s0;
    y=t0;
}
uint8_t buf[70000],bbuf[64];
uint8_t seed[32];
/*
uint8_t seed[32]={0x6E,0xFB,0x67,0x55,0xC5,0xF6,0xC5,0x95,0xC6,0x18,0xF8,0xC9,0x3A,0x86,0xB4,0x71,
				  0x2A,0x64,0x60,0x9E,0x8F,0x22,0x08,0x2A,0x9C,0xA2,0xBC,0xAE,0x7E,0x77,0x9B,0xBB};
*/
int main(){
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
    fread(buf,1,576,fp);
    bign p=bign::from_bytes(buf,256);
    bign q=bign::from_bytes(buf+256,32);
    bign g=bign::from_bytes(buf+288,256);
    bign x_false=bign::from_bytes(buf+544,32);
    uint16_t len;
    fread(&len,sizeof(uint16_t),1,fp);
    fread(buf,1,len,fp);
    H(buf,len);
	uint8_t tttmp[32];
    for(int i=0;i<=7;++i){
		uint32_t ttmp=_hash[i];
		for(int j=0;j<=3;++j){
			tttmp[(i<<2)+3-j]=ttmp;
			ttmp>>=8;
		}
	}
	//fwrite(tttmp,1,32,wp);
	bign Hm1=bign::from_bytes(tttmp,32);
	fread(buf,1,576,fp2);
	fread(&len,sizeof(uint16_t),1,fp2);
    fread(buf,1,len,fp2);
    H(buf,len);
    for(int i=0;i<=7;++i){
		uint32_t ttmp=_hash[i];
		for(int j=0;j<=3;++j){
			tttmp[(i<<2)+3-j]=ttmp;
			ttmp>>=8;
		}
	}
	bign Hm2=bign::from_bytes(tttmp,32);
	fread(buf,1,64,fp3);
	bign r1=bign::from_bytes(buf,32);
	bign s1=bign::from_bytes(buf+32,32);
	fread(buf,1,64,fp4);
	bign r2=bign::from_bytes(buf,32);
	bign s2=bign::from_bytes(buf+32,32);
	if(r1==r2){
		bign delta_s=s2-s1,inv_delta_s,inv_r,k_inv;
		bign y;
		ex_gcd(delta_s,q,inv_delta_s,y);
		bign k=(Hm2-Hm1)*inv_delta_s%q;
		cout<<"k: ";
		k.print();
		k.to_bytes(buf,32);
		ex_gcd(r1,q,inv_r,y);
		bign x=(s1*k-Hm1)*inv_r%q;
		cout<<"x: ";
		x.print();
		x.to_bytes(buf+32,32);
		fwrite(buf,1,64,wp);
		//validate x
		ex_gcd(k,q,k_inv,y);
		if(s2==(Hm2+x*r2)*k_inv%q) cout<<"pass";
	}
    return 0;
}

