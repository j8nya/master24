#pragma once
#include <vector>
#include <map>
#include <regex>
#include "pch.h"
typedef std::vector<size_t> sentence_numbers;//создаем псевдоним для названия типа данных для короткой записи sentence_numbers
void DeletePunctuation(std::string& target);

void Sortirovka(std::vector<std::string> words);

std::vector<std::string> Split(const std::string& target, char delimiter);

void SaveToMap(const std::vector<std::string>& sentence/*вектор всех слов*/,
	std::map<std::string, sentence_numbers>& output/*map-в нее запишем слово и номер предложения в которм оно находится*/,
	size_t serialNumber/*номер предложения*/);

std::map<std::string, sentence_numbers> ExtractText(const std::string& filePath);

bool IsParticiple(const std::string& word, const std::vector<std::regex>& patterns);

std::map<std::string, sentence_numbers> FilterParticiples(std::map<std::string,
	sentence_numbers>& words, const std::vector<std::regex>& patterns);

void SaveText(/*итоговая map (уже не просто слово, а деепричастие; номер предложения в котором оно находится)*/const std::map<std::string, sentence_numbers>& words,
	/*файл в который записываем*/const std::string& fileName, /*от куда берем исходный текст*/const std::string& filePath)
