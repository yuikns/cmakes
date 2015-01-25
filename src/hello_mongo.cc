//#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include <boost/scoped_ptr.hpp>

#include "mongo/client/dbclient.h"
#include "inih/INIReader.h"

using namespace mongo;

int main(int argc, char * argv[]) {
    INIReader reader("config.ini");
    
    const char *host = reader.Get("mongodb", "host", "127.0.0.1").c_str();
    const char *port = reader.Get("mongodb", "port", "27017").c_str();
    const char *db = reader.Get("mongodb", "db", "admin").c_str();
    int auth = reader.GetInteger("mongodb", "auth", 0);
    const char *username = reader.Get("mongodb", "username", "").c_str();
    const char *password = reader.Get("mongodb", "password", "").c_str();

    Status status = client::initialize();
    if (!status.isOK()) {
        fprintf(stderr, "failed to initialize the client driver: %s \n", status.toString().c_str());
        return EXIT_FAILURE;
    }else{
        fprintf(stdout, "inited \n");
	}

    std::string errmsg;
    ConnectionString cs = ConnectionString::parse(std::string(host) + ":" + port, errmsg);
    if (!cs.isValid()) {
        fprintf(stderr, "error parsing url: %s\n",errmsg.c_str());
        return EXIT_FAILURE;
    }else{
        fprintf(stdout,"url parsed ..\n");
	}

    boost::scoped_ptr<DBClientBase> conn(cs.connect(errmsg));
    if (!conn) {
        fprintf(stderr, "connect failed : %s \n" , errmsg.c_str());
        return EXIT_FAILURE;
    } else {
        fprintf(stdout, "connected \n");
	}

    BSONObj ret;    
    if(auth) {
        try {
            conn->auth(BSON("user" << username <<
                            "db" << db <<
                            "pwd" << password <<
                            "mechanism" << "MONGODB-CR"));
        } catch (const DBException& e) {
            // expected
            fprintf(stderr, "auth failed :: %s \n" , e.toString().c_str());
            return -1;
        }
        //std::cout << "authorized .." << std::endl;
        fprintf(stdout, "authorized\n");
    } else {
        fprintf(stdout, "no need to auth\n");
    }

    std::auto_ptr<DBClientCursor> cursor = conn->query("local.me", BSONObj());

    if (!cursor.get()) {
        cout << "query failure" << endl;
    }

    int offset = 0 ;
    while( cursor->more() && offset < 10 ) {
        BSONObj p = cursor->next();
        fprintf(stdout, "[firstElementFieldName]%s\t[_id_elem]%s\t[_id_val]%s\t[host]%s\n",
                    p.firstElementFieldName(),
                    std::string(p["_id"]).c_str(),
                    p["_id"].OID().toString().c_str(),
                    p.getStringField("host")
                    );
        offset ++ ;
    }

    return EXIT_SUCCESS;
}
