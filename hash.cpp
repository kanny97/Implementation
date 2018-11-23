#include<bits/stdc++.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/channels.h> 
#include <cryptopp/filters.h> 
#include<string.h>
using namespace CryptoPP;
using std::cout;
using std::endl;
using std::string;
int main()
{

string message = "Now is the time for all good men to come to the aide of their country";


string s1, s2, s3, s4;
SHA1 sha1; SHA224 sha224; SHA256 sha256; SHA512 sha512;

HashFilter f1(sha1, new HexEncoder(new StringSink(s1)));
HashFilter f2(sha224, new HexEncoder(new StringSink(s2)));
HashFilter f3(sha256, new HexEncoder(new StringSink(s3)));
HashFilter f4(sha512, new HexEncoder(new StringSink(s4)));

ChannelSwitch cs;
cs.AddDefaultRoute(f1);
cs.AddDefaultRoute(f2);
cs.AddDefaultRoute(f3);
cs.AddDefaultRoute(f4);

StringSource ss(message, true /*pumpAll*/, new Redirector(cs));

cout <<" Message: " << message << endl;
cout << "SHA-1: " << s1 <<"  len = "<<s1.length()<< endl;
cout << "SHA-224: " << s2<<"  len = "<<s2.length() << endl;
cout << "SHA-256: " << s3 <<"  len = "<<s3.length() << endl;
cout << "SHA-512: " << s4 <<"  len = "<<s4.length()<< endl;
}	
