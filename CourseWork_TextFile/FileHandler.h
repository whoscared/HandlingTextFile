#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "Language.cpp"

//list with the most frequently used words 
//list with size words
//list with count sentences

//top list 
//settings for handle : registers, ignore words with special symbols or on another language or with numbers

using namespace std;



class FileHandler
{
private:
	string path;
	bool ignoreRegister;
	bool ignoreWordsWithSpecialSymbol;
	bool ignoreWordsWithNumber;
	string specialSymbols;
	Language language;
	vector<string> files;

	bool checkIsTxt(string name);
	string changeString(string s);

public: 
	FileHandler();
	FileHandler(string path, bool ignoreRegister, bool ignoreWordsWithSpecialSymbol, bool ignoreWordsWithNumber ,Language language);

	void setPath(string path);
	void setIgnoreRegister(bool ignoreRegister);
	void setIgnoreWordsWithSpecialSymbol(bool ignoreWordsWithSpecialSymbol);
	void setIgnoreWordsWithNumber(bool ignoreWordsWithNumber);
	void setSpecialSymbols(string specialSymbols);
	void setLanguage(Language language);

	string getPath();
	bool getIgnoreRegister();
	bool getIgnoreWordsWithSpecialSymbol();
	bool getIgnoreWordsWithNumber();
	string getSpecialSymbols();
	Language getLanguage();
	vector<string> getFiles();

	map<string, map<string, int>> theMostFrequentlyUsedWords();
	map<string, map<string, int>> theBiggestWords();
	map<string, int> countSentences();

	vector<pair<string, int>> sortStringInt(map<string, int> mapForSort);
};

