//
//  NBSourceTranslator.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "NBSourceTranslator.h"


//------------------------
//-- Source relevance map
//------------------------

typedef struct STParserStrRelavanceMap_ {
	ENStringRelevance relevance;
	const char* name;
} STParserStrRelavanceMap;

static STParserStrRelavanceMap __parserStrRelevanceMap[] = {
	{ENStringRelevance_none, "" }
	, {ENStringRelevance_literal, "literal" }
	, {ENStringRelevance_comment, "comment" }
	, { ENStringRelevance_fileName, "file" }
	, { ENStringRelevance_folderName, "folder" }
	, {ENStringRelevance_memberName, "member" }
	, { ENStringRelevance_enumName, "enum" }
	, { ENStringRelevance_structName, "struct" }
	, { ENStringRelevance_className, "class" }
	, { ENStringRelevance_macro, "macro" }
};

static const char* Parser_strRelevanceName(const ENStringRelevance rel){
	const char* r = "";
	SI32 i; const SI32 count = (sizeof(__parserStrRelevanceMap) / sizeof(__parserStrRelevanceMap[0]));
	NBASSERT(count == ENStringRelevance_count)
	for(i = 0; i < count; i++){
		if(__parserStrRelevanceMap[i].relevance == rel){
			r = __parserStrRelevanceMap[i].name;
			break;
		}
	}
	return r;
}

static ENStringRelevance Parser_strRelevanceByName(const char* name){
	ENStringRelevance r = ENStringRelevance_none;
	SI32 i; const SI32 count = (sizeof(__parserStrRelevanceMap) / sizeof(__parserStrRelevanceMap[0]));
	NBASSERT(count == ENStringRelevance_count)
	for(i = 0; i < count; i++){
		if(AUCadena8::cadenasSonIguales(__parserStrRelevanceMap[i].name, name)){
			r = __parserStrRelevanceMap[i].relevance;
			break;
		}
	}
	return r;
}


//------------------------
//-- Source actions map
//------------------------

typedef struct STParserSrcActionMap_ {
	ENSrcAction action;
	const char* name;
} STParserSrcActionMap;

static STParserSrcActionMap __parserSrcActionMap[] = {
	{ENSrcAction_ignore, "ignore"}
	, {ENSrcAction_copy, "copy"}
	, {ENSrcAction_process, "process"}
};

const char* Parser_srcActionName(const ENSrcAction action){
	const char* r = "";
	SI32 i; const SI32 count = (sizeof(__parserSrcActionMap) / sizeof(__parserSrcActionMap[0]));
	NBASSERT(count == ENSrcAction_count)
	for(i = 0; i < count; i++){
		if(__parserSrcActionMap[i].action == action){
			r = __parserSrcActionMap[i].name;
			break;
		}
	}
	return r;
}

ENSrcAction Parser_srcActionByName(const char* name){
	ENSrcAction r = ENSrcAction_count;
	SI32 i; const SI32 count = (sizeof(__parserSrcActionMap) / sizeof(__parserSrcActionMap[0]));
	NBASSERT(count == ENSrcAction_count)
	for(i = 0; i < count; i++){
		if(AUCadena8::cadenasSonIguales(__parserSrcActionMap[i].name, name)){
			r = __parserSrcActionMap[i].action;
			break;
		}
	}
	return r;
}


//------------------------
//-- Source formats map
//------------------------

//Type of case sensitivity on a file's format
typedef enum ENFileFormatCase_ {
	ENFileFormatCase_Sensitive = 0,
	ENFileFormatCase_NoSensitive
}ENFileFormatCase;

//Start and end of a literal text (by example: "this is literal", 'this is literal')
typedef struct STLiteralLimits_ {
	char	start[2];
	char	end[2];
} STLiteralLimits;

//Scape of a literal limit in the literal conteo (by example '\"' for ["this \"is\" scaped"] or 'this [''is'' scaped'])
typedef struct STLiteralLimitScape_ {
	char	scape[3];
} STLiteralLimitScape;

//Start of a monoline commment (by example: '//' (c), '#' (mysql), '--' (mysql))
typedef struct STCommMonoLimits_ {
	char	start[3];
} STCommMonoLimits;

//Start and end of a multiline comment (by example: "/*" and "*/")
typedef struct STCommMultiLimits_ {
	char	start[3];
	char	end[3];
} STCommMultiLimits;

//
typedef struct STFileFormatMap_ {
	ENSrcFileFormat	format;
	const char*		name;
	//
	ENFileFormatCase caseSensitivity;
	const char**	reservedWordsPlain;		//NULL terminated list (to simplify the configuration to this part of the file)
	const char**	reservedWordsPlain2;	//NULL terminated list (to simplify the configuration to this part of the file)
	const STCommMonoLimits* monolinesCommLimits; // {'\0', '\0', '\0'} terminated list
	const STCommMultiLimits* multilinesCommLimits; // { {'\0', '\0', '\0'}, {'\0', '\0', '\0'} } terminated list
	//
	STStringsPairs	reservedWords;
} STFileFormatMap;

const STCommMonoLimits __globalCommentsMonolineLimits_none[] = {
	{'\0', '\0', '\0'},
};

const STCommMonoLimits __globalCommentsMonolineLimits_c[] = {
	{'/', '/', '\0'},
	{'\0', '\0', '\0'},
};

const STCommMonoLimits __globalCommentsMonolineLimits_mysql[] = {
	{'#', '\0', '\0'},
	{'-', '-', '\0'},
	{'\0', '\0', '\0'},
};

//

const STCommMultiLimits __globalCommentsMultilineLimits_none[] = {
	{ {'\0', '\0', '\0'}, {'\0', '\0', '\0'} },
};

const STCommMultiLimits __globalCommentsMultilineLimits_c[] = {
	{ {'/', '*', '\0'}, {'*', '/', '\0'} },
	{ {'\0', '\0', '\0'}, {'\0', '\0', '\0'} }
};

//

const char* __globalReservedWords_none[] = {
	NULL
};

//http://www.tutorialspoint.com/ansi_c/c_reserved_keywords.htm
const char* __globalReservedWords_c[] = {
	"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
	"extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
	"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile",
	"while", "_Packed",
	NULL
};

//http://en.cppreference.com/w/cpp/keyword
const char* __globalReservedWords_cpp[] = {
	"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept",
	"auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class",
	"compl", "concept", "const", "constexpr", "const_cast", "continue", "decltype", "default", "delete",
	"do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for",
	"friend", "goto", "if", "inline", "int", "import", "long", "module", "mutable", "namespace", "new",
	"noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public",
	"register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
	"static_cast", "struct", "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try",
	"typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t",
	"while", "xor", "xor_eq", "override", "final", "transaction_safe", "transaction_safe_dynamic", "_Pragma", "if",
	"elif", "else", "endif", "defined", "ifdef", "ifndef", "define", "undef", "include", "line", "error", "pragma",
	NULL
};

const char* __globalReservedWords_objc[] = {
	"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed",
	"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool",
	"_Complex", "_Imaginery", "", "BOOL", "Class", "bycopy", "byref", "id", "IMP", "in", "inout", "nil", "NO", "NULL",
	"oneway", "out", "Protocol", "SEL", "self", "super", "YES", "interface", "end", "implementation", "protocol",
	"class", "public", "protected", "private", "property", "try", "throw", "catch()", "finally", "synthesize",
	"dynamic", "selector", "atomic", "nonatomic", "retain",
	NULL
};

//https://docs.oracle.com/javase/tutorial/java/nutsandbolts/_keywords.html
const char* __globalReservedWords_java[] = {
	"abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const", "continue",
	"default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if",
	"implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private",
	"protected", "public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this",
	"throw", "throws", "transient", "try", "void", "volatile", "while",
	NULL
};

//http://php.net/manual/en/reserved.php
const char* __globalReservedWords_php[] = {
	"__halt_compiler", "abstract", "and", "array", "as", "break", "callable", "case", "catch", "class", "clone",
	"const", "continue", "declare", "default", "die", "do", "echo", "else", "elseif", "empty", "enddeclare",
	"endfor", "endforeach", "endif", "endswitch", "endwhile", "eval", "exit", "extends", "final", "for", "foreach",
	"function", "global", "goto", "if", "implements", "include", "include_once", "instanceof", "insteadof", "interface",
	"isset", "list", "namespace", "new", "or", "print", "private", "protected", "public", "require", "require_once",
	"return", "static", "switch", "throw", "trait", "try", "unset", "use", "var", "while", "xor", "__CLASS__", "__DIR__",
	"__FILE__", "__FUNCTION__", "__LINE__", "__METHOD__", "__NAMESPACE__", "__TRAIT__", "Directory", "stdClass",
	"__PHP_Incomplete_Class", "Exception", "ErrorException", "php_user_filter", "Closure", "Generator", "ArithmeticError",
	"AssertionError", "DivisionByZeroError", "Error", "Throwable", "ParseError", "TypeError", "self", "static", "parent",
	"PHP_VERSION", "PHP_MAJOR_VERSION", "PHP_MINOR_VERSION", "PHP_RELEASE_VERSION", "PHP_VERSION_ID", "PHP_EXTRA_VERSION",
	"PHP_ZTS", "PHP_DEBUG", "PHP_MAXPATHLEN", "PHP_OS", "PHP_SAPI", "PHP_EOL", "PHP_INT_MAX", "PHP_INT_MIN", "PHP_INT_SIZE",
	"DEFAULT_INCLUDE_PATH", "PEAR_INSTALL_DIR", "PEAR_EXTENSION_DIR", "PHP_EXTENSION_DIR", "PHP_PREFIX", "PHP_BINDIR",
	"PHP_BINARY", "PHP_MANDIR", "PHP_LIBDIR", "PHP_DATADIR", "PHP_SYSCONFDIR", "PHP_LOCALSTATEDIR", "PHP_CONFIG_FILE_PATH",
	"PHP_CONFIG_FILE_SCAN_DIR", "PHP_SHLIB_SUFFIX", "PHP_FD_SETSIZE", "E_ERROR", "E_WARNING", "E_PARSE", "E_NOTICE",
	"E_CORE_ERROR", "E_CORE_WARNING", "E_COMPILE_ERROR", "E_COMPILE_WARNING", "E_USER_ERROR", "E_USER_WARNING",
	"E_USER_NOTICE", "E_DEPRECATED", "E_USER_DEPRECATED", "E_ALL", "E_STRICT", "__COMPILER_HALT_OFFSET__", "TRUE", "FALSE", "NULL",
	"int", "float", "bool", "string", "true", "false", "null", "resource", "object", "mixed", "numeric",
	NULL
};

//https://dev.mysql.com/doc/refman/5.7/en/keywords.html
const char* __globalReservedWords_mysql[] = {
	"ACCESSIBLE", "ADD", "AGGREGATE", "ALTER", "ANALYZE", "AS", "ASENSITIVE", "AUTO_INCREMENT", "BACKUP", "BETWEEN",
	"BINLOG", "BLOCK", "BOTH", "BYTE", "CASCADE", "CATALOG_NAME", "CHANGED", "CHARACTER", "CHECKSUM", "CLIENT", "CODE",
	"COLUMN", "COLUMN_NAME", "COMMITTED", "COMPRESSED", "CONDITION", "CONSTRAINT", "CONSTRAINT_SCHEMA", "CONTINUE",
	"CREATE", "CURRENT", "CURRENT_TIMESTAMP", "CURSOR_NAME", "DATABASES", "DATETIME", "DAY_MICROSECOND", "DEALLOCATE",
	"DECLARE", "DEFINER", "DELETE", "DES_KEY_FILE", "DIRECTORY", "DISK", "DIV", "DROP", "DUPLICATE", "ELSE", "ENCLOSED",
	"ENDS", "ENUM", "ESCAPE", "EVENTS", "EXECUTE", "EXPANSION", "EXPORT", "FALSE", "FETCH", "FILE_BLOCK_SIZE", "FIXED",
	"FLOAT8", "FOR", "FORMAT", "FULL", "GENERAL", "GEOMETRYCOLLECTION", "GLOBAL", "GROUP", "HASH", "HIGH_PRIORITY", "HOUR",
	"HOUR_SECOND", "IGNORE", "IN", "INFILE", "INOUT", "INSERT_METHOD", "INT", "INT3", "INTEGER", "INVOKER", "IO_BEFORE_GTIDS",
	"IS", "ITERATE", "KEY", "KILL", "LEADING", "LEFT", "LIKE", "LINES", "LOAD", "LOCALTIMESTAMP", "LOGFILE", "LONGBLOB",
	"LOW_PRIORITY", "MASTER_BIND", "MASTER_HEARTBEAT_PERIOD", "MASTER_LOG_POS", "MASTER_RETRY_COUNT", "MASTER_SSL_CA",
	"MASTER_SSL_CIPHER", "MASTER_SSL_KEY", "MASTER_USER", "MAX_CONNECTIONS_PER_HOUR", "MAX_SIZE", "MAX_USER_CONNECTIONS",
	"MEDIUMINT", "MERGE", "MIDDLEINT", "MINUTE_MICROSECOND", "MOD", "MODIFY", "MULTIPOINT", "MYSQL_ERRNO", "NATIONAL", "NDB",
	"NEW", "NODEGROUP", "NOT", "NULL", "NVARCHAR", "ON", "OPEN", "OPTION", "OR", "OUTER", "PACK_KEYS", "PARSE_GCOL_EXPR",
	"PARTITIONING", "PHASE", "PLUGIN_DIR", "PORT", "PREPARE", "PRIMARY", "PROCESSLIST", "PROXY", "QUERY", "READ", "READ_WRITE",
	"RECOVER", "REDUNDANT", "RELAY", "RELAY_LOG_POS", "RELOAD", "REORGANIZE", "REPEATABLE", "REPLICATE_DO_TABLE", "REPLICATE_REWRITE_DB",
	"REPLICATION", "RESIGNAL", "RESUME", "RETURNS", "RIGHT", "ROLLUP", "ROW", "ROW_FORMAT", "SCHEDULE", "SCHEMA_NAME",
	"SECURITY", "SEPARATOR", "SERVER", "SHARE", "SIGNAL", "SLAVE", "SNAPSHOT", "SONAME", "SPATIAL", "SQLEXCEPTION",
	"SQL_AFTER_GTIDS", "SQL_BIG_RESULT", "SQL_CALC_FOUND_ROWS", "SQL_THREAD", "SQL_TSI_MINUTE", "SQL_TSI_SECOND", "SSL",
	"STARTING", "STATS_PERSISTENT", "STOP", "STRAIGHT_JOIN", "SUBJECT", "SUPER", "SWITCHES", "TABLESPACE", "TEMPORARY", "TEXT",
	"TIME", "TIMESTAMPDIFF", "TINYTEXT", "TRANSACTION", "TRUE", "TYPES", "UNDO", "UNICODE", "UNIQUE", "UNSIGNED", "UPGRADE",
	"USER", "USING", "UTC_TIMESTAMP", "VALUES", "VARCHARACTER", "VIEW", "WARNINGS", "WHEN", "WITH", "WRAPPER", "XA", "XOR",
	"ZEROFILL", "ACCOUNT", "AFTER", "ALGORITHM", "ALWAYS", "AND", "ASC", "AT", "AVG", "BEFORE", "BIGINT", "BIT", "BOOL", "BTREE",
	"CACHE", "CASCADED", "CHAIN", "CHANNEL", "CHARSET", "CIPHER", "CLOSE", "COLLATE", "COLUMNS", "COMMENT", "COMPACT",
	"COMPRESSION", "CONNECTION", "CONSTRAINT_CATALOG", "CONTAINS", "CONVERT", "CROSS", "CURRENT_DATE", "CURRENT_USER",
	"DATA", "DATAFILE", "DAY", "DAY_MINUTE", "DEC", "DEFAULT", "DELAYED", "DESC", "DETERMINISTIC", "DISABLE", "DISTINCT",
	"DO", "DUAL", "DYNAMIC", "ELSEIF", "ENCRYPTION", "ENGINE", "ERROR", "ESCAPED", "EVERY", "EXISTS", "EXPIRE", "EXTENDED",
	"FAST", "FIELDS", "FILTER", "FLOAT", "FLUSH", "FORCE", "FOUND", "FULLTEXT", "GENERATED", "GET", "GRANT", "GROUP_REPLICATION",
	"HAVING", "HOST", "HOUR_MICROSECOND", "IDENTIFIED", "IGNORE_SERVER_IDS", "INDEX", "INITIAL_SIZE", "INSENSITIVE", "INSTALL",
	"INT1", "INT4", "INTERVAL", "IO", "IO_THREAD", "ISOLATION", "JOIN", "KEYS", "LANGUAGE", "LEAVE", "LESS", "LIMIT", "LINESTRING",
	"LOCAL", "LOCK", "LOGS", "LONGTEXT", "MASTER", "MASTER_CONNECT_RETRY", "MASTER_HOST", "MASTER_PASSWORD", "MASTER_SERVER_ID",
	"MASTER_SSL_CAPATH", "MASTER_SSL_CRL", "MASTER_SSL_VERIFY_SERVER_CERT", "MATCH", "MAX_QUERIES_PER_HOUR", "MAX_STATEMENT_TIME",
	"MEDIUM", "MEDIUMTEXT", "MESSAGE_TEXT", "MIGRATE", "MINUTE_SECOND", "MODE", "MONTH", "MULTIPOLYGON", "NAME", "NATURAL",
	"NDBCLUSTER", "NEXT", "NONBLOCKING", "NO_WAIT", "NUMBER", "OFFSET", "ONE", "OPTIMIZE", "OPTIONALLY", "ORDER", "OUTFILE",
	"PAGE", "PARTIAL", "PARTITIONS", "PLUGIN", "POINT", "PRECEDES", "PRESERVE", "PRIVILEGES", "PROFILE", "PURGE", "QUICK",
	"READS", "REAL", "REDOFILE", "REFERENCES", "RELAYLOG", "RELAY_THREAD", "REMOVE", "REPAIR", "REPLACE", "REPLICATE_IGNORE_DB",
	"REPLICATE_WILD_DO_TABLE", "REQUIRE", "RESTORE", "RETURN", "REVERSE", "RLIKE", "ROTATE", "ROWS", "RTREE", "SCHEMA", "SECOND",
	"SELECT", "SERIAL", "SESSION", "SHOW", "SIGNED", "SLOW", "SOCKET", "SOUNDS", "SPECIFIC", "SQLSTATE", "SQL_AFTER_MTS_GAPS",
	"SQL_BUFFER_RESULT", "SQL_NO_CACHE", "SQL_TSI_DAY", "SQL_TSI_MONTH", "SQL_TSI_WEEK", "STACKED", "STARTS", "STATS_SAMPLE_PAGES",
	"STORAGE", "STRING", "SUBPARTITION", "SUSPEND", "TABLE", "TABLE_CHECKSUM", "TEMPTABLE", "THAN", "TIMESTAMP", "TINYBLOB", "TO",
	"TRIGGER", "TRUNCATE", "UNCOMMITTED", "UNDOFILE", "UNINSTALL", "UNKNOWN", "UNTIL", "USAGE", "USER_RESOURCES", "UTC_DATE",
	"VALIDATION", "VARBINARY", "VARIABLES", "VIRTUAL", "WEEK", "WHERE", "WITHOUT", "WRITE", "XID", "YEAR", "ACTION", "AGAINST",
	"ALL", "ANALYSE", "ANY", "ASCII", "AUTOEXTEND_SIZE", "AVG_ROW_LENGTH", "BEGIN", "BINARY", "BLOB", "BOOLEAN", "BY", "CALL",
	"CASE", "CHANGE", "CHAR", "CHECK", "CLASS_ORIGIN", "COALESCE", "COLLATION", "COLUMN_FORMAT", "COMMIT", "COMPLETION", "CONCURRENT",
	"CONSISTENT", "CONSTRAINT_NAME", "CONTEXT", "CPU", "CUBE", "CURRENT_TIME", "CURSOR", "DATABASE", "DATE", "DAY_HOUR", "DAY_SECOND",
	"DECIMAL", "DEFAULT_AUTH", "DELAY_KEY_WRITE", "DESCRIBE", "DIAGNOSTICS", "DISCARD", "DISTINCTROW", "DOUBLE", "DUMPFILE", "EACH",
	"ENABLE", "END", "ENGINES", "ERRORS", "EVENT", "EXCHANGE", "EXIT", "EXPLAIN", "EXTENT_SIZE", "FAULTS", "FILE", "FIRST", "FLOAT4",
	"FOLLOWS", "FOREIGN", "FROM", "FUNCTION", "GEOMETRY", "GET_FORMAT", "GRANTS", "HANDLER", "HELP", "HOSTS", "HOUR_MINUTE", "IF",
	"IMPORT", "INDEXES", "INNER", "INSERT", "INSTANCE", "INT2", "INT8", "INTO", "IO_AFTER_GTIDS", "IPC", "ISSUER", "JSON",
	"KEY_BLOCK_SIZE", "LAST", "LEAVES", "LEVEL", "LINEAR", "LIST", "LOCALTIME", "LOCKS", "LONG", "LOOP", "MASTER_AUTO_POSITION",
	"MASTER_DELAY", "MASTER_LOG_FILE", "MASTER_PORT", "MASTER_SSL", "MASTER_SSL_CERT", "MASTER_SSL_CRLPATH", "MASTER_TLS_VERSION",
	"MAXVALUE", "MAX_ROWS", "MAX_UPDATES_PER_HOUR", "MEDIUMBLOB", "MEMORY", "MICROSECOND", "MINUTE", "MIN_ROWS", "MODIFIES", "MULTILINESTRING",
	"MUTEX", "NAMES", "NCHAR", "NEVER", "NO", "NONE", "NO_WRITE_TO_BINLOG", "NUMERIC", "OLD_PASSWORD", "ONLY", "OPTIMIZER_COSTS", "OPTIONS",
	"OUT", "OWNER", "PARSER", "PARTITION", "PASSWORD", "PLUGINS", "POLYGON", "PRECISION", "PREV", "PROCEDURE", "PROFILES", "QUARTER", "RANGE",
	"READ_ONLY", "REBUILD", "REDO_BUFFER_SIZE", "REGEXP", "RELAY_LOG_FILE", "RELEASE", "RENAME", "REPEAT", "REPLICATE_DO_DB",
	"REPLICATE_IGNORE_TABLE", "REPLICATE_WILD_IGNORE_TABLE", "RESET", "RESTRICT", "RETURNED_SQLSTATE", "REVOKE", "ROLLBACK", "ROUTINE",
	"ROW_COUNT", "SAVEPOINT", "SCHEMAS", "SECOND_MICROSECOND", "SENSITIVE", "SERIALIZABLE", "SET", "SHUTDOWN", "SIMPLE", "SMALLINT", "SOME",
	"SOURCE", "SQL", "SQLWARNING", "SQL_BEFORE_GTIDS", "SQL_CACHE", "SQL_SMALL_RESULT", "SQL_TSI_HOUR", "SQL_TSI_QUARTER", "SQL_TSI_YEAR",
	"START", "STATS_AUTO_RECALC", "STATUS", "STORED", "SUBCLASS_ORIGIN", "SUBPARTITIONS", "SWAPS", "TABLES", "TABLE_NAME", "TERMINATED",
	"THEN", "TIMESTAMPADD", "TINYINT", "TRAILING", "TRIGGERS", "TYPE", "UNDEFINED", "UNDO_BUFFER_SIZE", "UNION", "UNLOCK", "UPDATE", "USE",
	"USE_FRM", "UTC_TIME", "VALUE", "VARCHAR", "VARYING", "WAIT", "WEIGHT_STRING", "WHILE", "WORK", "X509", "XML", "YEAR_MONTH",
	NULL
};

static STFileFormatMap __parserSrcFormatMap[] = {
	{ ENSrcFileFormat_plain, "plain", ENFileFormatCase_Sensitive, __globalReservedWords_none, __globalReservedWords_none, __globalCommentsMonolineLimits_none, __globalCommentsMultilineLimits_none },
	{ ENSrcFileFormat_c, "c", ENFileFormatCase_Sensitive, __globalReservedWords_c, __globalReservedWords_none, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_cpp, "c++", ENFileFormatCase_Sensitive, __globalReservedWords_cpp, __globalReservedWords_none, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_objc, "objc", ENFileFormatCase_Sensitive, __globalReservedWords_c, __globalReservedWords_objc, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_objcpp, "objc++", ENFileFormatCase_Sensitive, __globalReservedWords_cpp, __globalReservedWords_objc, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_php, "php", ENFileFormatCase_Sensitive, __globalReservedWords_php, __globalReservedWords_none, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_java, "java", ENFileFormatCase_Sensitive, __globalReservedWords_java, __globalReservedWords_none, __globalCommentsMonolineLimits_c, __globalCommentsMultilineLimits_c },
	{ ENSrcFileFormat_mysql, "mysql", ENFileFormatCase_NoSensitive, __globalReservedWords_mysql, __globalReservedWords_none, __globalCommentsMonolineLimits_mysql, __globalCommentsMultilineLimits_c },
};

const char* Parser_srcFormatName(const ENSrcFileFormat format){
	const char* r = "";
	SI32 i; const SI32 count = (sizeof(__parserSrcFormatMap) / sizeof(__parserSrcFormatMap[0]));
	NBASSERT(count == ENSrcAction_count)
	for(i = 0; i < count; i++){
		if(__parserSrcFormatMap[i].format == format){
			r = __parserSrcFormatMap[i].name;
			break;
		}
	}
	return r;
}

ENSrcFileFormat Parser_srcFormatByName(const char* name){
	ENSrcFileFormat r = ENSrcFileFormat_count;
	SI32 i; const SI32 count = (sizeof(__parserSrcFormatMap) / sizeof(__parserSrcFormatMap[0]));
	NBASSERT(count == ENSrcFileFormat_count)
	for(i = 0; i < count; i++){
		if(AUCadena8::cadenasSonIguales(__parserSrcFormatMap[i].name, name)){
			r = __parserSrcFormatMap[i].format;
			break;
		}
	}
	return r;
}


//------------------------
//-- Strings pairs
//------------------------

void ItmCollectionStats_init(STItmCollectionStats* obj){
	obj->maxRelevance		= ENStringRelevance_none;
	obj->timesFound			= 0;	//times found
	obj->firstApprRefStart	= 0;	//first file, index of first char at 'firstsAppareancesRefs' string
	obj->lastApprRefStart	= 0;	//last file, index of first char at 'lastsAppareancesRefs' string
}

void ItmCollectionStats_finalize(STItmCollectionStats* obj){
	obj->maxRelevance		= ENStringRelevance_none;
	obj->timesFound			= 0;		//times found
	obj->firstApprRefStart	= 0;	//first file, index of first char at 'firstsAppareancesRefs' string
	obj->lastApprRefStart	= 0;	//last file, index of first char at 'lastsAppareancesRefs' string
}

void StringsPairs_init(STStringsPairs* obj){
	obj->pairs				= new AUArregloNativoMutableP<STStringPair>();
	obj->strings			= new AUCadenaLargaMutable8();
	//First string is zero-sized
	STStringPair first;
	first.iOrig				= 0;
	first.iNew				= 0;
	first.ignore			= true;
	first.isPathWord		= false;
	//
	ItmCollectionStats_init(&first.collStats);
	//
	obj->pairs->agregarElemento(first);
	obj->strings->agregar('\0');
	//
	obj->_srchIndexByOrig	= new(ENMemoriaTipo_General) AUArregloNativoOrdenadoMutableP<STStringSrchHardIndex>();
	//
	obj->firstsAppareancesRefsLastRefStart = 0;
	obj->firstsAppareancesRefs = new(ENMemoriaTipo_General) AUCadenaLargaMutable8();
	obj->firstsAppareancesRefs->agregar('\0'); //first index must be empty-string
	//
	obj->lastsAppareancesRefsLastRefStart = 0;
	obj->lastsAppareancesRefs = new(ENMemoriaTipo_General) AUCadenaLargaMutable8();
	obj->lastsAppareancesRefs->agregar('\0'); //first index must be empty-string
}

void StringsPairs_finalize(STStringsPairs* obj){
	if(obj->_srchIndexByOrig != NULL) obj->_srchIndexByOrig->liberar(); obj->_srchIndexByOrig = NULL;
	//
	obj->pairs->liberar(); obj->pairs = NULL;
	obj->strings->liberar(); obj->strings = NULL;
	//
	if(obj->firstsAppareancesRefs != NULL) obj->firstsAppareancesRefs->liberar(); obj->firstsAppareancesRefs = NULL;
	if(obj->lastsAppareancesRefs != NULL) obj->lastsAppareancesRefs->liberar(); obj->lastsAppareancesRefs = NULL;
}

UI32 StringsPairs_add(STStringsPairs* obj, const char* str, const ENStringRelevance relevance, const char* curFile){
	STStringPair newDat;
	newDat.iOrig		= obj->strings->tamano(); obj->strings->agregar(str, AUCadena8::tamano(str) + 1);
	newDat.iNew			= 0;
	newDat.ignore		= false;
	newDat.isPathWord	= (relevance == ENStringRelevance_fileName || relevance == ENStringRelevance_folderName);
	//
	ItmCollectionStats_init(&newDat.collStats);
	newDat.collStats.maxRelevance = relevance;
	newDat.collStats.timesFound++;
	//Set first appareacen ref
	if(curFile != NULL){
		//Compare with last word (avoid adding the same origin multiple times)
		if(AUCadena8::cadenasSonIguales(curFile, &obj->firstsAppareancesRefs->str()[obj->firstsAppareancesRefsLastRefStart])){
			//Reuse the last word
			newDat.collStats.firstApprRefStart = obj->firstsAppareancesRefsLastRefStart;
		} else {
			//Add first reference to this origin
			obj->firstsAppareancesRefsLastRefStart = obj->firstsAppareancesRefs->tamano();
			newDat.collStats.firstApprRefStart = obj->firstsAppareancesRefsLastRefStart;
			obj->firstsAppareancesRefs->agregar(curFile);
			obj->firstsAppareancesRefs->agregar('\0');
		}
		//Compare with last word (avoid adding the same origin multiple times)
		if(AUCadena8::cadenasSonIguales(curFile, &obj->lastsAppareancesRefs->str()[obj->lastsAppareancesRefsLastRefStart])){
			//Reuse the last word
			newDat.collStats.lastApprRefStart = obj->lastsAppareancesRefsLastRefStart;
		} else {
			//Add last reference to this origin
			obj->lastsAppareancesRefsLastRefStart = obj->lastsAppareancesRefs->tamano();
			newDat.collStats.lastApprRefStart = obj->lastsAppareancesRefsLastRefStart;
			obj->lastsAppareancesRefs->agregar(curFile);
			obj->lastsAppareancesRefs->agregar('\0');
		}
	}
	obj->pairs->agregarElemento(newDat);
	//Add search index
	{
		STStringSrchHardIndex idx;
		idx.strStart	= newDat.iOrig;
		idx.strings		= obj->strings;
		idx.indexPos	= (obj->pairs->conteo - 1);
		obj->_srchIndexByOrig->agregarElemento(idx);
	}
	return (obj->pairs->conteo - 1);
}

UI32 StringsPairs_addIfNew(STStringsPairs* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance){
	UI32 r = StringsPairs_indexOfOrig(obj, strs, iStrStart);
	if(r == 0){
		const char* str = &strs->str()[iStrStart];
		r = StringsPairs_add(obj, str, relevance, curFile);
	} else {
		if(incrementAppareance){
			STStringPair* pair = obj->pairs->elemPtr(r);
			pair->collStats.timesFound++;
			//Compare with last word (avoid adding the same origin multiple times)
			if(AUCadena8::cadenasSonIguales(curFile, &obj->lastsAppareancesRefs->str()[obj->lastsAppareancesRefsLastRefStart])){
				//Reuse the last word
				pair->collStats.lastApprRefStart = obj->lastsAppareancesRefsLastRefStart;
			} else {
				//Add last reference to this origin
				obj->lastsAppareancesRefsLastRefStart = obj->lastsAppareancesRefs->tamano();
				pair->collStats.lastApprRefStart = obj->lastsAppareancesRefsLastRefStart;
				obj->lastsAppareancesRefs->agregar(curFile);
				obj->lastsAppareancesRefs->agregar('\0');
			}
		}
	}
	return r;
}

UI32 StringsPairs_indexOfOrig(const STStringsPairs* obj, AUCadenaLarga8* strs, const UI32 iStrStart){
	UI32 r = 0;
	//Binary seach (optimized)
	STStringSrchHardIndex idx;
	idx.strStart	= iStrStart;
	idx.strings		= strs;
	idx.indexPos	= 0;
	const SI32 iFound	= obj->_srchIndexByOrig->indiceDe(idx);
	if(iFound != -1){
		const STStringSrchHardIndex* rIdx = obj->_srchIndexByOrig->elemPtr(iFound);
		r = rIdx->indexPos;
		NBASSERT(r >= 0 && r < obj->pairs->conteo)
	}
	return r;
}

//

void StringsPairs_StringScapeForXml(const char* strArr, AUCadenaLargaMutable8* strDst){
	strDst->vaciar();
	while(*strArr != '\0'){
		if(*strArr == '<'){
			strDst->agregar("&lt;");
		} else if(*strArr == '>'){
			strDst->agregar("&gt;");
		} else {
			strDst->agregar(*strArr);
		}
		strArr++;
	}
}

void StringsPairs_StringUnscapeFromXml(const char* strArr, AUCadenaLargaMutable8* strDst){
	bool consume = false;
	strDst->vaciar();
	while(*strArr != '\0'){
		consume = true;
		if(*strArr == '&'){
			if(*(strArr + 1) == 'l'){
				if(*(strArr + 2) == 't'){
					if(*(strArr + 3) == ';'){
						strDst->agregar('<');
						consume = false;
						strArr += 3;
					}
				}
			} else if(*(strArr + 1) == 'g'){
				if(*(strArr + 2) == 't'){
					if(*(strArr + 3) == ';'){
						strDst->agregar('>');
						consume = false;
						strArr += 3;
					}
				}
			}
		}
		if(consume){
			strDst->agregar(*strArr);
		}
		strArr++;
	}
}

//

void StringsPairs_StringScapeForCsv(const char* strArr, AUCadenaLargaMutable8* strDst){
	strDst->vaciar();
	while(*strArr != '\0'){
		if(*strArr == '\"'){
			strDst->agregar("\"\"");
		} else {
			strDst->agregar(*strArr);
		}
		strArr++;
	}
}

//

bool StringsPairs_loadFromFileCsv(STStringsPairs* obj, const char* filePath){
	bool r = false;
	FILE* file = fopen(filePath, "rb");
	if(file != NULL){
		STStringPair newDat;
		newDat.iOrig		= 0;
		newDat.iNew			= 0;
		newDat.ignore		= false;
		newDat.isPathWord	= false;
		//
		ItmCollectionStats_init(&newDat.collStats);
		//
		typedef enum {
			ENColumn_strOrig = 0,
			ENColumn_strNew,
			ENColumn_ignore,
			ENColumn_isPathWord,
			ENColumn_count
		} ENColumn;
		ENColumn currentColumn = ENColumn_strOrig;
		//
		typedef enum {
			ENParserState_content = 0,			//when reading content
			ENParserState_insideLiteral,		//when reading inside a literal
			ENParserState_posibleLiteralClosing	//when reading a closing quotation mark (if the next char is also a quotation mark then the literal continues)
		} ENParserState;
		const SI32 bufferSize = 4096;
		char buffer[4096];
		SI32 bufferUse = (SI32)fread(buffer, 1, bufferSize, file);
		ENParserState parsingType = ENParserState_content;
		while(bufferUse > 0){
			//Consume content
			SI32 i;
			for(i = 0; i < bufferUse; i++){
				const char c = buffer[i];
				const ENParserState origParsingType = parsingType;
				bool addChar = true;
				//Determine if literal was closed
				if(parsingType == ENParserState_posibleLiteralClosing){
					if(c == '\"'){ //still inside literal
						parsingType = ENParserState_insideLiteral;
					} else { //exited from literal
						parsingType = ENParserState_content;
					}
				}
				//
				switch (parsingType) {
					case ENParserState_content:
						if(c == '\"'){ //get into a literal
							parsingType = ENParserState_insideLiteral;
							addChar = false;
						} else {
							//Ignore unwanted chars
							if(c == '\r'){
								addChar = false;
							}
							//Close current column and move to next
							if(c == ',' || c == ';' || c == '\n'){
								if(currentColumn < ENColumn_count){
									switch(currentColumn){
										case ENColumn_strOrig:
										case ENColumn_strNew:
											//add '\0' to the string buffer
											obj->strings->agregar('\0');
											break;
										default:
											break;
									}
									currentColumn = (ENColumn)(SI32)(currentColumn + 1);
								}
								addChar = false;
							}
							//Close current record and open a new one
							if(c == '\n'){
								// Optimization: if 'old' and 'new' strings are the same,
								// only remove the 'new' copy and point to the same 'old'.
								if(newDat.iOrig > 0 && newDat.iNew > newDat.iOrig){
									const char* strOrg = &obj->strings->str()[newDat.iOrig];
									const char* strNew = &obj->strings->str()[newDat.iNew];
									if(AUCadena8::cadenasSonIguales(strOrg, strNew)){
										const UI32 strLen = AUCadena8::tamano(strNew);
										const UI32 lenFromEnd = (obj->strings->tamano() - newDat.iNew - strLen - 1);
										if(lenFromEnd == 0){
											newDat.iNew = newDat.iOrig;
											obj->strings->quitarDerecha(strLen + 1);
											//PRINTF_INFO("#%d Optimization: old and new string are the same: '%s'.\n", (dbgRepeatOptimiz + 1), strOrg);
										}
									}
								}
								//Add
								obj->pairs->agregarElemento(newDat);
								//Add search index
								{
									STStringSrchHardIndex idx;
									idx.strStart	= newDat.iOrig;
									idx.strings		= obj->strings;
									idx.indexPos	= (obj->pairs->conteo - 1);
									obj->_srchIndexByOrig->agregarElemento(idx);
								}
								//Reset structure
								newDat.iOrig		= 0;
								newDat.iNew			= 0;
								newDat.ignore		= false;
								newDat.isPathWord	= false;
								//
								ItmCollectionStats_init(&newDat.collStats);
								//
								currentColumn		= ENColumn_strOrig;
								addChar = false;
							}
						}
						break;
					case ENParserState_insideLiteral:
						if(c == '\"' && origParsingType != ENParserState_posibleLiteralClosing){ //posible exit from literal
							parsingType = ENParserState_posibleLiteralClosing;
							addChar = false;
						}
						break;
					default:
						NBASSERT(false)
						break;
				}
				//
				if(addChar){
					switch(currentColumn){
						case ENColumn_strOrig:
							if(newDat.iOrig == 0) newDat.iOrig = obj->strings->tamano();
							obj->strings->agregar(c);
							break;
						case ENColumn_strNew:
							if(newDat.iNew == 0) newDat.iNew = obj->strings->tamano();
							obj->strings->agregar(c);
							break;
						case ENColumn_ignore:
							newDat.ignore = true;
							break;
						case ENColumn_isPathWord:
							newDat.isPathWord = true;
							break;
						default:
							break;
					}
				}
			}
			//refill buffer
			bufferUse = (SI32)fread(buffer, 1, bufferSize, file);
		}
		//Process last item
		if(newDat.iOrig != 0){
			//Close current column and record
			if(currentColumn < ENColumn_count){
				switch(currentColumn){
					case ENColumn_strOrig:
					case ENColumn_strNew:
						//add '\0' to the string buffer
						obj->strings->agregar('\0');
						break;
					default:
						break;
				}
			}
			obj->pairs->agregarElemento(newDat);
			//Add search index
			{
				STStringSrchHardIndex idx;
				idx.strStart	= newDat.iOrig;
				idx.strings		= obj->strings;
				idx.indexPos	= (obj->pairs->conteo - 1);
				obj->_srchIndexByOrig->agregarElemento(idx);
			}
		}
		//
		fclose(file);
		r = true;
	}
	return r;
}

bool StringsPairs_saveToFileCsv(STStringsPairs* obj, const char* filePath, bool includeCollStats){
	bool r = false;
	FILE* file = fopen(filePath, "wb");
	if(file != NULL){
		AUCadenaLargaMutable8* strTmp2 = new AUCadenaLargaMutable8();
		UI32 i; const UI32 count = obj->pairs->conteo;
		if(!includeCollStats){
			//First write existing pairs
			for(i = 1; i < count; i++){
				STStringPair* pair = obj->pairs->elemPtr(i);
				if(pair->iNew != 0){
					StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
				}
			}
			//Write strings without a pair
			for(i = 1; i < count; i++){
				STStringPair* pair = obj->pairs->elemPtr(i);
				if(pair->iNew == 0){
					StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
				}
			}
		} else {
			//First, write pairs found on current collection execution
			{
				//First write existing pairs
				for(i = 1; i < count; i++){
					STStringPair* pair = obj->pairs->elemPtr(i);
					if(pair->iNew != 0 && pair->collStats.timesFound != 0){
						StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
					}
				}
				//Write strings without a pair
				for(i = 1; i < count; i++){
					STStringPair* pair = obj->pairs->elemPtr(i);
					if(pair->iNew == 0 && pair->collStats.timesFound != 0){
						StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
					}
				}
			}
			//Last, write pairs not-found on current collection execution
			{
				//First write existing pairs
				for(i = 1; i < count; i++){
					STStringPair* pair = obj->pairs->elemPtr(i);
					if(pair->iNew != 0 && pair->collStats.timesFound == 0){
						StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
					}
				}
				//Write strings without a pair
				for(i = 1; i < count; i++){
					STStringPair* pair = obj->pairs->elemPtr(i);
					if(pair->iNew == 0 && pair->collStats.timesFound == 0){
						StringsPairs_saveToFileCsvRow(file, obj, pair, includeCollStats, strTmp2);
					}
				}
			}
		}
		strTmp2->liberar();
		//
		fclose(file);
		r = true;
		//PRINTF_INFO("StringLib saved with %d strings list CSV.\n", obj->pairs->conteo);
	}
	return r;
}

void StringsPairs_saveToFileCsvRow(FILE* file, STStringsPairs* pairs, STStringPair* pair, bool includeCollStats, AUCadenaLargaMutable8* strTmp2){
	StringsPairs_StringScapeForCsv(&pairs->strings->str()[pair->iOrig], strTmp2);
	fwrite("\"", 1, 1, file);
	fwrite(strTmp2->str(), 1, strTmp2->tamano(), file);
	fwrite("\"", 1, 1, file);
	//
	StringsPairs_StringScapeForCsv(&pairs->strings->str()[pair->iNew], strTmp2);
	fwrite(",\"", 1, 2, file);
	fwrite(strTmp2->str(), 1, strTmp2->tamano(), file);
	fwrite("\"", 1, 1, file);
	//
	fwrite(",\"", 1, 2, file);
	if(pair->ignore){ fwrite("i", 1, 1, file); }
	fwrite("\"", 1, 1, file);
	//
	fwrite(",\"", 1, 2, file);
	if(pair->isPathWord){ fwrite("f", 1, 1, file); }
	fwrite("\"", 1, 1, file);
	//
	if(includeCollStats){
		StringsPairs_StringScapeForCsv(Parser_strRelevanceName(pair->collStats.maxRelevance), strTmp2);
		fwrite(",\"", 1, 2, file);
		fwrite(strTmp2->str(), 1, strTmp2->tamano(), file);
		fwrite("\"", 1, 1, file);
		//
		fwrite(",\"", 1, 2, file);
		strTmp2->vaciar(); strTmp2->agregarNumerico(pair->collStats.timesFound);
		fwrite(strTmp2->str(), 1, strTmp2->tamano(), file);
		fwrite("\"", 1, 1, file);
		//
		fwrite(",\"", 1, 2, file);
		const char* firstRef = &pairs->firstsAppareancesRefs->str()[pair->collStats.firstApprRefStart];
		fwrite(firstRef, 1, AUCadena8::tamano(firstRef), file);
		fwrite("\"", 1, 1, file);
		//
		fwrite(",\"", 1, 2, file);
		const char* lastRef = &pairs->lastsAppareancesRefs->str()[pair->collStats.lastApprRefStart];
		fwrite(lastRef, 1, AUCadena8::tamano(lastRef), file);
		fwrite("\"", 1, 1, file);
	}
	//
	fwrite("\n", 1, 1, file);
}

void StringLib_init(STStringsLib* obj){
	obj->pairsArr		= NULL;
	obj->pairsArrSize	= 0;
	obj->curPairsArr	= 0;
}

void StringLib_finalize(STStringsLib* obj){
	if(obj->pairsArr != NULL){
		SI32 i;
		for(i = 0; i < obj->pairsArrSize; i++){
			STStringsPairs* pairsList = &obj->pairsArr[i];
			StringsPairs_finalize(pairsList);
		}
		NBGestorMemoria::liberarMemoria(obj->pairsArr);
		obj->pairsArr = NULL;
		obj->pairsArrSize	= 0;
	}
}

UI32 StringLib_openNewList(STStringsLib* obj){
	UI32 r = obj->pairsArrSize;
	const UI32 newSize = obj->pairsArrSize + 1;
	STStringsPairs* newArr = (STStringsPairs*)NBGestorMemoria::reservarMemoria(sizeof(STStringsPairs) * newSize, ENMemoriaTipo_General);
	if(obj->pairsArr != NULL){
		SI32 i;
		for(i = 0; i < obj->pairsArrSize; i++){
			newArr[i] = obj->pairsArr[i];
		}
		NBGestorMemoria::liberarMemoria(obj->pairsArr);
	}
	StringsPairs_init(&newArr[newSize - 1]);
	obj->pairsArr = newArr;
	obj->pairsArrSize = newSize;
	return r;
}

void StringLib_setFillingList(STStringsLib* obj, const SI32 listIndex){
	if(listIndex < obj->pairsArrSize){
		obj->curPairsArr = listIndex;
	}
}

UI32 StringLib_size(STStringsLib* obj){
	UI32 r = 0;
	SI32 i;
	for(i = 0; i < obj->pairsArrSize; i++){
		STStringsPairs* pairs = &obj->pairsArr[i];
		NBASSERT(pairs->pairs->conteo > 0)
		r += (pairs->pairs->conteo - 1); //The first element is allways the empty string
	}
	return r;
}

STStrLibIndex StringLib_add(STStringsLib* obj, const char* str, const ENStringRelevance relevance, const char* curFile){
	STStrLibIndex r;
	r.iArray = 0;
	r.iItem = 0;
	if(obj->pairsArrSize > 0){
		NBASSERT(obj->curPairsArr >= 0)
		NBASSERT(obj->curPairsArr < obj->pairsArrSize)
		r.iArray = obj->curPairsArr;
		r.iItem = StringsPairs_add(&obj->pairsArr[obj->curPairsArr], str, relevance, curFile);
	}
	return r;
}

STStrLibIndex StringLib_addIfNew(STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance){
	STStrLibIndex r = StringLib_indexOfOrig(obj, strs, iStrStart);
	if(r.iArray == 0 && r.iItem == 0){
		const char* str = &strs->str()[iStrStart];
		r = StringLib_add(obj, str, relevance, curFile);
	} else {
		STStringsPairs*	pairsArr = &obj->pairsArr[r.iArray];
		STStringPair* pair = pairsArr->pairs->elemPtr(r.iItem);
		//
		if(relevance == ENStringRelevance_folderName || relevance == ENStringRelevance_fileName){
			pair->isPathWord = true;
		}
		//
		if(incrementAppareance){
			pair->collStats.timesFound++;
			//Compare with last word (avoid adding the same origin multiple times)
			if(AUCadena8::cadenasSonIguales(curFile, &pairsArr->lastsAppareancesRefs->str()[pairsArr->lastsAppareancesRefsLastRefStart])){
				//Reuse the last word
				pair->collStats.lastApprRefStart = pairsArr->lastsAppareancesRefsLastRefStart;
			} else {
				//Add last reference to this origin
				pairsArr->lastsAppareancesRefsLastRefStart = pairsArr->lastsAppareancesRefs->tamano();
				pair->collStats.lastApprRefStart = pairsArr->lastsAppareancesRefsLastRefStart;
				pairsArr->lastsAppareancesRefs->agregar(curFile);
				pairsArr->lastsAppareancesRefs->agregar('\0');
			}
		}
	}
	return r;
}

STStrLibIndex StringLib_addOrUpdate(STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance){
	STStrLibIndex r;
	r.iArray = 0;
	r.iItem = 0;
	{
		const STStrLibIndex indx = StringLib_indexOfOrig(obj, strs, iStrStart);
		if(indx.iArray != 0 || indx.iItem != 0){
			//Update relevance (if greather)
			STStringsPairs*	pairsArr = &obj->pairsArr[indx.iArray];
			STStringPair* pair = pairsArr->pairs->elemPtr(indx.iItem);
			if(relevance == ENStringRelevance_folderName || relevance == ENStringRelevance_fileName){
				pair->isPathWord = true;
			}
			if(pair->collStats.maxRelevance < relevance){
				pair->collStats.maxRelevance = relevance;
			}
			if(incrementAppareance){
				pair->collStats.timesFound++;
				//Set first appareacen ref
				if(pair->collStats.firstApprRefStart == 0 && curFile != NULL){
					//Compare with last word (avoid adding the same origin multiple times)
					if(AUCadena8::cadenasSonIguales(curFile, &pairsArr->firstsAppareancesRefs->str()[pairsArr->firstsAppareancesRefsLastRefStart])){
						//Reuse the last word
						pair->collStats.firstApprRefStart = pairsArr->firstsAppareancesRefsLastRefStart;
					} else {
						//Add first reference to this origin
						pairsArr->firstsAppareancesRefsLastRefStart = pairsArr->firstsAppareancesRefs->tamano();
						pair->collStats.firstApprRefStart = pairsArr->firstsAppareancesRefsLastRefStart;
						pairsArr->firstsAppareancesRefs->agregar(curFile);
						pairsArr->firstsAppareancesRefs->agregar('\0');
					}
				}
				//Set last appareacen ref
				if(pair->collStats.lastApprRefStart == 0 && curFile != NULL){
					//Compare with last word (avoid adding the same origin multiple times)
					if(AUCadena8::cadenasSonIguales(curFile, &pairsArr->lastsAppareancesRefs->str()[pairsArr->lastsAppareancesRefsLastRefStart])){
						//Reuse the last word
						pair->collStats.lastApprRefStart = pairsArr->lastsAppareancesRefsLastRefStart;
					} else {
						//Add last reference to this origin
						pairsArr->lastsAppareancesRefsLastRefStart = pairsArr->lastsAppareancesRefs->tamano();
						pair->collStats.lastApprRefStart = pairsArr->lastsAppareancesRefsLastRefStart;
						pairsArr->lastsAppareancesRefs->agregar(curFile);
						pairsArr->lastsAppareancesRefs->agregar('\0');
					}
				}
			}
		} else {
			//Add
			const char* str = &strs->str()[iStrStart];
			r = StringLib_add(obj, str, relevance, curFile);
		}
	}
	return r;
}

STStrLibIndex StringLib_indexOfOrig(const STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart){
	STStrLibIndex r;
	r.iArray = 0;
	r.iItem = 0;
	if(obj->pairsArrSize > 0){
		SI32 i;
		for(i = 0; i < obj->pairsArrSize; i++){
			STStringsPairs* pairs = &obj->pairsArr[i];
			const UI32 iPair = StringsPairs_indexOfOrig(pairs, strs, iStrStart);
			if(iPair != 0){
				r.iArray = i;
				r.iItem = iPair;
				break;
			}
		}
	}
	return r;
}

UI32 StringLib_loadMissingTranslationsFromPairsCsv(STStringsLib* obj, const char* filePath){
	UI32 r = 0;
	STStringsPairs pairsRef;
	StringsPairs_init(&pairsRef);
	if(StringsPairs_loadFromFileCsv(&pairsRef, filePath)){
		//Compare
		SI32 i, j;
		for(i = 0; i < obj->pairsArrSize; i++){
			STStringsPairs* pairs = &obj->pairsArr[i];
			for(j = 0; j < pairs->pairs->conteo; j++){
				STStringPair* pair = pairs->pairs->elemPtr(j);
				if(pair->iNew == 0){
					const UI32 indexRef = StringsPairs_indexOfOrig(&pairsRef, pairs->strings, pair->iOrig);
					if(indexRef != 0){
						const UI32 iNewRef = pairsRef.pairs->elem(indexRef).iNew;
						if(iNewRef != 0){
							pair->iNew = pairs->strings->tamano();
							pairs->strings->agregar(&pairsRef.strings->str()[iNewRef]);
							pairs->strings->agregar('\0');
							r++;
						}
					}
				}
			}
		}
	}
	StringsPairs_finalize(&pairsRef);
	return r;
}

void StringLib_findDuplicatedTranslations(STStringsLib* obj, AUArregloNativoMutableP<STStrLibIndex>* dst, bool onlyExplicitTranslations){
	if(obj != NULL && dst != NULL){
		SI32 i, j;
		for(i = 0; i < obj->pairsArrSize; i++){
			STStringsPairs* pairs = &obj->pairsArr[i];
			for(j = 0; j < pairs->pairs->conteo; j++){
				STStringPair* pair	= pairs->pairs->elemPtr(j);
				//Determine the word to compare
				UI32 iWordToCompare	= 0;
				if(pair->iNew > 0){
					iWordToCompare = pair->iNew;
				} else if(pair->iOrig > 0 && !onlyExplicitTranslations){
					iWordToCompare = pair->iOrig;
				}
				//Search duplicates
				if(iWordToCompare > 0){
					//Avoid already found items
					bool alreadyFound = false;
					{
						SI32 f;
						for(f = (dst->conteo - 1); f >= 0; f--){
							const STStrLibIndex* found = dst->elemPtr(f);
							if(found->iArray == i && found->iItem == j){
								alreadyFound = true;
								break;
							}
						}
					}
					//Search other copy
					if(!alreadyFound){
						SI32 i2, j2;
						for(i2 = i; i2 < obj->pairsArrSize; i2++){
							STStringsPairs* pairs2 = &obj->pairsArr[i2];
							for(j2 = (i2 == i ? (j + 1) : 0); j2 < pairs2->pairs->conteo; j2++){
								STStringPair* pair2 = pairs2->pairs->elemPtr(j2);
								//Determine the word to compare
								UI32 iWordToCompare2 = 0;
								if(pair2->iNew > 0){
									iWordToCompare2 = pair2->iNew;
								} else if(pair2->iOrig > 0 && !onlyExplicitTranslations){
									iWordToCompare2 = pair2->iOrig;
								}
								//Compare
								if(iWordToCompare2 > 0){
									if(AUCadena8::cadenasSonIguales(&pairs->strings->str()[iWordToCompare], &pairs2->strings->str()[iWordToCompare2])){
										if(!alreadyFound){
											STStrLibIndex org;
											org.iArray = i;
											org.iItem = j;
											dst->agregarElemento(org);
											alreadyFound = true;
										}
										STStrLibIndex other;
										other.iArray = i2;
										other.iItem = j2;
										dst->agregarElemento(other);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


//------------------------
//-- Translating line (strings sequence)
//-- (words and separators on a line, literal text, or comment)
//------------------------

void TranslatedLine_init(STTranslatedLine* obj){
	obj->strSeq = new(ENMemoriaTipo_General) AUArregloNativoMutableP<STTranslatedString>();
}

void TranslatedLine_finalize(STTranslatedLine* obj){
	obj->strSeq->liberar(NB_RETENEDOR_NULL);
	obj->strSeq = NULL;
}

void TranslatedLine_addStrSeq(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, const char* strOrig, const ENTranslatedStrType origType){
	STTranslatedString pair;
	pair.origType = origType;
	pair.iOrig = pair.iFinal = strsStorage->tamano();
	strsStorage->agregar(strOrig, AUCadenaLarga8::tamano(strOrig) + 1); //include the '\0'
	obj->strSeq->agregarElemento(pair);
}

void TranslatedLine_updateStrSeqFinal(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, const UI32 strIndex, const char* strFinal){
	if(strIndex < obj->strSeq->conteo){
		STTranslatedString* pair = obj->strSeq->elemPtr(strIndex);
		pair->iFinal = strsStorage->tamano();
		strsStorage->agregar(strFinal, AUCadenaLarga8::tamano(strFinal) + 1); //include the '\0'
	}
}

UI32 TranslatedLine_flushToFile(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, FILE* file){
	UI32 r = 0;
	if(file){
		SI32 i; const SI32 count = obj->strSeq->conteo;
		for(i = 0; i < count; i++){
			STTranslatedString* strPair = obj->strSeq->elemPtr(i);
			const char* strFinal = &strsStorage->str()[strPair->iFinal];
			const SI32 strFinalSz = AUCadenaLarga8::tamano(strFinal);
			fwrite(strFinal, 1, strFinalSz, file);
			r += strFinalSz;
		}
	}
	return r;
}



//------------------------
//-- Actions stats
//------------------------

void TranslStat_init(STTranslStat* obj){
	obj->countFiles				= 0;
	obj->countFilesIgnore		= 0;
	obj->countFilesCopy			= 0;
	obj->countFilesProcess		= 0;
	//
	obj->countStrs				= 0;
	obj->countStrsNewFound		= 0;
	obj->countStrsTranslated	= 0;
	//
	obj->countBytesRead			= 0;
	obj->countBytesWritten		= 0;
	//
	obj->countLinesRead			= 0;
	obj->countLinesWritten		= 0;
}

void TranslStat_restart(STTranslStat* obj){
	obj->countFiles				= 0;
	obj->countFilesIgnore		= 0;
	obj->countFilesCopy			= 0;
	obj->countFilesProcess		= 0;
	//
	obj->countStrs				= 0;
	obj->countStrsNewFound		= 0;
	obj->countStrsTranslated	= 0;
	//
	obj->countBytesRead			= 0;
	obj->countBytesWritten		= 0;
	//
	obj->countLinesRead			= 0;
	obj->countLinesWritten		= 0;
}

void TranslStat_add(STTranslStat* obj, const STTranslStat* toAdd){
	obj->countFiles				+= toAdd->countFiles;
	obj->countFilesIgnore		+= toAdd->countFilesIgnore;
	obj->countFilesCopy			+= toAdd->countFilesCopy;
	obj->countFilesProcess		+= toAdd->countFilesProcess;
	//
	obj->countStrs				+= toAdd->countStrs;
	obj->countStrsNewFound		+= toAdd->countStrsNewFound;
	obj->countStrsTranslated	+= toAdd->countStrsTranslated;
	//
	obj->countBytesRead			+= toAdd->countBytesRead;
	obj->countBytesWritten		+= toAdd->countBytesWritten;
	//
	obj->countLinesRead			+= toAdd->countLinesRead;
	obj->countLinesWritten		+= toAdd->countLinesWritten;
}





//------------------------
//-- Extension acumulated stats
//------------------------

void TranslStatGrp_init(STTranslStatGrp* obj, const char* grpName){
	const SI32 grpNameSz = AUCadena8::tamano(grpName);
	obj->grpName = (char*)NBGestorMemoria::reservarMemoria(sizeof(char) * (grpNameSz + 1), ENMemoriaTipo_General);
	//Copy extension string
	{
		SI32 i;
		for(i = 0; i < grpNameSz; i++)
			obj->grpName[i] = grpName[i];
		obj->grpName[i] = '\0';
	}
	TranslStat_init(&obj->stats);
}

void TranslStatGrp_finalize(STTranslStatGrp* obj){
	if(obj->grpName != NULL){
		NBGestorMemoria::liberarMemoria(obj->grpName);
		obj->grpName = NULL;
	}
}




//------------------------
//-- Grouped stats (usefull for configuration analysis)
//------------------------

void TranslStatsGrouped_init(STTranslStatsGrouped* obj){
	obj->byExt = new(ENMemoriaTipo_Temporal) AUArregloNativoOrdenadoMutableP<STTranslStatGrp>();
	obj->byFolder = new(ENMemoriaTipo_Temporal) AUArregloNativoOrdenadoMutableP<STTranslStatGrp>();
	obj->byFile = new(ENMemoriaTipo_Temporal) AUArregloNativoOrdenadoMutableP<STTranslStatGrp>();
}

void TranslStatsGrouped_finalize(STTranslStatsGrouped* obj){
	if(obj->byExt != NULL){
		SI32 i; const SI32 count = obj->byExt->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byExt->elemPtr(i);
			TranslStatGrp_finalize(stat);
		}
		obj->byExt->vaciar();
		obj->byExt->liberar();
		obj->byExt = NULL;
	}
	if(obj->byFolder != NULL){
		SI32 i; const SI32 count = obj->byFolder->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byFolder->elemPtr(i);
			TranslStatGrp_finalize(stat);
		}
		obj->byFolder->vaciar();
		obj->byFolder->liberar();
		obj->byFolder = NULL;
	}
	if(obj->byFile != NULL){
		SI32 i; const SI32 count = obj->byFile->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byFile->elemPtr(i);
			TranslStatGrp_finalize(stat);
		}
		obj->byFile->vaciar();
		obj->byFile->liberar();
		obj->byFile = NULL;
	}
}

void TranslStatsGrouped_add(STTranslStatsGrouped* obj, const STTranslStatsGrouped* toAdd) {
	if(obj->byFile != NULL && toAdd->byFile != NULL){
		SI32 i; const SI32 count = toAdd->byFile->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = toAdd->byFile->elemPtr(i);
			{
				STTranslStatGrp* dstStat = NULL;
				STTranslStatGrp srchStat;
				srchStat.grpName = stat->grpName;
				const SI32 iFound = obj->byFile->indiceDe(srchStat);
				if(iFound != -1){
					dstStat = obj->byFile->elemPtr(iFound);
				} else {
					STTranslStatGrp newStat;
					TranslStatGrp_init(&newStat, stat->grpName);
					const UI32 iPos = obj->byFile->agregarElemento(newStat);
					dstStat = obj->byFile->elemPtr(iPos);
				}
				TranslStat_add(&dstStat->stats, &stat->stats);
			}
		}
	}
	if(obj->byExt != NULL && toAdd->byExt != NULL){
		SI32 i; const SI32 count = toAdd->byExt->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = toAdd->byExt->elemPtr(i);
			{
				STTranslStatGrp* dstStat = NULL;
				STTranslStatGrp srchStat;
				srchStat.grpName = stat->grpName;
				const SI32 iFound = obj->byExt->indiceDe(srchStat);
				if(iFound != -1){
					dstStat = obj->byExt->elemPtr(iFound);
				} else {
					STTranslStatGrp newStat;
					TranslStatGrp_init(&newStat, stat->grpName);
					const UI32 iPos = obj->byExt->agregarElemento(newStat);
					dstStat = obj->byExt->elemPtr(iPos);
				}
				TranslStat_add(&dstStat->stats, &stat->stats);
			}
		}
	}
	if(obj->byFolder != NULL && toAdd->byFolder != NULL){
		SI32 i; const SI32 count = toAdd->byFolder->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = toAdd->byFolder->elemPtr(i);
			{
				STTranslStatGrp* dstStat = NULL;
				STTranslStatGrp srchStat;
				srchStat.grpName = stat->grpName;
				const SI32 iFound = obj->byFolder->indiceDe(srchStat);
				if(iFound != -1){
					dstStat = obj->byFolder->elemPtr(iFound);
				} else {
					STTranslStatGrp newStat;
					TranslStatGrp_init(&newStat, stat->grpName);
					const UI32 iPos = obj->byFolder->agregarElemento(newStat);
					dstStat = obj->byFolder->elemPtr(iPos);
				}
				TranslStat_add(&dstStat->stats, &stat->stats);
			}
		}
	}
}

void TranslStatsGrouped_printf(STTranslStatsGrouped* obj){
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	/*if(obj->byFile != NULL){
		PRINTF_INFO("------------------------\n");
		PRINTF_INFO("-- STATS BY FILE --\n");
		PRINTF_INFO("------------------------\n");
		SI32 i; const SI32 count = obj->byFile->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byFile->elemPtr(i);
			PRINTF_INFO("#%d Folder [%s] '%s': %d files (%d process, %d copy, %d ignore).\n", (i + 1), (stat->stats.countFilesProcess != 0 ? "PRC" : stat->stats.countFilesCopy != 0 ? "CPY" : "IGN"), stat->grpName, stat->stats.countFiles, stat->stats.countFilesProcess, stat->stats.countFilesCopy, stat->stats.countFilesIgnore);
		}
		PRINTF_INFO("----------------------.\n");
	}*/
	if(obj->byExt != NULL){
		PRINTF_INFO("------------------------\n");
		PRINTF_INFO("-- STATS BY EXTENSION --\n");
		PRINTF_INFO("------------------------\n");
		SI32 i; const SI32 count = obj->byExt->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byExt->elemPtr(i);
			int actionGrpCount = 0;
			strTmp->vaciar();
			strTmp->agregarConFormato("#%d Ext [%s] '%s': %d files", (i + 1), (stat->stats.countFilesProcess != 0 ? "PRC" : stat->stats.countFilesCopy != 0 ? "CPY" : "IGN"), stat->grpName, stat->stats.countFiles);
			if(stat->stats.countFilesProcess > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d processed", stat->stats.countFilesProcess);
				actionGrpCount++;
			}
			if(stat->stats.countFilesCopy > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d copied", stat->stats.countFilesCopy);
				actionGrpCount++;
			}
			if(stat->stats.countFilesIgnore > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d ignored", stat->stats.countFilesIgnore);
				actionGrpCount++;
			}
			if(actionGrpCount != 0) strTmp->agregar(")");
			PRINTF_INFO("%s.\n", strTmp->str());
		}
		PRINTF_INFO("----------------------.\n");
	}
	if(obj->byFolder != NULL){
		PRINTF_INFO("---------------------\n");
		PRINTF_INFO("-- STATS BY FOLDER --\n");
		PRINTF_INFO("---------------------\n");
		SI32 i; const SI32 count = obj->byFolder->conteo;
		for(i = 0; i < count; i++){
			STTranslStatGrp* stat = obj->byFolder->elemPtr(i);
			int actionGrpCount = 0;
			strTmp->vaciar();
			strTmp->agregarConFormato("#%d Folder [%s] '%s': %d files", (i + 1), (stat->stats.countFilesProcess != 0 ? "PRC" : stat->stats.countFilesCopy != 0 ? "CPY" : "IGN"), stat->grpName, stat->stats.countFiles);
			if(stat->stats.countFilesProcess > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d processed", stat->stats.countFilesProcess);
				actionGrpCount++;
			}
			if(stat->stats.countFilesCopy > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d copied", stat->stats.countFilesCopy);
				actionGrpCount++;
			}
			if(stat->stats.countFilesIgnore > 0){
				strTmp->agregar(actionGrpCount == 0 ? " (" : ", ");
				strTmp->agregarConFormato("%d ignored", stat->stats.countFilesIgnore);
				actionGrpCount++;
			}
			if(actionGrpCount != 0) strTmp->agregar(")");
			PRINTF_INFO("%s.\n", strTmp->str());
		}
		PRINTF_INFO("----------------------.\n");
	}
	strTmp->liberar();
}

//------------------------
//-- Grouped stats per task (usefull for configuration analysis)
//------------------------

void TranslStatsPerTask_init(STTranslStatsPerTask* obj){
	obj->grpdStatsPerTask = new(ENMemoriaTipo_General) AUArregloNativoMutableP<STTranslStatsGrouped>();
}

void TranslStatsPerTask_empty(STTranslStatsPerTask* obj){
	if(obj->grpdStatsPerTask != NULL){
		SI32 i; const SI32 count = obj->grpdStatsPerTask->conteo;
		for(i = 0; i < count; i++){
			STTranslStatsGrouped* stats = obj->grpdStatsPerTask->elemPtr(i);
			TranslStatsGrouped_finalize(stats);
		}
		obj->grpdStatsPerTask->vaciar();
	}
}

void TranslStatsPerTask_finalize(STTranslStatsPerTask* obj){
	if(obj->grpdStatsPerTask != NULL){
		SI32 i; const SI32 count = obj->grpdStatsPerTask->conteo;
		for(i = 0; i < count; i++){
			STTranslStatsGrouped* stats = obj->grpdStatsPerTask->elemPtr(i);
			TranslStatsGrouped_finalize(stats);
		}
		obj->grpdStatsPerTask->vaciar();
		obj->grpdStatsPerTask->liberar();
		obj->grpdStatsPerTask = NULL;
	}
}



//------------------------
//-- File parser
//------------------------

void ParsingFileState_init(STParsingFileState* obj, const char* curFileName, bool initLinesBuffers, bool createStatsRecord){
	obj->curFileName		= curFileName;
	obj->charCurrnt			= obj->charBefore = '\0';
	obj->literalCharScaped	= false;
	//
	obj->readingBlqType		= ENTranslatedStrType_Code;
	obj->readingBlqTypeDefIndex = -1;
	obj->readingLastValidWord = new AUCadenaLargaMutable8();
	obj->readingStrAcum		= new AUCadenaLargaMutable8();
	//
	obj->linesSeq			= NULL;
	obj->linesStorage		= NULL;
	if(initLinesBuffers){
		obj->linesSeq		= new(ENMemoriaTipo_General) AUArregloNativoMutableP<STTranslatedLine>();
		//Add first empty line
		{
			STTranslatedLine firstLine;
			TranslatedLine_init(&firstLine);
			obj->linesSeq->agregarElemento(firstLine);
		}
		obj->linesStorage = new(ENMemoriaTipo_General) AUCadenaLargaMutable8();
		obj->linesStorage->prepararBufferParaTamano(1024 * 100);
	}
	//Stats
	obj->stats				= NULL;
	if(createStatsRecord){
		obj->stats			= (STTranslStat*)NBGestorMemoria::reservarMemoria(sizeof(STTranslStat), ENMemoriaTipo_General);
		TranslStat_init(obj->stats);
	}
}

void ParsingFileState_finalize(STParsingFileState* obj){
	if(obj->readingLastValidWord != NULL) obj->readingLastValidWord->liberar();
	if(obj->readingStrAcum != NULL) obj->readingStrAcum->liberar();
	//lines seq
	if(obj->linesSeq != NULL){
		SI32 i; const SI32 count = obj->linesSeq->conteo;
		for(i = 0; i < count; i++){
			STTranslatedLine* objLine = obj->linesSeq->elemPtr(i);
			TranslatedLine_finalize(objLine);
		}
		obj->linesSeq->vaciar();
		obj->linesSeq->liberar(NB_RETENEDOR_NULL);
		obj->linesSeq = NULL;
	}
	if(obj->linesStorage != NULL) obj->linesStorage->liberar(); obj->linesStorage = NULL;
	//Stats
	if(obj->stats != NULL){
		NBGestorMemoria::liberarMemoria(obj->stats);
		obj->stats = NULL;
	}
}

STTranslatedLine* ParsingFileState_lastLine(STParsingFileState* obj){
	STTranslatedLine* r = NULL;
	if(obj->linesSeq != NULL){
		NBASSERT(obj->linesSeq->conteo > 0) //At least one empty-line must exists
		r = obj->linesSeq->elemPtr(obj->linesSeq->conteo - 1);
	}
	return r;
}

STTranslatedLine* ParsingFileState_addNewLine(STParsingFileState* obj){
	STTranslatedLine* r = NULL;
	if(obj->linesSeq != NULL){
		STTranslatedLine objLine;
		TranslatedLine_init(&objLine);
		obj->linesSeq->agregarElemento(objLine);
		r = obj->linesSeq->elemPtr(obj->linesSeq->conteo - 1);
	}
	return r;
}

bool ParsingFileState_flushToFile(STParsingFileState* obj, FILE* file){
	bool r = false;
	if(obj->linesSeq != NULL && file != NULL){
		SI32 i; const SI32 count = obj->linesSeq->conteo;
		if(count != 0){
			for(i = 0; i < count; i++){
				STTranslatedLine* objLine = obj->linesSeq->elemPtr(i);
				const UI32 bytesWritten = TranslatedLine_flushToFile(objLine, obj->linesStorage, file);
				if(obj->stats != NULL){
					obj->stats->countBytesWritten += bytesWritten;
					obj->stats->countLinesWritten++;
				}
			}
			//Remove last line from counting (if empty)
			if(obj->stats != NULL){
				if(obj->linesSeq->elemPtr(count - 1)->strSeq->conteo == 0){
					obj->stats->countLinesWritten--;
				}
			}
		}
		r = true;
	}
	return r;
}

//

void Parser_init(){
	//Prepare file-format's reserved-words lists
	{
		AUCadenaLargaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaLargaMutable8();
		SI32 i; const SI32 count = (sizeof(__parserSrcFormatMap) / sizeof(__parserSrcFormatMap[0]));
		for(i = 0; i < count; i++){
			STFileFormatMap* map = &__parserSrcFormatMap[i]; NBASSERT(map->format == i)
			StringsPairs_init(&map->reservedWords);
			//Add primary NULL-terminated word list
			if(map->reservedWordsPlain != NULL){
				const char** word = &map->reservedWordsPlain[0];
				while(*word != NULL){
					strTmp->establecer(*word);
					StringsPairs_addIfNew(&map->reservedWords, strTmp, 0, ENStringRelevance_none, "RESERVED_WORDS_LIST", true);
					word++; //Next
				}
			}
			//Add secondary NULL-terminated word list
			if(map->reservedWordsPlain2 != NULL){
				const char** word = &map->reservedWordsPlain2[0];
				while(*word != NULL){
					strTmp->establecer(*word);
					StringsPairs_addIfNew(&map->reservedWords, strTmp, 0, ENStringRelevance_none, "RESERVED_WORDS_LIST", true);
					word++; //Next
				}
			}
		}
		NBASSERT(i == ENSrcFileFormat_count)
		strTmp->liberar();
		strTmp = NULL;
	}
}

void Parser_finalize(){
	//Free file-format's reserved-words lists
	{
		SI32 i; const SI32 count = (sizeof(__parserSrcFormatMap) / sizeof(__parserSrcFormatMap[0]));
		for(i = 0; i < count; i++){
			STFileFormatMap* map = &__parserSrcFormatMap[i];
			StringsPairs_finalize(&map->reservedWords);
		}
	}
}


//


bool Parser_processStringsMustIgnoreSequence(const char* strArr, const UI32 strSize){
	bool isInteger = true, intTypeFound = false;
	bool isDecimal = true, decFoundPoint = false;
	bool isHexadec = true, hexFoundX = false, hexTypeFound = false;
	UI32 i = 0;
	while((isInteger || isDecimal || isHexadec) && i < strSize){
		const char c = strArr[i];
		if(c < 48 || c > 57){ //Not digit
			//0123456789L | 0123456789LL | 0123456789U | 0123456789ULL
			if(isInteger){
				if(c == 'u' || c == 'U' || c == 'l' || c == 'L'){ intTypeFound = true; }
				else { isInteger = false; }
			}
			//0.0f
			if(isDecimal) {
				if(c == '.'){ if(decFoundPoint){ isDecimal = false; } decFoundPoint = true; }
				else if(c != 'f' || (i + 1) != strSize){ isDecimal = false; }
			}
			//0xABCDEFG | 0xABCDEFGL | 0xABCDEFGLL | 0xABCDEFGU | | 0xABCDEFGULL
			if(isHexadec){
				if(c == 'u' || c == 'U' || c == 'l' || c == 'L'){ if(!hexFoundX){ isHexadec = false; } hexTypeFound = true; }
				else if(c == 'x' || c == 'X'){ if(hexFoundX){ isHexadec = false; } hexFoundX = true; }
				else if(!((c >= 65 && c <= 70) || (c >= 97 && c <= 102))){ isHexadec = false; }
			}
		} else { //is digit
			if(isInteger){
				if(intTypeFound) { isInteger = false; /*cases like: "123UULL123"*/ }
			}
			if(isHexadec){
				if(hexTypeFound) { isHexadec = false; /*cases like: "0x123UULL123"*/ }
			}
		}
		i++;
	}
	return (isInteger || isDecimal || isHexadec);
}

void Parser_processStringsFlush(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, STTranslatedLine* dstLine, const ENTranslatedStrType blqType, const ENStringRelevance newRelevance, AUCadenaLargaMutable8* lastValidWord, AUCadenaLargaMutable8* strAcum, const UI32 parsingOptionsMask){
	const char* strArr = strAcum->str();
	UI32 strSize = strAcum->tamano();
	if(strSize != 0){
		NBASSERT(blqType == ENTranslatedStrType_Code || blqType == ENTranslatedStrType_LiteralSingle || blqType == ENTranslatedStrType_LiteralDouble || blqType == ENTranslatedStrType_CommentMonoLine || blqType == ENTranslatedStrType_CommentMultiLine)
		//Add to current line
		if(dstLine != NULL){
			/*if(AUCadena8::esIgual(fileState->orgFileName, "NBParserXmlP.h")){
				if(fileState->readingLine == 144){
					//if(strAcum->esIgual("AUCadenaP") || strAcum->esIgual("AUCadenaP<")){
					PRINTF_INFO("%s::%d word: '%s'\n", fileState->orgFileName, fileState->readingLine, strAcum->str());
					PRINTF_INFO("-\n");
					//}
				}
			}*/
			TranslatedLine_addStrSeq(dstLine, fileState->linesStorage, strArr, blqType);
		}
		//Add to pairs library
		if((parsingActionsMask & TRANSLATE_ACTION_COLLECT_WORDS) != 0){
			//--------------------------
			//- FIRST IGNORE STAGE: single chars.
			//--------------------------
			//Ignore by length
			bool ignore = (strSize < 2); //if(ignore){ PRINTF_INFO("Ignored by length (%d chars).\n", strSize)}
			if(!ignore){
				//Backup string
				lastValidWord->establecer(strAcum->str());
			}
			//--------------------------
			//- SECOND IGNORE STAGE: literals and comments.
			//--------------------------
			//Ignore by litteral (single)
			if(!ignore){
				ignore = (blqType == ENTranslatedStrType_LiteralSingle && (parsingOptionsMask & TRANSLATE_OPT_COLLECT_LITERALS_SINGLE) == 0);
				//if(ignore){ PRINTF_INFO("Ignored by literal-single ('%s').\n", strAcum->str())}
			}
			//Ignore by litteral (double)
			if(!ignore){
				ignore = (blqType == ENTranslatedStrType_LiteralDouble && (parsingOptionsMask & TRANSLATE_OPT_COLLECT_LITERALS_DOUBLE) == 0);
				//if(ignore){ PRINTF_INFO("Ignored by literal-double ('%s').\n", strAcum->str())}
			}
			//Ignore by commentary (c monoline)
			if(!ignore){
				ignore = (blqType == ENTranslatedStrType_CommentMonoLine && (parsingOptionsMask & TRANSLATE_OPT_COLLECT_COMMENTS_C_MONOLINE) == 0);
				//if(ignore){ PRINTF_INFO("Ignored by commentary-c-monoline ('%s').\n", strAcum->str())}
			}
			//Ignore by commentary (c multiline)
			if(!ignore){
				ignore = (blqType == ENTranslatedStrType_CommentMultiLine && (parsingOptionsMask & TRANSLATE_OPT_COLLECT_COMMENTS_C_MULTILINE) == 0);
				//if(ignore){ PRINTF_INFO("Ignored by commentary-c-multiline ('%s').\n", strAcum->str())}
			}
			//--------------------------
			//- THIRD IGNORE STAGE: string is valid
			//--------------------------
			//Ignore by pattern (numeric values)
			if(!ignore){
				ignore = Parser_processStringsMustIgnoreSequence(strArr, strSize); //if(ignore){ PRINTF_INFO("Ignored by pattern ('%s').\n", strAcum->str())}
			}
			//Ignore by file format's reserved word
			if(!ignore){
				if(formatType < ENSrcFileFormat_count){
					const STFileFormatMap* map = &__parserSrcFormatMap[formatType]; NBASSERT(map->format == formatType)
					if(StringsPairs_indexOfOrig(&map->reservedWords, strAcum, 0) != 0){
						ignore = true;
					}
				}
			}
			//Process
			if(!ignore){
				StringLib_addOrUpdate(strlib, strAcum, 0, newRelevance, fileState->curFileName, true);
			}
		}
	}
	strAcum->vaciar();
}

bool Parser_loadPairsFromCSVs(const STSrcTranslatorParams* params, STStringsLib* strlib){
	SI32 i;
	for(i = 0; i < params->transTasksCount; i++){
		const STSrcTraslationTask* task = &params->transTasks[i];
		//Create (if necesary) the new pairs-list.
		while(strlib->pairsArrSize <= i){
			StringLib_openNewList(strlib);
		}
		//Process
		STStringsPairs* pairsList = &strlib->pairsArr[i];
		NBASSERT(pairsList->pairs->conteo == 1) //List must be new
		if(StringsPairs_loadFromFileCsv(pairsList, task->pathPairsCsv)){
			PRINTF_INFO("CSV-pairs-file loaded: '%s'.\n", task->pathPairsCsv);
		} else {
			PRINTF_ERROR("CSV-pairs-file couldnt be loaded: '%s'.\n", task->pathPairsCsv);
		}
	}
	return true;
}

bool Parser_savePairsToCSVs(const STSrcTranslatorParams* params, STStringsLib* strlib, bool includeCollStats){
	SI32 i;
	for(i = 0; i < params->transTasksCount && i < strlib->pairsArrSize; i++){
		const STSrcTraslationTask* task = &params->transTasks[i];
		STStringsPairs* pairsList = &strlib->pairsArr[i];
		if(StringsPairs_saveToFileCsv(pairsList, task->pathPairsCsv, includeCollStats)){
			PRINTF_INFO("CSV-pairs-file saved: '%s'.\n", task->pathPairsCsv);
		} else {
			PRINTF_ERROR("CSV-pairs-file couldnt be saved: '%s'.\n", task->pathPairsCsv);
		}
	}
	return true;
}

UI32 Parser_applyIgnoresFromXml(STStringsLib* strlib, const char* filePath){
	UI32 r = 0;
	AUDatosXMLMutable* xmlFile = new AUDatosXMLMutable();
	if(xmlFile->cargaDatosXMLDesdeArchivo(filePath)){
		const sNodoXML* strs = xmlFile->nodoHijo("strs");
		if(strs != NULL){
			AUCadenaLargaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaLargaMutable8();
			UI32 nodesStrCount = 0;
			UI32 nodesStrWithData = 0;
			const sNodoXML* str = xmlFile->nodoHijo("str", strs);
			while(str != NULL){
				const char* val = xmlFile->cadenaValorDeNodo(str);
				if(val[0] != '\0'){
					strTmp->establecer(val);
					const STStrLibIndex indx = StringLib_addIfNew(strlib, strTmp, 0, ENStringRelevance_none, filePath, true);
					NBASSERT(indx.iArray != 0 || indx.iItem != 0)
					if(indx.iArray != 0 || indx.iItem != 0){
						STStringPair* pair = strlib->pairsArr[indx.iArray].pairs->elemPtr(indx.iItem);
						pair->ignore = true;
						nodesStrWithData++;
					}
				}
				//
				nodesStrCount++;
				str = xmlFile->nodoHijo("str", strs, str);
			}
			strTmp->liberar();
			r = nodesStrWithData;
			//PRINTF_INFO("Ignore list loaded with %d strings (%d xml nodes).\n", nodesStrWithData, nodesStrCount);
		}
	}
	xmlFile->liberar();
	return r;
}

void Parser_printfGroupedStatsPerTask(const STSrcTranslatorParams* params, STTranslStatsPerTask* grpStatsPerTask){
	if(grpStatsPerTask != NULL && params->transTasksCount != 0){
		SI32 i;
		for(i = 0; i < params->transTasksCount; i++){
			const STSrcTraslationTask* task = &params->transTasks[i];
			//Create (if necesary) the new grpdStats for this task
			while(grpStatsPerTask->grpdStatsPerTask->conteo <=  i){
				STTranslStatsGrouped newGrpdStats;
				TranslStatsGrouped_init(&newGrpdStats);
				grpStatsPerTask->grpdStatsPerTask->agregarElemento(newGrpdStats);
			}
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("++ STATS for task (start)\n");
			PRINTF_INFO("++ '%s'\n", task->taskName);
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			TranslStatsGrouped_printf(grpStatsPerTask->grpdStatsPerTask->elemPtr(i));
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("++ '%s'\n", task->taskName);
			PRINTF_INFO("++ STATS for task (end)\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
		}
		PRINTF_INFO("\n");
		PRINTF_INFO("\n");
		PRINTF_INFO("\n");
	}
}

STTranslStat Parser_processTasks(const STSrcTranslatorParams* params, STStringsLib* strlib, const SI32 parsingActionsMask, STTranslStatsGrouped* grpdStats, STTranslStatsPerTask* grpStatsPerTask){
	STTranslStat r; TranslStat_init(&r);
	//Reset stats per task
	if(grpStatsPerTask != NULL){
		TranslStatsPerTask_empty(grpStatsPerTask);
	}
	//Process each task
	{
		SI32 i;
		for(i = 0; i < params->transTasksCount; i++){
			const STSrcTraslationTask* task = &params->transTasks[i];
			//Create (if necesary) the new pairs-list.
			while(strlib->pairsArrSize <= i){
				StringLib_openNewList(strlib);
			}
			//Create (if necesary) the new grpdStats for this task
			if(grpStatsPerTask != NULL){
				while(grpStatsPerTask->grpdStatsPerTask->conteo <=  i){
					STTranslStatsGrouped newGrpdStats;
					TranslStatsGrouped_init(&newGrpdStats);
					grpStatsPerTask->grpdStatsPerTask->agregarElemento(newGrpdStats);
				}
			}
			//Prepare stats container
			STTranslStatsGrouped* taskGrpStats = NULL;
			if(grpdStats != NULL || grpStatsPerTask != NULL){
				taskGrpStats = (STTranslStatsGrouped*)NBGestorMemoria::reservarMemoria(sizeof(STTranslStatsGrouped), ENMemoriaTipo_General);
				TranslStatsGrouped_init(taskGrpStats);
			}
			//Process
			StringLib_setFillingList(strlib, i);
			const STTranslStat folderStats = Parser_processFolder(params, strlib, parsingActionsMask, taskGrpStats, task, "/", "/", task->defaultPriorityWeight, task->defaultAction, task->defaultOptionsMask);
			TranslStat_add(&r, &folderStats);
			//
			if(taskGrpStats != NULL){
				if(grpdStats != NULL) TranslStatsGrouped_add(grpdStats, taskGrpStats);
				if(grpStatsPerTask != NULL) TranslStatsGrouped_add(grpStatsPerTask->grpdStatsPerTask->elemPtr(i), taskGrpStats);
				//
				TranslStatsGrouped_finalize(taskGrpStats);
				NBGestorMemoria::liberarMemoria(taskGrpStats);
				taskGrpStats = NULL;
			}
		}
	}
	return r;
}

STTranslStat Parser_processFolder(const STSrcTranslatorParams* params, STStringsLib* strlib, const SI32 parsingActionsMask, STTranslStatsGrouped* grpdStats, const STSrcTraslationTask* task, const char* virtualPathOrg, const char* virtualPathDst, const SI32 defaultPriorityWeight, const ENSrcAction defaultAction, const UI32 defaultOptionsMask){
	STTranslStat r; TranslStat_init(&r);
	AUCadena8* fullpathSrc = new AUCadena8(task->srcFolder, virtualPathOrg);
	AUCadena8* fullpathDst = new AUCadena8(task->dstFolder, virtualPathDst);
	//Validation
	if(fullpathSrc->esIgual(fullpathDst->str())){
		//Not allowed, same output folder
		PRINTF_ERROR("ERROR, ignoring folder '%s' (source and destinarion are the same).\n", fullpathSrc->str());
	} else {
		//Create destination folder
		bool dstPathExists = false;
		if(NBGestorArchivos::existeCarpeta(fullpathDst->str())){
			dstPathExists = true;
		} else {
			//Create new path
			if((parsingActionsMask & TRANSLATE_ACTION_TRANSLATE) != 0){
				if(NBGestorArchivos::crearCarpeta(fullpathDst->str())){
					dstPathExists = true;
				} else {
					PRINTF_ERROR("ERROR, could not create destination folder.\n");
				}
			}
		}
		//
		AUCadenaLargaMutable8* longStrTmp = new AUCadenaLargaMutable8();
		AUCadenaMutable8* onlyNameTmp = new AUCadenaMutable8();
		AUCadenaMutable8* onlyExtTmp = new AUCadenaMutable8();
		AUCadenaMutable8* virtualPathTmp = new AUCadenaMutable8();
		AUCadenaMutable8* pathTmp = new AUCadenaMutable8();
		AUCadenaMutable8* pathTmp2 = new AUCadenaMutable8();
		AUArregloMutable* subfiles = new AUArregloMutable();
		AUArregloMutable* subfolders = new AUArregloMutable();
		NBGestorArchivos::listarSubcarpetasDe(fullpathSrc->str(), subfolders);
		NBGestorArchivos::listarArchivosDe(fullpathSrc->str(), subfiles);
		UI32 i;
		//Process subfiles
		for(i = 0; i < subfiles->conteo; i++){
			AUCadena8* fileName = (AUCadena8*)subfiles->elem(i);
			ENSrcAction action = defaultAction;
			ENSrcFileFormat fileFormatType = ENSrcFileFormat_plain;
			UI32 optionsMask = defaultOptionsMask;
			SI32 priorityWeight = defaultPriorityWeight;
			//Build virtual path
			virtualPathTmp->vaciar();
			virtualPathTmp->agregar((virtualPathOrg[0] == '/' ? &virtualPathOrg[1] : &virtualPathOrg[0]));
			virtualPathTmp->agregar(fileName->str());
			//Get extension
			{
				onlyExtTmp->vaciar();
				onlyNameTmp->vaciar();
				const SI32 fileNameSz = fileName->tamano();
				const SI32 lastDotPos = fileName->ultimoIndiceDe('.');
				if(lastDotPos != -1){
					if((lastDotPos + 1) < fileNameSz){
						onlyExtTmp->agregar(&fileName->str()[lastDotPos + 1]);
						onlyNameTmp->agregar(fileName->str(), lastDotPos); //filename has extension
					} else {
						onlyNameTmp->agregar(fileName->str(), fileNameSz - 1); //filename ends with dot
					}
				} else {
					onlyNameTmp->agregar(fileName->str(), fileNameSz); //filename has no extension
				}
			}
			//Determine action to execute to file:
			{
				//Search at Subpaths-list
				{
					SI32 i;
					for(i = 0; i < task->pathsConfgCount; i++){
						const STSrcPathConfig* pathConfg = &task->pathsConfg[i];
						if(AUCadena8::cadenasSonIguales(virtualPathTmp->str(), pathConfg->virtualPath)){
							if(pathConfg->priorityWeight >= priorityWeight){
								action = pathConfg->action;
								optionsMask = pathConfg->optionsMask;
								priorityWeight = pathConfg ->priorityWeight;
								break;
							}
						}
					}
				}
				//Search at global-extensions list
				{
					SI32 i;
					for(i = 0; i < params->extsConfgCount; i++){
						const STSrcExtConfig* extConfig = &params->extsConfg[i];
						if(AUCadena8::cadenasSonIguales(onlyExtTmp->str(), extConfig->extension)){
							fileFormatType = extConfig->fileFormat;
							if(extConfig->priorityWeight >= priorityWeight){
								action = extConfig->action;
								optionsMask = extConfig->optionsMask;
								priorityWeight = extConfig->priorityWeight;
								break;
							}
						}
					}
				}
				//Search at task-extensions list
				{
					SI32 i;
					for(i = 0; i < task->extraExtsConfgCount; i++){
						const STSrcExtConfig* extConfig = &task->extraExtsConfg[i];
						if(AUCadena8::cadenasSonIguales(onlyExtTmp->str(), extConfig->extension)){
							fileFormatType = extConfig->fileFormat;
							if(extConfig->priorityWeight >= priorityWeight){
								action = extConfig->action;
								optionsMask = extConfig->optionsMask;
								priorityWeight = extConfig->priorityWeight;
								break;
							}
							
						}
					}
				}
			}
			//Find global stats
			STTranslStatGrp* extStats = NULL;
			STTranslStatGrp* pathStats = NULL;
			if(grpdStats != NULL){
				//By extension
				{
					STTranslStatGrp srchStat;
					srchStat.grpName = (char*)onlyExtTmp->str();
					const SI32 iFound = grpdStats->byExt->indiceDe(srchStat);
					if(iFound != -1){
						extStats = grpdStats->byExt->elemPtr(iFound);
					} else {
						STTranslStatGrp newStat;
						TranslStatGrp_init(&newStat, onlyExtTmp->str());
						const UI32 iPos = grpdStats->byExt->agregarElemento(newStat);
						extStats = grpdStats->byExt->elemPtr(iPos);
					}
				}
				//By file path
				{
					STTranslStatGrp srchStat;
					srchStat.grpName = (char*)virtualPathTmp->str();
					const SI32 iFound = grpdStats->byFile->indiceDe(srchStat);
					if(iFound != -1){
						pathStats = grpdStats->byFile->elemPtr(iFound);
					} else {
						STTranslStatGrp newStat;
						TranslStatGrp_init(&newStat, virtualPathTmp->str());
						const UI32 iPos = grpdStats->byFile->agregarElemento(newStat);
						pathStats = grpdStats->byFile->elemPtr(iPos);
					}
				}
			}
			//Process file
			{
				//Build origin-full-path
				pathTmp->vaciar();
				pathTmp->agregar(fullpathSrc->str());
				pathTmp->agregar(fileName->str());
				//Build destination-full-path
				pathTmp2->vaciar();
				pathTmp2->agregar(fullpathDst->str());
				pathTmp2->agregar(fileName->str());
				//Translated destination-full-path
				if((defaultOptionsMask & TRANSLATE_OPT_TRANSLATE_OUTPUT_FILENAME) != 0){
					if(onlyNameTmp->tamano() > 0){
						longStrTmp->establecer(onlyNameTmp->str());
						const STStrLibIndex indx = StringLib_indexOfOrig(strlib, longStrTmp, 0);
						if(indx.iArray != 0 || indx.iItem != 0){
							STStringPair* pair = strlib->pairsArr[indx.iArray].pairs->elemPtr(indx.iItem);
							if(pair->iNew > 0){
								const char* newStr = &strlib->pairsArr[indx.iArray].strings->str()[pair->iNew];
								pathTmp2->vaciar();
								pathTmp2->agregar(fullpathDst->str());
								pathTmp2->agregar(newStr);
								pathTmp2->agregar(&fileName->str()[onlyNameTmp->tamano()]);
							}
						}
					}
				}
				//
				if(action == ENSrcAction_ignore){
					PRINTF_INFO("File ignored: '%s'.\n", pathTmp->str());
					r.countFilesIgnore++;
					if(extStats != NULL) extStats->stats.countFilesIgnore++;
					if(pathStats != NULL) pathStats->stats.countFilesIgnore++;
				} else {
					//Open/create destination file
					FILE* fileDst = NULL;
					if(dstPathExists){
						if((parsingActionsMask & TRANSLATE_ACTION_TRANSLATE) != 0){
							fileDst = fopen(pathTmp2->str(), "wb");
						}
					}
					if(action == ENSrcAction_copy){
						if(fileDst != NULL){
							if(!Parser_processFileCopy(pathTmp->str(), fileDst)){
								PRINTF_ERROR("ERROR copying file: '%s'.\n", pathTmp->str());
							} else {
								PRINTF_INFO("File copied: '%s'.\n", pathTmp->str());
							}
						}
						r.countFilesCopy++;
						if(extStats != NULL) extStats->stats.countFilesCopy++;
						if(pathStats != NULL) pathStats->stats.countFilesCopy++;
					} else {
						//Add file name (remove extension)
						if((parsingActionsMask & TRANSLATE_ACTION_COLLECT_WORDS) != 0){
							longStrTmp->establecer(onlyNameTmp->str());
							StringLib_addOrUpdate(strlib, longStrTmp, 0, ENStringRelevance_fileName, fileName->str(), true);
						}
						//Translate
						const UI32 strLibSizeBefore = StringLib_size(strlib);
						STParsingFileState fileState;
						ParsingFileState_init(&fileState, fileName->str(), (fileDst != NULL), true);
						if(!Parser_processFileParseStringsSeqs(&fileState, strlib, fileFormatType, parsingActionsMask, optionsMask, pathTmp->str())){
							PRINTF_ERROR("ERROR parsing lines and strings sequences of file: '%s' (as '%s').\n", pathTmp->str(), __parserSrcFormatMap[fileFormatType].name);
						} else {
							const UI32 strLibSizeAfter = StringLib_size(strlib);
							if((parsingActionsMask & TRANSLATE_ACTION_TRANSLATE) == 0){
								PRINTF_INFO("File parsed: '%s' (as %s, %d new word pairs, %d lines).\n", pathTmp->str(), __parserSrcFormatMap[fileFormatType].name, (strLibSizeAfter - strLibSizeBefore), fileState.stats->countLinesRead);
							} else {
								const UI32 translCount = Parser_processStringsTranslate(&fileState, strlib, fileFormatType, optionsMask);
								PRINTF_INFO("File translated: '%s' (as '%s', %d new words found, %d words translated, %d lines).\n", pathTmp->str(), __parserSrcFormatMap[fileFormatType].name, (strLibSizeAfter - strLibSizeBefore), translCount, fileState.stats->countLinesRead);
							}
						}
						ParsingFileState_flushToFile(&fileState, fileDst);
						//Acumulate stats
						TranslStat_add(&r, fileState.stats);
						if(extStats != NULL) TranslStat_add(&extStats->stats, fileState.stats);
						if(pathStats != NULL) TranslStat_add(&pathStats->stats, fileState.stats);
						//
						ParsingFileState_finalize(&fileState);
						r.countFilesProcess++;
						if(extStats != NULL) extStats->stats.countFilesProcess++;
						if(pathStats != NULL) pathStats->stats.countFilesProcess++;
					}
					if(fileDst != NULL){
						fclose(fileDst);
						fileDst = NULL;
					}
				}
			}
			r.countFiles++;
			if(extStats != NULL) extStats->stats.countFiles++;
			if(pathStats != NULL) pathStats->stats.countFiles++;
		}
		//Process subfolders
		for(i = 0; i < subfolders->conteo; i++){
			ENSrcAction action = defaultAction; //ENSrcAction_process;
			UI32 optionsMask = defaultOptionsMask;
			SI32 priorityWeight = defaultPriorityWeight;
			AUCadena8* folderName = (AUCadena8*)subfolders->elem(i);
			//Build virtual path
			virtualPathTmp->vaciar();
			virtualPathTmp->agregar((virtualPathOrg[0] == '/' ? &virtualPathOrg[1] : &virtualPathOrg[0]));
			virtualPathTmp->agregar(folderName->str());
			//Search at paths-list
			{
				SI32 i;
				for(i = 0; i < task->pathsConfgCount; i++){
					const STSrcPathConfig* pathConfg = &task->pathsConfg[i];
					if(AUCadena8::cadenasSonIguales(virtualPathTmp->str(), pathConfg->virtualPath)){
						if(pathConfg->priorityWeight >= priorityWeight){
							action = pathConfg->action;
							optionsMask = pathConfg->optionsMask;
							priorityWeight = pathConfg->priorityWeight;
							break;
						}
					}
				}
			}
			//Build origin-virtual-path
			pathTmp->vaciar();
			pathTmp->agregar(virtualPathOrg);
			pathTmp->agregar(folderName->str());
			pathTmp->agregar("/");
			//Build destination-virtual-path
			pathTmp2->vaciar();
			pathTmp2->agregar(virtualPathDst);
			pathTmp2->agregar(folderName->str());
			pathTmp2->agregar("/");
			//Translated destination-virtual-path
			if((defaultOptionsMask & TRANSLATE_OPT_TRANSLATE_OUTPUT_FILENAME) != 0){
				if(onlyNameTmp->tamano() > 0){
					longStrTmp->establecer(onlyNameTmp->str());
					const STStrLibIndex indx = StringLib_indexOfOrig(strlib, longStrTmp, 0);
					if(indx.iArray != 0 || indx.iItem != 0){
						STStringPair* pair = strlib->pairsArr[indx.iArray].pairs->elemPtr(indx.iItem);
						if(pair->iNew > 0){
							const char* newStr = &strlib->pairsArr[indx.iArray].strings->str()[pair->iNew];
							pathTmp2->vaciar();
							pathTmp2->agregar(virtualPathDst);
							pathTmp2->agregar(newStr);
							pathTmp2->agregar("/");
						}
					}
				}
			}
			//Process
			const STTranslStat subfolderStats = Parser_processFolder(params, strlib, parsingActionsMask, grpdStats, task, pathTmp->str(), pathTmp2->str(), priorityWeight, action, optionsMask);
			//Find global stats
			STTranslStatGrp* pathStats = NULL;
			if(grpdStats != NULL){
				//By file path
				{
					STTranslStatGrp srchStat;
					srchStat.grpName = (char*)virtualPathTmp->str();
					const SI32 iFound = grpdStats->byFolder->indiceDe(srchStat);
					if(iFound != -1){
						pathStats = grpdStats->byFolder->elemPtr(iFound);
					} else {
						STTranslStatGrp newStat;
						TranslStatGrp_init(&newStat, virtualPathTmp->str());
						const UI32 iPos = grpdStats->byFolder->agregarElemento(newStat);
						pathStats = grpdStats->byFolder->elemPtr(iPos);
					}
				}
			}
			//Acumulate stats
			TranslStat_add(&r, &subfolderStats);
			if(pathStats != NULL) TranslStat_add(&pathStats->stats, &subfolderStats);
			//Add folder name (if any internal file was processed)
			if((parsingActionsMask & TRANSLATE_ACTION_COLLECT_WORDS) != 0){
				if(subfolderStats.countFilesProcess != 0){
					longStrTmp->establecer(folderName->str());
					StringLib_addOrUpdate(strlib, longStrTmp, 0, ENStringRelevance_folderName, virtualPathTmp->str(), true);
				}
			}
		}
		//
		pathTmp2->liberar();
		pathTmp->liberar();
		virtualPathTmp->liberar();
		onlyExtTmp->liberar();
		onlyNameTmp->liberar();
		longStrTmp->liberar();
		//
		subfolders->liberar();
		subfiles->liberar();
		//
		fullpathDst->liberar();
		fullpathSrc->liberar();
	}
	return r;
}

bool Parser_processFileCopy(const char* filePath, FILE* fileDst){
	bool r = false;
	if(fileDst != NULL){
		FILE* file = fopen(filePath, "rb");
		if(file != NULL){
			BYTE buffer[PARSER_BUFFER_SIZE];
			size_t bytesRead = fread(buffer, 1, PARSER_BUFFER_SIZE, file);
			while(bytesRead > 0){
				fwrite(buffer, 1, bytesRead, fileDst);
				bytesRead = fread(buffer, 1, PARSER_BUFFER_SIZE, file);
			}
			fclose(file);
			r =true;
		}
	}
	return r;
}

ENStringRelevance Parser_wordRelevance(const char* wordBefore, const char currentChar){
	ENStringRelevance r = ENStringRelevance_none;
	if(AUCadenaLarga8::esIgual(wordBefore, "class")){
		//ex: " ... class AUSomething ... "
		r = ENStringRelevance_className;
	} else if(AUCadenaLarga8::esIgual(wordBefore, "struct")){
		//ex: " ... struct STSomething ... "
		r = ENStringRelevance_structName;
	} else if(AUCadenaLarga8::esIgual(wordBefore, "enum")){
		//ex: " ... enum ENSomething ... "
		r = ENStringRelevance_enumName;
	} else if(AUCadenaLarga8::esIgual(wordBefore, "define") || AUCadenaLarga8::esIgual(wordBefore, "ifdef") || AUCadenaLarga8::esIgual(wordBefore, "ifndef")){
		//ex: " ... define SOMETHING ... " or " ... ifdef SOMETHING ... " or " ... ifndef SOMETHING ... "
		r = ENStringRelevance_macro;
	} else if(currentChar == '('){
		//ex: " ... something( ... "
		r = ENStringRelevance_memberName;
	}
	return r;
}

bool Parser_processStringsSeqsInit(STParsingFileState* fileState, STStringsLib* strlib, const SI32 parsingActionsMask, const UI32 parsingOptionsMask){
	fileState->charCurrnt			= '\0';
	fileState->charBefore			= '\0';
	fileState->literalCharScaped	= false;
	//
	fileState->readingBlqType		= ENTranslatedStrType_Code;
	fileState->readingBlqTypeDefIndex = -1;
	fileState->readingStrAcum->vaciar();
	fileState->readingLastValidWord->vaciar();
	return true;
}

bool Parser_processStringsSeqsFromBuffer(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask, const BYTE* buffer, const UI32 bufferInitialPos, const UI32 bufferBytesData, const bool processAllAsCode){
	bool r = false;
	{
		UI32 bufferPos = bufferInitialPos;
		char cBefore = fileState->charBefore;
		STFileFormatMap* formatMap = NULL;
		if(formatType < ENSrcFileFormat_count){
			formatMap = &__parserSrcFormatMap[formatType]; NBASSERT(formatMap->format == formatType)
		}
		STTranslatedLine* currLine = ParsingFileState_lastLine(fileState); //can return NULL
		if(processAllAsCode){
			fileState->readingBlqType = ENTranslatedStrType_Code;
			fileState->readingBlqTypeDefIndex = -1;
		}
		//Process buffer
		while(bufferPos < bufferBytesData){
			const char c = fileState->charCurrnt = buffer[bufferPos++];
			switch (fileState->readingBlqType) {
				case ENTranslatedStrType_LiteralSingle: // 'content \'and\' more'
					if((cBefore != '\\' || fileState->literalCharScaped) && c == '\''){
						//Add to strlib
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_literal, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						fileState->readingBlqType = ENTranslatedStrType_Code;
						fileState->readingBlqTypeDefIndex = -1;
						//Process char
						fileState->readingStrAcum->agregar(c);
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
					} else {
						//Avoid cases like 'hi \\my friend\\' (the single-quote at the end can be confused as "\'")
						fileState->literalCharScaped = ((cBefore == '\\' && !fileState->literalCharScaped) && c == '\\');
						fileState->readingStrAcum->agregar(c);
					}
					break;
				case ENTranslatedStrType_LiteralDouble: // "content \"and\" more"
					if((cBefore != '\\' || fileState->literalCharScaped) && c == '\"'){
						//Add to strlib
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_literal, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						fileState->readingBlqType = ENTranslatedStrType_Code;
						fileState->readingBlqTypeDefIndex = -1;
						//Process char
						fileState->readingStrAcum->agregar(c);
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
					} else {
						//Avoid cases like "hi \\my friend\\" (the double-quote at the end can be confused as "\"")
						fileState->literalCharScaped = ((cBefore == '\\' && !fileState->literalCharScaped) && c == '\\');
						fileState->readingStrAcum->agregar(c);
					}
					break;
				case ENTranslatedStrType_CommentMonoLine: // //content
					if(c == '\n'){
						//Remove '\r'
						bool crRemoved = false;
						if(cBefore == '\r'){
							crRemoved = true;
							fileState->readingStrAcum->quitarDerecha(1);
						}
						//Add to strlib
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_comment, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						fileState->readingBlqType = ENTranslatedStrType_Code;
						fileState->readingBlqTypeDefIndex = -1;
						//Process char
						if(crRemoved){
							fileState->readingStrAcum->agregar('\r');
							Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						}
						fileState->readingStrAcum->agregar(c);
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
					} else {
						fileState->readingStrAcum->agregar(c);
					}
					break;
				case ENTranslatedStrType_CommentMultiLine: // /*content*/
					{
						NBASSERT(formatMap != NULL)
						NBASSERT(fileState->readingBlqTypeDefIndex >= 0)
						const STCommMultiLimits* limits = &formatMap->multilinesCommLimits[fileState->readingBlqTypeDefIndex];
						if(((cBefore == limits->end[0] && c == limits->end[1] && limits->end[2] == '\0') || (c == limits->end[0] && limits->end[1] == '\0'))
						   && fileState->readingStrAcum->tamano() > 0 //2016-07-13: added to avoid considering case "/*/" as an mixed opening/closing multiline commentary (instead of "/**/").
						   ){
							//Remove '*'
							fileState->readingStrAcum->quitarDerecha(1);
							//Add to strlib
							Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_comment, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
							fileState->readingBlqType = ENTranslatedStrType_Code;
							fileState->readingBlqTypeDefIndex = -1;
							//Process char
							fileState->readingStrAcum->agregar('*');
							Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
							fileState->readingStrAcum->agregar('/');
							Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						} else {
							fileState->readingStrAcum->agregar(fileState->charCurrnt);
						}
					}
					break;
				default:
					NBASSERT(fileState->readingBlqType == ENTranslatedStrType_Code)
					if((c >= 48 && c <= 57) //ASCII 0-9
					   || (c >= 65 && c <= 90) //ASCII A-Z
					   || (c >= 97 && c <= 122) //ASCII a-z
					   || c == 95 //ASCII '_'
					   ){
						fileState->readingStrAcum->agregar(c);
					} else {
						ENStringRelevance newRelevance = ENStringRelevance_none;
						if((parsingActionsMask & TRANSLATE_ACTION_COLLECT_WORDS) != 0){
							newRelevance = Parser_wordRelevance(fileState->readingLastValidWord->str(), c);
						}
						//Flush buffer
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, newRelevance, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						//Process this char
						fileState->readingStrAcum->agregar(c);
						Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, ENStringRelevance_none, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
						//
						if(!processAllAsCode){
							if(c == '\''){
								fileState->readingBlqType = ENTranslatedStrType_LiteralSingle;
								fileState->literalCharScaped = false;
							} else if(c == '\"'){
								fileState->readingBlqType = ENTranslatedStrType_LiteralDouble;
								fileState->literalCharScaped = false;
							} else {
								if(formatMap != NULL){
									//Analyze monoline comments start
									if(fileState->readingBlqType == ENTranslatedStrType_Code){
										const STCommMonoLimits* limitsDef = formatMap->monolinesCommLimits;
										while(limitsDef->start[0] != '\0'){
											if((cBefore == limitsDef->start[0] && c == limitsDef->start[1] && limitsDef->start[2] == '\0')
											   || (c == limitsDef->start[0] && limitsDef->start[1] == '\0')){
												fileState->readingBlqType = ENTranslatedStrType_CommentMonoLine;
												fileState->readingBlqTypeDefIndex = (SI32)(limitsDef - formatMap->monolinesCommLimits);
												fileState->literalCharScaped = false;
												break;
											}
											limitsDef++;
										}
									}
									//Analyze multiline comment start
									if(fileState->readingBlqType == ENTranslatedStrType_Code){
										const STCommMultiLimits* limitsDef = formatMap->multilinesCommLimits;
										while(limitsDef->start[0] != '\0'){
											if((cBefore == limitsDef->start[0] && c == limitsDef->start[1] && limitsDef->start[2] == '\0')
											   || (c == limitsDef->start[0] && limitsDef->start[1] == '\0')){
												fileState->readingBlqType = ENTranslatedStrType_CommentMultiLine;
												fileState->readingBlqTypeDefIndex = (SI32)(limitsDef - formatMap->multilinesCommLimits);
												fileState->literalCharScaped = false;
												break;
											}
											limitsDef++;
										}
									}
								}
							}
							/*} else if(cBefore == '/' && c == '/'){
								fileState->readingBlqType = ENTranslatedStrType_CommentMonoLine;
							} else if(cBefore == '/' && c == '*'){
								fileState->readingBlqType = ENTranslatedStrType_CommentMultiLine;
							}*/
						}
					}
					break;
			}
			//Start new line
			if(c == '\n'){
				currLine = ParsingFileState_addNewLine(fileState); //can return NULL
				if(fileState->stats != NULL){
					fileState->stats->countLinesRead++;
				}
			}
			//
			fileState->charBefore = cBefore = c;
			//Update stats
			if(fileState->stats != NULL){
				fileState->stats->countBytesRead++;
			}
		}
		r = true;
	}
	return r;
}

bool Parser_processStringsSeqsFinish(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask){
	STTranslatedLine* currLine = ParsingFileState_lastLine(fileState); //can return NULL
	//Process last word
	{
		ENStringRelevance newRelevance = ENStringRelevance_none;
		if((parsingActionsMask & TRANSLATE_ACTION_COLLECT_WORDS) != 0){
			newRelevance = Parser_wordRelevance(fileState->readingLastValidWord->str(), fileState->charBefore);
		}
		Parser_processStringsFlush(fileState, strlib, formatType, parsingActionsMask, currLine, fileState->readingBlqType, newRelevance, fileState->readingLastValidWord, fileState->readingStrAcum, parsingOptionsMask);
	}
	//Acumulate last line (if not empty)
	if(fileState->stats != NULL && currLine != NULL){
		if(currLine->strSeq->conteo != 0){
			fileState->stats->countLinesRead++;
		}
	}
	return true;
}

bool Parser_processFileParseStringsSeqs(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask, const char* filePath){
	bool r  = false;
	FILE* file = fopen(filePath, "rb");
	if(file != NULL){
		BYTE buffer[PARSER_BUFFER_SIZE];
		NBASSERT(fileState->charCurrnt == '\0' && fileState->charBefore == '\0' && fileState->literalCharScaped == false)
		//Start parsing variables
		Parser_processStringsSeqsInit(fileState, strlib, parsingActionsMask, parsingOptionsMask);
		while(1){
			//Load data to buffer
			const size_t bytesRead = fread(buffer, 1, PARSER_BUFFER_SIZE, file);
			if(bytesRead <= 0){
				break;
			}
			//Process buffer
			Parser_processStringsSeqsFromBuffer(fileState, strlib, formatType, parsingActionsMask, parsingOptionsMask, buffer, 0, (UI32)bytesRead, false);
		}
		//Process last word
		Parser_processStringsSeqsFinish(fileState, strlib, formatType, parsingActionsMask, parsingOptionsMask);
		//
		fclose(file);
		r = true;
	}
	return r;
}

UI32 Parser_processStringsTranslate(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const UI32 parsingOptionsMask){
	UI32 r = 0;
	if(fileState->linesSeq != NULL && fileState->linesStorage != NULL){
		AUCadenaLargaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaLargaMutable8();
		SI32 i; const SI32 linesCount = fileState->linesSeq->conteo;
		const bool translateOnlyPaths = ((parsingOptionsMask & TRANSLATE_OPT_TRANSLATE_ONLY_PATHS) != 0);
		for(i = 0 ; i < linesCount; i++){
			STTranslatedLine* line = fileState->linesSeq->elemPtr(i);
			SI32 j; const SI32 strsCount = line->strSeq->conteo;
			for(j = 0 ; j < strsCount; j++){
				STTranslatedString* strTrans = line->strSeq->elemPtr(j);
				const bool isLiteralBlock = (strTrans->origType == ENTranslatedStrType_LiteralSingle || strTrans->origType == ENTranslatedStrType_LiteralDouble);
				const bool isCommentBlock = (strTrans->origType == ENTranslatedStrType_CommentMonoLine || strTrans->origType == ENTranslatedStrType_CommentMultiLine);
				//Is reserved word?
				bool isReservedWord = false;
				if(formatType < ENSrcFileFormat_count && !isLiteralBlock && !isCommentBlock){
					const STFileFormatMap* map = &__parserSrcFormatMap[formatType]; NBASSERT(map->format == formatType)
					if(StringsPairs_indexOfOrig(&map->reservedWords, fileState->linesStorage, strTrans->iOrig) != 0){
						isReservedWord = true;
						//PRINTF_INFO("Ignoring reserved word '%s'.\n", &fileState->linesStorage->str()[strTrans->iOrig]);
					}
				}
				//Translate
				if(!isReservedWord){
					//Direct translation
					bool translated = false;
					const STStrLibIndex indx = StringLib_indexOfOrig(strlib, fileState->linesStorage, strTrans->iOrig);
					if(indx.iArray != 0 || indx.iItem != 0){
						STStringPair* pair = strlib->pairsArr[indx.iArray].pairs->elemPtr(indx.iItem);
						if(pair->iNew > 0 && !pair->ignore){
							if(!translateOnlyPaths || pair->isPathWord){
								const char* newStr = &strlib->pairsArr[indx.iArray].strings->str()[pair->iNew];
								TranslatedLine_updateStrSeqFinal(line, fileState->linesStorage, j, newStr);
								r++;
								translated = true;
							}
						}
					}
					//Translate literal block (recursive-once)
					if(!translated
					   && (
						   (isLiteralBlock && (parsingOptionsMask & (TRANSLATE_OPT_TRANSLATE_STRS_IN_LITERALS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS)) != 0)
						   || (isCommentBlock && (parsingOptionsMask & (TRANSLATE_OPT_TRANSLATE_STRS_IN_COMMENTS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS)) != 0)
						   )
					   ){
						const bool translateOnlyPaths = ((isLiteralBlock && (parsingOptionsMask & TRANSLATE_OPT_TRANSLATE_STRS_IN_LITERALS) == 0) || (isCommentBlock && (parsingOptionsMask & TRANSLATE_OPT_TRANSLATE_STRS_IN_COMMENTS) == 0));
						const char* strBlock = &fileState->linesStorage->str()[strTrans->iOrig];
						const SI32 strBlockSz = AUCadenaLarga8::tamano(strBlock);
						STParsingFileState sublockState;
						//Init
						AUCadenaMutable8* strInternalRelPath = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
						strInternalRelPath->agregar(fileState->curFileName);
						strInternalRelPath->agregar("::literalBlock");
						ParsingFileState_init(&sublockState, strInternalRelPath->str(), true, true);
						//Parse all words
						Parser_processStringsSeqsInit(&sublockState, strlib, TRANSLATE_ACTION_NONE, TRANSLATE_OPT_NONE);
						Parser_processStringsSeqsFromBuffer(&sublockState, strlib, ENSrcFileFormat_plain, TRANSLATE_ACTION_NONE, TRANSLATE_OPT_NONE, (const BYTE*)strBlock, 0, strBlockSz, true);
						Parser_processStringsSeqsFinish(&sublockState, strlib, ENSrcFileFormat_plain, TRANSLATE_ACTION_NONE, TRANSLATE_OPT_NONE);
						//Translate words
						const UI32 translCount = Parser_processStringsTranslateSubBlock(&sublockState, strlib, translateOnlyPaths);
						if(translCount != 0){
							//Build and apply new value
							strTmp->vaciar();
							SI32 i; const SI32 linesCount = sublockState.linesSeq->conteo;
							for(i = 0 ; i < linesCount; i++){
								STTranslatedLine* line = sublockState.linesSeq->elemPtr(i);
								SI32 j; const SI32 strsCount = line->strSeq->conteo;
								for(j = 0 ; j < strsCount; j++){
									STTranslatedString* strTrans = line->strSeq->elemPtr(j);
									const char* strBlock = &sublockState.linesStorage->str()[strTrans->iFinal];
									NBASSERT(strTrans->origType == ENTranslatedStrType_Code) //Only code-type is allowed inside a literal block, recursivity risk.
									strTmp->agregar(strBlock);
								}
							}
							TranslatedLine_updateStrSeqFinal(line, fileState->linesStorage, j, strTmp->str());
							r++;
						}
						//Finish
						strInternalRelPath->liberar();
						ParsingFileState_finalize(&sublockState);
					}
				}
			}
		}
		strTmp->liberar();
		strTmp = NULL;
	}
	return r;
}

UI32 Parser_processStringsTranslateSubBlock(STParsingFileState* fileState, STStringsLib* strlib, const bool onlyPaths){
	UI32 r = 0;
	if(fileState->linesSeq != NULL && fileState->linesStorage != NULL){
		SI32 i; const SI32 linesCount = fileState->linesSeq->conteo;
		for(i = 0 ; i < linesCount; i++){
			STTranslatedLine* line = fileState->linesSeq->elemPtr(i);
			SI32 j; const SI32 strsCount = line->strSeq->conteo;
			for(j = 0 ; j < strsCount; j++){
				STTranslatedString* strTrans = line->strSeq->elemPtr(j);
				const STStrLibIndex indx = StringLib_indexOfOrig(strlib, fileState->linesStorage, strTrans->iOrig);
				//Direct translation
				if(indx.iArray != 0 || indx.iItem != 0){
					STStringPair* pair = strlib->pairsArr[indx.iArray].pairs->elemPtr(indx.iItem);
					if(pair->iNew > 0 && !pair->ignore){
						if(!onlyPaths || pair->isPathWord){
							const char* newStr = &strlib->pairsArr[indx.iArray].strings->str()[pair->iNew];
							TranslatedLine_updateStrSeqFinal(line, fileState->linesStorage, j, newStr);
							r++;
						}
					}
				}
			}
		}
	}
	return r;
}













