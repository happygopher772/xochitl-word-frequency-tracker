#include <iostream>
#include <mecab.h>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>

//freq 
#include <unordered_map>
using namespace std;

int Gallbladder()
{
    return 0;
}

//uses MeCab to format JapTxtFile
int formatJapTxt(string oldFile)
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
}

int freq(string txtFile)
{
    string fileChoice = txtFile;
    ifstream file(fileChoice);
    if (!file) {
        std::cerr << "Failed to open " << fileChoice << "\n";
        return 1;
    }

    unordered_map<string, int> freq;

    string line;
    while (getline(file,line))
    {
        istringstream iss(line);
        string word;
        if (iss >> word)
        {
            freq[word]++;
        }
    }

    vector<pair<string, int>> vec(freq.begin(), freq.end());

    sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.second < b.second; });

    for (const auto& p : vec) 
    {
        cout << p.first << ": " << p.second << "\n";
    }

}

int outputFile(vector<pair<string, int>> sortedVector)
{

    ofstream out("output.txt");

    for (const auto& p : sortedVector) {
        out << p.first << ": " << p.second << "\n";
    }


    for (const auto& p : sortedVector) {
        cout << left << setw(10) << p.first   // string left-aligned in 10 spaces
            << right << setw(3) << p.second  // number right-aligned in 3 spaces
            << "\n";
    }


    out.close();
}

string getDateString() 
{
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    stringstream ss;

    ss << setw(2) << setfill('0') << now->tm_mon + 1 << "-"
        << setw(2) << setfill('0') << now->tm_mday << "-"
        << now->tm_year + 1900;

    std::ifstream infile("file-logs.txt");
    if (!infile) {
        std::cerr << "Failed to open file-logs.txt input\n";
        return "";
    }

    string line;
    string currentDate = ss.str();
    int dateIssue = 1;

    while (getline(infile, line))
    {
        istringstream iss(line);
        string inFileDate;
        

        while (iss >> inFileDate)
        {
            string inFileDataPure = inFileDate.substr(0, 10);
           
            if (currentDate == inFileDataPure)
            {
                //we need to find numbered date
                size_t start = 10;
                size_t end = inFileDate.find(' ', start);

                string result;
                if (end != string::npos)
                {
                    result = inFileDate.substr(start, end - start);
                }
                else
                {
                    result = inFileDate.substr(start);
                }
                dateIssue = stoi(result) + 1;
            }
            
        }
    }
    infile.close();

    currentDate = currentDate + "#" + to_string(dateIssue);
    
    std::ofstream outfile("file-logs.txt", std::ios::app);
    if (!outfile) {
        std::cerr << "Failed to file-logs.txt output\n";
        return "";
    }
    outfile << currentDate << "\n";
    outfile.close();

    return currentDate;
}


//FileUtils.h/cpp → getDateString(), outputFile()

//TextProcessing.h / cpp → formatJapTxt(), freq()

//Main.cpp → orchestrates everything