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

string hash(string message)
{
	string l_m;
	SHA512 sha512;
	HashFilter f1(sha512, new HexEncoder(new StringSink(l_m)));
	ChannelSwitch cs;
	cs.AddDefaultRoute(f1);
	StringSource ss(message, true /*pumpAll*/, new Redirector(cs));
	return l_m;
}

mpz_t r_i,IDm,R_m,f_m,r_m,X_m,V_m,PIDm,l_m;
string strr_i, strIDm, strl_m, strPIDm, strV_m,strX_m;
string Tm1 = "00005",Ts1;



string sendRequest()
{
	int base = 16;
	mpz_inits(r_i,IDm,NULL);
	gmp_randstate_t st;
	RandomInit(st);

	mpz_urandomm(r_i,st,q);
	mpz_urandomm(IDm,st,q);

	string message;
	strr_i = largeIntToStr(r_i,base);
	strIDm = largeIntToStr(IDm,base);

	message = strIDm+strr_i;
	strl_m = hash(message);

	string M1;
	M1 = strIDm+"#"+strl_m+"#";
	
	return M1;
}

void verifyResponse(string M2)
{
	int base = 16;
	mpz_t check,temp;
	string strf_m,strR_m,message;
	mpz_inits(check,R_m,f_m,temp,l_m,NULL);
	
	//Get R_m and l_m from the message.
	stringstream tokenizer(M2);
	getline(tokenizer,strR_m,'#');
	getline(tokenizer,strf_m,'#');

	//conversion to large integers.
	mpz_set_str(f_m,strf_m.c_str(),base);
	mpz_set_str(R_m,strR_m.c_str(),base);


	mpz_set_str(check,getCipher(f_m).c_str(),base);
	strl_m = largeIntToStr(l_m,base);
	message = strIDm+strR_m+strl_m;
	message = hash(message);

	mpz_set_str(temp,message.c_str(),base);
	string tempstr = getCipher(temp);
	mpz_set_str(temp,tempstr.c_str(),base);
	mpz_add(temp,temp,R_m);

	if(mpz_cmp(temp,check)!=0)
	{
		cout<<"\nInvalid Reply Recieved from server Aborting connection.\n";
		exit(0);
	}

	cout<<"\nVaild Reply recieved from server. Saving R_m , f_m, l_m.\n";
	// save R_m, f_m, l_m,
}


string initAuthentication()
{
	int base = 16;
	mpz_inits(r_m,X_m,V_m,NULL);
	gmp_randstate_t st;
	RandomInit(st);

	//Select r_m.
	mpz_urandomm(r_m,st,q);

	mpz_set_str(X_m,getCipher(r_m).c_str(),base);
	mpz_set_str(V_m,getCipher(r_m).c_str(),base);   //Change this for multiplation with P_pub and not base point.

	string tempstr;
	tempstr = largeIntToStr(X_m,base) ;
	tempstr+=Tm1;
	tempstr = hash(tempstr);

	strPIDm = calcXor(strIDm , tempstr);

	string M1;
	strV_m = largeIntToStr(V_m,base);
	strX_m = largeIntToStr(X_m,base);
	M1 = strPIDm +"#" + strV_m+ "#"+strX_m+"#";
	return M1;

}



string verifyAuthentication(string M2)
{
	int base = 16;
	mpz_inits(NULL);
	gmp_randstate_t st;
	RandomInit(st);	

	string strR_s, Aut;

	stringstream tokenizer(M2);
	getline(tokenizer,Aut,'#');
	getline(tokenizer,strR_s,'#');
	getline(tokenizer,Ts1,'#');

	string Aut1 = strIDm + Tm1 + Ts1 + strX_m + strR_s + strl_m;
	Aut1 = hash(Aut1);

	if(Aut1 !=Aut)
	{
		cout<<"\n Authentication Failed. Connection Not secure. Aborting!!!!\n";
		exit(0);
	}

	cout<<"\nAuthentication successful!!!!\n";

	string K,temp;
	temp = getCipher(r_m);  //Multiply with R_s.
	temp+=strl_m;

	K = hash(K);
	cout<<"K is = "<<K<<endl;
	return K;
}


int main()
{
	return 0;
}