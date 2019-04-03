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
	����� "������� �������". ����������� �� ��������� ������� ������ ���������� ���� ������� ��������� �������;
	����������� � ���������� char ������� ������ � ��� ������, ��� ��� ������� ��� �������� char.
*/
class ColorObject {

	char color;
public:
	ColorObject() {
		int k = rand() % 3;
		switch (k)
		{
		case 0: color = '�';
			break;
		case 1: color = '�';
			break;
		case 2: color = '�';
			break;
		}
	};

	ColorObject(char Color) { color = Color; };

	inline char getColor() const
	{
		return color;
	}
};
 
std::list<ColorObject> colorobjects; // ������, �������� ������� ������ ColorObject

std::mutex mutex;					 // ������� ��� ������������� �������
std::atomic<bool> controller = 0;		 // ��������� ���������� ��� ������������ ������ ������� - 
									 //���� ����� 0, �� ������� ����� �������� ������, ����� - ���

std::atomic<bool> flag = false;		 // ��������� ���������� ��� ���.������������� ������� - ���������� ��������, ����� ���� ����� 
									 // ����������� ������� ������ ������ (���������� ��� ��������� 2 ���� ������).

std::atomic<bool> live = false;		 // ��������� ���������� ��� ������ ��������� ������ ������� - ���� ����� true, ������ ���������
									 // ���� ������, ��� ������ ���������� ������ false - ������ ������� ��������������� �� ������.


/*
	������� ���������������� ����������. ������������ ������ ������� �������� colorobjects, ��������� 3 ����� - x, y � z,
	������� ������� ��������� ������� � �������-������� ������ � ��������.
*/
void my_sort(std::list<ColorObject>& colorobjects, char x, char y, char z)

{	// ������� ��������� ��������� ����� y; ��� ��� ������ ������������ ����� �� ���������� std::find - ��������� ���� �������;
	// ���� ������� � ������ y �� ���������� ������, �� ���������� �������� end().
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

	// � ������, ���� ������ � ������ y �� ��� ������, �� ���������� ����������: x < z.
	// ���� �� ������ ���� x ��� ���� z, �� ���������� ��������� � ������� ������.
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
	// ����� ���������� ���������� ��������� ����� y, ��������� ��������� ������� � ������ z ("�������" ������) � ������ �������
	// �� y, � ������� � ������ x - � ����� �������.
	// ��� std::list �������� ������� � �������� ������ �������� ����������� �� ����������� �����; �������������, �������������� ���������
	// �������� �������������� � O(n), ��� n - ���������� �������� "��������" � "��������" ����� � �������� ������.
	else {
		
		for (auto i = colorobjects.begin(); i != middle;)
		{
			if (i->getColor() == z)
			{
				colorobjects.push_back(*i);
				i = colorobjects.erase(i);

			}
			// ��� ��� �� ���������� ��������� ��������� ����� y � ������, ����� ��������� ��������, ����� ���� x ���������� � ������ y.
			// ��� ����� �� ���������� � ������ ��� ����������� ������� � ������ x, ����� ������ �������.
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
			// ��� ��������, ����������� ������ �� ���������� ������� � ������ y, �������������� �� ����� ����������� ������ � y,
			// ������� ������ ����� ��������������� ������ � ����� ����������� �������� ����� x � ����� �����.
			else i++;
		}
	}
}

/*
	�������� ������� ���������� �� ������������.
	���� ������ ������ � �������� �� ����� 9, �� ������������ 1.
	���� ������, ������� �� ������� �����, �� ����� ">" ��� "<", �� ������������ 2.
	���� ������� ������ �� ����� ���� ����� (����������������), �� ������������ 3.
	���� ������� ������ ����� ���� �����, ������������ 4.
	���� ���� �� �������� ������ �� ����� �, � ��� �, ������������ 5.
	���� ������� ���������, ������������ 0.
*/
int Checkrule(std::string rule)
{
	if (rule.size() != 9) return 1;

	else if ((rule[2] != '<' && rule[2] != '>') || (rule[6] != '<' && rule[6] != '>')) return 2;

	else if (rule[2] != rule[6]) return 3;

	else if (rule[0] == rule[4] || rule[0] == rule[8] || rule[4] == rule[8]) return 4;

	else if (rule[0] != '�' && rule[0] != '�' && rule[0] != '�'
		|| rule[4] != '�' && rule[4] != '�' && rule[4] != '�'
		|| rule[8] != '�' && rule[8] != '�' && rule[8] != '�')
		return 5;

	else return 0;
}

/*
	Generator ���������� 25 �������� ������ ColorObject(), ����������� �� ��������� �������� �������������
	��������� ����� ������ �� ���� ������ �������.
	�� ��������� ���������� ������ Generator,
	���� ������ colors �������� ����� ������, �������� "������", �� ��� ��������� �������� ����� ��������������
	����������� ColorObject(char), ���������� �������� �������� ��������������� ���� �� ������ colors (���� ���� - �, � ��� �).
	���� ������ colors ������, �� ��� ��������� �������� ����� ����������� ����������� �� ���������.

	Generator ����� ���������� ���� ������, ���� �������� live ����� ������ true.
	��� ������� ������������ �������� live ������������ ������ false, ��� ����� �������� ����������� ���������� ����� ���������.

	�������� flag ������������ ��� �������������� ������������� ������� - ���� ���� �� ��� ����������� �������, ��� ��������� ��������,
	����� ������� �������� ������ ������ ����� � ��� �� �������. �������������, ����������� �������� ������������ ��������� ��������
	������, � ����� ���������� ������� ����������.

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
						if (i == '�' || i == '�' || i == '�')
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
������� ����������� ����� � ������� rule � �������� ���������� ���������� � ���������� �������.
�������, ���������� ���� "������" (A > B > C) ����� ����������� ��� ������� �� ������ ������,
������������ ������� ����� (C < B < A).

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
ControllingThreads() - ������� ��� ���������� �������� ��������� � ���������� �� �������� ������.
� ������� ������ �������� ������ �� ����������.
���� ���� ������ - ���� �������, �� ����������� �������� ����������� �� ���������������.
���� ������ ������� E, �� ���������� ������ �������������� ������� � ��������� ������ ���������.
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
		else if (off == 'E' || off == '�' || off == '�' || off == 'e')
		{
			live = 0;
			break;
		}
	}
}
