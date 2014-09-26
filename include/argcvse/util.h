#ifndef __ARGCV_SE_UTIL_H__
#define __ARGCV_SE_UTIL_H__

#include <string>
#include <vector>

#include <cmath>

#include <ctime>

#include <sstream>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/cache.h"

namespace argcvse
{
	namespace util
	{



		// for hash
		void crypt_init(); // for hash key generator
		unsigned long hash(const char * k, unsigned long offset);
		unsigned long hash(const std::string & k, unsigned long offset);

		// tf-idf 
		// assume : k in document D
		// stid : size of term k in  D
		// atsid :  all term size in D 
		// ads : all document size
		// dscct : document size contains current term
		inline double tf_idf(size_t stid,size_t atsid,size_t ads,size_t dscct)
		{
			//#define MATH_LG_10 2.302585
			// tf * idf
			if(ads == 0 || atsid == 0 || dscct == 0) return 0;
			return ((double) stid / atsid) * log((double) ads / (dscct))/2.302585; 
		}

		inline static std::string random_str(const int len) {
			static const char alphanum[] =
				"0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						"abcdefghijklmnopqrstuvwxyz";

			std::stringstream ss;
			for (int i = 0; i < len; ++i) {
				ss << alphanum[rand() % (sizeof(alphanum) - 1)] ;
			}
			return ss.str();
		}


		std::vector<std::string> &split(const std::string &s,const std::string & delim, std::vector<std::string> &elems);
		std::vector<std::string> split(const std::string &s,const std::string & delim);

		// web dict 
		class Dict
		{
			public :
			static Dict & instance()
			{
				static Dict dict;
				return dict;
			}
			virtual ~ Dict();
			//static Dict & instance(const std::string & dict_path);
			bool build(const std::string & file_path);
			int level(const std::string & key);
			long freq(const std::string & key); 
			int level(const std::string & key,bool & status);
			long freq(const std::string & key,bool & status);
			long operator [](const std::string & key)
			{
				return freq(key);
			}
			private :
			Dict ();// Private constructor
			Dict (const Dict &);// Prevent copy-construction
			Dict&  operator=(const Dict &);// Prevent assignment
			//const std::string m_dict_path;
			leveldb::DB* db;
			leveldb::Options options;
		};


	}
}

#endif // __ARGCV_SE_UTIL_H__

