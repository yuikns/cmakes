#include "argcvse/util.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>

#include "leveldb/db.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"

#ifndef __ARGCVSE_UTIL_DICT_DB_PREFIX__
#define __ARGCVSE_UTIL_DICT_DB_PREFIX__ "argcvse::dict::freq::"
#endif // __ARGCVSE_UTIL_DICT_DB_PREFIX__


#ifndef __ARGCVSE_UTIL_DICT_DB_PATH__
#define __ARGCVSE_UTIL_DICT_DB_PATH__ "dict.db"
#endif // __ARGCVSE_UTIL_DICT_DB_PATH__

namespace argcvse
{
	namespace util
	{

		Dict::Dict()
		{
			std::cout << "dict start" << std::endl;
			// db init
			options.create_if_missing = true; // 如果没有则创建
			options.block_cache = leveldb::NewLRUCache(30 * 1048576L);  // 1000MB cache

			//leveldb::Status status = 
			leveldb::DB::Open(options,__ARGCVSE_UTIL_DICT_DB_PATH__, &db);
			//assert(status.ok());
		}

		Dict::~Dict()
		{
			std::cout << "dict destory" << std::endl;
			delete db;
			delete options.block_cache;
		}


		bool Dict::build(const std::string & file_path)
		{
			std::string line;
			std::ifstream doc (file_path);
			size_t off = 0;
			if (doc.is_open())
			{
				while ( getline (doc,line) )
				{
					std::vector<std::string> kv = argcvse::util::split(line," ");
					if(kv.size() == 2)
					{
						off ++ ;
						//std::cout << "[" << kv[0] << "]\t#\t[" << kv[1] << "]" <<std::endl;
						//db->Put(leveldb::WriteOptions(),kv[0],kv[1]);
						if((db->Put(leveldb::WriteOptions(), __ARGCVSE_UTIL_DICT_DB_PREFIX__+kv[0],kv[1])).ok())
						{
							//std::cout << "put ok" << std::endl;
						}else
						{
							std::cerr << "insert failed : " << kv[0] << "\t#\t" << kv[1] << std::endl;
						}
						if(off % 10000 == 0)
						{
							std::cout << std::to_string(off) << " ..." << std::endl;
						}
					}else
					{
						std::cerr<< "error line : " << line << std::endl;
					}
				}
				doc.close();
			}else 
			{
				//std::cerr << "Unable to open file" << std::endl; 
				return false;
			}
			return true;
		}

		int Dict::level(const std::string & key)
		{
			std::string s_val;
			if(!(db->Get(leveldb::ReadOptions(), __ARGCVSE_UTIL_DICT_DB_PREFIX__+key, &s_val)).ok())
			{
				//std::cout << "key : " << line << " val : " << " N/A" << std::endl;
				return -1;
			}else {
				return s_val.length();
			}
	
		}

		long Dict::freq(const std::string & key)
		{
			std::string s_val;
			if(!(db->Get(leveldb::ReadOptions(),__ARGCVSE_UTIL_DICT_DB_PREFIX__+key, &s_val)).ok())
			{
				//std::cout << "key : " << line << " val : " << " N/A" << std::endl;
				return -1;
			}else {
				//std::stoi(s_val)//std::stol(long), std::stof(float), std::stod(double)
				return std::stol(s_val);
			}
		}

		int Dict::level(const std::string & key,bool & status)
		{
			std::string s_val;
			if(!(db->Get(leveldb::ReadOptions(), __ARGCVSE_UTIL_DICT_DB_PREFIX__+key, &s_val)).ok())
			{
				//std::cout << "key : " << line << " val : " << " N/A" << std::endl;
				status = false;
				return -1;
			}else {
				status = true;
				return s_val.length();
			}
		}

		long Dict::freq(const std::string & key,bool & status)
		{
			std::string s_val;
			if(!(db->Get(leveldb::ReadOptions(), __ARGCVSE_UTIL_DICT_DB_PREFIX__+key, &s_val)).ok())
			{
				//std::cout << "key : " << line << " val : " << " N/A" << std::endl;
				status = false;
				return -1;
			}else {
				//std::stoi(s_val)//std::stol(long), std::stof(float), std::stod(double)
				status = true;
				return std::stol(s_val);
			}
		}

	}
}


