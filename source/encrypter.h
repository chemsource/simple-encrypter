#ifndef ENCRYPTER_H
#define ENCRYPTER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
using namespace std;
void buildKeyboardMap(unordered_map<char, char>& map, bool reverse = false);
string keyboardReplace(const string& input, const unordered_map<char, char>& keyboardMap);
string keyToLetters(int key);
int lettersToKey(const string& letters);
string transformWords(const string& input, bool reverse = false);
string caesarCipher(const string& input, int key, bool decrypt = false);
void xorFile(const string& inputFilename, const string& outputFilename = "");
void encryptFile();
void encryptFileBinaryMode();
void decryptFile();
void decryptFileBinaryMode();
void xorOnlyEncrypt();
void xorOnlyDecrypt();
#endif
