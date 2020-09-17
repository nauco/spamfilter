#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include "parameter.h"
using namespace std;

#define WORDNUMBER 55

string table[WORDNUMBER] = { "make", "address", "all", "3d", "our", "over", "remove", "internet",
"order", "mail", "receive", "will", "people", "report", "addresses", "free",
"business", "email", "you", "credit", "your", "font", "000", "money", "hp", "hpl", "george",
"650", "lab", "labs", "telnet", "857", "data", "415", "85", "technology", "1999", "parts", "pm",
"direct", "cs", "meeting", "original", "project", "re", "edu", "table", "conference",
";", "(", "[", "!", "$", "#", "is_spam" };

bool vector[WORDNUMBER];


int main(int argc, char **argv)
{
	int spam_spam[6] = { 0, 0, 0, 0, 0, 0 }, spam_ham[6] = { 0, 0, 0, 0, 0, 0 };
	int ham_spam[6] = { 0, 0, 0, 0, 0, 0 }, ham_ham[6] = { 0, 0, 0, 0, 0, 0 };

	if (argc != 2)
	{
		cout << "SpamFilter.exe" << endl;
		return 0;
	}
	string input_path = argv[1];
	string path = input_path + "spam\\*.*";
	string _path = input_path + "spam\\";
	string ham = input_path + "ham\\*.*";
	string _ham = input_path + "ham\\";
	string str;

	double prob_spam[6] = { 0, 0, 0, 0, 0, 0 }, prob_ham[6] = { 0, 0, 0, 0, 0, 0 };

	struct _finddata_t fd;
	intptr_t handle;

	//spam: tc=0 ham: tc=1
	for (size_t tc = 0; tc < 2; tc++)
	{
		if (tc==1)
		{
			path = ham;
			_path = _ham;
		}
		if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
		{
			cout << "No file in directory!" << endl;
			return 0;
		}
		_findnext(handle, &fd);	//.
		_findnext(handle, &fd);	//..
		do
		{
			//cout << fd.name << endl;
			ifstream testset(_path + fd.name);

			while (testset >> str)	//parser
			{
				for (size_t i = 0; i < WORDNUMBER - 1; i++)
				{
					if (str.compare(table[i]) == 0)
					{
						vector[i] = true;
					}
				}
			}

			for (size_t i = 0; i < 6; i++)
			{
				prob_spam[i] = spam_parameter[i][WORDNUMBER - 1];
				prob_ham[i] = ham_parameter[i][WORDNUMBER - 1];
			}

			for (size_t i = 0; i < WORDNUMBER - 1; i++)
			{
				if (vector[i])
				{
					for (size_t j = 0; j < 6; j++)
					{
						prob_spam[j] = prob_spam[j] * spam_parameter[j][i];
						prob_ham[j] = prob_ham[j] * ham_parameter[j][i];
					}
				}
				else
				{
					for (size_t j = 0; j < 6; j++)
					{
						prob_spam[j] = prob_spam[j] * (1 - spam_parameter[j][i]);
						prob_ham[j] = prob_ham[j] * (1 - ham_parameter[j][i]);
					}
				}
			}

			for (size_t i = 0; i < 6; i++)
			{
				if (prob_ham[i] > prob_spam[i])
				{
					if (tc == 0)
					{
						spam_ham[i]++;	//false negative
					}
					else
					{
						ham_ham[i]++;	//true negative
					}
				}
				else
				{
					if (tc == 0)
					{
						spam_spam[i]++;	//true positive
					}
					else
					{
						ham_spam[i]++;	//false positive
					}
				}
			}

			memset(vector, 0, WORDNUMBER);
			testset.close();

		} while (_findnext(handle, &fd) == 0);
		_findclose(handle);
	}
	

	for (size_t i = 0; i < 6; i++)
	{
		cout << "Classifier " << i + 1 << ":\t" << spam_spam[i] << "\t" << ham_ham[i] << "\t" << ham_spam[i] << "\t" << spam_ham[i]
			<< "\t" << spam_spam[i] / (double)(spam_spam[i] + ham_spam[i]) << "\t" << spam_spam[i] / (double)(spam_ham[i] + spam_spam[i]) << endl;
	}



	return 0;
}
