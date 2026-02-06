#include<iostream>
#include<vector>
#include<cstring>
#include<cstdint>
#include<fstream>
#include<fcntl.h>
#include<wmmintrin.h>
#include<immintrin.h>
#include<emmintrin.h> 
#include<algorithm>
using namespace std;
/*
FILE *fp=fopen("dump (10).bin","rb");
FILE *p=fopen("output.bin","wb");
*/

#define fp stdin
#define p stdout

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

    static bign mul(const bign& a,const bign& b){
        if((a.len==1 && a.d[0]==0) || (b.len==1 && b.d[0]==0)) return bign(0);
        bign res;
        res.len=a.len+b.len;
        if(res.len>MAXN) res.len=MAXN;
        memset(res.d,0,sizeof(uint64_t)*res.len);
        for(int i=0;i<a.len;++i){
            uint64_t r_c=0,hi,lo,v;
            for(int j=0;j<b.len;++j){
                if(i+j>=MAXN) break;
                lo=mulx_u64(a.d[i],b.d[j],&hi);
				v=res.d[i + j];
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
            printf("0\n");
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
        printf("%llu",c.back());
        for(int i=c.size()-2;i>=0;--i) printf("%019llu",c[i]);
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
bign mont_mul(bign& a,bign& b,bign& n,uint64_t n_prime){
    bign T=a*b;
    int k=n.len;
    for(int i=0;i<k;++i){
        uint64_t u=T.d[i]*n_prime,m_carry=0;
        for(int j=0;j<k;++j){
            uint64_t hi,lo;
            lo=mulx_u64(u,n.d[j],&hi);
            uint64_t v=T.d[i+j];
            uint8_t k1=addcarry_u64(0,v,lo,&v);
            uint8_t k2=addcarry_u64(0,v,m_carry,&T.d[i+j]);
            m_carry=hi+k1+k2;
        }
        uint8_t c=0;
        uint64_t val=T.d[i+k];
        c=addcarry_u64(0,val,m_carry,&T.d[i+k]);
        int idx=i+k+1;
        while(c && idx<MAXN){
            c=addcarry_u64(c,T.d[idx],0,&T.d[idx]);
            idx++;
        }
    }
    bign res;
    res.len=k;
    for(int i=0;i<k;++i) res.d[i]=T.d[i+k];
    bool overflow=false;
    if(2*k<MAXN) overflow=(T.d[2*k]!=0);
    if(overflow){
        uint8_t borrow=0;
        for(int i=0;i<k;++i){
            uint64_t ai=res.d[i];
            uint64_t bi=(i<n.len)?n.d[i]:0;
            borrow=subborrow_u64(borrow,ai,bi,&res.d[i]);
        }
    }
	else if(res>=n) res-=n;
    res.clean();
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

char s[6000],s2[6000],sp[6000];
uint8_t tmp[1024],seed[32];

/*
uint8_t seed[32]={0x84,0xEE,0x1D,0x92,0xBE,0xFC,0x55,0x96,
				  0x6F,0x20,0xB6,0xFD,0x18,0xFC,0x45,0x20,
				  0xCF,0x17,0x0B,0xD8,0x92,0xE2,0xE0,0xD3,
				  0x4F,0xE7,0xA0,0x10,0x17,0xC8,0x6B,0x67};
				  */
const uint8_t null_hash[32]={0xE3,0xB0,0xC4,0x42,0x98,0xFC,0x1C,0x14,
							 0x9A,0xFB,0xF4,0xC8,0x99,0x6F,0xB9,0x24,
							 0x27,0xAE,0x41,0xE4,0x64,0x9B,0x93,0x4C,
							 0xA4,0x95,0x99,0x1B,0x78,0x52,0xB8,0x55};					 
int t;
bign x,y;
vector<uint8_t> MGF(uint8_t seed[],int seedlen,int len){
    vector<uint8_t> v,data;
    v.reserve(len+32);
    uint32_t cnt=0,hash_res[8];
    while(v.size()<len){
        data.clear();
        data.insert(data.end(),seed,seed+seedlen);
        data.push_back((cnt>>24)&0xFF);
        data.push_back((cnt>>16)&0xFF);
        data.push_back((cnt>>8)&0xFF);
        data.push_back(cnt&0xFF);
        uint64_t currentLen=data.size(),bitLen=currentLen*8;
        data.push_back(0x80);
        while((data.size()%64)!=56) data.push_back(0x00);
        for(int i=7;i>=0;--i) data.push_back((bitLen>>(i*8))&0xFF);
        for(int i=0;i<data.size();i+=64){
            sha_256(data.data()+i,hash_res,(i==0?0:1));
        }
		for(int i=0;i<8;++i){
            uint32_t val=hash_res[i];
            v.push_back((val>>24)&0xFF);
            v.push_back((val>>16)&0xFF);
            v.push_back((val>>8)&0xFF);
            v.push_back(val&0xFF);
        }
        cnt++;
    }
    v.resize(len);
    return v;
}
int main(){
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
    uint8_t p0[128],q[128],n[256],d[256],d_mod_pm1[128],d_mod_qm1[128],q_inv[128],C[256];
    fread(p0,1,128,fp);
    fread(q,1,128,fp);
    fread(n,1,256,fp);
    fread(d,1,256,fp);
    fread(d_mod_pm1,1,128,fp);
    fread(d_mod_qm1,1,128,fp);
    fread(q_inv,1,128,fp);
    if(fread(C,1,256,fp)!=256) return 0;
    bign p_bn=bign::from_bytes(p0,128);
    bign q_bn=bign::from_bytes(q,128);
    bign d_mod_pm1_bn=bign::from_bytes(d_mod_pm1,128);
    bign d_mod_qm1_bn=bign::from_bytes(d_mod_qm1,128);
    bign qInv_bn=bign::from_bytes(q_inv,128);
    bign C_bn=bign::from_bytes(C,256);
    //CRT
    bign cp=C_bn%p_bn,cq=C_bn%q_bn,m1,m2;
    m1=mont_pow(cp,d_mod_pm1_bn,p_bn);
    m2=mont_pow(cq,d_mod_qm1_bn,q_bn);
    bign ttmp=((m1-m2)*qInv_bn)%p_bn;
    bign m=m2+ttmp*q_bn;
    uint8_t EM[256];
    m.to_bytes(EM,256);
    if(EM[0]!=0x00) return 0;
    uint8_t* MaskedSeed=EM+1;
    uint8_t* MaskedDB=EM+33;
    vector<uint8_t> seedMask=MGF(MaskedDB,223,32);
    uint8_t Seed[32];
    for(int i=0;i<32;++i) Seed[i]=MaskedSeed[i]^seedMask[i];
    vector<uint8_t> dbMask=MGF(Seed,32,223);
    uint8_t DB[256]; 
    for(int i=0;i<223;++i){
    	DB[i]=MaskedDB[i]^dbMask[i];
    	//cout<<hex<<(int)DB[i]<<' ';
	}
    for(int i=0;i<32;++i) if(DB[i]!=null_hash[i]) return 0;
    int idx=-1;
    for(int i=32;i<223;++i){
        if(DB[i]==0x01){
            idx=i;
            break;
        }
        if(DB[i]!=0x00) return 0; 
    }
    if(idx==-1) return 0;
    //cout<<idx<<endl;
    int mStart=idx+1;
    int mLen=223-mStart;
    if(mLen>0) fwrite(DB+mStart,1,mLen,p);
    return 0;
}
