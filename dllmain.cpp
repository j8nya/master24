// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#include <algorithm>//std::remove_if-удаления элементов из контейнера
#include <iostream>//ввод-вывод
#include <conio.h>//getch
#include <limits>
#include <fstream>//функционал считывания данных из файла и для записи в файл
#include <vector>//использование вектора
#include <regex>//использование регулярных выражений
#include <string>//использование строк
#include <sstream>//поддержка потоковой работы со строчным типом данных
#include <windows.h>//для использования SetConsoleCP(1251); SetConsoleOutputCP(1251)
#include <map>//ассоциативный контейнер, имеет структуру бинарного дерева и хранят пару - ключ и значение
//map-хранит уникальные значения, построение происходит по ключу, а значение просто подвязывается

typedef std::vector<size_t> sentence_numbers;//создаем псевдоним для названия типа данных для короткой записи sentence_numbers
const std::string path_to_input("C:/Users/grish/source/repos/participles/message.txt");
const std::string path_to_output("C:/Users/grish/source/repos/participles/output.txt");

std::vector<std::string> NumericSentence;

__declspec(dllexport)
// Удаление символов пунктуации , мешающих поиску регулярных выражений
void DeletePunctuation(std::string& target) {

	//********************************************** не совсем правильно************************
	//remove_if-смотрит от начала до конца строки (слова), рассматривает каждый char и ф-и передается этот каждый char и ispunct решает удалять его или нет,
	//если это знак прпепинания ispunct возвращает true и remove_if когда ему лямбда ф-я возвращает true удяляет этот char
	//********************************************** не совсем правильно************************

	//remove_if принимает 3 параметра: итератор на начало предложения, итератор на конец предложения, (какое-то условие программиста) логическую функцию
	//(у нас лямбда функция которя смотрит является ли символ (char) символом пунктуации, если true то стирает знак препинания)
	//по сути remove_if не удаляет символ а перемещает его в конец и дает нам итератор на него
	//а уже метод erase принимает 2 итератора(диапозон) от какого до какого надо удалить

	//Лямбда-функции - это функции внутри другой функции, у которой фактически нет имени 
	target.erase(remove_if(target.begin(), target.end(), [](char w) {
		//ispunct() возвращает ненулевое значение, если аргумент ch является знаком пунктуации или пробелом; в противном случае возвращается 0
		return ispunct(static_cast<unsigned char>(w));
		}), target.end());
}

__declspec(dllexport)
//функция сортировки (шейкерная)
//words-вектор всех слов
void Sortirovka(std::vector<std::string> words) {
	//std::sort(words.begin(), words.end());
	const int n = words.size();  //константа, содержащая размер вектора (колво элементов)
	int flag;  //переменная, показывающая, были ли перестановки
	int left = 1;  //левая граница сортируемого вектора
	int right = n - 1;  //правая граница сортируемого вектора
	do {
		flag = 1;
		for (int i = left; i <= right; i++) {
			if (words[i - 1] > words[i]) {  //сравниваем соседние элементы
				swap(words[i - 1], words[i]);  //меняем их местами
				flag = 0;  //проверка на наличие перестановки
			}
		}
		right--;
		for (int i = right; i >= left; i--) {
			if (words[i] < words[i - 1]) {  //сравниваем соседние элементы
				swap(words[i], words[i - 1]);  //меняем их местами
				flag = 0;  //проверка на наличие перестановки
			}
		}
		left++;
	} while (flag == 0);
}

__declspec(dllexport)
// Функция, разделяющая слова по заданному символу
std::vector<std::string> Split(const std::string& target, char delimiter) {
	// istringstream - удобное средство для просмотра строки без ее изменения
	//нужна для того чтобы посмотреть что хранится в строке, эта штука быстрее чем если мы бы просто передали строку как аргумент функции
	std::istringstream iss(target);//чтобы считать информацию из потока, в string не получится писать числа, потребуется перевод их в char, т.к. в предложении есть цифры и из-за этого возникают сложности
	std::string item;//буферная переменная в которую считываются слова
	std::vector<std::string> words;//вектор всех слов
	// Считываем из строки в буферную переменную до разделителя (пробела)
	while (std::getline(iss, item, delimiter)) {
		//удаляем знаки препинания, т.к. они могут идти сразу за словом
		DeletePunctuation(item);
		//добавляем все слова, без знаков препинания в вектор
		words.push_back(item);
	}
	Sortirovka(words);
	return words;
}

__declspec(dllexport)
// Функция-добавления в map слов и номера предложений //счетчик повторяющихся слов
void SaveToMap(const std::vector<std::string>& sentence/*вектор всех слов*/,
	std::map<std::string, sentence_numbers>& output/*map-в нее запишем слово и номер предложения в которм оно находится*/,
	size_t serialNumber/*номер предложения*/) {
	for (const auto&/*const std::string&*/ word : sentence) {
		//вставляем в map пару слова (из вектора слов) и номер предложения в котором он находится
		auto/*std::pair<std::string, sentence_numbers>*/pair = output.insert(std::pair<std::string, sentence_numbers>(/*инициализируем словом из вектора и одним номером предложения(если предложений несколько)*/word,
			/*вектор в котором хранится номера предложений*/sentence_numbers{ serialNumber }));
		//Инсерт возращает тру вторым значением пары, если элемент вставился новый. И фолс если он там уже есть
		//Если инсерт произошел, то вставляем в элемент мапы нужные данные

		if (!pair.second) {//если это слово есть в нескольких предложениях, то добавляем этот новый номер предложения в вектор номеров предложений

			//Соответсвенно если такой элемент есть в мапе я увеличиваю его значение по итератору на 1

			// Создаем пару - слово и количество вхождений в текст
			// Добавляем в мар, если такого слова нет - то количество вхождений - 1
			// иначе увеличиваем на один
			pair.first->second.push_back(serialNumber);
		}
	}
}

__declspec(dllexport)
// Функция, считывающая предложения из "input.txt" и присваивающая порядковый номер
std::map<std::string, sentence_numbers> ExtractText(const std::string& filePath) {
	//обьявъляем файловую переменную file
	std::ifstream file(filePath); //создать объект класса ifstream и связываем его с файлом, из которого будет производиться считывание
	std::map<std::string, sentence_numbers> words;//создаем map (слово; номер предложения)
	// проверка существования файла для считывания
	if (!file.is_open()) { //<-если файл не открыт
		std::cout << std::endl << "Отсутсвует файл '" << path_to_input << "' в папке с программой." << std::endl
			<< "Пожалуйста, проверьте его наличие и попробуйте снова." << std::endl;
		system("pause");
		exit(0);  //выход из программы
		return words;
	}
	//<-если файл открыт
	std::string line;//буферная переменная для записи в нее предложений
	size_t sentenceNumber = 1;//нумерование предложений
	//std::vector<std::string> NumericSentence;
	//std::cout << std::endl << "Исходный текст, находящийся в файле " << filePath << ":" << std::endl;
	// delimiter слово по которому getline разделяет входящий поток символов
	while (std::getline(file, line, '.')) {
		line += " (" + std::to_string(sentenceNumber) + ").";
		//Функция-член to_string преобразует объект в строку
		NumericSentence.push_back(line);
		//использовать если нужно будет напротив каждого слова ставить его предложение
		//NumericSentence.resize(sentenceNumber, line);//sentenceNumber-на какое число поменять, line-чем заполнить вектор
		SaveToMap(Split(line, ' '), words/*map-в нее запишем слово и номер предложения в котором оно находится*/, sentenceNumber); // закладываем все слова в контейнер с номером предложения, откуда они взяты
		++sentenceNumber;
	}

	//std::cout << NumericSentence[0] << std::endl;

	return words;
}

__declspec(dllexport)
// Проверка слова на соответствие регулярным выражениям 
bool IsParticiple(const std::string& word, const std::vector<std::regex>& patterns) {
	std::smatch sm;//сравнитель
	//Туда просто сохраняется то, что подошло под регулярку

	for (const auto&/*std::regex*/ pattern : patterns) {
		//Функция regex_match проверяет, соответствует ли вся строка заданному RE
		//первым аргументом нужно указать строку(слово), далее нужно указать либо RE, либо переменную класса smatch, 
		//куда будет помещен результат сопоставления, а затем RE. Методы класса smatch позволяют получить подстроки, 
		//соответствующие подвыражениям, их позицию и длину 
		if (std::regex_match(word, sm, pattern)) {
			//если сравнение совпадает
			return true;
		}
	}
	return false;
}

__declspec(dllexport)
// Функция сравнения map (слово; номер предложения в котором оно находится) с vector регулярных выражений
std::map<std::string, sentence_numbers> FilterParticiples(std::map<std::string,
	sentence_numbers>& words, const std::vector<std::regex>& patterns) {
	std::map<std::string, sentence_numbers> answer;//итоговая map (уже не просто слово, а деепричастие; номер предложения в котором оно находится)
	for (const auto&/*std::pair<std::string, sentence_numbers>*/ word : words) {
		if (IsParticiple(word.first, patterns)) {
			//если сравнение совпадает (true), т.е. слово сопадает с регулярнымы выражениями, то добавляем пару в итогвую map (деепричастие; номер предложения в котором оно находится)

			//можно сделать сортировку здесь (только для деепричастий)
			answer.insert(word);
		}
	}
	return answer;
}

__declspec(dllexport)
// Запись отформатированных данных в файл
void SaveText(/*итоговая map (уже не просто слово, а деепричастие; номер предложения в котором оно находится)*/const std::map<std::string, sentence_numbers>& words,
	/*файл в который записываем*/const std::string& fileName, /*от куда берем исходный текст*/const std::string& filePath) {
	std::ofstream out(fileName);//открываем файл на запись
	if (words.empty()) out << std::endl << "Деепричастия не найдены" << std::endl;
	else//если же итоговая map не пустая, то
	{
		out << std::endl << "Деепричастия, встречающиеся во входном файле '" << filePath << "' :" << std::endl << std::endl;
		for (auto const& word : words) {
			out << "Деепричастие \"" << word.first << "\" встречается " << word.second.size();
			//Second это массив с номерами предложений в которых встречается слово. Его размер - количество встречающихся одинаковых слов
			word.second.size() % 10 >= 2 && word.second.size() % 10 <= 4 //если встречается 2,3,4 раза (% — остаток от деления)
				?// если условие вернет true
				out << " раза"
				: //если условие возвращает false
				out << " раз";

			out << " в предложени";
			word.second.size() > 1 ?/*true*/ out << "ях" :/*false*/ out << "и";
			out << ": ";
			int i = word.second.size();//количество встречающихся одинаковых слов, т.е. будет 2 или более номера предложения в котором хранится эти слова
			for (const auto& sentence_number : word.second) {
				out << sentence_number;
				if (i > 1)
				{
					out << ", ";
					i--;//чтобы не выводилась лишняя запятая после последнего номера предложения
				}
			}

			out << std::endl;
		}
	}
	out << std::endl << std::endl << "Исходный текст: " << std::endl << std::endl;

	//вывод пронумерованного исходного текста
	for (int i = 0; i < NumericSentence.size(); ++i)
	{
		out << NumericSentence[i];
	}


	out.close();
}

int main() {

	SetConsoleCP(1251);//русский язык в консоли
	SetConsoleOutputCP(1251);//русский язык в файле

	std::cout << "\nДобро пожаловать в программу по поиску деепричастий!\n";


	while (true) {   //бесконечный цикл для единственного выхода из программы (для реализации меню)
		std::cout << "\nТекст будет считан из файла: '" << path_to_input << "' в папке с программой.\n";
		std::cout << "\nДля отображения деепричастий в текстовом файле нажмите 1..\n"
			"Для выхода из программы нажмите esc.." << std::endl;

		switch (char go = _getch()) {
		case '1': {
			system("cls");  //очистка экрана

			//Регулярные выражения (фильтрация слов по шаблону) — это шаблоны, используемые для сопоставления последовательностей символов.
			//static можно убрать это повлияет только на производительность, с ним чуть быстрее (он компилируется один раз и этот объект используется используется везде)
			static std::regex participle1("^.*?(вш).*$");
			static std::regex participle2("^.*?(ав)$");
			static std::regex participle3("^.*?(яв)$");
			static std::regex participle4("^.*?(ув)$");
			static std::regex participle5("^.*?(вши).*$");
			static std::regex participle6("^.*?(ши)$");
			static std::regex participle7("^.*?(ясь)$");
			static std::regex participle8("^.*?(ив)$");
			//static std::regex participle9("^.*?(учи)$");
			//static std::regex participle10("^.*?(ючи)$");
			//static std::regex participle11("^.*?(а)$");
			//static std::regex participle12("^.*?(ая)$");
			static std::regex participle13("^.*?(ыв)$");
			//static std::regex participle14("^.*?(ась)$");
			static std::regex participle15("^.*?(яя)$");

			//точка-любой символ

			// вектор, заполненный созданными регулярными выражениями
			std::vector<std::regex> participles{
				participle1
				, participle2
				, participle3
				, participle4
				, participle5
				, participle6
				, participle7
				, participle8
				//, participle9
				//, participle10
				//, participle11
				//, participle12
				, participle13
				//, participle14
				, participle15
			};


			auto/*std::map<std::string, sentence_numbers>*/ words = ExtractText(path_to_input);//итог: map (слово; номер предложения в котором оно находится)
			auto/*std::map<std::string, sentence_numbers>*/ result = FilterParticiples(words, participles);//итог: map(деепричастие; номер предложения в котором оно находится)
			SaveText(result, path_to_output, path_to_input);

			std::cout << std::endl << "Результаты работы программы находятся в " << path_to_output << ".\n\nНажмите Enter для выхода из программы..\n";
			exit(0);
		}

		case 27: exit(0);

		default: {  //проверка на следование пунктам меню
			system("cls");  //очистка экрана
			std::cout << std::endl << "Пожалуйста, следуйте указаниям меню." << std::endl;
			break;
		}
		}
	}
	return 0;
}