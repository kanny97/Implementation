#include<gmp.h>
#include<bits/srdc++.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/channels.h> 
#include <cryptopp/filters.h> 
#include "Ecc.cpp"
using namespace std;
void RandomInit(gmp_randstate_t st);
//Various Curve parameters in string format.
string strCurve = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F";
string strXp = "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798";
string strYp = "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8";
string strQ = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141";
string strP = "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F";

//Variable to store curve parameters in large integer format.
mpz_t Xp,Yp,q,a,b,P,s,sInv,P_pub,IDm,X_m,r_s;
mpz_t w_s,f_m; 
string K;
string strIDm,l_m, h_m;;


//Generates various parameters and return the public key of server.
string genParameters()
{
	//initlizing random state variable.
	gmp_randstate_t st;
	mpz_t Xr,Yr;

	//initilizing various large intergers parameters
	mpz_inits(Xp,Yp,q,a,b,P,s,sInv,P_pub,Xr,Yr,r_s,NULL);
	int ret;
	
	ret = mpz_set_str(P,strP.c_str(),16);
	if(ret == 0)
		gmp_printf("P=%Zd\n",P);

	ret = mpz_set_str(Xp,strXp.c_str(),16);
	if(ret == 0)
		gmp_printf("Xp=%Zd\n",Xp);

	ret = mpz_set_str(Yp,strYp.c_str(),16);
	if(ret == 0)
		gmp_printf("Xy=%Zd\n",Yp);

	ret = mpz_set_str(q,strQ.c_str(),16);
	if(ret == 0)
		gmp_printf("q=%Zd\n",q);


	 //Generate a private key for Server s whose value lies [0 - q-1].
	RandomInit(st);
 	mpz_urandomm(s,st,q);
	
	//Now performing Scalar multiplication to generate Public key of server.
	string p_pub = getCipher(s);

	 return p_pub;

}


string userRegistration(string M)
{
	gmp_randstate_t st; //For random number generation.    
	mpz_inits(w_s,f_m,NULL);    

	//To tokenize the message M into IDm and lm.
	stringstream tokenizer(M);
	

	//For implementing the hash function.
	string message ;
	SHA512 sha512;
	


	//get lm and idm from the string
	getline(tokenizer,strIDm,'#');
	getline(tokenizer,l_m,'#');

	cout<<"\nIDm = "<<strIDm<<endl;
	cout<<"\nl_m = "<<l_m<<endl;

	mpz_set_str(IDm,strIDm,16);
	RandomInit(st);
 	mpz_urandomm(w_s,st,q);

 	string R_m = getCipher(w_s);
 	
 	message = strIDm+R_m+l_m;
 	HashFilter f1(sha512, new HexEncoder(new StringSink(h_m)));
	ChannelSwitch cs;
	cs.AddDefaultRoute(f1);
	StringSource ss(message, true /*pumpAll*/, new Redirector(cs));
	cout<<"\nh_m = "<<h_m<<endl;

	mpz_mul(f_m,hm,s);
	mpz_mod(f_m,f_m,P);
	mpz_add(f_m,w_s,f_m);
	char *str2;
	char *str = mpz_get_str(str2,16,f_m);
	
	string M1(str);
	string M2(R_m);
	M2+="#";
	M2+=M1;
	M2+="#";

	return M2;
}

string userAuthentication(string M)
{
	stringstream tokenizer(M);
	string strPIDm,v_m,Tm1;
	string xm_tm,check_hm;
	string X_m1,IDm1,message;
	gmp_randstate_t st;
	SHA512 sha512;

	RandomInit(st);

	getline(tokenizer,strPIDm,'#');
	getline(tokenizer,v_m,'#');
	getline(tokenizer,Tm1,'#');
	
	///////////////////////////////////////////////
//	Add code to check time stamp.



	//////////////////////////////////////////////
	mpz_invert(sInv,s,P);
	X_m1 = getCipher(sInv);

	message = x_m1+Tm1;
 	HashFilter f1(sha512, new HexEncoder(new StringSink(xm_tm)));
	ChannelSwitch cs;
	cs.AddDefaultRoute(f1);
	StringSource ss(message, true /*pumpAll*/, new Redirector(cs));
	cout<<"\nh_m = "<<xm_tm<<endl;

	IDm1 = strPIDm ^ xm_tm;
	message  = IDm1 + R_m + l_m;
	HashFilter f2(sha512, new HexEncoder(new StringSink(check_hm)));
	ChannelSwitch cs2;
	cs.AddDefaultRoute(f2);
	StringSource ss2(message, true /*pumpAll*/, new Redirector(cs2));

	if(check_hm != hm)
	{
		cout<<"\nError h_m not equal to H2(IDm||R_m||l_m)\nNot safe connection.\n";
		exit(0);
	}

	cout<<"\nVerified\n";

	mpz_urandomm(r_s,st,q);
	R_s = getCipher(r_s);

	string rs_Xm=getCipher(r_s,X_m1);
	message = rs_Xm+l_m;
	HashFilter f3(sha512, new HexEncoder(new StringSink(K)));
	ChannelSwitch cs3;
	cs.AddDefaultRoute(f3);
	StringSource ss3(message, true /*pumpAll*/, new Redirector(cs3));


	cout<<"\nThe Key K  = "<<K<<endl;
	string Aut1;
	string Ts1 = "0";
	message = IDm1+Tm1+Ts1+x_m1+R_s+l_m;
	HashFilter f4(sha512, new HexEncoder(new StringSink(K)));
	ChannelSwitch cs4;
	cs.AddDefaultRoute(f4);
	StringSource ss4(message, true /*pumpAll*/, new Redirector(cs4));
	cout<<"Aut1 = "<<Aut1<<endl;
	return Aut1;

}



void RandomInit(gmp_randstate_t st)
{
	unsigned long seed;
	seed= time(NULL);
	gmp_randinit_mt(st);
	gmp_randseed_ui(st,seed);
	return;
}