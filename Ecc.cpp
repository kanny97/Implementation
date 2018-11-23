#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

void RandomInit(gmp_randstate_t st)
{
	unsigned long seed;
	seed= time(NULL);
	gmp_randinit_mt(st);
	gmp_randseed_ui(st,seed);
	return;
}

void calculateSlope(mpz_t xp,mpz_t yp,mpz_t xq,mpz_t yq,mpz_t slope,mpz_t P,mpz_t a)
{
	mpz_t temp1,temp2,rem,den,num;
	mpz_inits(temp1,temp2,rem,den,num,NULL);
	if(mpz_cmp(xp,xq)==0&&mpz_cmp(yp,yq)==0)//if they are the same point P==Q
	{
		mpz_mul(temp1,xp,xp);
		mpz_mul_ui(temp1,temp1,3);
		mpz_add(temp1,temp1,a);
		mpz_mul_ui(temp2,yp,2);	
	}
	else //P!=Q
	{
		mpz_sub(temp1,yq,yp);
		mpz_sub(temp2,xq,xp);
	}
	if(mpz_cmp_ui(temp1,0)<0)//if it is neagtive
		mpz_add(temp1,temp1,P);
	if(mpz_cmp_ui(temp2,0)<0)//if it is neagtive
		mpz_add(temp2,temp2,P);
	mpz_fdiv_qr(slope,rem,temp1,temp2);
		if(mpz_cmp_ui(rem,0)==0)
		{
			mpz_fdiv_r(slope,slope,P);
		}
	//temp1*temp2 inverse in field
		else
		{	
		int x=mpz_invert(den,temp2,P);
		if(x!=0)
		{
			mpz_mul(num,den,temp1);
			mpz_fdiv_r(slope,num,P);
		}
		}
}

void findSum(mpz_t xp,mpz_t yp,mpz_t xq,mpz_t yq,mpz_t P,mpz_t a,mpz_t xr,mpz_t yr)
{
	mpz_t temp1,temp2,slope;
	mpz_inits(temp1,temp2,slope,NULL);
	calculateSlope(xp,yp,xq,yq,slope,P,a);
	//calculating xr
	mpz_mul(temp1,slope,slope);
	mpz_sub(temp1,temp1,xp);
	mpz_sub(temp1,temp1,xq);
	if(mpz_cmp_ui(temp1,0)<0)//if it is neagtive
		mpz_add(temp1,temp1,P);
	mpz_fdiv_r(xr,temp1,P);

	//calculating yr
	mpz_sub(temp2,xp,xr);
	mpz_mul(temp2,temp2,slope);
	mpz_sub(temp2,temp2,yp);
	if(mpz_cmp_ui(temp2,0)<0)//if it is neagtive
		mpz_add(temp2,temp2,P);
	mpz_fdiv_r(yr,temp2,P);
}

void scalarMultiply(mpz_t k,mpz_t xp,mpz_t yp,mpz_t xr,mpz_t yr,mpz_t P,mpz_t a)//finding kP
{
	if(mpz_cmp_ui(k,1)==0)
	{
		mpz_set(xr,xp);
		mpz_set(yr,yp);
		return;
	}
	else
	{
		mpz_t tempx,tempy,ktemp;
		mpz_inits(tempx,tempy,ktemp,NULL);
		mpz_sub_ui(k,k,1);
		scalarMultiply(k,xp,yp,tempx,tempy,P,a);//this is (k-1)P
		findSum(xp,yp,tempx,tempy,P,a,xr,yr);//kP=(k-1)P+P
		return;
	}
}

void scalarMultiplys(mpz_t k,mpz_t xp,mpz_t yp,mpz_t xr,mpz_t yr,mpz_t P,mpz_t a)//finding kP
{
		mpz_t tempx,tempy,tempcounter,counter;
		mpz_inits(tempx,tempy,counter,tempcounter,NULL);
		mpz_set(tempx,xp);
		mpz_set(tempy,yp);
		mpz_set_ui(counter,1);
	while(mpz_cmp(counter,k)<0)
	{
		findSum(tempx,tempy,tempx,tempy,P,a,xr,yr);
		mpz_set(tempx,xr);
		mpz_set(tempy,yr);
		mpz_mul_ui(counter,counter,2);
		mpz_mul_ui(tempcounter,counter,2);
		if(mpz_cmp(tempcounter,k)>0)
			break;
	}
	if(mpz_cmp(counter,k)<0)//odd k
	{
		findSum(tempx,tempy,xp,yp,P,a,xr,yr);
		return;
	}
	else
		return;
}

string convertPointToString(mpz_t xr,mpz_t yr)
{
	cout<<"Conversion1"<<endl;
	char *temp1,*temp2,*str1,*str2;
	temp1=mpz_get_str(str1,16,xr);
	cout<<temp1<<endl;
	cout<<"Conversion2"<<endl;
	temp2=mpz_get_str(str2,16,yr);
	cout<<temp2<<endl;
	cout<<"Conversion3"<<endl;
	cout<<temp1<<" "<<temp2<<endl;
	string s1(temp1),s2(temp2);
	string s3=s1+s2;
	return s3;
}

bool checkValidPoint(mpz_t a,mpz_t b,mpz_t p,mpz_t x,mpz_t y)//to check whether the point lies on the curve
{
	mpz_t lhs,rhs,temp1,temp2;
	mpz_inits(lhs,rhs,temp1,temp2,NULL);
	mpz_powm_ui(lhs,y,2,p);
	mpz_pow_ui(temp1,x,3);
	mpz_mul(temp2,a,x);
	mpz_add(rhs,temp1,temp2);
	mpz_add(rhs,rhs,b);
	mpz_fdiv_r(rhs,rhs,p);
	if(mpz_cmp(lhs,rhs)==0)
		return true;
	else
		return false;
}

void initialiseParameters(mpz_t basex,mpz_t basey,mpz_t a,mpz_t b,mpz_t P)
{
	//secp256k1 curve
	int x=mpz_set_str (P,"FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F",16);
	mpz_set_ui(a,0);
	mpz_set_ui(b,7);
	x=mpz_set_str(basex,"79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798",16);
	x=mpz_set_str(basey,"483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8",16);
	return;
}

string getCipher(mpz_t k)
{
	mpz_t basex,basey,a,b,P,xr,yr;
	mpz_inits(basex,basey,a,b,P,xr,yr,NULL);
	initialiseParameters(basex,basey,a,b,P);
	//now the curve parameters are initialised
	// scalarMultiply(k,basex,basey,xr,yr,P,a);
	// gmp_printf("XR=%Zd\n",xr);
	// gmp_printf("YR=%Zd\n",yr);
	// if(checkValidPoint(a,b,P,xr,yr))
	// 	cout<<"Valid base point"<<endl;
	// else
	// 	cout<<"Recheck base point"<<endl;
	// mpz_set_ui(xr,0);
	// mpz_set_ui(yr,0);
	scalarMultiplys(k,basex,basey,xr,yr,P,a);
	gmp_printf("XR=%Zd\n",xr);
	gmp_printf("YR=%Zd\n",yr);	
	if(checkValidPoint(a,b,P,xr,yr))
		cout<<"Valid base point"<<endl;
	else
		cout<<"Recheck base point"<<endl;
	return "23";	
}





