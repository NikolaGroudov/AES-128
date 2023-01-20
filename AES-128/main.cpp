/**
*  
* Solution to course project #02
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Nikola Groudov
* @idnumber 8MI0600270
* @compiler GCC
*
* 
*   Main function
*   User-friendly interface
*   Options menu
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "tables.h"
#include "source.cpp"

int main()
{
    unsigned char key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    unsigned short int command;
    std::cout<<"Do you want to encrypt or decrypt data?"<<std::endl;
    std::cout<<"Enter 1 to encrypt a file"<<std::endl
             <<"Enter 2 to decrypt a file"<<std::endl
             <<"Enter 0 to exit"<<std::endl;
    std::cin>>command;
    while(command < 0 or command > 2)
    {
        std::cout<<"Enter a number between 0 and 2!";
        std::cin>>command;
    }
    if(command == 1)
    {
                std::string msg;
                std::string nameOfFile;
                std::cout<<"Opening file. Enter the name of the file you want to encrypt. "<<std::endl;
                std::cout<<"Current files: "<<std::endl;
                std::cout<<"text1"<<std::endl;
                std::cout<<"text2"<<std::endl;
                std::cin>>nameOfFile;
                std::ifstream fout(("FilesForEncryption/"+nameOfFile+"forEnc.txt"));
                std::getline(fout, msg);
                fout.close();
                const int length = msg.length();
                char* message = new char[length + 1];
                strcpy(message, msg.c_str());

                int originalLen = length + 1;
                int lenOfPaddedMessage = originalLen;
                if(lenOfPaddedMessage %16 != 0)
                    lenOfPaddedMessage = (lenOfPaddedMessage / 16 + 1) * 16;
                unsigned char* paddedMessage = new unsigned char[lenOfPaddedMessage];
                for(int i = 0; i< lenOfPaddedMessage;i++)
                {
                    if(i >= originalLen)
                        paddedMessage[i] = 0;
                    else
                        paddedMessage[i] = message[i];
                }
                for(int i = 0; i < lenOfPaddedMessage; i += 16)
                    AES_encrypt(paddedMessage+i, key);
                
                nameOfFile=nameOfFile + "_Encrypted";
                std::ofstream fin(("FilesForDecryption/"+nameOfFile+".txt"));
                for(int i = 0; i < lenOfPaddedMessage;i++)
                    {
                        if(paddedMessage[i] / 16 < 10) 
                            fin<<(char)((paddedMessage[i]/16) + '0');
                        if(paddedMessage[i] / 16 >= 10)
                            fin<<(char)((paddedMessage[i]/16 - 10) + 'A');
                        if(paddedMessage[i] % 16 < 10)
                            fin<<(char)((paddedMessage[i]%16) + '0');
                        if(paddedMessage[i] % 16 >= 10)
                            fin<<(char)((paddedMessage[i]%16 - 10) + 'A');
                        fin<<" ";
                    }
                fin.close();
                delete[] paddedMessage;
                delete[] message;
    }
    else if(command == 2)
    {
                std::string msg;
                std::string nameOfFile;
                std::cout<<"Opening file. Enter the name of the file you want to decrypt.";
                std::cout<<"Current files: "<<std::endl;
                std::cout<<"text1"<<std::endl<<"text2"<<std::endl;
                std::cin>>nameOfFile;
                std::ifstream fout(("FilesForDecryption/"+nameOfFile+"forDec.txt"));
                std::getline(fout, msg);
                fout.close();
                const int length = msg.length();
                char* message = new char[length + 1];
                strcpy(message, msg.c_str());



    }
    else if(command == 0)
    {
        std::cout<<"Have a nice day!";
        return 0;
    }


    

    return 0;
}