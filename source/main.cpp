#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include "encrypter.h"
using namespace std;

// 主菜单
int main() {
    while (true) {
        cout << "\n===== Advanced File Encryption/Decryption System =====" << endl;
        cout << "1. Standard Encrypt a file (F3C)" << endl;
        cout << "2. Standard Decrypt a file (F3C)" << endl;
        cout << "3. Binary Encrypt a file (F3C + XOR)" << endl;
        cout << "4. Binary Decrypt a file (XOR + F3C)" << endl;
        cout << "5. XOR Encrypt Only (simple bit flip)" << endl;
        cout << "6. XOR Decrypt Only (simple bit flip)" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(); // 清除输入缓冲区
        
        switch (choice) {
            case 1:
                encryptFile();
                break;
            case 2:
                decryptFile();
                break;
            case 3:
                encryptFileBinaryMode();
                break;
            case 4:
                decryptFileBinaryMode();
                break;
            case 5:
                xorOnlyEncrypt();
                break;
            case 6:
                xorOnlyDecrypt();
                break;
            case 7:
                cout << "Exiting program..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}
