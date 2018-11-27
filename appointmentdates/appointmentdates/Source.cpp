
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <conio.h>
//
//struct appointmentStruct
//{
//	vector<char*> fileNumbers;
//	
//};
int maxAppointmentsPerDay = 30;
using namespace std;
//map<chrono::system_clock::time_point, vector<char*>> dataset;

std::fstream myfile;
struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

map<string, vector<string>> dataset;
void SetTextColour(WORD colour)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, colour);
}
void SetError()
{
	SetTextColour( FOREGROUND_RED);
}
void SetNormal()
{
	SetTextColour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
chrono::system_clock::time_point stringToTimePoint(const char* c)
{
	std::tm tm = {};
	std::stringstream ss(c);
	ss >> std::get_time(&tm, "%d:%m:%Y");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	return tp;
}

void timePointToString(chrono::system_clock::time_point tp,char* buffer, int buffersize)
{
	std::time_t now_c = std::chrono::system_clock::to_time_t(tp);
	std::tm now_tm = *std::localtime(&now_c);
	
	strftime(buffer, buffersize, "%d:%m:%Y", &now_tm);
}
void Save(string date,  string filenumber)
{
	/*HANDLE hFile = CreateFile("data.txt",
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwBytesWritten = 0;
	DWORD dwMoved = ::SetFilePointer(hFile, 0l, nullptr, FILE_END);
	string toWrite = date + "\t" + filenumber + "\n\r";

		WriteFile(hFile, toWrite.c_str(), toWrite.length() +3, &dwBytesWritten, NULL);

	CloseHandle(hFile);*/
	std::ofstream out("data.txt", std::ofstream::out | std::ofstream::app);
	out << date<<"\t"<< filenumber<<"\n";
	out.close();

	//
	////Here would be some error handling
	//for (int i = 0; i < 32; ++i) {
	//	//Some calculations to fill a[]
	//}
	//for (const auto& i : dataset)
	//{

	//}
	//myfile.close();

}
void ReadFile()
{

	std::ifstream infile("data.txt");


	char str[255];

	while (infile) {
		infile.getline(str, 255);  // delim defaults to '\n'
		if (infile)
		{
			char* del = strstr(str, "\t");
			char* date[32] = { 0 };
			*del = '\0';
			del++;
			auto dateData = dataset[str];
			dateData.push_back(del);
			dataset[str] = dateData;
		//	cout << str << endl;
		}

	}
	infile.close();
	//ifstream ifs(filename, ios::binary | ios::ate);
	//ifstream::pos_type pos = ifs.tellg();

	// What happens if the OS supports really big files.
	// It may be larger than 32 bits?
	// This will silently truncate the value/
	//int length = pos;

	// Manuall memory management.
	// Not a good idea use a container/.
	//char *pChars = new char[length];
	//ifs.seekg(0, ios::beg);
	//ifs.read(pChars, length);

	//// No need to manually close.
	//// When the stream goes out of scope it will close the file
	//// automatically. Unless you are checking the close for errors
	//// let the destructor do it.
	//ifs.close();
}
void Add()
{
	system("cls");
	while (true)
	{
		
		char date[32] = { 0 };
		cout << "enter Date (DD\\MM\\YYYY) or q to quit: ";
		cin >> date;
		if (date[0] == 'q')
			return;
		//auto tp = stringToTimePoint(date);
		
		auto dateData = dataset[date];
		int currentsize = dateData.size();
		if (currentsize >= maxAppointmentsPerDay)
		{
			SetError();
			cerr << "max per day reached\n\n";
			SetNormal();
			continue;
		}
		cout << "there are currently " << currentsize << " appointments for " <<  date << "\n";
		char file[32] = { 0 };
		cout << "enter filenumber : ";
		cin >> file;
		dateData.push_back(file);
		dataset[date] = dateData;
		Save(date, file);
		//myfile.write((char*)&tp, sizeof tp);
		system("cls");
	}

}

void view()
{
	system("cls");
	char date[32] = { 0 };
	cout << "enter a date to view (DD\\MM\\YYYY) or * to view all : ";
	cin >> date;
	//auto tp = stringToTimePoint(date);
	if (date[0] == '*')
	{
		for (const auto& e : dataset)
		{
			for (const auto& s : e.second)
			{
				cout << e.first << "\t" << s << "\n";
			}
		}
	}
	else
	{
		auto dateData = dataset[date];
		for (const auto& s : dateData)
		{
			cout << date << "\t" << s << "\n";
		}
	}

	system("pause");
}

void change()
{
	system("cls");
	while (true)
	{
		char newmax[32] = { 0 };
		cout << "enter new maximum per day (current=" << maxAppointmentsPerDay << ") : ";
		cin >> newmax;
		int val = atoi(newmax);
		char temp[32] = { 0 };
		_itoa(val, temp, 10);
		if (strcmp(temp, newmax) != 0)
		{
			SetError();
			cerr << "invalid number\n\n";
			SetNormal();
		}
		else
		{
			maxAppointmentsPerDay = val;
			system("cls");
			return;
		}
	}
}

int main()
{
	
	//myfile = std::fstream("file.binary", std::ios::out | std::ios::binary);
	auto t = stringToTimePoint("10:03:1988");
	auto d = chrono::duration_cast<std::chrono::hours>(t.time_since_epoch()).count();
	//auto r = chrono::system_clock::time_point{ d };
	char s[32] = { 0 };
	//timePointToString(r, s, 32);
	//cout << s;
	ReadFile();
	while (true)
	{
		SetNormal();
		system("cls");
		char option[32] = { 0 };
		cout << "Current max per date is : " << maxAppointmentsPerDay << "\n\n";
		cout << "Please select one of the following :\n (a) appointment date\n (v) view appointments for a date\n (c) change the maximum per day\n (q) quit\n <a/v/c/q> ? : ";
		cin >> option;
		if (option[0] == 'q')
			break;
		if (option[0] == 'a')
			Add();
		if (option[0] == 'v')
			view();
		if (option[0] == 'c')
			change();
	}
	return 0;
}

