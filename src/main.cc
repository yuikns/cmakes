#include <cstdio>

// #include "crfpp.h" // crfpp
#include "config.h"
#include "argcvse/util.h"
#include <iostream>
#include <assert.h>
// class Dict
// {
// public :
// 	static Dict & instance()
// 	{
// 		static Dict dict;
// 		return dict;
// 	}
// 	virtual ~ Dict();
// 	//static Dict & instance(const std::string & dict_path);
// 	bool build(const std::string & file_path);
// 	int level(const std::string & key);
// 	long freq(const std::string & key); 
// 	int level(const std::string & key,bool & status);
// 	long freq(const std::string & key,bool & status);
// 	long operator [](const std::string & key)
// 	{
// 		return freq(key);
// 	}
// private :
// 	Dict ();// Private constructor
// 	Dict (const Dict &);// Prevent copy-construction
// 	Dict&  operator=(const Dict &);// Prevent assignment
// 	//const std::string m_dict_path;
// 	leveldb::DB* db;
// 	leveldb::Options options;
// };

int main(int argc, char *argv[])
{
	
	printf("cmake sample starting ... in  version : %d.%.2f , compiled by  %s @ %s (%s) %s \n",
		GLO_VERSION_MAJOR,GLO_VERSION_MINOR,
		CMAKE_CXX_COMPILER_ID,CMAKE_SYSTEM,CMAKE_SYSTEM_NAME,CMAKE_SYSTEM_PROCESSOR);
	printf("loading dict \n");
	argcvse::util::Dict &d = argcvse::util::Dict::instance();
	
	printf("loaded \n");
	char buff[100];
	
	printf ("input a chinese phrase , it will return a fequency .. \n");
	while(scanf("%s",buff)  >= 1 )
	{
		printf("%s %ld ~ %d \n",buff , d[buff], d.level(buff));
		printf ("input a chinese phrase , it will return a fequency .. \n");
	}
	return 0;
}

