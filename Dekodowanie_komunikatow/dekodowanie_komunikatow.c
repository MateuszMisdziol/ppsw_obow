#define MAX_TOKEN_NR 3              //dopuszalna max ilosc tokenow
#define MAX_KEYWORD_STRING_LTH 10   //max dlugosc komendy (komunikatu, zdania?)
#define MAX_KEYWORD_NR 3            //max ilosc slow kluczowych (keyword)

typedef enum TokenType 
{KEYWORD, NUMBER, STRING} TokenType;

typedef enum KeywordCode 
{LD, ST, RST} KeywordCode;

typedef union TokenValue
{
	enum KeywordCode eKeyword;  //jezeli KEYWORD
	unsigned int uiNumber;      //jezeli NUMBER
	char * pcString;            //jezeli STRING
} TokenValue;

typedef struct Token
{
	enum TokenType eType;       //KEYWORD, NUMBER, STRING
	union TokenValue uValue;    //enum, unsigned int, char *
} Token;

struct Token asToken[MAX_TOKEN_NR];

typedef struct Keyword
{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

struct Keyword asKeywordList[MAX_KEYWORD_NR]=
{
	{RST, "reset"}, {LD, "load"}, {ST, "store"}
};

typedef enum Result
{OK, ERROR} Result;

typedef enum CompResult 
{ DIFFERENT , EQUAL } CompResult;

unsigned char ucTokenNr = 0;

//----------------------FUNKCJE----------------------------

unsigned char ucFindTokensInString(char *pcString){

	unsigned char ucTokenPointer;
	char cCurrentChar;

	enum eState {TOKEN, DELIMITER};

	for (ucTokenPointer = 0;; ++ucTokenPointer){

		cCurrentChar = pcString[ucTokenPointer];

		switch(eState){
			case TOKEN:
				if(cCurrentChar == '\0'){
					return ucTokenNr;
				}
				else if(cCurrentChar != ' '){}
				else if(ucTokenNr == MAX_TOKEN_NR){
					return ucTokenNr;
				}
				else{
					eState = DELIMITER;
				}
				break;
			case DELIMITER:
				if(cCurrentChar == '\0'){
					return ucTokenNr;
				}
				else if(cCurrentChar == ' '){}
				else{
					eState = TOKEN;
					asToken[ucTokenNr].uValue.pcString = pcString + ucTokenPointer;
					ucTokenNr++;
				}
				break;
		}
	}
}

enum eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode){

	unsigned char ucTokenCounter;

	for (ucTokenCounter = 0; ucTokenCounter < MAX_TOKEN_NR; ++ucTokenCounter)
	{
		if(eCompareString(pcStr, asKeywordList[ucTokenCounter]) == EQUAL){
			*peKeywordCode = asKeywordList[ucTokenCounter].eCode;
			return OK;
		}
		else{
			return ERROR;
		}
	}
}

void DecodeTokens(void){

	unsigned char ucTokenCounter;

	Token *tValue;

	for (ucTokenCounter = 0; ucTokenCounter < MAX_TOKEN_NR; ++ucTokenCounter)
	{
		tValue = &asToken[ucTokenCounter];
		if(eStringToKeyword(tValue->uValue.pcString, &tValue->uValue.eKeyword) == OK){
			tValue->eType == KEYWORD;
		}
		else if(eHexStringToUInt(tValue->uValue.pcString, &tValue->uValue.uiNumber) == OK){
			tValue->eType == NUMBER; 
		}
		else{
			tValue->eType == STRING;
		}
	}
}