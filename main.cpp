#include "task.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	srand(time(NULL));

	std::string colors;
	
	std::cout << "��������� ���������� ������� ��������" << std::endl;
	std::cout << "��������� ������������������ �������� �������������� � �������������� ������." << std::endl;
	std::cout << "������� ���������� ����� ���: \"A < B < C \" ��� \"A > B > C\", ��� A, B � C - ���� �� ��������� ������:\n"
		"� - �������, � - �����, � - �������." << std::endl;
	std::cout << std::endl << "���������� �������� �������������� ��� ������ ������� Space.\n";
	std::cout << "��� ������ ��������� �������, � �������� ������ ������� ������� E (exit)" << std::endl;
	std::string rule;
	std::cout << "������� ������� ����������, ��������������� ������� ����: ";
	while (true)
	{
		std::getline(std::cin, rule);
		if (Checkrule(rule))
		{
			std::cout << "������� �����������, ���������� ��� ���" << std::endl;
		}
		else
		{
			std::cout << "OK, ��� ������� ������� ������� Space" << std::endl;
			break;
		}
	}
	
	std::thread t1;
	std::thread t2;

	char off = 0;
	while(off = _getch())
	if (off == ' ')
	{
		live = true;
		t1 = std::thread(Generator, std::ref(colorobjects), colors);
		t2 = std::thread(CompareObjects, std::ref(colorobjects), rule);
		t1.detach();
		t2.detach();
		break;
	}
	ControllingThreads();

	system("PAUSE");
    return 0;
}

