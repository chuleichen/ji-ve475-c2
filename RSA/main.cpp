#include <iostream>
#include <stdio.h>
#include <gmp.h>
#include <cmath>

using namespace std;

int main() {
    //variables for generating keys
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);

    // Get the security rate
    int security;
    unsigned int bits;
    cout << "Please enter the security rate: " << endl;
    cin >> security;
    switch(security) {
        case 80: bits = 1024; break;
        case 112: bits = 2048; break;
        case 128: bits = 3072; break;
        case 192: bits = 7680; break;
        case 256: bits = 15360; break;
    }

    //generate p and q according to the security level
    gmp_randstate_t state;
    gmp_randinit_default(state);
    while (mpz_sizeinbase(n, 2) < bits) { //keep generating until n meets the security level
        mpz_urandomb(p, state, bits);
        while (mpz_probab_prime_p(p, 30) == 0) {
            mpz_urandomb(p, state, bits);
        } //keep generating p until p is a prime number
        mpz_urandomb(q, state, bits);
        while (mpz_probab_prime_p(q, 30) == 0) {
            mpz_urandomb(q, state, bits);
        } //keep generating q until q is a prime number
        mpz_mul(n, p, q); //calculate n
    }
    gmp_printf("Generating key finished, with p = %Zd, \n q = %Zd, \n n = %Zd. \n", p, q, n);

    //calculate remaining variables
    mpz_t phi;
    mpz_t e;
    mpz_t d;
    mpz_init(phi);
    mpz_init(e);
    mpz_init(d);
    mpz_t temp1;
    mpz_t temp2;
    mpz_init(temp1);
    mpz_init(temp2);
    //phi
    mpz_sub_ui(temp1, p, 1);
    mpz_sub_ui(temp2, q, 1);
    mpz_mul(phi, temp1, temp2);
    //e, assume e is a 16-bit number
    mpz_urandomb(e, state, 16);
    mpz_gcd(temp1, e, phi);
    while (mpz_cmp_ui(temp1, 1) != 0) {
        mpz_urandomb(e, state, 16);
        mpz_gcd(temp1, e, phi);
    }
    //d
    mpz_invert(d, e, phi);
    gmp_printf("The public key (n, d) is (%Zd, %Zd). \n", n ,d);

    //encipher
    unsigned long int m0;
    cout << "Please enter the message:" << endl;
    cin >> m0;
    mpz_t m;
    mpz_t c;
    mpz_init(m);
    mpz_init(c);
    mpz_set_ui(m, m0);
    mpz_powm(c, m ,e, n);
    gmp_printf("The ciphertext is %Zd. \n", c);

    //decipher
    mpz_t newm;
    mpz_init(newm);
    mpz_powm(newm, c, d, n);
    gmp_printf("The plaintext is %Zd. \n", d);

    //test whether the result is correct
    if (mpz_cmp(m, newm) == 0) cout << "The result is correct!" << endl;
    else cout <<  "The result is wrong!" << endl;

    return 0;
}
