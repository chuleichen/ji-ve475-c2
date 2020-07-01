#include <iostream>
#include <fstream>
#include <stdio.h>
#include <gmp.h>
#include <cmath>

using namespace std;

void GenerateKey(){
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    while (mpz_sizeinbase(n, 2) < 1024) { //keep generating until n meets the security level
        mpz_urandomb(p, state, 1024);
        while (mpz_probab_prime_p(p, 40) == 0) {
            mpz_urandomb(p, state, 1024);
        } //keep generating p until p is a prime number
        mpz_urandomb(q, state, 1024);
        while (mpz_probab_prime_p(q, 40) == 0) {
            mpz_urandomb(q, state, 1024);
        } //keep generating q until q is a prime number
        mpz_mul(n, p, q); //calculate n
    }
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
    gmp_printf("%Zd \n %Zd \n %Zd \n %Zd \n %Zd \n %Zd \n", p, q, n, phi, e, d);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(temp1);
    mpz_clear(temp2);
}

void encrypt(const char * message, string key){
    //read key
    ifstream keyfile;
    keyfile.open(key);
    string p0, q0, n0, phi0, e0, d0;
    keyfile >> p0 >> q0 >> n0 >> phi0 >> e0 >> d0;
    const char* e1 = e0.c_str();
    const char* n1 = n0.c_str();
    //encryption
    mpz_t m;
    mpz_t c;
    mpz_t e;
    mpz_t n;
    mpz_init(m);
    mpz_init(c);
    mpz_init(e);
    mpz_init(n);
    mpz_set_str(m, message, 10);
    mpz_set_str(e, e1, 10);
    mpz_set_str(n, n1, 10);
    mpz_powm(c, m ,e, n);
    gmp_printf("%Zd \n", c);
    mpz_clear(m);
    mpz_clear(c);
    mpz_clear(e);
    mpz_clear(n);
}

void decrypt(const char * message, string key){
    //read key
    ifstream keyfile;
    keyfile.open(key);
    string p0, q0, n0, phi0, e0, d0;
    keyfile >> p0 >> q0 >> n0 >> phi0 >> e0 >> d0;
    const char* d1 = d0.c_str();
    const char* n1 = n0.c_str();
    //encryption
    mpz_t m;
    mpz_t c;
    mpz_t d;
    mpz_t n;
    mpz_init(m);
    mpz_init(c);
    mpz_init(d);
    mpz_init(n);
    mpz_set_str(m, message, 10);
    mpz_set_str(d, d1, 10);
    mpz_set_str(n, n1, 10);
    mpz_powm_sec(m, c, d, n);
    if (mpz_cmp_ui(m, 0) == 0) cout << "really zero?" << endl;
    gmp_printf("%Zd \n", m);
    mpz_clear(m);
    mpz_clear(c);
    mpz_clear(d);
    mpz_clear(n);
}

int main(int argu, char * argv[]) {
    if (argu == 2) GenerateKey();
    else if (argu == 3 && argv[1][2] == 'e') encrypt(argv[2], "keyfile.txt");
    else if (argu == 3 && argv[1][2] == 'd') {
        //test if is using the default key to decript the ciphertext
        ifstream cipher;
        cipher.open("ciphertext.txt");
        string ciphertext0;
        const char* ciphertext1 = ciphertext0.c_str();
        mpz_t ciphertext;
        mpz_init(ciphertext);
        mpz_set_str(ciphertext, ciphertext1, 10);
        mpz_t text;
        mpz_init(text);
        mpz_set_str(text, argv[2], 10);
        if (mpz_cmp(text, ciphertext) == 0) {
            mpz_clear(text);
            mpz_clear(ciphertext);
            cout << "cheater: it is forbidden to decrypt the challenge ciphertext" << endl;
            return 0;
        }
        decrypt(argv[2], "keyfile.txt");
        mpz_clear(text);
        mpz_clear(ciphertext);
    }
    else if (argu == 5 && argv[1][2] == 'e') encrypt(argv[2], argv[4]);
    else if (argu == 5 && argv[1][2] == 'd') decrypt(argv[2], argv[4]);
    return 0;
}
