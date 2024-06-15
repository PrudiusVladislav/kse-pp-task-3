#include <iostream>
#include <cstring>
#include <windows.h>

typedef char*(*function_ptr)(char*);

char* getDllPath(const char *dllName)
{
    std::string path = R"(C:\C\kse-pp-task-3\)";
    path += dllName;
    char *result = new char[path.length() + 1];
    strcpy(result, path.c_str());
    return result;
}

function_ptr getFunction(const HINSTANCE handle, const char *functionName)
{
    return reinterpret_cast<function_ptr>(GetProcAddress(handle, functionName));
}

void print_help()
{
    std::cout << "Available operations: encrypt, decrypt" << std::endl;
}

char *prompt_user()
{
    std::cout << "Enter operation: ";
    char *operation_name = new char[20];
    std::cin >> operation_name;
    return operation_name;
}

const char *handle_operation(HINSTANCE handle, const char *operation_name)
{
    function_ptr operation;
    if (strcmp(operation_name, "encrypt") == 0)
    {
        operation = getFunction(handle, "encrypt");
    }
    else if (strcmp(operation_name, "decrypt") == 0)
    {
        operation = getFunction(handle, "decrypt");
    }
    else
    {
        return "Invalid operation";
    }

    if (operation == nullptr)
    {
        return "Operation not found";
    }

    char *message = new char[100];
    std::cout << "Enter message: ";
    std::cin >> message;
    return operation(message);
}



int main() {
    const char *dllName = "encrypt.dll";
    HINSTANCE handle = LoadLibrary(TEXT(getDllPath(dllName)));

    if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
    {
        std::cout << "Lib '" << dllName << "' not found" << std::endl;
        return -1;
    }

    print_help();
    const char *operation_name = prompt_user();
    const char *message = handle_operation(handle, operation_name);
    std::cout << message << std::endl;
    return 0;
}
