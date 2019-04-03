#pragma once
#include <string>
#include <list>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <conio.h>
#include <math.h>
#include <time.h>

/*
	Класс "Цветные объекты". Конструктор по умолчанию данного класса генерирует цвет объекта случайным образом;
	конструктор с параметром char создает объект с тем цветом, что был передан как параметр char.
*/
class ColorObject {

	char color;
public:
	ColorObject() {
		int k = rand() % 3;
		switch (k)
		{
		case 0: color = 'К';
			break;
		case 1: color = 'З';
			break;
		case 2: color = 'С';
			break;
		}
	};

	ColorObject(char Color) { color = Color; };

	inline char getColor() const
	{
		return color;
	}
};
 
std::list<ColorObject> colorobjects; // Список, хранящий объекты класса ColorObject

std::mutex mutex;					 // Мьютекс для синхронизации потоков
std::atomic<bool> controller = 0;		 // Атомарная переменная для приостановки работы потоков - 
									 //если равна 0, то главный поток разрешил работу, иначе - нет

std::atomic<bool> flag = false;		 // Атомарная переменная для доп.синхронизации потоков - исключение ситуаций, когда один поток 
									 // захватывает мьютекс дважды подряд (генерирует или сортирует 2 раза подряд).

std::atomic<bool> live = false;		 // Атомарная переменная для полной остановки работы потоков - пока равна true, потоки продолжат
									 // свою работу, как только становится равной false - работа потоков останавливается на совсем.


/*
	Функция непосредственной сортировки. Обрабатывает список цветных объектов colorobjects, принимает 3 цвета - x, y и z,
	порядок которых определен заранее в функции-парсере строки с правилом.
*/
void my_sort(std::list<ColorObject>& colorobjects, char x, char y, char z)

{	// находим последнее вхождение цвета y; так как нельзя использовать поиск по контейнеру std::find - использую свой вариант;
	// если объекта с цветом y не существует вообще, то возвращаем итератор end().
	auto iterOnMiddleColor = [&colorobjects](char y)
	{
		std::list<ColorObject>::iterator var_it = colorobjects.end();
		for (auto it = colorobjects.begin(); it != colorobjects.end(); it++)
		{
			if (it->getColor() == y) var_it = it;
		}
		return var_it;
	};

	auto middle = iterOnMiddleColor(y);

	// В случае, если объект с цветом y не был найден, то производим сортировку: x < z.
	// Если не найден цвет x или цвет z, то сортировка сработает в штатном режиме.
	if (middle == colorobjects.end())
	{
		for (auto i = colorobjects.begin(); i != middle;)
		{
			if (i->getColor() == x && i != colorobjects.begin())
			{
				colorobjects.push_front(*i);
				i = colorobjects.erase(i);

			}
			else i++;
		}
	}
	// После нахождения последнего вхождения цвета y, перемещаю найденные объекты с цветом z ("старшим" цветом) в правую сторону
	// от y, а объекты с цветом x - в левую сторону.
	// Для std::list операции вставки и удаления одного элемента выполняются за константное время; следовательно, вычислительная сложность
	// сводится приблизительно к O(n), где n - количество объектов "старшего" и "младшего" цвета в исходном списке.
	else {
		
		for (auto i = colorobjects.begin(); i != middle;)
		{
			if (i->getColor() == z)
			{
				colorobjects.push_back(*i);
				i = colorobjects.erase(i);

			}
			// Так как мы используем последнее вхождение цвета y в список, нужно исключить ситуации, когда цвет x чередуется с цветом y.
			// Для этого мы записываем в начало все встреченные объекты с цветом x, кроме самого первого.
			else if (i->getColor() == x && i != colorobjects.begin())
			{
				colorobjects.push_front(*i);
				i = colorobjects.erase(i);

			}
			else i++;
		}
		for (auto i = middle; i != colorobjects.end();)
		{
			if (i->getColor() == x)
			{
				colorobjects.push_front(*i);
				i = colorobjects.erase(i);
			}
			// для объектов, находящихся справа от последнего объекта с цветом y, гарантированно не будет чередования цветов с y,
			// поэтому правая часть рассматривается только с целью перенесения объектов цвета x в левую часть.
			else i++;
		}
	}
}

/*
	Проверка правила сортировки на корректность.
	Если размер строки с правилом не равен 9, то возвращается 1.
	Если символ, стоящий на позиции знака, не равен ">" или "<", то возвращается 2.
	Если символы знаков не равны друг другу (неопределенность), то возвращается 3.
	Если символы цветов равны друг другу, возвращается 4.
	Если один из символов цветов не равен З, С или К, возвращается 5.
	Если правило корректно, возвращается 0.
*/
int Checkrule(std::string rule)
{
	if (rule.size() != 9) return 1;

	else if ((rule[2] != '<' && rule[2] != '>') || (rule[6] != '<' && rule[6] != '>')) return 2;

	else if (rule[2] != rule[6]) return 3;

	else if (rule[0] == rule[4] || rule[0] == rule[8] || rule[4] == rule[8]) return 4;

	else if (rule[0] != 'К' && rule[0] != 'З' && rule[0] != 'С'
		|| rule[4] != 'К' && rule[4] != 'З' && rule[4] != 'С'
		|| rule[8] != 'К' && rule[8] != 'З' && rule[8] != 'С')
		return 5;

	else return 0;
}

/*
	Generator генерирует 25 объектов класса ColorObject(), конструктор по умолчанию которого предоставляет
	случайный выбор одного из трех цветов объекта.
	Во избежание перегрузки метода Generator,
	Если строка colors содержит набор цветов, например "КЗСЗСК", то при генерации объектов будет использоваться
	конструктор ColorObject(char), параметром которого является соответствующий цвет из строки colors (если цвет - К, С или З).
	Если строка colors пустая, то при генерации объектов будет использован конструктор по умолчанию.

	Generator будет продолжать свою работу, пока параметр live будет равным true.
	Для функции тестирования параметр live выставляется равным false, что будет означать однократное выполнение цикла генерации.

	Параметр flag используется для дополнительной синхронизации потоков - если один из них выполняется быстрее, это исключает ситуацию,
	когда мьютекс захвачен дважды подряд одним и тем же потоком. Следовательно, исключаются ситуации многократной генерации объектов
	подряд, а также сортировки пустого контейнера.

*/

void Generator(std::list<ColorObject>& str, std::string colors)
{
	do
	{
		if (controller == false)
		{
			if (!flag)
			{
				mutex.lock();
				std::cout << "Generator is working" << std::endl;
				str.clear();
				if (colors.empty())
					for (int i = 0; i < 25; i++)
						str.push_back(ColorObject());
				else
				{
					for (auto i : colors)
					{
						if (i == 'К' || i == 'С' || i == 'З')
							str.push_back(ColorObject(i));
					}
				}

				for (auto i : str)
					std::cout << i.getColor() << " ";
				std::cout << std::endl;
				flag = true;
				mutex.unlock();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	} while (live);
}

/*
Функция определения знака в правиле rule и передачи аргументов сортировки в правильном порядке.
Правила, содержащие знак "больше" (A > B > C) можно представить как правила со знаком меньше,
инвертировав крайние цвета (C < B < A).

*/
void CompareObjects(std::list<ColorObject>& str, std::string rule)
{
	do
	{
		if (controller == false)
		{
			if (flag)
			{
				mutex.lock();
				std::cout << "Sorting is working" << std::endl;
				if (rule[2] == '<')
				{
					my_sort(str, rule[0], rule[4], rule[8]);
				}
				else if (rule[2] == '>')
				{
					my_sort(str, rule[8], rule[4], rule[0]);
				}
				for (auto i : str)
					std::cout << i.getColor() << " ";
				std::cout << std::endl;
				flag = false;
				mutex.unlock();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	} while (live);
}
/*
ControllingThreads() - функция для управления потоками генерации и сортировки из главного потока.
В главном потоке получаем символ из клавиатуры.
Если этот символ - знак пробела, то переключаем значение контроллера на противоположное.
Если нажата клавиша E, то прекращаем работу второстепенных потоков и завершаем работу программы.
*/
void ControllingThreads()
{
	char off;
	while (true)
	{
		off = _getch();
		if (off == ' ' && controller == false)
		{
			controller = true;
		}
		else if (off == ' ' && controller == true)
		{
			controller = false;
		}
		else if (off == 'E' || off == 'У' || off == 'у' || off == 'e')
		{
			live = 0;
			break;
		}
	}
}
