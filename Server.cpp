#include<gmp.h>
#include<bits/stdc++.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/channels.h> 
#include <cryptopp/filters.h> 
#include "Ecc.cpp"
using namespace CryptoPP;
using std::cout;
using std::endl;
using std::string;
void RandomInit(gmp_randstate_t st);


//Variable to store curve parameters in large integer format.
mpz_t s,sInv,P_pub,IDm,X_m,r_s;
mpz_t w_s,f_m; 
string K;
string strIDm,l_m, h_m;
string R_m,R_s;

//Function to has input message using SHA512 hashing technique.
string hash(string message)
{
	string l_m;
	SHA sha512;
	HashFilter f1(sha512, new HexEncoder(new StringSink(l_m)));
	ChannelSwitch cs;
	cs.AddDefaultRoute(f1);
	StringSource ss(message, true /*pumpAll*/, new Redirector(cs));
	return l_m;
}


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
		gmp_printf("Yp=%Zd\n",Yp);

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
	int base = 16; 
	mpz_t H_m;
	mpz_inits(w_s,f_m,H_m,NULL);    

	//To tokenize the message M into IDm and lm.
	stringstream tokenizer(M);
	

	//For implementing the hash function.
	string message ;
	


	//get lm and idm from the string
	getline(tokenizer,strIDm,'#');
	getline(tokenizer,l_m,'#');

	cout<<"\nIDm = "<<strIDm<<endl;
	cout<<"\nl_m = "<<l_m<<endl;

	mpz_set_str(IDm,strIDm.c_str(),base);
	RandomInit(st);
 	mpz_urandomm(w_s,st,q);

 	R_m= getCipher(w_s);
 	
 	message = strIDm+R_m+l_m;
 	h_m = hash(message);
	cout<<"\nh_m = "<<h_m<<endl;

	mpz_set_str(H_m,h_m.c_str(),base);
	mpz_mul(f_m,H_m,s);
	mpz_mod(f_m,f_m,P);
	mpz_add(f_m,w_s,f_m);
	char *str2;
	char *str = mpz_get_str(str2,base,f_m);
	
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

	RandomInit(st);

	getline(tokenizer,strPIDm,'#');
	getline(tokenizer,v_m,'#');
	getline(tokenizer,Tm1,'#');
	
	///////////////////////////////////////////////
//	Add code to check time stamp.



	//////////////////////////////////////////////
	mpz_invert(sInv,s,P);
	X_m1 = getCipher(sInv);

	message = X_m1+Tm1;
 	xm_tm = hash(message);
	cout<<"\nxm_tm = "<<xm_tm<<endl;

/////////////////////////////////////////////////////////////////
	IDm1 = calcXor(strPIDm , xm_tm);			//verify if this works.
/////////////////////////////////////////////////////////////////	
	message  = IDm1 + R_m + l_m;
	check_hm = hash(message);

	if(check_hm != h_m)
	{
		cout<<"\nError h_m not equal to H2(IDm||R_m||l_m)\nNot safe connection.\n";
		exit(0);
	}

	cout<<"\nVerified\n";

	mpz_urandomm(r_s,st,q);
	R_s = getCipher(r_s);

	string rs_Xm=getCipher(r_s);   //multiply with X_m1.
	message = rs_Xm+l_m;
	K = hash(message);


	cout<<"\nThe Key K  = "<<K<<endl;
	string Aut1;
	string Ts1 = "0";
	message = IDm1+"#"+Tm1+"#"+Ts1+"#"+X_m1+"#"+R_s+"#"+l_m+"#";
	Aut1 = hash(message);
	cout<<"Aut1 = "<<Aut1<<endl;
	return Aut1;

}





int main()
{
	string p_pub=genParameters();
	cout<<"Public key is="<<p_pub<<endl;
	return 0;
}