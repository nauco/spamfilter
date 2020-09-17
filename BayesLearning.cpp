#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define WORDNUMBER 55
#define DATANUMBER 4601

double spam_idx[6] = { 300, 600, 900, 1200, 1500, 1813 };
double ham_idx[6] = { 400, 800, 1200, 1700, 2200, 2788 };

double spam[6][WORDNUMBER];
double ham[6][WORDNUMBER];


int main()
{
	ifstream spambase("Spambase.txt");
	ofstream parameter("parameter.h");

	string str;
	int condition;

	for (size_t i = 0; i < WORDNUMBER; i++)
	{
		spambase >> str;
		cout << str << endl;
	}
	int idx = 0;

	for (size_t i = 1; i <= spam_idx[5]; i++)
	{
		if (i-1 == spam_idx[idx])
		{
			for (size_t j = 0; j < WORDNUMBER; j++)
			{
				spam[idx + 1][j] = spam[idx][j];
			}
			idx++;
		}
		for (size_t j = 0; j < WORDNUMBER; j++)
		{
			spambase >> condition;
			if (condition == 1)
			{
				spam[idx][j]++;
			}
		}
	}
	idx = 0;
	for (size_t i = 1; i <= ham_idx[5]; i++)
	{
		if (i - 1 == ham_idx[idx])
		{
			for (size_t j = 0; j < WORDNUMBER; j++)
			{
				ham[idx + 1][j] = ham[idx][j];
			}
			idx++;
		}
		for (size_t j = 0; j < WORDNUMBER; j++)
		{
			spambase >> condition;
			if (condition == 1)
			{
				ham[idx][j]++;
			}
		}
	}

	parameter << "double spam_parameter[6][55] = {";

	for (size_t i = 0; i < 6; i++)
	{
		parameter << "{";
		for (size_t j = 0; j < WORDNUMBER-1; j++)
		{
			parameter << (spam[i][j] / spam_idx[i]) << ", ";
		}
		parameter << spam_idx[i] / (spam_idx[i] + ham_idx[i]) << "}," << endl;
	}
	parameter << "};";


	parameter << "double ham_parameter[6][55] = {";
	for (size_t i = 0; i < 6; i++)
	{
		parameter << "{";
		for (size_t j = 0; j < WORDNUMBER -1; j++)
		{
			parameter << (ham[i][j] / ham_idx[i]) << ", ";
		}
		parameter << ham_idx[i] / (spam_idx[i] + ham_idx[i]) << "}," << endl;
	}
	parameter << "};";


	return 0;
}
