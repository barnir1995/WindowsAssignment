#include <Windows.h>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Please give me path to DLL to load!\n");
        return 1;
    }
    const std::string path(argv[1]);
    LoadLibraryA(path.c_str());
    return 0;
}