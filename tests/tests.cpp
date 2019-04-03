#include "../task.h"

#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test)

BOOST_AUTO_TEST_CASE(checkrule)
{
	BOOST_CHECK(Checkrule("С < З < К") == 0); // штатное выполнение
	BOOST_CHECK(Checkrule("З < С < К") == 0);
	BOOST_CHECK(Checkrule("С > З > К") == 0);

	BOOST_CHECK(Checkrule("С < З <К") == 1);  // неверная длина выражения 
	BOOST_CHECK(Checkrule("С< З < К") == 1);

	BOOST_CHECK(Checkrule("С = З = К") == 2); // неверный знак в выражении
	BOOST_CHECK(Checkrule("С ! З ~ К") == 2);

	BOOST_CHECK(Checkrule("С > З < К") == 3); // неопределенность отношения С и К
	BOOST_CHECK(Checkrule("С < З > К") == 3);

	BOOST_CHECK(Checkrule("С < С < З") == 4); // одинаковые цвета
	BOOST_CHECK(Checkrule("К < С < К") == 4);

	BOOST_CHECK(Checkrule("A < B < C") == 5); // неизвестные обозначения цветов
	BOOST_CHECK(Checkrule("К < М < С") == 5);
}

BOOST_AUTO_TEST_CASE(checkgenerator)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string colors = "КСЗЗСК";
	std::list<ColorObject> container;
	Generator(container, colors); // создаем объекты с цветами в строке

	// число сгенерированных элементов должно совпадать с числом элементов в переданной строке
	BOOST_CHECK(container.size() == colors.size()); 
	
	int index = 0;
	for (auto i : container)
	{
		// проверка на правильность созданных элементов
		BOOST_CHECK(i.getColor() == colors[index]);
		index++;
	}

	index = 0;
	flag = false; // Generator выставляет флаг окончания своей работы в true для CompareObjects;
				  // чтобы генерировать несколько раз подряд, вручную отключаем flag после каждого
				  // вызова Generator

	colors = "АБВГД"; // строка с неправильными цветами
	Generator(container, colors);
	std::cout << container.size();
	BOOST_CHECK(container.size() == 0); // не должно создаться ни одного объекта

	flag = false;

	colors = "ССКЗСК";
	Generator(container, colors);
	for (auto i : container)
	{
		// проверка на правильность созданных элементов
		BOOST_CHECK(i.getColor() == colors[index]);
		index++;
	}

	
	
}

BOOST_AUTO_TEST_CASE(checksort)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	std::string rule = "С < З < К";	   // правило сортировки
	std::string colors = "ЗСККСЗЗСКК"; // исходная последовательность
	std::string result = "СССЗЗЗКККК"; // результат корректной сортировки
	std::list<ColorObject> container;
	int index = 0;
	flag = false;
	Generator(container, colors);	// создаем объекты исходной последовательности
	CompareObjects(container, rule);// сортируем контейнер по правилу

	// Проверка на соответствие содержимого контейнера предполагаемому результату
	for (auto i : container)
	{
		// проверка на корректность сортировки
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}
	index = 0;
	colors = "СКЗСККЗЗКЗСКЗС";
	result = "ССССЗЗЗЗЗККККК";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}

	index = 0;

	// Проверка алгоритма сортировки, если цвета в последовательности не 3, а 2
	colors = "ККККСССС";
	result = "ССССКККК";

	Generator(container, colors);
	CompareObjects(container, rule);

	
	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}

	index = 0;

	// Проверка с новым правилом сортировки 
	rule = "К > С > З";
	colors = "ЗСКККЗСЗКЗСЗ";
	result = "ЗЗЗЗЗСССКККК";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}
	index = 0;

	// Тестовый пример из задания
	rule = "З < С < К";
	colors = "ССЗСКЗЗЗККСЗССКЗ";
	result = "ЗЗЗЗЗЗССССССКККК";

	Generator(container, colors);
	CompareObjects(container, rule);


	for (auto i : container)
	{
		BOOST_CHECK(i.getColor() == result[index]);
		index++;
	}


}

BOOST_AUTO_TEST_SUITE_END()

