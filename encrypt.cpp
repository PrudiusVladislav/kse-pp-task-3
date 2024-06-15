#include "encrypt.h"

#include <cctype>

char shift_char(const char c, const int key) {
    if (!isalpha(c)) {
        return c;
    }

    const char offset = isupper(c) ? 'A' : 'a';
    return (c - offset + key + 26) % 26 + offset;
}

char *encrypt(char *message, const int key)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        message[i] = shift_char(message[i], key);
    }
    return message;
}

char *decrypt(char *message, const int key)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        message[i] = shift_char(message[i], -key);
    }
    return message;
}
