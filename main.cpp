#include <iostream>
#include <cstring>
#include <windows.h>

typedef char*(*function_ptr)(char*, int);

struct Options
{
    bool copy_to_clipboard = false;
    int key = 3;
};

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

const Options *parse_options(int argc, char *argv[])
{
    auto *options = new Options();
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--copy") == 0)
        {
            options->copy_to_clipboard = true;
        }
        else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--key") == 0)
        {
            options->key = strtol(argv[i + 1], nullptr, 10);
        }
    }

    return options;
}

void print_help()
{
    std::cout << "Available operations: encrypt, decrypt" << std::endl;
}

char *prompt_user()
{
    std::cout << "Enter operation: " << std::flush;
    char *operation_name = new char[20];
    std::cin >> operation_name;
    return operation_name;
}

void copy_to_clipboard(const char *text)
{
    const size_t len = strlen(text) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);
    OpenClipboard(nullptr);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);
}

const char *handle_operation(
    HINSTANCE handle,
    const char *operation_name,
    bool *running,
    const Options *options)
{
    function_ptr operation;
    if (strcmp(operation_name, "exit") == 0)
    {
        *running = false;
        return "Exiting...";
    }
    if (operation_name[0] == 'e')
    {
        operation = getFunction(handle, "encrypt");
    }
    else if (operation_name[0] == 'd')
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

    const auto message = new char[30];
    std::cout << "Enter message: ";
    std::cin.ignore();
    std::cin.getline(message, 30);

    const char *result = operation(message, options->key);
    if (options->copy_to_clipboard)
    {
        copy_to_clipboard(result);
    }

    return result;
}


int main(int argc, char *argv[]) {
    std::cout << isalpha(',') << std::endl;
    const char *dllName = "encrypt.dll";
    HINSTANCE handle = LoadLibrary(TEXT(getDllPath(dllName)));

    if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
    {
        std::cout << "Lib '" << dllName << "' not found" << std::endl;
        return -1;
    }

    bool running = true;
    const Options *options = parse_options(argc, argv);
    std::cout << "Options: copy to clipboard - " << options->copy_to_clipboard << ", key - " << options->key << std::endl;
    print_help();

    while (running) {
        const char *operation_name = prompt_user();

        const char *message = handle_operation(handle, operation_name, &running, options);
        std::cout << message << std::endl;
    }

    FreeLibrary(handle);
    delete options;
    return 0;
}
