#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include "encrypter.h"
using namespace std;
// 键盘布局映射表
void buildKeyboardMap(unordered_map<char, char>& map, bool reverse /*= false*/) {
    string qwerty = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    if (reverse) {
        for (size_t i = 0; i < abc.size(); ++i) {
            map[abc[i]] = qwerty[i];
        }
    } else {
        for (size_t i = 0; i < qwerty.size(); ++i) {
            map[qwerty[i]] = abc[i];
        }
    }
}

// 键盘布局替换
string keyboardReplace(const string& input, const unordered_map<char, char>& keyboardMap) {
    string result = input;
    for (char& c : result) {
        auto it = keyboardMap.find(c);
        if (it != keyboardMap.end()) {
            c = it->second;
        }
    }
    return result;
}

// 数字密钥转字母表示（使用键盘第一行）
string keyToLetters(int key) {
    key = (key % 26 + 26) % 26;
    string letters = "QWERTYUIOP"; // 0-9分别对应Q-P
    
    string result;
    if (key >= 10) {
        result += letters[key / 10]; // 十位数
    }
    result += letters[key % 10];     // 个位数
    return result;
}

// 字母密钥转数字
int lettersToKey(const string& letters) {
    string keyMap = "QWERTYUIOP"; // Q=0, W=1, ..., P=9
    int key = 0;
    
    for (char c : letters) {
        size_t pos = keyMap.find(c);
        if (pos != string::npos) {
            key = key * 10 + static_cast<int>(pos);
        }
    }
    return key;
}

// 奇偶位置字符处理
string transformWords(const string& input, bool reverse /*= false*/) {
    if (input.empty()) return "";
    
    if (!reverse) {
        // 加密：奇偶位置重组
        string result;
        string word;
        
        for (char c : input) {
            if (isalpha(c)) {
                word += c;
            } else {
                if (!word.empty()) {
                    string odd, even;
                    for (size_t i = 0; i < word.size(); ++i) {
                        if (i % 2 == 0) odd += word[i];
                        else even += word[i];
                    }
                    result += odd + even;
                    word.clear();
                }
                result += c; // 添加非字母字符
            }
        }
        
        if (!word.empty()) {
            string odd, even;
            for (size_t i = 0; i < word.size(); ++i) {
                if (i % 2 == 0) odd += word[i];
                else even += word[i];
            }
            result += odd + even;
        }
        
        return result;
    } else {
        // 解密：奇偶位置还原
        string result;
        string word;
        
        for (char c : input) {
            if (isalpha(c)) {
                word += c;
            } else {
                if (!word.empty()) {
                    int n = word.length();
                    int len1 = (n + 1) / 2; // 前半部分长度（奇数位置）
                    string original;
                    
                    for (int i = 0; i < len1; ++i) {
                        original += word[i];
                        if (i + len1 < n) {
                            original += word[i + len1];
                        }
                    }
                    result += original;
                    word.clear();
                }
                result += c;
            }
        }
        
        if (!word.empty()) {
            int n = word.length();
            int len1 = (n + 1) / 2;
            string original;
            
            for (int i = 0; i < len1; ++i) {
                original += word[i];
                if (i + len1 < n) {
                    original += word[i + len1];
                }
            }
            result += original;
        }
        
        return result;
    }
}

// 凯撒加密/解密
string caesarCipher(const string& input, int key, bool decrypt/* = false*/) {
    if (decrypt) key = -key;
    
    string result = input;
    key %= 26;
    if (key < 0) key += 26;
    
    for (char& c : result) {
        if (isupper(c)) {
            c = 'A' + (c - 'A' + key) % 26;
        } else if (islower(c)) {
            c = 'a' + (c - 'a' + key) % 26;
        }
    }
    return result;
}

// 对文件进行异或1操作（翻转每个字节的最低位）
void xorFile(const string& inputFilename, const string& outputFilename/* = ""*/) {
    // 确定输出文件名
    string outFile = outputFilename;
    if (outFile.empty()) {
        outFile = inputFilename;
    }
    
    // 以二进制模式打开文件
    ifstream infile(inputFilename, ios::binary);
    if (!infile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return;
    }
    
    ofstream outfile(outFile, ios::binary);
    if (!outfile) {
        cerr << "Error creating output file: " << outFile << endl;
        infile.close();
        return;
    }
    
    // 读取并处理文件
    char c;
    while (infile.get(c)) {
        c ^= 1; // 翻转最低位
        outfile.put(c);
    }
    
    infile.close();
    outfile.close();
    
    cout << "XOR operation completed. Input: " << inputFilename 
         << ", Output: " << outFile << endl;
}

// 标准加密函数
void encryptFile() {
    string filename;
    cout << "Enter source filename: ";
    cin >> filename;
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
    
    unordered_map<char, char> keyboardMap;
    buildKeyboardMap(keyboardMap);
    
    vector<string> encryptedLines;
    
    for (const string& origLine : lines) {
        // 步骤1：键盘布局替换
        string step1 = keyboardReplace(origLine, keyboardMap);
        
        // 步骤2：奇偶位置重组
        string step2 = transformWords(step1);
        
        // 步骤3：读取凯撒密钥
        int key;
        cout << "Enter key for line: \"" << origLine << "\"\n> ";
        cin >> key;
        
        // 凯撒加密
        string encrypted = caesarCipher(step2, key);
        
        // 将密钥转换为字母形式并添加到行首
        string keyLetters = keyToLetters(key);
        encryptedLines.push_back(keyLetters + " " + encrypted);
    }
    
    string outputFilename = filename + ".f3c";
    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error creating output file." << endl;
        return;
    }
    
    for (const string& eline : encryptedLines) {
        outfile << eline << endl;
    }
    outfile.close();
    
    cout << "Encryption completed. Output file: " << outputFilename << endl;
}

// 文件加密模式（标准F3C加密+异或1）
void encryptFileBinaryMode() {
    string filename;
    cout << "Enter source filename: ";
    cin >> filename;
    
    // 先进行标准加密
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
    
    unordered_map<char, char> keyboardMap;
    buildKeyboardMap(keyboardMap);
    
    vector<string> encryptedLines;
    
    for (const string& origLine : lines) {
        string step1 = keyboardReplace(origLine, keyboardMap);
        string step2 = transformWords(step1);
        
        int key;
        cout << "Enter key for line: \"" << origLine << "\"\n> ";
        cin >> key;
        
        string encrypted = caesarCipher(step2, key);
        string keyLetters = keyToLetters(key);
        encryptedLines.push_back(keyLetters + " " + encrypted);
    }
    
    string tempOutput = filename + ".temp.f3c";
    ofstream outfile(tempOutput);
    if (!outfile) {
        cerr << "Error creating temp file." << endl;
        return;
    }
    
    for (const string& eline : encryptedLines) {
        outfile << eline << endl;
    }
    outfile.close();
    
    // 对加密文件进行异或1操作
    string finalOutput = filename + ".f3c";
    xorFile(tempOutput, finalOutput);
    
    // 删除临时文件
    remove(tempOutput.c_str());
    
    cout << "Binary encryption completed. Final output: " << finalOutput << endl;
}

// 标准解密函数
void decryptFile() {
    string filename;
    cout << "Enter encrypted filename (.f3c): ";
    cin >> filename;
    
    // 检查文件扩展名
    if (filename.size() < 5 || filename.substr(filename.size() - 4) != ".f3c") {
        cerr << "Invalid file format. Please provide a .f3c file." << endl;
        return;
    }
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
    
    unordered_map<char, char> reverseKeyboardMap;
    buildKeyboardMap(reverseKeyboardMap, true);
    
    vector<string> decryptedLines;
    
    for (const string& encLine : lines) {
        // 分离密钥字母和加密文本
        size_t spacePos = encLine.find(' ');
        if (spacePos == string::npos) {
            decryptedLines.push_back(encLine);
            continue;
        }
        
        string keyLetters = encLine.substr(0, spacePos);
        int key = lettersToKey(keyLetters);
        string encryptedText = encLine.substr(spacePos + 1);
        
        string step1 = caesarCipher(encryptedText, key, true);
        string step2 = transformWords(step1, true);
        decryptedLines.push_back(keyboardReplace(step2, reverseKeyboardMap));
    }
    
    string outputFilename = filename.substr(0, filename.size() - 4) + ".txt";
    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error creating output file: " << outputFilename << endl;
        return;
    }
    
    for (const string& dline : decryptedLines) {
        outfile << dline << endl;
    }
    outfile.close();
    
    cout << "Decryption completed. Output file: " << outputFilename << endl;
}

// 文件解密模式（异或1 + 标准F3C解密）
void decryptFileBinaryMode() {
    string filename;
    cout << "Enter encrypted filename (.f3c): ";
    cin >> filename;
    
    if (filename.size() < 5 || filename.substr(filename.size() - 4) != ".f3c") {
        cerr << "Invalid file format. Please provide a .f3c file." << endl;
        return;
    }
    
    // 先创建临时文件
    string tempFile = filename + ".temp";
    
    // 对文件进行异或1操作（恢复原始内容）
    xorFile(filename, tempFile);
    
    // 然后进行标准解密
    ifstream infile(tempFile);
    if (!infile) {
        cerr << "Error opening temp file: " << tempFile << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
    
    unordered_map<char, char> reverseKeyboardMap;
    buildKeyboardMap(reverseKeyboardMap, true);
    
    vector<string> decryptedLines;
    
    for (const string& encLine : lines) {
        size_t spacePos = encLine.find(' ');
        if (spacePos == string::npos) {
            decryptedLines.push_back(encLine);
            continue;
        }
        
        string keyLetters = encLine.substr(0, spacePos);
        int key = lettersToKey(keyLetters);
        string encryptedText = encLine.substr(spacePos + 1);
        
        string step1 = caesarCipher(encryptedText, key, true);
        string step2 = transformWords(step1, true);
        decryptedLines.push_back(keyboardReplace(step2, reverseKeyboardMap));
    }
    
    string outputFilename = filename.substr(0, filename.size() - 4) + ".txt";
    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error creating output file: " << outputFilename << endl;
        remove(tempFile.c_str());
        return;
    }
    
    for (const string& dline : decryptedLines) {
        outfile << dline << endl;
    }
    outfile.close();
    
    // 删除临时文件
    remove(tempFile.c_str());
    
    cout << "Binary decryption completed. Output file: " << outputFilename << endl;
}

// 仅异或加密模式
void xorOnlyEncrypt() {
    string inputFile, outputFile;
    cout << "Enter source filename: ";
    cin >> inputFile;
    cout << "Enter output filename: ";
    cin >> outputFile;
    
    xorFile(inputFile, outputFile);
    cout << "XOR encryption completed." << endl;
}

// 仅异或解密模式
void xorOnlyDecrypt() {
    string inputFile, outputFile;
    cout << "Enter encrypted filename: ";
    cin >> inputFile;
    cout << "Enter output filename: ";
    cin >> outputFile;
    
    xorFile(inputFile, outputFile);
    cout << "XOR decryption completed." << endl;
}
