#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<fstream>
#include<cstdint>
#include<fcntl.h>
using namespace std;
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
        d[0]=0;
        len=1;
        sign=false;
    }
    bign(int v){
        if(v<0){
            sign=true;
            v=-v;
        }
		else sign=false;
        d[0]=(uint64_t)v;
        len=1;
    }
    bign(long long v){
        if(v<0){
            sign=true;
            v=-v;
        }
		else sign=false;
        d[0]=(uint64_t)v;
        len=1;
    }
    bign(unsigned long long v){
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

    void shift_words(int k){
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

    static bign mul(const bign& a,const bign& b){
        if((a.len==1 && a.d[0]==0) || (b.len==1 && b.d[0]==0)) return bign(0);
        bign res;
        res.len=a.len+b.len;
        if(res.len>MAXN) res.len=MAXN;
        memset(res.d,0,sizeof(uint64_t)*res.len);
        for(int i=0;i<a.len;++i){
            uint64_t r_c=0;
            for(int j=0;j<b.len;++j){
                if(i+j>=MAXN) break;
                unsigned __int128 prod=(unsigned __int128)a.d[i]*b.d[j]+res.d[i+j]+r_c;
                res.d[i+j]=(uint64_t)prod;
                r_c=(uint64_t)(prod>>64);
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
    for(int i=0;i<a.len;++i){
        uint64_t ai=a.d[i];
        if(ai==0) continue;
        uint64_t carry=0;
        uint64_t *tp=t+i;
        for(int j=0;j<b.len;++j){
            unsigned __int128 prod=(unsigned __int128)ai*b.d[j]+tp[j]+carry;
            tp[j]=(uint64_t)prod;
            carry=(uint64_t)(prod>>64);
        }
        tp[b.len]+=carry;
    }
    for(int i=0;i<k;++i){
        uint64_t m=t[i]*n_prime;
        uint64_t carry=0;
        uint64_t *tp=t+i;
        for(int j=0;j<k;++j){
            unsigned __int128 prod=(unsigned __int128)m*n.d[j]+tp[j]+carry;
            tp[j]=(uint64_t)prod;
            carry=(uint64_t)(prod>>64);
        }
        int current=i+k;
        while(carry){
            unsigned __int128 sum=(unsigned __int128)t[current]+carry;
            t[current]=(uint64_t)sum;
            carry=(uint64_t)(sum>>64);
            current++;
        }
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
uint64_t get_n_prime(const bign& n){
    uint64_t x=1;
    uint64_t n0=n.d[0];
    for(int i=0;i<6;++i) x=x*(2-n0*x);
    return -x;
}
bign get_R2_mod_n(const bign& n){
    bign R;
    R.len=n.len+1;
    R.d[n.len]=1;
    bign R_mod_n=R%n;
    return (R_mod_n*R_mod_n)%n;
}

bign mont_pow(bign a,bign b,bign n, uint64_t n_prime, const bign& R2_mod_n){
    if(n.len==1 && n.d[0]==1) return zero;
    if(b.len==1 && b.d[0]==0) return one;
    bign a_mont=mont_mul(a,R2_mod_n,n,n_prime);
    const int w=5;
    bign precomp[1<<w];
    precomp[0]=mont_mul(one,R2_mod_n,n,n_prime);
    precomp[1]=a_mont;
    for(int i=2;i<(1<<w);++i){
        precomp[i]=mont_mul(precomp[i-1],a_mont,n,n_prime);
    }
    bign ans=precomp[0];
    int bit_len=b.len*64-__builtin_clzll(b.d[b.len-1]);
    int i=bit_len-1;
    while(i>=0){
        if(((b.d[i/64]>>(i%64))&1)==0){
            ans=mont_mul(ans,ans,n,n_prime);
            i--;
        } 
		else{
            int s=max(0,i-w+1);
            while(s<i && (((b.d[s/64]>>(s%64))&1)==0)) s++;
            int value=0;
            for(int j=i;j>=s;--j){
                value=(value<<1) | ((b.d[j/64]>>(j%64)) & 1);
            }
            for(int j=0;j<(i-s+1);++j) ans=mont_mul(ans,ans,n,n_prime);
            ans=mont_mul(ans,precomp[value],n,n_prime);
            i=s-1;
        }
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
uint8_t buf[8192];
int main(){
    int t;
    scanf("%d",&t);
    scanf("%s",buf);
    bign p=bign::from_string(buf);
    uint64_t n_prime=get_n_prime(p);
    bign R2_mod_p=get_R2_mod_n(p);
    while(t--){
        scanf("%s",buf);
        bign n=bign::from_string(buf);
        scanf("%s",buf);
        bign m=bign::from_string(buf);
        bign _add=(n+m)%p;
        _add.print();
        bign _sub=(n-m)%p;
        if(_sub.sign) _sub=_sub+p;
        _sub.print();
        /*
        bign _mul=mont_mul(n,m,p,n_prime);
        _mul=mont_mul(_mul,R2_mod_p,p,n_prime);
        */
        bign _mul=(n*m)%p;
        _mul.print();
        bign x,y;
        ex_gcd(n,p,x,y);
        x=x%p;
        if(x.sign) x=x+p;
        x.print();
        bign _pow=mont_pow(n,m,p,n_prime,R2_mod_p);
        _pow.print();
    }
    return 0;
}
