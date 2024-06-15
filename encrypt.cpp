#include "encrypt.h"


extern "C"
{
    __declspec(dllexport) char *encrypt(char *message, const int key)
    {
        for (int i = 0; message[i] != '\0'; i++)
        {
            message[i] += key;
        }
        return message;
    }

    __declspec(dllexport) char *decrypt(char *message, const int key)
    {
        for (int i = 0; message[i] != '\0'; i++)
        {
            message[i] -= key;
        }
        return message;
    }
}
