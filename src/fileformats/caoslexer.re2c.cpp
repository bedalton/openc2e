#include "caoslexer.h"
#include "caostoken.h"

#include <algorithm>

void lexcaos(std::vector<caostoken>& v, const std::string& p) {
	return lexcaos(v, p.c_str());
}

void lexcaos(std::vector<caostoken>& v, const char* p) {
#define make_word(str) \
	v.push_back(caostoken(caostoken::TOK_WORD, str, yylineno))
	int yylineno = 1;
	const char *basep, *YYMARKER;
std:
	basep = p;
/*!re2c
	re2c:define:YYCTYPE = "unsigned char";
	re2c:define:YYCURSOR = p;
	re2c:yyfill:enable = 0;
	re2c:yych:conversion = 1;
	re2c:indent:top = 1;

	noneoi     = [\001-\377];
	eoi        = [\000];
	any        = noneoi | eoi;
	DIGIT      = [0-9];
	WORDINIT   = [a-zA-Z_];
	WORDANY    = [$#a-zA-Z0-9:?!_+-];
	SIGN       = [-+];

	("\r\n" | "\n") {
		v.push_back(caostoken(caostoken::TOK_NEWLINE, std::string(basep, p - basep), yylineno));
		++yylineno;
		goto std;
	}
	[ \r\t]+ {
		v.push_back(caostoken(caostoken::TOK_WHITESPACE, std::string(basep, p - basep), yylineno));
		goto std;
	}
	[,] {
		v.push_back(caostoken(caostoken::TOK_COMMA, std::string(basep, p - basep), yylineno));
		goto std;
	}
	"*" [^\r\n\000]* {
		v.push_back(caostoken(caostoken::TOK_COMMENT, std::string(basep, p - basep), yylineno));
		goto std;
	}
	"'" noneoi "'"  {
		v.push_back(caostoken(caostoken::TOK_CHAR, std::string(basep, p - basep), yylineno));
		goto std;
	}
	[fF] "**" [kK] {
		make_word(std::string(basep, p - basep));
		goto std;
	}
	WORDANY* WORDINIT WORDANY* {
		make_word(std::string(basep, p - basep));
		goto std;
	}
	"%" [01]* {
		v.push_back(caostoken(caostoken::TOK_BINARY, std::string(basep, p - basep), yylineno));
		goto std;
	}
	SIGN? DIGIT* "." DIGIT* {
		v.push_back(caostoken(caostoken::TOK_FLOAT, std::string(basep, p - basep), yylineno));
		goto std;
	}
	SIGN? DIGIT+  {
		v.push_back(caostoken(caostoken::TOK_INT, std::string(basep, p - basep), yylineno));
		goto std;
	}
	"<>" { make_word(std::string(basep, p - basep)); goto std; }
	"<=" { make_word(std::string(basep, p - basep)); goto std; }
	">=" { make_word(std::string(basep, p - basep)); goto std; }
	"<"  { make_word(std::string(basep, p - basep)); goto std; }
	">"  { make_word(std::string(basep, p - basep)); goto std; }
	"="  { make_word(std::string(basep, p - basep)); goto std; }

	"["  { goto bytestr; }
	"\"" { goto str; }
	eoi { goto out; }

	any { v.push_back(caostoken(caostoken::TOK_ERROR, yylineno)); goto out; }
*/
bytestr:
	/*!re2c
	[\000\r\n] { v.push_back(caostoken(caostoken::TOK_ERROR, yylineno)); goto out; }
	"]" { v.push_back(caostoken(caostoken::TOK_BYTESTR, std::string(basep, p - basep), yylineno)); goto std;  }
	noneoi { goto bytestr; }
*/

str:
/*!re2c
	[\000\r\n] { v.push_back(caostoken(caostoken::TOK_ERROR, yylineno)); goto out; }
	"\\" . { goto str; }
	"\"" { v.push_back(caostoken(caostoken::TOK_STRING, std::string(basep, p - basep), yylineno)); goto std;  }
	noneoi { goto str; }
*/
out:
	v.push_back(caostoken(caostoken::TOK_EOI, yylineno)); // EOI
	for (size_t i = 0; i < v.size(); i++)
		v[i].index = i;
}
