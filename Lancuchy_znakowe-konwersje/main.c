#define HEX_bm 0x000F

enum Result {OK, ERROR};


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
	pcStr[6] = 'f';
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

unsigned int uiTestDestination;
enum Result eReturnResult;

int main(void){
  

  
  eReturnResult = eHexStringToUInt("0x*", &uiTestDestination);
  
}
