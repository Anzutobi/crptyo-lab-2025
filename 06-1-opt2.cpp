#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>
#include<cstdint>
#include<cstdlib>
#include<ctime>
#include<map>
#include<unordered_map>
#include<cmath>
using namespace std;

typedef unsigned __int128 u128;
typedef uint64_t u64;

u128 n,R2_mod_p,p;
u64 p_inv;
struct u128_hash{
    size_t operator()(const u128& k)const{
        return (size_t)(k^(k>>64));
    }
};
unordered_map<u128,u64,u128_hash> table;

u128 parse_u128(const char* str){
    u128 res=0;
    while(*str){
        res=(res<<3)+(res<<1)+(*str-'0');
        str++;
    }
    return res;
}
void print_u128(u128 n){
    if(n==0){
        printf("0\n");
        return;
    }
    char buf[64];
    int i=0;
    while(n>0){
        buf[i++]=(char)(n%10+'0');
        n/=10;
    }
    for(int j=i-1;j>=0;j--) putchar(buf[j]);
    putchar('\n');
}
u64 calc_inv(u64 n){
    u64 x=1;
    for(int i=0;i<6;++i) x=x*(2-n*x);
    return -x;
}
inline u128 mont_mul(u128 a,u128 b){
    u64 a_lo=(u64)a;
    u64 a_hi=(u64)(a>>64);
    u64 b_lo=(u64)b;
    u64 b_hi=(u64)(b>>64);
    u64 p_lo=(u64)p;
    u64 p_hi=(u64)(p>>64);
    u128 C=(u128)a_lo*b_lo;
    u64 m=(u64)C*p_inv;
    u128 Mp=(u128)m*p_lo;
    u128 carry=(C>>64)+(Mp>>64)+(((u128)((u64)C)+(u128)((u64)Mp))>>64);
    u128 T0=carry+(u128)a_lo*b_hi+(u128)m*p_hi;
    u128 C2_part=(u128)a_hi*b_lo;
    u64 m2=(u64)(T0+C2_part)*p_inv;
    u128 Mp2=(u128)m2*p_lo;
    u128 carry2=(T0>>64)+(C2_part>>64)+(Mp2>>64)+(((u128)((u64)T0)+(u128)((u64)C2_part)+(u128)((u64)Mp2))>>64);
    u128 T1=carry2+(u128)a_hi*b_hi+(u128)m2*p_hi;
    while(T1>=p) T1-=p;
    return T1;
}

u128 mont_pow(u128 a,u64 b){
    u128 a_mont=mont_mul(a,R2_mod_p);
    u128 res_mont=mont_mul(1,R2_mod_p);
    while(b>0){
        if(b&1) res_mont=mont_mul(res_mont,a_mont);
        a_mont=mont_mul(a_mont,a_mont);
        b>>=1;
    }
    return mont_mul(res_mont,1);
}

u64 mod_inverse(u64 a,u64 m){
    long long m0=m,t,q;
    long long x0=0,x1=1;
    long long a_signed=a;
    if(m==1) return 0;
    while(a_signed>1){
        q=a_signed/m0;
        t=m0;
        m0=a_signed%m0;
        a_signed=t;
        t=x0;
        x0=x1-q*x0;
        x1=t;
    }
    if(x1<0) x1+=m;
    return x1;
}

inline u64 rand64(){
    return ((u64)rand()<<45)^((u64)rand()<<30)^((u64)rand()<<15)^(u64)rand();
}

void init_mont() {
    u128 R_mod_p=1;
    for(int i=0;i<128;++i){
        R_mod_p<<=1;
        if(R_mod_p>=p) R_mod_p-=p;
    }
    R2_mod_p=R_mod_p;
    for(int i=0;i<128;++i){
        R2_mod_p<<=1;
        if(R2_mod_p>=p) R2_mod_p-=p;
    }
}

u64 kangaroo(u128 alpha,u128 beta){
    init_mont();
    u64 n_val=(u64)n;
    u64 m_val=(u64)sqrt((double)n_val);
    int k=32;
    vector<u64> S(k);
    vector<u128> M(k);
    u64 curr_s=1;
    u64 m_limit=m_val;
    for(int i=0;i<k;++i){
        S[i]=curr_s;
        if(S[i]>m_limit) S[i]=m_limit;
        u128 val=mont_pow(alpha,S[i]);
        M[i]=mont_mul(val,R2_mod_p); 
        curr_s+=curr_s;
    }
    u64 mask=0xFF;
    u64 target=std::max((u64)0xFF,m_val/260000);
    while(mask<target) mask=(mask<<1)|1;
    u64 xT=n_val;
    u128 yT=mont_pow(alpha,n_val);
    u128 yT_mont=mont_mul(yT,R2_mod_p);
    int limit=std::max((u64)(1.1*m_val),(u64)1114514);
    table.clear();
    for(int i=0;i<limit;++i){
        int j=(int)(yT_mont%k);
        xT=xT+S[j];
        yT_mont=mont_mul(yT_mont,M[j]);
        if((yT_mont & mask)==0){
            table[yT_mont]=xT;
            if(table.size()>260000) break;
        }
    }
    u64 xW=0;
    u128 yW_mont=mont_mul(beta,R2_mod_p);
    for(int i=0;i<limit;++i){
        int j=(int)(yW_mont%k);
        xW=xW+S[j];
        yW_mont=mont_mul(yW_mont,M[j]);
        if((yW_mont&mask)==0){
            if(table.count(yW_mont)){
                u64 target_x=table[yW_mont];
                if(target_x>xW) return(target_x-xW)%n_val;
            }
        }
    }
    return 0;
}

u64 rho(u128 alpha,u128 beta){
    init_mont();
    srand(time(0));
    int r_branches=20;
    vector<u128> M_mont(r_branches);
    vector<u64> u_val(r_branches);
    vector<u64> v_val(r_branches);
    for(int i=0;i<r_branches;++i){
        u_val[i]=rand64()%n;
        v_val[i]=rand64()%n;
        u128 t1=mont_pow(alpha,u_val[i]);
        u128 t2=mont_pow(beta,v_val[i]);
        u128 t1_mont=mont_mul(t1,R2_mod_p);
        u128 t2_mont=mont_mul(t2,R2_mod_p);
        M_mont[i]=mont_mul(t1_mont,t2_mont);
    }
    while(1){
        u64 a=rand64()%n,b=rand64()%n;
        u128 t1=mont_pow(alpha,a),t2=mont_pow(beta,b);
        u128 x_mont=mont_mul(mont_mul(t1,R2_mod_p),mont_mul(t2,R2_mod_p));
        u64 aa=a,bb=b;
        u128 save_x=x_mont;
        u64 save_a=aa,save_b=bb;
        long long steps=0,limit=2;
        while(1){
            int tag=(int)(x_mont%20);
            x_mont=mont_mul(x_mont,M_mont[tag]);
            aa+=u_val[tag];
            if(aa>=n) aa-=n;
            bb+=v_val[tag];
            if(bb>=n) bb-=n;
            steps++;
            if(x_mont==save_x){
                u64 delta_b=(bb>=save_b)?(bb-save_b):(n-(save_b-bb));
                u64 delta_a=(save_a>=aa)?(save_a-aa):(n-(aa-save_a));
                u64 temp_b=delta_b;
                u64 temp_n=n;
                while(temp_n){
                    u64 t=temp_b%temp_n;
                    temp_b=temp_n;
                    temp_n=t;
                }
                u64 g=temp_b;
                if(g==1){
                    u64 b_inv=mod_inverse(delta_b,n);
                    u128 ans=((u128)delta_a*b_inv)%n;
                    return (u64)ans;
                }
				else break;
            }
            if(steps==limit){
                save_x=x_mont;
                save_a=aa;
                save_b=bb;
                limit<<=1;
                steps=0;
            }
        }
    }
}
char buf[1024];
int main(){
    scanf("%s",buf);
    p=parse_u128(buf);
    scanf("%s",buf);
    n=(u64)parse_u128(buf);
    scanf("%s",buf);
    u128 alpha=parse_u128(buf);
    scanf("%s",buf);
    u128 beta=parse_u128(buf);
    alpha%=p;
    beta%=p;
    p_inv=calc_inv((u64)p);
    u64 res;
    if(n<(1<<30)) res=rho(alpha,beta);
    else res=kangaroo(alpha,beta);
    print_u128(res);
    return 0;
}
