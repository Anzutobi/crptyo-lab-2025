#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <ctime>

using namespace std;

// Global constants for the modulus P
__int128 P;
uint64_t P_inv; // -P^{-1} mod 2^64
__int128 R2_mod_P; // R^2 mod P

// Global constants for the order N
uint64_t N;

// Helper to parse __int128 from string
__int128 parse_u128(const char* str) {
    __int128 res = 0;
    while (*str) {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

// Helper to print __int128
void print_u128(__int128 n) {
    if (n == 0) {
        printf("0\n");
        return;
    }
    char buf[64];
    int i = 0;
    while (n > 0) {
        buf[i++] = (char)(n % 10 + '0');
        n /= 10;
    }
    for (int j = i - 1; j >= 0; j--) putchar(buf[j]);
    putchar('\n');
}

// Calculate P_inv = -P^{-1} mod 2^64
uint64_t calc_inv(uint64_t n) {
    uint64_t x = 1;
    for (int i = 0; i < 6; ++i) x = x * (2 - n * x);
    return -x;
}

// Montgomery Multiplication
// Computes a * b * R^-1 mod P
// P is 120 bits, so it fits in __int128.
// We treat __int128 as 2 x 64-bit limbs for the multiplication logic to handle 240-bit product.
inline __int128 mont_mul(__int128 a, __int128 b) {
    uint64_t a_lo = (uint64_t)a;
    uint64_t a_hi = (uint64_t)(a >> 64);
    uint64_t b_lo = (uint64_t)b;
    uint64_t b_hi = (uint64_t)(b >> 64);
    uint64_t p_lo = (uint64_t)P;
    uint64_t p_hi = (uint64_t)(P >> 64);

    // i = 0
    // T = a_lo * B
    __int128 C = (__int128)a_lo * b_lo;
    uint64_t m = (uint64_t)C * P_inv;
    __int128 MP = (__int128)m * p_lo;
    __int128 sum = C + MP;
    __int128 carry = sum >> 64; // This carry is part of the division by 2^64
    
    // T_current = carry + a_lo * b_hi + m * p_hi
    __int128 T0 = carry + (__int128)a_lo * b_hi + (__int128)m * p_hi;

    // i = 1
    // We add a_hi * B to T0 (which is effectively T / 2^64 from previous step)
    // But we are doing the reduction for the next limb of T.
    // The "current T" is T0.
    // We add a_hi * b_lo to the lower part of T0.
    __int128 C2 = T0 + (__int128)a_hi * b_lo;
    uint64_t m2 = (uint64_t)C2 * P_inv;
    __int128 MP2 = (__int128)m2 * p_lo;
    __int128 sum2 = C2 + MP2;
    __int128 carry2 = sum2 >> 64;

    // Final result
    // T_final = carry2 + a_hi * b_hi + m2 * p_hi
    __int128 T1 = carry2 + (__int128)a_hi * b_hi + (__int128)m2 * p_hi;

    if (T1 >= P) T1 -= P;
    return T1;
}

// Standard modular exponentiation using Montgomery
__int128 mont_pow(__int128 a, uint64_t b) { // a^b mod P
    // Convert a to Montgomery form: a * R mod P
    __int128 a_mont = mont_mul(a, R2_mod_P);
    __int128 res_mont = mont_mul(1, R2_mod_P); // 1 in Mont form

    while (b > 0) {
        if (b & 1) res_mont = mont_mul(res_mont, a_mont);
        a_mont = mont_mul(a_mont, a_mont);
        b >>= 1;
    }
    
    // Convert back from Montgomery form: mont_mul(res, 1)
    return mont_mul(res_mont, 1);
}

// Robust modular inverse for uint64_t
uint64_t mod_inverse(uint64_t a, uint64_t m) {
    int64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;
    int64_t a_signed = a;
    if (m == 1) return 0;
    while (a_signed > 1) {
        q = a_signed / m0;
        t = m0;
        m0 = a_signed % m0;
        a_signed = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m;
    return x1;
}

// Rho algorithm
uint64_t rho(__int128 alpha, __int128 beta) {
    // Precompute R^2 mod P
    __int128 R_mod_P = 1;
    for (int i = 0; i < 128; ++i) {
        R_mod_P <<= 1;
        if (R_mod_P >= P) R_mod_P -= P;
    }
    
    R2_mod_P = R_mod_P;
    for (int i = 0; i < 128; ++i) {
        R2_mod_P <<= 1;
        if (R2_mod_P >= P) R2_mod_P -= P;
    }

    // Setup random walk
    srand(time(0));
    int r_branches = 32;
    vector<__int128> M_mont(r_branches);
    vector<uint64_t> u_val(r_branches);
    vector<uint64_t> v_val(r_branches);

    for (int i = 0; i < r_branches; ++i) {
        u_val[i] = (uint64_t)rand() * rand() % N;
        v_val[i] = (uint64_t)rand() * rand() % N;
        
        // M[i] = alpha^u * beta^v
        __int128 t1 = mont_pow(alpha, u_val[i]);
        __int128 t2 = mont_pow(beta, v_val[i]);
        
        // Convert to Mont form for the walk
        __int128 t1_mont = mont_mul(t1, R2_mod_P);
        __int128 t2_mont = mont_mul(t2, R2_mod_P);
        M_mont[i] = mont_mul(t1_mont, t2_mont);
    }

    while (true) {
        uint64_t a = (uint64_t)rand() * rand() % N;
        uint64_t b = (uint64_t)rand() * rand() % N;
        
        // x = alpha^a * beta^b
        __int128 t1 = mont_pow(alpha, a);
        __int128 t2 = mont_pow(beta, b);
        __int128 x_mont = mont_mul(mont_mul(t1, R2_mod_P), mont_mul(t2, R2_mod_P));
        
        uint64_t aa = a;
        uint64_t bb = b;
        
        __int128 save_x = x_mont;
        uint64_t save_a = aa;
        uint64_t save_b = bb;
        
        long long steps = 0;
        long long limit = 2;
        
        while (true) {
            // Walk
            int tag = (int)(x_mont & (r_branches - 1));
            x_mont = mont_mul(x_mont, M_mont[tag]);
            aa = (aa + u_val[tag]);
            if (aa >= N) aa -= N;
            bb = (bb + v_val[tag]);
            if (bb >= N) bb -= N;
            
            steps++;
            
            if (x_mont == save_x) {
                // Collision
                uint64_t delta_b = (bb >= save_b) ? (bb - save_b) : (N - (save_b - bb));
                uint64_t delta_a = (save_a >= aa) ? (save_a - aa) : (N - (aa - save_a));
                
                uint64_t temp_b = delta_b;
                uint64_t temp_n = N;
                while(temp_n) {
                    uint64_t t = temp_b % temp_n;
                    temp_b = temp_n;
                    temp_n = t;
                }
                uint64_t g = temp_b;
                
                if (g == 1) {
                    uint64_t binv = mod_inverse(delta_b, N);
                    __int128 ans = ((__int128)delta_a * binv) % N;
                    return (uint64_t)ans;
                } else {
                    break; // Retry
                }
            }
            
            if (steps == limit) {
                save_x = x_mont;
                save_a = aa;
                save_b = bb;
                limit <<= 1;
                steps = 0;
            }
        }
    }
}

int main() {
    char buf[1024];
    scanf("%s", buf);
    P = parse_u128(buf);
    scanf("%s", buf);
    N = (uint64_t)parse_u128(buf);
    scanf("%s", buf);
    __int128 alpha = parse_u128(buf);
    scanf("%s", buf);
    __int128 beta = parse_u128(buf);
    P_inv = calc_inv((uint64_t)P);
    uint64_t res = rho(alpha, beta);
    print_u128(res);
    
    return 0;
}
