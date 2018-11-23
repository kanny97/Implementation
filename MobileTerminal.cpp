#include <bits/stdc++.h>
#include <gmp.h>
#include "Ecc.cpp"
using namespace std;
//define the global parameters of the mobile terminal
string n="FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141";
//curve used for ECC is secp256k1 curve

int main()
{
	//First the user needs to register to the server(User registration phase)
	mpz_t q,r_i;
	mpz_inits(q,r_i,NULL);
	int temp=mpz_set_str (q,"FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141",16);
	if(temp!=0)
	return 0;
	gmp_randstate_t st;
	RandomInit(st);
	mpz_urandomm(r_i,st,q);
	gmp_printf("r_i=%Zd\n",r_i);
	string x=getCipher(r_i);
	cout<<"Returned point=\n"<<x;
	return 0;
}