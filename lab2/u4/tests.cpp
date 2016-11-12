#include "testslib.hpp"
#include "texteditorcore.hpp"

#include <cassert>
#include <sstream>


DECLARE_OOP_TEST( empty_constructor) {
	

	TEC test;

	assert(test.getCursorColumn() == 0ul);
	assert(test.getCursorRow() == 0ul);

};

DECLARE_OOP_TEST(stream_constructor) {



	setlocale(LC_ALL, "Russian");

	std::ifstream infile1("testIO.txt"), infile2("loremIO.txt");

	infile1.close();
	infile2.close();

	TEC test1(infile1);
	TEC test2(infile2);


	assert(test1.getCursorColumn() == 0ul);
	assert(test1.getCursorRow() == 0ul);


	assert(test2.getCursorColumn() == 0ul);
	assert(test2.getCursorRow() == 0ul);

};


DECLARE_OOP_TEST( cursor ) {
	std::ifstream  infile2("loremIO.txt");

	TEC test2(infile2);

	infile2.close();


	assert(test2.getCursorColumn() == 0ul);
	assert(test2.getCursorRow() == 0ul);

	test2.cursorGoOneDown();
	

	assert(test2.getCursorColumn() == 0ul);
	assert(test2.getCursorRow() == 1ul);

	try {
		test2.cursorGoTo(0, 200);
	} catch (const std::logic_error err) {
		assert(!strcmp(err.what(), Messages::InvalidPosition));
	};

	assert(test2.getCursorColumn() == 0ul);
	assert(test2.getCursorRow() == 1ul);

	
	test2.cursorGoTo(0, 5);


	assert(test2.getCursorColumn() == 5ul);
	assert(test2.getCursorRow() == 0ul);

	

	try {
	
		test2.cursorGoTo(10000, 5);
			
	} catch (const std::logic_error err){
		assert( !strcmp(err.what(), Messages::InvalidPosition) );
	};


	test2.cursorGoTo(14, 4);
	assert(test2.getCursorRow() == 14 && test2.getCursorColumn() == 4);


	test2.cursorGoTo(0, 0);

	test2.cursorGoOneLeft();
	test2.cursorGoOneUp();

		
	for (UL i = 0; i < test2.getTotal()  ; i++) {
		test2.cursorGoOneRight();
	};
	assert(test2.getCursorRow() == 18 && test2.getCursorColumn() == 30);

	test2.cursorGoTo(18, 30);
	
	test2.cursorGoTo(0, 0);
	for (UL i = 0; i < test2.getMaxRows()  ; i++) {
		test2.cursorGoOneDown();
	};
	assert(test2.getCursorRow() == 18 && test2.getCursorColumn() == 0);


	
	for (int i = 0; i < 33; i++) {
		test2.cursorGoOneRight();
	};	
	assert(test2.getCursorRow() == 18 && test2.getCursorColumn() == 30);

};

DECLARE_OOP_TEST(insert) {

	std::ifstream  infile2("loremIO.txt");

	TEC test2(infile2);
	infile2.close();
	
	std::string str1{ "\nABCDE\nFGHIJ\nKLMNO\nPQRST\nUVWXY\nZ\n" };


/*

	for (int i = 0; i < test2.getMaxRows(); i++) {
		test2.cursorGoOneDown();
	};

	for (int i = 0; i < 100; i++) {
		test2.cursorGoOneRight();
	};
*/	test2
		.insert('\n')
		.cursorGoTo(8,5)
		.insert(str1)
		.cursorGoTo(26, 30)
		.insert("\n")
		.cursorGoOneDown()
		.insert('\n')
		.cursorGoOneDown()
		.insert("\n")
		.cursorGoOneUp()
		.cursorGoOneUp()
		.cursorGoOneUp()
		.cursorGoOneUp()
		.cursorGoOneUp()
		.cursorGoOneUp()
		.insert(str1)
		.insert("KU")
		.insert("\nKA")
		.insert("\nREKU\n")
		;
	assert(test2.getCursorRow() == 33 && test2.getCursorColumn() == 0);


};


DECLARE_OOP_TEST(_delete) {
	std::ifstream  infile2("loremIO.txt");
	TEC test2(infile2);
	infile2.close();


	assert(!strcmp( test2.cursorGoTo(13,0).insert("SOMETHING").cursorGoTo(13, 0).selectBegin().cursorGoTo(13,9).selectEnd().deleteSelectedSegment().selectSelected().c_str(), ""  ));
	assert(!strcmp(test2.selectBegin().CtrlENDkey().selectEnd().deleteSelectedSegment().selectSelected().c_str() ,  "" ) );
};


DECLARE_OOP_TEST(buttons) {
	std::ifstream  infile2("loremIO.txt");
	TEC test2(infile2);
	infile2.close();

	test2.ENDkey();
	assert(test2.getCursorRow() == 0 && test2.getCursorColumn() == 30);
	test2.CtrlENDkey();
	assert(test2.getCursorRow() == 18 && test2.getCursorColumn() == 30);
	test2.HOMEkey();
	assert(test2.getCursorRow() == 18 && test2.getCursorColumn() == 0);
	test2.CtrlHOMEkey();
	assert(test2.getCursorRow() == 0 && test2.getCursorColumn() == 0);

};


DECLARE_OOP_TEST(saveTo) {
	std::ifstream  infile("loremIO.txt");
	TEC test2(infile);
	infile.close();
	

	std::ofstream ofile3("newIO.txt");
	if (!ofile3) {
		std::cerr << "cannot open file\n";
	};
	test2.saveTo( ofile3 );
	ofile3.close();

	std::ifstream ifile3("loremIO.txt");
	std::ifstream ifile4("newIO.txt");


	std::stringstream first,second;

	first << ifile3.rdbuf();
	second << ifile4.rdbuf();



	assert( strcmp(first.str().c_str(), second.str().c_str()) );
	
	ifile3.close();
	ifile4.close();


	std::remove("newIO.txt");	
};

DECLARE_OOP_TEST(select) {
	std::string test_str{ "abcde\nfghijklmno\npqrstuvwxy\nz" };

	std::ifstream  infile2("loremIO.txt");
	TEC test2(infile2);
	infile2.close();

	assert( strcmp(
		test2
		.insert(test_str)
		.cursorGoTo(0, 0)
		.selectBegin()
		.cursorGoTo(2, 10)
		.selectEnd()
		.selectSelected().c_str(), test_str.c_str() ) );

	assert(strcmp(
		test2
		.selectBegin()
		.cursorGoTo(0, 0)
		.selectEnd()
		.selectSelected().c_str(), test_str.c_str()));


};



DECLARE_OOP_TEST(find) {
	

	std::ifstream  infile2("loremIO.txt");
	TEC test2(infile2);
	infile2.close();

	
	test2
		.cursorGoTo(5, 5)
		.insert("THIS IS UNIQUE TEXT")
		.cursorGoTo(10, 20)
		.insert("THIS IS NOT UNIQUE TEXT")
		.cursorGoTo(14, 0)
		.insert("THIS IS NOT UNIQUE TEXT")
		//.outputAll()
		;
	
	auto temp = test2.find("THIS IS UNIQUE TEXT");
	assert( temp.row == 5 && temp.column == 5);

	temp = test2.find("THIS IS NOT UNIQUE TEXT");
	assert(temp.row == 10 && temp.column == 20);

	temp = test2.findNext();

	assert(temp.row == 14 && temp.column == 0);

	try {
		temp = test2.findNext();
	}
	catch (const std::logic_error err) {
		assert( ! strcmp(err.what(),Messages::NotFound) );
	};



};

DECLARE_OOP_TEST(replace) {
	std::ifstream  infile2("loremIO.txt");
	TEC test2(infile2);
	infile2.close();

	
	test2.insert("Hello World").replace(std::string{ "Hello World" }, std::string{ "~" });
	try {
		test2.find("Hello World");
	}
	catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::NotFound));
	};


	auto temp = test2.find("~");
	assert(temp.row == 0 && temp.column == 0);

	
	test2.replaceAll(std::string{ "0123456789" }, std::string{ "*" });

	try {
		test2.find("0123456789");
	} catch ( std::logic_error err ) {
		assert(!strcmp(err.what(), Messages::NotFound));
	};
	
};


DECLARE_OOP_TEST(mem_tst_usng_brpnts) {
	TEC* ptest;

	std::ifstream  infile2("loremIO.txt");
	ptest = new TEC[1000];



	infile2.close();


	delete[] ptest;
};

