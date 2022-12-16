#include <iostream>
#include <string>
#include <Windows.h>
#include "FileHandler.h"

using namespace std;

int numberFromConsole(string fromConsole) {
	for (int i = 0; i < fromConsole.size(); i++) {
		if (fromConsole[i] < '0' || fromConsole[i] > '9')
			return -1;
	}
	return stoi(fromConsole);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	FileHandler fileHandler;
	bool menu = true;

	while (menu) {
		int x = -1;
		cout << "1.Ввести путь к папке\n" <<
			"2.Задать настройки для обработки текстовых файлов\n" <<
			"3.Вывести самые часто употребляемые слова\n" <<
			"4.Вывести самые длинные слова\n" <<
			"5.Вывести количество предложений \n" <<
			"6.Посмотреть текущие настройки\n" <<
			"7.Выход" << endl;
		while (x == -1) {
			string fromConsole;
			cin >> fromConsole;
			x = numberFromConsole(fromConsole);
			if (x == -1) {
				cout << "Введите цифру" << endl;
			}
		}
		switch (x)
		{
		case 1:
		{
			bool truepath = true;

			while (truepath) {
				string path;
				cout << "Введите путь к папке: ";
				cin >> path;

				if (filesystem::is_directory(path)) {
					truepath = false;
					fileHandler.setPath(path);
				}
				else {
					cout << "Путь введен некорректно" << endl;
				}
			}
			break;
		}
		case 2:
		{
			bool settings = true;

			while (settings) {
				int y;
				cout << "1.Выбрать учет регистров\n" <<
					"2.Выбрать учет специальных символов\n" <<
					"3.Выбрать учет цифр\n" <<
					"4.Выбрать язык\n" <<
					"5.Завершить настройки" << endl;
				cin >> y;

				switch (y)
				{
				case 1:
				{
					int reg;
					cout << "1.С учетом регистра\n" <<
						"2.Без учета регистра\n" << 
						"Default 2" << endl;
					cin >> reg;

					if (reg == 1) {
						fileHandler.setIgnoreRegister(false); // HeLLo != hello
					}
					else {
						fileHandler.setIgnoreRegister(true); // HeLLo == hello
					}
					break;
				}
				case 2:
				{
					int spec;
					cout << "1.С учетом специальных символов\n" <<
						"2.Без учета специальных символов\n" <<
						"Default 2" << endl;
					cin >> spec;

					if (spec == 1) {
						string special;
						cout << "Введите без пробелов/запятых список специальных символов\n" << endl;
						cin >> special;
						fileHandler.setIgnoreWordsWithSpecialSymbol(false); //hol@ != hol
						fileHandler.setSpecialSymbols(special); //@
					}
					else {
						fileHandler.setIgnoreWordsWithSpecialSymbol(true); //hol@ == hol
					}
					break;
				}
				case 3:
				{
					int spec;
					cout << "1.С учетом цифр\n" <<
						"2.Без учета цифр\n" <<
						"Default 2" << endl;
					cin >> spec;

					if (spec == 1) {
						fileHandler.setIgnoreWordsWithNumber(false);//he11o != heo
					}
					else {
						fileHandler.setIgnoreWordsWithNumber(true); //he11o == heo
					}
					break;
				}
				case 4:
				{
					int lang;
					cout << "1.Русский\n" <<
						"2.Английский\n" <<
						"Default Русский" << endl;
					cin >> lang;

					if (lang == 2) {
						fileHandler.setLanguage(ENGLISH);
					}
					else {
						fileHandler.setLanguage(RUSSIAN);
					}
					break;
				}
				case 5:
				{
					settings = false;
					break;
				}
				default:
					cout << "Данный пункт отсутствует в меню\n" << endl;
					break;
				}
			}
			break;
		}
		case 3:
		{
			cout << "Список файлов:" << endl;
			vector<string> files = fileHandler.getFiles();
			vector<string>::iterator iter = files.begin();

			map<int, string> index_nameOfFile;

			int index = 1;
			while (iter != files.end()) {
				cout << to_string(index) + ". " + *iter << endl;
				index_nameOfFile.emplace(index, *iter);
				iter++;
				index++;
			}

			vector<int> numbers;
			string s_numbers;

			cout << "\nВведите номера нужных файлов через запятую" << endl;
			cin >> s_numbers;

			for (int i = 0; i < s_numbers.length(); i++) {
				int temp = 0;
				int j = i;
				while (i < s_numbers.length() && s_numbers[i] != ',') {
					i++;
				}
				temp += stoi(s_numbers.substr(j, i));
				if (temp != 0) {
					numbers.push_back(temp);
				}
			}

			bool sort = false;
			cout << "Требуется ли сортировка?\n" <<
				"1. Да 2. Нет (Default: 2)" << endl;
			int sort_i;
			cin >> sort_i;

			if (sort_i == 1) {

				sort = true;
			}

			int count = 0;
			cout << "Количество выведенных значений:\n" <<
				"Введите цифру, 0 == вывод всех значений\n";
			cin >> count;

			map<string, map<string, int>> result = fileHandler.theMostFrequentlyUsedWords();

			vector<int>::iterator numbers_it = numbers.begin();

			while (numbers_it != numbers.end()) {
				auto temp_file = index_nameOfFile.find(*numbers_it); //find name file by number

				if (temp_file != index_nameOfFile.end()) {

					cout << "\nФайл: " + temp_file->second + "\n" << endl;
					auto temp_map = result.find(temp_file->second); //find map for this file 

					//we dont check contain this file because lists of files names the same


					if (sort) {
						vector<pair<string, int>> sortByInt = fileHandler.sortStringInt(temp_map->second);

						vector<pair<string, int>>::iterator vectorPairIt = sortByInt.begin();

						count = count == 0 || count > sortByInt.size() ? sortByInt.size() : count;

						int index = 0;

						while (index < count && vectorPairIt != sortByInt.end()) {
							cout << vectorPairIt->first + " == " + to_string(vectorPairIt->second) << endl;
							vectorPairIt++;
							index++;
						}
					}
					else {
						map<string, int>::iterator map_it = temp_map->second.begin();

						count = count == 0 || count > temp_map->second.size() ? temp_map->second.size() : count;

						int index = 0;

						while (index < count && map_it != temp_map->second.end()) {
							cout << map_it->first + " == " + to_string(map_it->second) << endl;
							map_it++;
							index++;
						}
					}
					temp_file++;
				}

				numbers_it++;
			}
			cout << "\n";
			break;
		}
		case 4:
		{
			cout << "Список файлов:" << endl;
			vector<string> files = fileHandler.getFiles();
			vector<string>::iterator iter = files.begin();

			map<int, string> index_nameOfFile;

			int index = 1;
			while (iter != files.end()) {
				cout << to_string(index) + ". " + *iter << endl;
				index_nameOfFile.emplace(index, *iter);
				iter++;
				index++;
			}

			vector<int> numbers;
			string s_numbers;

			cout << "Введите номера нужных файлов через запятую" << endl;
			cin >> s_numbers;

			for (int i = 0; i < s_numbers.length(); i++) {
				int temp = 0;
				int j = i;
				while (i < s_numbers.length() && s_numbers[i] != ',') {
					i++;
				}
				temp += stoi(s_numbers.substr(j, i));
				if (temp != 0) {
					numbers.push_back(temp);
				}
			}

			bool sort = false;
			cout << "Требуется ли сортировка?\n" <<
				"1. Да 2. Нет (Default: 2)" << endl;
			int sort_i;
			cin >> sort_i;

			if (sort_i == 1) {

				sort = true;
			}

			int count = 0;
			cout << "Количество выведенных значений:\n" <<
				"Введите цифру, 0 == вывод всех значений" << endl;
			cin >> count;

			map<string, map<string, int>> result = fileHandler.theBiggestWords();

			vector<int>::iterator numbers_it = numbers.begin();
			while (numbers_it != numbers.end()) {
				auto temp_file = index_nameOfFile.find(*numbers_it); //find name file by number


				if (temp_file != index_nameOfFile.end()) {

					cout << "\nФайл: " + temp_file->second + "\n" << endl;
					auto temp_map = result.find(temp_file->second); //find map for this file 

					//we dont check contain this file because lists of files names the same


					if (sort) {
						vector<pair<string, int>> sortByInt = fileHandler.sortStringInt(temp_map->second);

						vector<pair<string, int>>::iterator vectorPairIt = sortByInt.begin();

						count = count == 0 || count > sortByInt.size() ? sortByInt.size() : count;

						int index = 0;

						while (index < count && vectorPairIt != sortByInt.end()) {
							cout << vectorPairIt->first + " == " + to_string(vectorPairIt->second) << endl;
							vectorPairIt++;
							index++;
						}
					}
					else {
						map<string, int>::iterator map_it = temp_map->second.begin();

						count = count == 0 || count > temp_map->second.size() ? temp_map->second.size() : count;

						int index = 0;

						while (index < count && map_it != temp_map->second.end()) {
							cout << map_it->first + " == " + to_string(map_it->second) << endl;
							map_it++;
							index++;
						}
					}

					temp_file++;
				}
				numbers_it++;
			}
			cout << "\n";
			break;
		}
		case 5:
		{
			cout << "Список файлов:" << endl;
			vector<string> files = fileHandler.getFiles();
			vector<string>::iterator iter = files.begin();

			map<int, string> index_nameOfFile;

			int index = 1;
			while (iter != files.end()) {
				cout << to_string(index) + ". " + *iter << endl;
				index_nameOfFile.emplace(index, *iter);
				iter++;
				index++;
			}

			vector<int> numbers;
			string s_numbers;

			cout << "Введите номера нужных файлов через запятую" << endl;
			cin >> s_numbers;

			for (int i = 0; i < s_numbers.length(); i++) {
				int temp = 0;
				int j = i;
				while (i < s_numbers.length() && s_numbers[i] != ',') {
					i++;
				}
				temp += stoi(s_numbers.substr(j, i));
				if (temp != 0) {
					numbers.push_back(temp);
				}
			}

			map<string, int> result = fileHandler.countSentences();

			vector<int>::iterator numbers_it = numbers.begin();
			while (numbers_it != numbers.end()) {
				auto temp_file = index_nameOfFile.find(*numbers_it); //find name file by number

				if (temp_file != index_nameOfFile.end()) {

					cout << "\nФайл: " + temp_file->second + "\n" << endl;
					auto temp_map = result.find(temp_file->second); //find map for this file 
					cout << "Количество предложений == " + to_string(temp_map->second) << endl;
					temp_file++;
				}
				numbers_it++;
			}
			cout << "\n";
			break;
		}
		case 6:
		{
			cout << "Путь к папке: " + fileHandler.getPath() << endl;

			string reg = fileHandler.getIgnoreRegister() ? "не учитываются" : "учитываются";
			cout << "Регистры " + reg << endl;

			string spec = fileHandler.getIgnoreWordsWithSpecialSymbol() ? "не учитываются" : "учитываются";
			cout << "Специальные символы " + spec << endl;
			if (!fileHandler.getIgnoreWordsWithSpecialSymbol()) {
				cout << "Список специальных символов: " + fileHandler.getSpecialSymbols() << endl;
			}

			string number = fileHandler.getIgnoreWordsWithNumber() ? "не учитываются" : "учитываются";
			cout << "Числа " + number << endl;

			string language = fileHandler.getLanguage() == Language::ENGLISH ? "английский" : "русский";
			cout << "Язык " + language << endl;

			break;

		}
		case 7:
		{
			menu = false;
			break;
		}
		default:
			cout << "Данный пункт отсутствует в меню" << endl;
			break;
		}
	}

}


