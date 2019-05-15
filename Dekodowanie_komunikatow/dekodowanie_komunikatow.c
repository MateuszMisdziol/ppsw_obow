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
	char *pcString;            //jezeli STRING
} TokenValue;

typedef struct Token
{
	enum TokenType eType;       //KEYWORD, NUMBER, STRING
	union TokenValue uValue;    //enum, unsigned int, char *
} Token;

Token asToken[MAX_TOKEN_NR];

typedef struct Keyword
{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

Keyword asKeywordList[MAX_KEYWORD_NR]=
{
	{RST, "reset"}, {LD, "load"}, {ST, "store"}
};

typedef enum Result
{OK, ERROR} Result;

typedef enum CompResult 
{ DIFFERENT , EQUAL } CompResult;

unsigned char ucTokenNr = 0;

//----------------------FUNKCJE----------------------------

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
  
  unsigned char ucCharNumber;
  
  for(ucCharNumber = 0; pcStr1[ucCharNumber] == pcStr2[ucCharNumber]; ucCharNumber++){
    if(pcStr1[ucCharNumber] == '\0'){
      return EQUAL;
    }
  }
  return DIFFERENT;
}

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

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue){

	unsigned char ucCharCounter;

	if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0')){
		return ERROR;
	}

	for(ucCharCounter = 2; ucCharCounter != '\0'; ucCharCounter++){
		if(pcStr[ucCharCounter] == '\0'){
			return OK;
		}
		else if (ucCharCounter >= 6){
			return ERROR;
		}

		*puiValue = *puiValue << 4;

		if(pcStr[ucCharCounter] >= 'A'){
			*puiValue = *puiValue | ((pcStr[ucCharCounter] - 'A') + 10);
		}
		else{
			*puiValue = *puiValue | (pcStr[ucCharCounter] - '0');
		}
	}
	return OK;
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar){
  
  unsigned char ucCharNumber;
  
  for(ucCharNumber = 0; pcString[ucCharNumber] != '\0'; ucCharNumber++){
    if(pcString[ucCharNumber] == cOldChar){
      pcString[ucCharNumber] = cNewChar;
    }
  }
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

//***********************MAIN*********************

char napis[]="0x2137 siemanko store";

int main(void){
  DecodeMsg(napis);
}
