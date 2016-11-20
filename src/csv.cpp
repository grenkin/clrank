#include "csv.h"
#include <fstream>

InputData ReadInputData(std::ifstream &fcsv)
{
	InputData input_data;
	int line = 0;
	double pw;
	while (1) {
		std::string s;
		getline(fcsv, s);
		if (!fcsv)
			break;
		++line;
		int cnt = 0;
		for (int i = 0; i < s.length(); ++i) {
			if (s[i] == ';')
				++cnt;
		}
		if (cnt != 2)
			throw EWrongFieldsNumber(line);
		if (line == 1) {
			if (s != "Категория;Частота;Ранг")
				throw EWrongHeader();
		}
		else {
			int i = 0;
			std::string name;
			while (s[i] != ';') {
				name += s[i];
				++i;
			}
			++i;
			double w = 0.0;
			while (s[i] != ',' && s[i] != ';') {
				if (!(s[i] >= '0' && s[i] <= '9'))
					throw EWrongSymbol(line, i+1);
				int digit = s[i]-'0';
				w = w*10 + digit;
				++i;
			}
			if (s[i] == ',') {
				++i;
				double z = 1.0;
				while (s[i] != ';') {
					if (!(s[i] >= '0' && s[i] <= '9'))
						throw EWrongSymbol(line, i+1);
					int digit = s[i]-'0';
					z /= 10;
					w = w + digit*z;
					++i;
				}
			}
			++i;
			int r = 0;
			while (i < s.length()) {
				if (!(s[i] >= '0' && s[i] <= '9'))
					throw EWrongSymbol(line, i+1);
				int digit = s[i]-'0';
				r = r*10 + digit;
				++i;
			}

			if (w == 0)
				throw EWEquals0(line);
			if (r != line-1)
				throw EWrongR(line);
			if (line >= 3) {
				if (w < pw)
					throw ENotIncreasingW(line);
			}
			pw = w;

			InputDataRecord input_data_record(name, w);
			input_data.push_back(input_data_record);
		}
	}
	if (line == 0)
		throw EEmptyFile();
	return input_data;
}