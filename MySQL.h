/* mysql.h */

#ifndef MYSQL_H
#define MYSQL_H

#ifdef GODOT4
	#include "core/object/ref_counted.h"  //Subistituto para reference.h
	#include "core/core_bind.h"
#else
	#include "core/reference.h"
	#include "core/bind/core_bind.h"
	#include "core/crypto/crypto_core.h"
#endif



#include <vector>
#include <sstream>
#include "core/io/marshalls.h"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#include <cppconn/build_config.h>
#include <cppconn/config.h>
#include <cppconn/connection.h>
#include <cppconn/datatype.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/metadata.h>
#include <cppconn/parameter_metadata.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/statement.h>
#include <cppconn/sqlstring.h>
#include <cppconn/warning.h>
#include <cppconn/version_info.h>
#include <cppconn/variant.h>

#pragma once


class MySQL : public Reference{
	GDCLASS(MySQL,Reference);


protected:
	static void _bind_methods();


public:

	typedef Dictionary MySQLException;
	MySQLException sqlError;
	std::string sqlstate ;

	enum ConnectionStatus {
		NO_CONNECTION, 
		CLOSED, 
		CONNECTED, 
		DISCONNECTED
	};
	
	enum DataFormat {
		ARRAY,
		DICTIONARY
	};
	
	enum MetaCollection {	
		COLUMNS_NAMES,	// returns an array with only one element (names of columns)
		COLUMNS_TYPES,	// returns an array with only one element (types of the columns)
		METADATA,		// return a dictionary only with metadata 
		INFO			// return the fields info (is...)
	};



private:

	sql::ConnectOptionsMap connection_properties;
	sql::mysql::MySQL_Driver *driver;
	std::unique_ptr<sql::Connection> conn;


	const std::string void_properties [2] = {  //void
		"OPT_CONNECT_ATTR_RESET", 
		"OPT_NAMED_PIPE"
	};
	
	const std::string sqlmap_properties [2] = { //map
		"OPT_CONNECT_ATTR_ADD",
		"OPT_CONNECT_ATTR_DELETE",
	};
	
	const std::string int_properties [5] = { //int
		"port",
		"OPT_CONNECT_TIMEOUT",
		"OPT_LOCAL_INFILE",
		"OPT_WRITE_TIMEOUT",
		"OPT_READ_TIMEOUT",
	};
	
	const std::string bool_properties [17] = { //bool
		"OPT_RECONNECT",
		"CLIENT_COMPRESS",
		"CLIENT_FOUND_ROWS",
		"CLIENT_IGNORE_SIGPIPE",
		"CLIENT_IGNORE_SPACE",
		"CLIENT_INTERACTIVE",
		"CLIENT_LOCAL_FILES",
		"CLIENT_MULTI_STATEMENTS",
		"CLIENT_MULTI_STATEMENTS",
		"CLIENT_NO_SCHEMA",
		"OPT_REPORT_DATA_TRUNCATION",
		"OPT_CAN_HANDLE_EXPIRED_PASSWORDS",
		"OPT_ENABLE_CLEARTEXT_PLUGIN",
		"OPT_GET_SERVER_PUBLIC_KEY",
		"sslEnforce",
		"sslVerify",
		"useLegacyAuth"
	};
	
	const std::string string_properties [26] = { //string
		"hostName",
		"userName",
		"password",
		"schema",
		"pipe",
		"pluginDir",
		"postInit",
		"preInit",
		"readDefaultFile",
		"readDefaultGroup",
		"rsaKey",
		"socket",
		"sslCA",
		"sslCAPath",
		"sslCert",
		"sslCipher",
		"sslCRL",
		"sslCRLPath",
		"sslKey",
		"characterSetResults",
		"charsetDir",
		"defaultAuth",
		"defaultPreparedStatementResultType",
		"defaultStatementResultType",
		"OPT_TLS_VERSION",
		"OPT_CHARSET_NAME"
	};

	//QUERTY
	int _execute( String p_sqlquery, Array p_values, bool prep_st);

//	int set_datatype(std::shared_ptr <sql::PreparedStatement> prep_stmt, Array p_values, bool query );

//	int set_datatype( sql::SQLString query, Array p_values);


	void set_datatype(std::shared_ptr<sql::PreparedStatement> prep_stmt, std::stringstream * blob, Variant arg, int index);
	
	Array _query(String p_sqlquery, Array p_values = Array(), DataFormat data_model = DICTIONARY, bool return_string = false, PoolIntArray meta_col = PoolIntArray(), bool _prep = false);



	//ERRORS
	void print_SQLException(sql::SQLException &e);
	void print_runtime_error(std::runtime_error &e);


	//CHECKERS
	std::string	get_prop_type( std::string p_prop );
	bool is_json( Variant p_arg );
	bool is_mysql_time(String time);
	
	
	//HELPERS
	Array format_time(String str, bool return_string);
	int get_time_format(String time);
	
	String SQLstr2GDstrP( const sql::SQLString *p_str );
	String SQLstr2GDstrS( sql::SQLString &p_string );
//	String SQLstr2GDstr( sql::SQLString p_string );
	
	sql::SQLString GDstr2SQLstr(String &p_str); 



public:


	PoolByteArray test(PoolByteArray arg1, Array arg2);

	// CONNECTION
	ConnectionStatus get_connection_status();
	ConnectionStatus stop_connection();
	MySQLException start_connection();
	void set_credentials( String p_host, String p_user, String p_pass ); // For quickly connections
	
	
	// PROPERTIES
	Variant get_property(String p_property);
	void set_property(String p_property, Variant p_value);
	
	//Dictionary get_properties_kit(Array p_properties);
	//void set_properties_kit(Dictionary p_properties);


	// EXECUTE
	int execute(String p_sqlquery);
	int execute_prepared(String p_sqlquery, Array p_values);
	
	
	// QUERY
	Array query(String p_sqlquery, DataFormat data_model = DICTIONARY, bool return_string = false, PoolIntArray meta_col = PoolIntArray());  
	Array query_prepared(String p_sqlquery, Array prep_values = Array(), DataFormat data_model = DICTIONARY, bool return_string = false, PoolIntArray meta_col = PoolIntArray()); 


/*
	// TRANSACTION
	
	commit();
	rollback();
	getAutoCommit();
	setAutoCommit(bool autoCommit);
	getTransactionIsolation();

	//Variant transaction( [] );
	//Variant transaction_prepared( {} );


*/



/*
	std::string MySQL_Connection::getSessionVariable(const std::string & varname)
	void MySQL_Connection::setSessionVariable(const std::string & varname, const std::string & value)

*/



	MySQL();
	~MySQL();

};

VARIANT_ENUM_CAST(MySQL::DataFormat);
VARIANT_ENUM_CAST(MySQL::MetaCollection);
VARIANT_ENUM_CAST(MySQL::ConnectionStatus);


#endif	// MYSQL_H

//TODO:
	//Savepoint *savept;



	



