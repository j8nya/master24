#pragma once
#include <vector>
#include <map>
#include <regex>
#include "pch.h"
typedef std::vector<size_t> sentence_numbers;//������� ��������� ��� �������� ���� ������ ��� �������� ������ sentence_numbers
void DeletePunctuation(std::string& target);

void Sortirovka(std::vector<std::string> words);

std::vector<std::string> Split(const std::string& target, char delimiter);

void SaveToMap(const std::vector<std::string>& sentence/*������ ���� ����*/,
	std::map<std::string, sentence_numbers>& output/*map-� ��� ������� ����� � ����� ����������� � ������ ��� ���������*/,
	size_t serialNumber/*����� �����������*/);

std::map<std::string, sentence_numbers> ExtractText(const std::string& filePath);

bool IsParticiple(const std::string& word, const std::vector<std::regex>& patterns);

std::map<std::string, sentence_numbers> FilterParticiples(std::map<std::string,
	sentence_numbers>& words, const std::vector<std::regex>& patterns);

void SaveText(/*�������� map (��� �� ������ �����, � ������������; ����� ����������� � ������� ��� ���������)*/const std::map<std::string, sentence_numbers>& words,
	/*���� � ������� ����������*/const std::string& fileName, /*�� ���� ����� �������� �����*/const std::string& filePath)
