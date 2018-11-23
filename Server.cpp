#include<gmp.h>
#include<bits/srdc++.h>
using namespace std;
void RandomInit(gmp_randstate_t st);
//Various Curve parameters in string format.
string strCurve = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F";
string strXp = "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798";
string strXy = "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8";
string strQ = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141";
string strP = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F";

//Variable to store curve parameters in large integer format.
mpz_t Xp,Xy,q,a,b,P,s;


void genParameters()
{
	//initlizing random state variable.
	gmp_randstate_t st;

	//initilizing various large intergers parameters
	mpz_inits(Xp,Xy,q,a,b,P,NULL);
	int ret;
	
	ret = mpz_set_str(P,strP,16);
	if(ret == 0)
		gmp_printf("P=%Zd\n",P.;

	ret = mpz_set_str(Xp,strXp,16);
	if(ret == 0)
		gmp_printf("Xp=%Zd\n",Xp);

	ret = mpz_set_str(Xy,strXy,16);
	if(ret == 0)
		gmp_printf("Xy=%Zd\n",Xy);

	ret = mpz_set_str(q,strQ,16);
	if(ret == 0)
		gmp_printf("q=%Zd\n",q);


	 //Generate a private key for Server s whose value lies [0 - q-1].
	RandomInit(st);
 	mpz_urandomm(s,st,q);   

}

void RandomInit(gmp_randstate_t st)
{
	unsigned long seed;
	seed= time(NULL);
	gmp_randinit_mt(st);
	gmp_randseed_ui(st,seed);
	return;
}