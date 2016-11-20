#include "types.h"
#include "csv.h"
#include "deltamin.h"
#include "maxintervals.h"
#include <fstream>
#include <math.h>

class EWrongParamsFile {
};

class EDelta0LessThan0 {
};

class ENu0LessThan0 {
};

class EWrongX {
};

class EWrongCSVFile {
};

int main()
{
	std::ifstream fparam("input.txt");
	std::ifstream fcsv("input.csv");
	std::ofstream fhtml("output.html");

	fhtml << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n";
	fhtml << "<html>\n";
	fhtml << "<head>\n";
	fhtml << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\">\n";
	fhtml << "<title>ClRank 2.0</title>\n";
	fhtml << "<style type=\"text/css\">\n";
	fhtml << "  .text { font-family: Courier New }\n";
	fhtml << "  TR.text TD { font-family: Courier New }\n";
	fhtml << "  TABLE.text TD { font-family: Courier New }\n";
	fhtml << "  TABLE.text { text-align: center }\n";
	fhtml << "  .error { color: red; font-family: Courier New }\n";
	fhtml << "</style>\n";
	fhtml << "</head>\n";
	fhtml << "<body>\n";
	fhtml << "  <h2>ClRank 2.0</h2>\n\n";

	try {
		if (!fparam)
			throw EWrongParamsFile();
		double delta0;
		int nu0;
		int x;
		fparam >> delta0 >> nu0 >> x;
		if (delta0 < 0)
			throw EDelta0LessThan0();
		if (nu0 < 0)
			throw ENu0LessThan0();
		if (!(x == 1 || x == 2))
			throw EWrongX();

		if (!fcsv)
			throw EWrongCSVFile();
		InputData input_data = ReadInputData(fcsv);
		int n = input_data.size();

		Points points;
		int N = x == 1 ? n+1 : 2*n+1;
		for (int i = 0; i < n; ++i) {
			int r = i+1;
			double x = log((double)(N-r)/r);
			double y = log(input_data[i].w);
			Point p(x, y);
			points.push_back(p);
		}

		fhtml << "  <h3>������� ������</h3>\n";
		fhtml << "  <table border=\"1\" cellpadding=\"2\" cellspacing=\"0\">\n";
		fhtml << "  <tr>\n";
		fhtml << "  <th>���������</th>\n";
		fhtml << "  <th><i>w</i></th>\n";
		fhtml << "  <th><i>r</i></th>\n";
		fhtml << "  <th>ln <i>w</i></th>\n";
		fhtml << "  <th>ln <i>R</i></th>\n";
		fhtml << "  </tr>\n";
		for (int i = 0; i < n; ++i) {
			fhtml << "  <tr class=\"text\">\n";
			fhtml << "  <td>" << input_data[i].name << "</td>\n";
			fhtml << "  <td>" << input_data[i].w << "</td>\n";
			fhtml << "  <td>" << i+1 << "</td>\n";
			fhtml << "  <td>" << points[i].y << "</td>\n";
			fhtml << "  <td>" << points[i].x << "</td>\n";
			fhtml << "  </tr>\n";
		}
		fhtml << "  </table>\n\n";

		fhtml << "  <h3>������� ���������</h3>\n";
		fhtml << "  <p><i>&delta;</i><sub>0</sub> = <span class=\"text\">" << delta0 << "</span></p>\n";
		fhtml << "  <p><i>&nu;</i><sub>0</sub> = <span class=\"text\">" << nu0 << "</span></p>\n";
		if (x == 1)
			fhtml << "  <p>�������������� �����������: <i>N</i> = <i>n</i> + 1.</p>\n";
		else
			fhtml << "  <p>�������������� �����������: <i>N</i> = 2<i>n</i> + 1.</p>\n";
		fhtml << "\n";

		std::vector<std::vector<double> > deltamin;
		deltamin.resize(n);
		for (int i = 0; i < n; ++i)
			deltamin[i].resize(n);
		int k = 2 > nu0-1 ? 2 : nu0-1;
		for (int a = 0; a < n; ++a) {
			for (int b = a+k; b < n; ++b) {
				double gamma1;
				deltamin[a][b] = Deltamin(points, a, b, nu0, gamma1);
			}
		}
		MaxIntervals max_intervals = MaxIntervalsSet(deltamin, delta0, nu0);

		fhtml << "  <h3>��������� ������������ �����������</h3>\n";
		fhtml << "  <span class=\"text\">\n";
		for (int i = 0; i < max_intervals.size(); ++i)
			fhtml << "  [" << max_intervals[i].a+1 << " " << max_intervals[i].b+1 << "]<br>\n";
		fhtml << "  </span>\n";
	}
	catch (EWrongParamsFile) {
		fhtml << "  <p class=\"error\">������! �� ������� ������� ���� input.txt.</p>\n";
	}
	catch (EDelta0LessThan0) {
		fhtml << "  <p class=\"error\">������ � ����� input.txt! <i>&delta;</i><sub>0</sub> ������ ���� ��������������� ������.</p>\n";
	}
	catch (ENu0LessThan0) {
		fhtml << "  <p class=\"error\">������ � ����� input.txt! <i>&nu;</i><sub>0</sub> ������ ���� ��������������� ������.</p>\n";
	}
	catch (EWrongX) {
		fhtml << "  <p class=\"error\">������ � ����� input.txt! <i>x</i> ������ ���� 1 ��� 2.</p>\n";
	}
	catch (EWrongCSVFile) {
		fhtml << "  <p class=\"error\">������! �� ������� ������� ���� input.csv.</p>\n";
	}
	catch (EWrongFieldsNumber e) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ������ " << e.line << ": ���������� ����� � ������ ������ ���� ����� 3.</p>\n";
	}
	catch (EWrongHeader) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ��������� ������ ���� \"���������;�������;����\".</p>\n";
	}
	catch (EWrongSymbol e) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ������ " << e.line << ", ������� " << e.pos << ": ������������ ������.</p>\n";
	}
	catch (EWEquals0 e) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ������ " << e.line << ": ������� ������ ���� ������������� ������.</p>\n";
	}
	catch (EWrongR e) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ������ " << e.line << ": ���� ������ ���� " << e.line-1 << ".</p>\n";
	}
	catch (ENotIncreasingW e) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ������ " << e.line << ": �������� ������� ������ ���� �� �����������.</p>\n";
	}
	catch (EEmptyFile) {
		fhtml << "  <p class=\"error\">������ � ����� input.csv! ���� ����.</p>\n";
	}

	fhtml << "</body>\n";
	fhtml << "</html>";

	return 0;
}