#include "argcvse/util.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>



static unsigned long crypt[0x500];
static bool is_crypted = false;

void argcvse::util::crypt_init()  
{
	if(is_crypted) return ;
	else is_crypted = true;
	unsigned long seed = 0x00100001, idx_a = 0, idx_b = 0, i;  

	for( idx_a = 0; idx_a < 0x100; idx_a++ )  
	{   
		for( idx_b = idx_a, i = 0; i < 5; i++, idx_b += 0x100 )  
		{   
			unsigned long t1, t2;  
			seed = (seed * 125 + 3) % 0x2AAAAB;  
			t1 = (seed & 0xFFFF) << 0x10;  
			seed = (seed * 125 + 3) % 0x2AAAAB;  
			t2 = (seed & 0xFFFF);  
			crypt[idx_b] = ( t1 | t2 );   
		}   
	}   
}

unsigned long argcvse::util::hash(const char * k, unsigned long offset)
{
	unsigned long seed_a = 0x7FED7FED, seed_b = 0xEEEEEEEE;  
	unsigned long ch;
	while(*k != 0)  
	{
		ch = (*k++);
		//ch = toupper(*k++);
		seed_a = crypt[(offset << 8) + ch] ^ (seed_a + seed_b);  
		seed_b = ch + seed_a + seed_b + (seed_b << 5) + 3;   
	}  
	return seed_a;
}

unsigned long argcvse::util::hash(const std::string & k, unsigned long offset)
{
	return argcvse::util::hash(k.c_str(),offset);
}

std::vector<std::string> &argcvse::util::split(const std::string &s,const std::string & delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	if (delim.empty()) {
		elems.push_back(s);
		return elems;
	}
	std::string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		std::string temp(substart, subend);
		//if (keep_empty || !temp.empty()) {
		if (!temp.empty()) {
			elems.push_back(temp);
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return elems;
}

std::vector<std::string> argcvse::util::split(const std::string &s,const std::string & delim) 
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}





