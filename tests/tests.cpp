#include "../task.h"

#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test)

BOOST_AUTO_TEST_CASE(checkrule)
{
	BOOST_CHECK(Checkrule("� < � < �") == 0); // ������� ����������
	BOOST_CHECK(Checkrule("� < � < �") == 0);
	BOOST_CHECK(Checkrule("� > � > �") == 0);

	BOOST_CHECK(Checkrule("� < � <�") == 1);  // �������� ����� ��������� 
	BOOST_CHECK(Checkrule("�< � < �") == 1);

	BOOST_CHECK(Checkrule("� = � = �") == 2); // �������� ���� � ���������
	BOOST_CHECK(Checkrule("� ! � ~ �") == 2);

	BOOST_CHECK(Checkrule("� > � < �") == 3); // ���������������� ��������� � � �
	BOOST_CHECK(Checkrule("� < � > �") == 3);

	BOOST_CHECK(Checkrule("� < � < �") == 4); // ���������� �����
	BOOST_CHECK(Checkrule("� < � < �") == 4);

	BOOST_CHECK(Checkrule("A < B < C") == 5); // ����������� ����������� ������
	BOOST_CHECK(Checkrule("� < � < �") == 5);
}

BOOST_AUTO_TEST_CASE(checkgenerator)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string colors = "������";
	std::list<ColorObject> container;
	Generator(container, colors); // ������� ������� � ������� � ������

	// ����� ��������������� ��������� ������ ��������� � ������ ��������� � ���������� ������
	BOOST_CHECK(container.size() == colors.size()); 
	
	int index = 0;
	for (auto i : container)
	{
		// �������� �� ������������ ��������� ���������
		BOOST_CHECK(i.getColor() == colors[index]);
		index++;
	}

	index = 0;
	flag = false; // Generator ���������� ���� ��������� ����� ������ � true ��� CompareObjects;
				  // ����� ������������ ��������� ��� ������, ������� ��������� flag ����� �������
				  // ������ Generator

	colors = "�����"; // ������ � ������������� �������
	Generator(container, colors);
	std::cout << container.size();
	BOOST_CHECK(container.size() == 0); // �� ������ ��������� �� ������ �������

	flag = false;

	colors = "������";
	Generator(container, colors);
	for (auto i : container)
	{
		// �������� �� ������������ ��������� ���������
		BOOST_CHECK(i.getColor() == colors[index]);
		index++;
	}

	
	
}

BOOST_AUTO_TEST_CASE(checksort)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	std::string rule = "� < � < �";	   // ������� ����������
	std::string colors = "����������"; // �������� ������������������
	std::string result = "����������"; // ��������� ���������� ����������
	std::list<ColorObject> container;
	int index = 0;
	flag = false;
	Generator(container, colors);	// ������� ������� �������� ������������������
	CompareObjects(container, rule);// ��������� ��������� �� �������

	// �������� �� ������������ ����������� ���������� ��������������� ����������
	for (auto i : container)
	{
		// �������� �� ������������ ����������
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}
	index = 0;
	colors = "��������������";
	result = "��������������";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}

	index = 0;

	// �������� ��������� ����������, ���� ����� � ������������������ �� 3, � 2
	colors = "��������";
	result = "��������";

	Generator(container, colors);
	CompareObjects(container, rule);

	
	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}

	index = 0;

	// �������� � ����� �������� ���������� 
	rule = "� > � > �";
	colors = "������������";
	result = "������������";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}
	index = 0;

	// �������� ������ �� �������
	rule = "� < � < �";
	colors = "����������������";
	result = "����������������";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}


}

BOOST_AUTO_TEST_SUITE_END()

