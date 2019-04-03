#include "task.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	srand(time(NULL));

	std::string colors;
	
	std::cout << "Программа сортировки цветных объектов" << std::endl;
	std::cout << "Генерация последовательности объектов осуществляется в автоматическом режиме." << std::endl;
	std::cout << "Правило сортировки имеет вид: \"A < B < C \" или \"A > B > C\", где A, B и C - один из возможных цветов:\n"
		"К - красный, С - синий, З - зеленый." << std::endl;
	std::cout << std::endl << "Управление потоками осуществляется при помощи клавиши Space.\n";
	std::cout << "Для полной остановки потоков, в процессе работы нажмите клавишу E (exit)" << std::endl;
	std::string rule;
	std::cout << "Введите правило сортировки, соответствующее образцу выше: ";
	while (true)
	{
		std::getline(std::cin, rule);
		if (Checkrule(rule))
		{
			std::cout << "Правило некорректно, попробуйте еще раз" << std::endl;
		}
		else
		{
			std::cout << "OK, для запуска потоков нажмите Space" << std::endl;
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

