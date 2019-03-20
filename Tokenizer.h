#pragma once 

#include <vector>

using namespace std;

class Tokenizer {
public:
	static vector<string> Parse(string line, string seperator) {
		vector<string> tokens;
		int startPos = 0;
		size_t foundPos = line.find(seperator, startPos);

		while (foundPos != string::npos) {
			int count = foundPos - startPos;
			string token = line.substr(startPos, count);
			tokens.push_back(token);

			startPos = foundPos + seperator.length();

			foundPos = line.find(seperator, startPos);
		}

		//Phan con lai chinh la phan tu cuoi
		int count = line.length() - startPos;
		string token = line.substr(startPos, count);
		tokens.push_back(token);

		return tokens;
	}

	static vector<string> Split(string line, string seperator, int count) {
		vector<string> tokens;
		int startPos = 0;
		size_t foundPos = line.find(seperator, startPos);
		int c = 1;
		while (foundPos != string::npos) {
			if (c == count) {
				string token = line.substr(0, foundPos);
				tokens.push_back(token);
				token = line.substr(foundPos + seperator.length(), line.length());
				tokens.push_back(token);
				return tokens;
			}
			else {
				c++;
				startPos = foundPos + seperator.length();
				foundPos = line.find(seperator, startPos);
			}
		}
		return tokens;
	}
};