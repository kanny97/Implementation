#include <bits/stdc++.h>
#include <gmp.h>
// #include "Ecc.cpp"
#include "EccTemp.cpp"
using namespace std;
//define the global parameters of the mobile terminal
string n="FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141";
//curve used for ECC is secp256k1 curve

// void UserRegistration(mpz_t q)
// {
// 	RandomInit(st);
// 	mpz_urandomm(r_i,st,q);
// 	gmp_printf("r_i=%Zd\n",r_i);
// 	string x=getCipher(r_i);
// }

void RandomInit(gmp_randstate_t st)
{
	unsigned long seed;
	seed= time(NULL);
	gmp_randinit_mt(st);
	gmp_randseed_ui(st,seed);
	return;
}

int main()
{
	//First the user needs to register to the server(User registration phase)
	mpz_t q,r_i;
	mpz_inits(q,r_i,NULL);
	int temp=mpz_set_str (q,"FFFFFFFF 00000000 FFFFFFFF FFFFFFFF BCE6FAAD A7179E84 F3B9CAC2 FC63255",16);
	if(temp!=0)
	return 0;
	gmp_randstate_t st;
	RandomInit(st);
	mpz_urandomm(r_i,st,q);
	gmp_printf("r_i=%Zd\n",r_i);
	// string x=getCipher(r_i);
	// cout<<"Returned point=\n"<<x;
	string x=getCipher(r_i);
	return 0;
}