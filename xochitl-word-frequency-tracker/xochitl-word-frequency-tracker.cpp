#include <iostream>
#include <mecab.h>
#include <string>
#include <fstream>
#include <windows.h>
#include <cstring>

int main()
{
    std::ifstream infile("japText.txt", std::ios::binary);
    if (!infile) {
        std::cerr << "Failed to open txt\n";
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>());
    infile.close();

    MeCab::Tagger* tagger = MeCab::createTagger(
        R"(-d C:\utf8\new\mecab-ipadic-2.7.0-20070610 -Ochasen)"
    );
    if (!tagger) {
        std::cerr << "Failed to create MeCab tagger.\n";
        return 1;
    }

    const char* result = tagger->parse(input.c_str());
    if (!result) {
        std::cerr << "Failed to parse text.\n";
        delete tagger;
        return 1;
    }

    std::ofstream outfile("output.txt", std::ios::binary);
    if (!outfile) {
        std::cerr << "Failed to open output.txt\n";
        delete tagger;
        return 1;
    }

    unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    outfile.write(reinterpret_cast<char*>(bom), 3);

    outfile.write(result, std::strlen(result));
    outfile.close();

    std::cout << "Job's complete Sir\n";

    delete tagger;
    return 0;
}
