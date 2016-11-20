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

class EWrongIntervalsFile {
};

class EWrongInterval1 {
};

class EWrongInterval {
public:
	int num, a, b;
	EWrongInterval(int num, int a, int b)
		: num(num), a(a), b(b)
	{
	}
};

int main()
{
	std::ifstream fparam("input.txt");
	std::ifstream fintervals("input_gamma.txt");
	std::ifstream fcsv("input.csv");
	std::ofstream fhtml("output_gamma.html");

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

		if (!fintervals)
			throw EWrongIntervalsFile();
		std::vector<Interval> intervals;
		while (1) {
			int a, b;
			fintervals >> a;
			if (!fintervals)
				break;
			fintervals >> b;
			if (!fintervals)
				throw EWrongInterval1();
			if (!(a >= 1 && a <= n && b >= 1 && b <= n && b-a+1 >= 3))
				throw EWrongInterval(intervals.size()+1, a, b);
			intervals.push_back(Interval(a-1, b-1));
		}

		std::vector<std::vector<double> > deltamin;
		std::vector<std::vector<double> > gamma;
		deltamin.resize(intervals.size());
		gamma.resize(intervals.size());
		for (int i = 0; i < intervals.size(); ++i) {
			deltamin[i].resize(nu0+1);
			gamma[i].resize(nu0+1);
			for (int nu = 0; nu <= nu0; ++nu) {
				if (nu <= intervals[i].b-intervals[i].a-1)
					deltamin[i][nu] = Deltamin(points, intervals[i].a, intervals[i].b, nu, gamma[i][nu]);
			}
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
		fhtml << "  <p><i>&nu;</i><sub>0</sub> = <span class=\"text\">" << nu0 << "</span></p>\n";
		if (x == 1)
			fhtml << "  <p>�������������� �����������: <i>N</i> = <i>n</i> + 1.</p>\n";
		else
			fhtml << "  <p>�������������� �����������: <i>N</i> = 2<i>n</i> + 1.</p>\n";
		fhtml << "\n";

		fhtml << "  <h3>�������� ��������� <i>&gamma;</i></h3>\n";
		fhtml << "  <table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" class=\"text\">\n";
		fhtml << "  <tr>\n";
		fhtml << "    <th><i>&nu;</i></th>\n";
		for (int nu = 0; nu <= nu0; ++nu)
			fhtml << "    <th>" << nu << "</th>\n";
		fhtml << "  </tr>\n";
		for (int i = 0; i < intervals.size(); ++i) {
			fhtml << "  <tr>\n";
			fhtml << "    <th>[" << intervals[i].a+1 << "&nbsp;" << intervals[i].b+1 << "]</th>\n";
			for (int nu = 0; nu <= nu0; ++nu) {
				fhtml << "    <td>";
				if (nu <= intervals[i].b-intervals[i].a-1)
					fhtml << gamma[i][nu];
				else
					fhtml << "&nbsp;";
				fhtml << "</td>\n";
			}
			fhtml << "  </tr>\n";
		}
		fhtml << "  </table>\n\n";

		fhtml << "  <h3>�������� ����������� ������ ������ � <i>&nu;</i><sub>0</sub> ���������� �����</h3>\n";
		fhtml << "  <table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" class=\"text\">\n";
		fhtml << "  <tr>\n";
		fhtml << "    <th><i>&nu;</i></th>\n";
		for (int nu = 0; nu <= nu0; ++nu)
			fhtml << "    <th>" << nu << "</th>\n";
		fhtml << "  </tr>\n";
		for (int i = 0; i < intervals.size(); ++i) {
			fhtml << "  <tr>\n";
			fhtml << "    <th>[" << intervals[i].a+1 << "&nbsp;" << intervals[i].b+1 << "]</th>\n";
			for (int nu = 0; nu <= nu0; ++nu) {
				fhtml << "    <td>";
				if (nu <= intervals[i].b-intervals[i].a-1)
					fhtml << deltamin[i][nu];
				else
					fhtml << "&nbsp;";
				fhtml << "</td>\n";
			}
			fhtml << "  </tr>\n";
		}
		fhtml << "  </table>\n\n";
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
	catch (EWrongIntervalsFile) {
		fhtml << "  <p class=\"error\">������! �� ������� ������� ���� input_gamma.txt.</p>\n";
	}
	catch (EWrongInterval1) {
		fhtml << "  <p class=\"error\">������! ��� ���������� ���������� �� ������ �����.</p>\n";
	}
	catch (EWrongInterval e) {
		fhtml << "  <p class=\"error\">������! ������������ ���������� ����� " << e.num << ": [" << e.a << " " << e.b << "].</p>\n";
	}

	fhtml << "</body>\n";
	fhtml << "</html>";

	return 0;
}