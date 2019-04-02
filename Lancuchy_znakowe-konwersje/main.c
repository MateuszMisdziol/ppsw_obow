#define HEX_bm 0x000F

enum Result {OK, ERROR};

unsigned int xCorrect = 0;
char StrCorrect[7] = "0xF47B";
enum Result eCorrect;
unsigned int xShort = 0;
char StrShort[7] = "0xA7";
enum Result eShort;
unsigned int xLong = 0;
char StrLong[8] = "0xF47BC4";
enum Result eLong;
unsigned int xBadCharacter = 0;
char StrBadCharacter[7] = "0xF47J";
enum Result eBadCharacter;
unsigned int xNo0x = 0;
char StrNo0x[7] = "F47B";
enum Result eNo0x;


void UIntToHexStr (unsigned int uiValue, char pcStr[]){

	unsigned char ucTetradaCounter;
	unsigned char ucCurrentTetrada;

	pcStr[0] = '0';
	pcStr[1] = 'x';

	for(ucTetradaCounter = 0; ucTetradaCounter < 4; ++ucTetradaCounter){
		
		ucCurrentTetrada = ((uiValue >> (ucTetradaCounter * 4)) & HEX_bm);

		if(ucCurrentTetrada>9){
			pcStr[5-ucTetradaCounter] = ucCurrentTetrada - 10 + 'A'; // w ascii 'A' == 65
		}
		else{
			pcStr[5-ucTetradaCounter] = ucCurrentTetrada + '0'; //w ascii '0' == 48
		}
	}
	pcStr[6] = '\0';
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

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){

	unsigned char PointerNumber;

	for(PointerNumber = 0; pcDestinationStr[PointerNumber] != '\0'; PointerNumber++)
	UIntToHexStr(uiValue, pcDestinationStr+PointerNumber);
}

int main(void){
  
  eCorrect = eHexStringToUInt(StrCorrect, &xCorrect);
  eShort = eHexStringToUInt(StrShort, &xShort);
  eLong = eHexStringToUInt(StrLong, &xLong);
  eBadCharacter = eHexStringToUInt(StrBadCharacter, &xBadCharacter);
  eNo0x = eHexStringToUInt(StrNo0x, &xNo0x);
  
}
