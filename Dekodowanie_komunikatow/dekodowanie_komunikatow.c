#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10
#define MAX_KEYWORD_NR 3

typedef enum TokenType
{KEYWORD, NUMBER, STRING} TokenType;

typedef enum KeywordCode
{LD, ST, RST} KeywordCode;

typedef union TokenValue
{
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char *pcString;
} TokenValue;

typedef struct Token
{
	enum TokenType eType;
	union TokenValue uValue;
} Token;

Token asToken[MAX_TOKEN_NR];

typedef struct Keyword
{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

Keyword asKeywordList[MAX_KEYWORD_NR] =
{
	{RST, "reset"}, {LD, "load"}, {ST, "store"}
};

typedef enum Result
{OK, ERROR} Result;

typedef enum CompResult
{ DIFFERENT , EQUAL } CompResult;

unsigned char ucTokenNr = 0;


unsigned char ucFindTokensInString(char *pcString){
  
  enum State {TOKEN, DELIMITER};
  enum State eState = DELIMITER;
  char cCurrentChar;
  unsigned char ucWhichToken;

  for(ucWhichToken = 0; ;ucWhichToken++){
    
    cCurrentChar = pcString[ucWhichToken];
    
    switch(eState){
      case TOKEN:
        if(cCurrentChar == '\0'){
          return ucTokenNr;
        }
        else if(ucTokenNr == MAX_TOKEN_NR){
          return ucTokenNr;
        }
        else if(cCurrentChar == ' '){
          eState = DELIMITER;
        }
        break;
      case DELIMITER:
        if(cCurrentChar == '\0'){
          return ucTokenNr;
        }
        else if(cCurrentChar != ' '){
          eState = TOKEN;
          asToken[ucTokenNr].uValue.pcString = pcString + ucWhichToken;
          ucTokenNr++;
        }
        break;
    }
  }
}




enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode){
  
  unsigned char ucKeywordCounter;
  
  for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++){
    if(eCompareString(pcStr, asKeywordList[ucKeywordCounter].cString) == EQUAL){
      *peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
      return OK;
    }
  }
  return ERROR;
}


void DecodeTokens(void){
  
  unsigned char ucTokenCounter;
  
  for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++){

    if(eStringToKeyword(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.eKeyword) == OK){
      asToken[ucTokenCounter].eType = KEYWORD;
    }
    else if(eHexStringToUInt(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.uiNumber) == OK){
      asToken[ucTokenCounter].eType = NUMBER;
    }
    else{
      asToken[ucTokenCounter].eType = STRING;
    }
  }
}


void DecodeMsg(char *pcString){
  ucTokenNr = ucFindTokensInString(pcString);
  ReplaceCharactersInString(pcString, ' ', '\0');
  DecodeTokens();
}