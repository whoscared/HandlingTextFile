#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include "FileHandler.h"

bool FileHandler::checkIsTxt(string name)
{
	int size = name.length();
	if (name.compare(size - 3, 3, "txt") == 0) {
		return true;
	}
	return false;
}

string FileHandler::changeString(string s)
{
	if (s != "" && ignoreWordsWithNumber && ignoreWordsWithSpecialSymbol) {
		for (int i = 0; i < s.length(); i++){
			if (!(s.at(i) > 'À' && s.at(i) < 'ÿ') && !(s.at(i) > 'A' && s.at(i) < 'z')) {
				s.at(i) = ' ';
				continue;
			}
			if (language == Language::RUSSIAN) {
				s.at(i) = !(s.at(i) > 'À' && s.at(i) < 'ÿ') ? ' ' : s.at(i);
			}
			if (language == Language::ENGLISH) {
				s.at(i) = !(s.at(i) > 'A' && s.at(i) < 'z') ? ' ' : s.at(i);
			}
		}
	}

	if (s != "" && !ignoreWordsWithSpecialSymbol) {
		for (int i = 0; i < s.length(); i++) {
			if (specialSymbols.find(s.at(i)) != string::npos || (s.at(i) >= '0' && s.at(i) <= '9')) {
				continue;
			}
			if (!(s.at(i) > 'À' && s.at(i) < 'ÿ') && !(s.at(i) > 'A' && s.at(i) < 'z')) {
				s.at(i) = ' ';
				continue;
			}
			if (language == Language::RUSSIAN) {
				s.at(i) = !(s.at(i) > 'À' && s.at(i) < 'ÿ') ? ' ' : s.at(i);
			}
			if (language == Language::ENGLISH) {
				s.at(i) = !(s.at(i) > 'A' && s.at(i) < 'z') ? ' ' : s.at(i);
			}
		}
	}

	if (s != "" && ignoreRegister) {

		for (int i = 0; i < s.length(); i++) {
			s[i] = s[i] == '¨' ? '¸' : s[i];
			s[i] = (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'À' && s[i] <= 'ß') ? s[i] + 32 : s[i];
		}

	}

	if (s != "" && ignoreWordsWithNumber) {
		replace_if(s.begin(), s.end(), [](char c) {
			return (c >= '0' && c <= '9');
			}, ' ');
	}

	if (s != "") {
		s.erase(remove(s.begin(), s.end(), ' '), s.end());
	}

	return s;
}


FileHandler::FileHandler()
{
	path = "C:\\Users\\coklo\\source\\repos\\CourseWork_TextFile\\CourseWork_TextFile\\Example\\";

	for (const auto& file : filesystem::directory_iterator(path)) {
		if (!filesystem::is_directory(file) && checkIsTxt(file.path().filename().string())) {
			files.push_back(file.path().filename().string());
		}
	}

	ignoreRegister = true;
	ignoreWordsWithSpecialSymbol = true;
	ignoreWordsWithNumber = true;
	language = RUSSIAN;
}

FileHandler::FileHandler(string path, bool ignoreRegister, bool ignoreWordsWithSpecialSymbol, bool ignoreWordsWithNumber, Language language)
{
	this->path = path;

	for (const auto& file : filesystem::directory_iterator(this->path)) {
		if (!filesystem::is_directory(file) && checkIsTxt(file.path().filename().string())) {
			files.push_back(file.path().filename().string());
		}
	}

	this->ignoreRegister = ignoreRegister;
	this->ignoreWordsWithSpecialSymbol = ignoreWordsWithSpecialSymbol;
	this->ignoreWordsWithNumber = ignoreWordsWithNumber;
	this->language = language;
}


void FileHandler::setPath(string path)
{
	this->path = path;
}

void FileHandler::setIgnoreRegister(bool ignoreRegister)
{
	this->ignoreRegister = ignoreRegister;
}

void FileHandler::setIgnoreWordsWithSpecialSymbol(bool ignoreWordsWithSpecialSymbol)
{
	this->ignoreWordsWithSpecialSymbol = ignoreWordsWithSpecialSymbol;
}

void FileHandler::setIgnoreWordsWithNumber(bool ignoreWordsWithNumber)
{
	this->ignoreWordsWithNumber = ignoreWordsWithNumber;
}

void FileHandler::setSpecialSymbols(string specialSymbols)
{
	this->specialSymbols = specialSymbols;
}

void FileHandler::setLanguage(Language language)
{
	this->language = language;
}


string FileHandler::getPath()
{
	return path;
}

bool FileHandler::getIgnoreRegister()
{
	return ignoreRegister;
}

bool FileHandler::getIgnoreWordsWithSpecialSymbol()
{
	return ignoreWordsWithSpecialSymbol;
}

bool FileHandler::getIgnoreWordsWithNumber()
{
	return ignoreRegister;
}

string FileHandler::getSpecialSymbols()
{
	return specialSymbols;
}

Language FileHandler::getLanguage()
{
	return language;
}

vector<string> FileHandler::getFiles()
{
	return files;
}


map<string, map<string, int>> FileHandler::theMostFrequentlyUsedWords()
{
	map<string, map<string, int>> frequentlyUsedWords;
	ifstream filein;

	vector<string>::iterator iter = files.begin();

	while (iter != files.end()) {
		map <string, int> tempMap;
		filein.open(path + *iter);

		while (!filein.eof()) {
			string tempS;
			filein >> tempS;

			tempS = changeString(tempS);

			if (tempS.length() == 0 || tempS == " ") {
				continue;
			}

			int countOfTempString = 1;

			auto contain = tempMap.find(tempS);

			if (contain != tempMap.end()) {
				countOfTempString += contain->second;
				tempMap.erase(contain);
				tempMap.emplace(tempS, countOfTempString);
			}

			tempMap.emplace(tempS, countOfTempString);
		}

		frequentlyUsedWords.emplace(*iter, tempMap);
		filein.close();
		iter++;
	}

	return frequentlyUsedWords;
}

map<string, map<string, int>> FileHandler::theBiggestWords() //theLenghtsWords
{
	map<string, map<string, int>> lenghtOfWords;
	ifstream filein;

	vector<string>::iterator iter = files.begin();

	while (iter != files.end()) {
		map <string, int> tempMap;
		filein.open(path + *iter);

		while (!filein.eof()) {
			string tempS;
			filein >> tempS;

			tempS = changeString(tempS);

			if (tempS.length() == 0 || tempS == " ") {
				continue;
			}

			auto contain = tempMap.find(tempS);

			if (contain == tempMap.end()) {
				tempMap.emplace(tempS, tempS.length());
			}
		}

		lenghtOfWords.emplace(*iter, tempMap);
		filein.close();
		iter++;
	}

	return lenghtOfWords;
}

map<string, int> FileHandler::countSentences()
{
	char endOfSentences[] = { '!', '…', '.', '?', ':', ';' };

	map<string, int> countsOfSentences;
	ifstream filein;

	vector<string>::iterator iter = files.begin();

	while (iter != files.end()) {

		filein.open(path + *iter);
		int count = 0;

		while (!filein.eof()) {
			string tempS;
			filein >> tempS;
			for (int i = 0; i < 6; i++) {
				if (tempS.find(endOfSentences[i]) != string::npos) {
					count++;
				}
			}
		}

		countsOfSentences.emplace(*iter, count);
		filein.close();
		iter++;
	}

	return countsOfSentences;
}


vector<pair<string, int>> FileHandler::sortStringInt(map<string, int> mapForSort)
{
	vector<pair<string, int>> forSort (mapForSort.begin(), mapForSort.end());

	sort(forSort.begin(), forSort.end(), [](const auto &a, const auto &b) {
		return a.second > b.second;
		});

	return forSort;
}
