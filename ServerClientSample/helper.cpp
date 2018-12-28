#include "pch.h"
#include "Server.h"

 void Helper::PrintChars(char* word) {
	for (int i = 0; i < std::strlen(word); i++) {
		std::cout << word[i];
	}
}