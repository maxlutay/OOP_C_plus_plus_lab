#include "testslib.hpp"
#include "texteditorcore.hpp"

#include <cassert>
#include <sstream>



//note: if: c_strings are equal strcmp => ret 0
//		if: strings are equal std::basic_string::compare => ret 0

DECLARE_OOP_TEST( empty_constructor) {
	TextEditorCore t1{};
	TextEditorCore::position t1_p = t1.getCursorPosition();

	assert( t1_p == TextEditorCore::position(0,0) );

	t1
		.cursorGoOneDown()
		.cursorGoOneRight();
	
	assert(t1_p == TextEditorCore::position(0, 0));

	t1
		.cursorGoOneUp()
		.cursorGoOneLeft();
	assert(t1_p == TextEditorCore::position(0, 0));

	try {
		t1.cursorGoTo(TextEditorCore::position{ 100, 100 });
	} catch (std::logic_error err) {
		assert( ! strcmp(  err.what(), Messages::InvalidPosition  ) );
	};


	
	assert(! t1.selectBegin()		
				.cursorGoOneDown()
				.cursorGoOneDown()
				.selectEnd()
				.selectSelected()
				.compare("") );
		
	try {
		t1.find("Hello");
	} catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::NotFound));
	};

	try {
		t1.replaceAll("","Hello");
	}catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::EmptyReplacingNotAllowed));
	};



};


DECLARE_OOP_TEST(istream_constructor) {

//	setlocale(LC_ALL, "Russian");


	std::ifstream infile("loremIO.txt");
	TextEditorCore t1{ infile };

	infile.close();

	TextEditorCore::position t1_p = t1.getCursorPosition();

	
	assert(t1_p == TextEditorCore::position(0, 0));//initial position is  0,0
	

	std::ifstream infile2("empty.txt");
	try {
		TextEditorCore t2{ infile2 };
	} catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::EmptyEntryFile));
	};
	infile2.close();


};


DECLARE_OOP_TEST( getCursorPosition_and_cursormoving) {
	TextEditorCore t1{};
	TextEditorCore::position t1_p = t1.getCursorPosition();
	assert(t1_p == TextEditorCore::position(0, 0));


	std::ifstream infile("loremIO.txt");
	
	TextEditorCore t2{ infile };
	
	infile.close();
	
	TextEditorCore::position t2_p = t2.getCursorPosition();
	assert(t2_p == TextEditorCore::position(0, 0));


	for (int i = 0; i <= 50; i += 1) {
		for (int j = 0; j <= 50; j += 1) {
			try {
				t2.cursorGoTo(TextEditorCore::position((long)i, (long)j));

				assert(t2.getCursorPosition() == TextEditorCore::position((long)i, (long)j));

			}
			catch (std::logic_error err) {
				assert(!strcmp(err.what(), Messages::InvalidPosition));
			};
		};
	};

	assert(t2.getCursorPosition() == TextEditorCore::position(18, 30));




	for (int i = 0; i <= 50; i += 1) {
		for (int j = 0; j <= 50; j += 1) {
				t2.cursorGoOneLeft();
		};
	};

	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));





	for (int i = 0; i <= 50; i += 1) {
		for (int j = 0; j <= 50; j += 1) {
			t2.cursorGoOneRight();
		};
	};
	
	assert(t2.getCursorPosition() == TextEditorCore::position( 18, 30));


	t2.cursorGoTo(TextEditorCore::position(0, 0));

	for (int i = 0; i <= 50; i += 1) {
			t2.cursorGoOneDown();
	};
	
	assert(t2.getCursorPosition() == TextEditorCore::position(18, 0));



	for (int i = 0; i <= 50; i += 1) {
		t2.cursorGoOneUp();
	};

	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));


	t2.cursorGoTo( TextEditorCore::position{ 1,30 } );
	t2.cursorGoOneDown();

	assert(t2.getCursorPosition() == TextEditorCore::position(2, 3));



	t2.cursorGoTo(TextEditorCore::position{ 5,30 });
	t2.cursorGoOneUp();

	assert(t2.getCursorPosition() == TextEditorCore::position(4, 2));


	t2.cursorGoTo(TextEditorCore::position{ 0,30 });
	t2.cursorGoOneRight();

	assert(t2.getCursorPosition() == TextEditorCore::position(1, 0));

	t2.cursorGoOneLeft();
	assert(t2.getCursorPosition() == TextEditorCore::position(0, 30));




};



DECLARE_OOP_TEST(insertion) {
	TextEditorCore t1{};

	TextEditorCore::position t1_p = t1.getCursorPosition();

	t1.insert("0123456789")	;

	try {
		
		t1.cursorGoTo(TextEditorCore::position{ 0, 30 });
	
	} catch ( std::logic_error err ) {
		assert(!strcmp(err.what(), Messages::InvalidPosition));
	};


	t1.insert('\n');


	t1.cursorGoTo(TextEditorCore::position{ 1, 0 });



	t1.insert("\n0123456789\n0123456789\n");



	t1.cursorGoTo(TextEditorCore::position{ 4, 0 });

	assert( t1.getCursorPosition() == TextEditorCore::position( 4, 0 ) );



};


DECLARE_OOP_TEST( keys ) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	t1.ENDkey();
	assert(t1.getCursorPosition()  == TextEditorCore::position(0, 30));
	
	t1.CtrlENDkey();
	assert(t1.getCursorPosition() == TextEditorCore::position(18, 30));
	
	t1.HOMEkey();
	assert(t1.getCursorPosition() == TextEditorCore::position(18, 0));

	t1.CtrlHOMEkey();
	assert(t1.getCursorPosition() == TextEditorCore::position(0, 0));
	

	TextEditorCore t2{};

	t2.ENDkey();
	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));

	t2.CtrlENDkey();
	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));

	t2.HOMEkey();
	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));

	t2.CtrlHOMEkey();
	assert(t2.getCursorPosition() == TextEditorCore::position(0, 0));

	
};


DECLARE_OOP_TEST(saveTo) {

std::ifstream  infile("loremIO.txt");

TextEditorCore t(infile);

infile.close();


std::ofstream ofile("newIO.txt");

if (!ofile) {
	std::cerr << "cannot open file\n";
};

t.saveTo( ofile );

ofile.close();


// check files are equal

std::ifstream ifile1("loremIO.txt");
std::ifstream ifile2("newIO.txt");


std::stringstream first,second;

first << ifile1.rdbuf();
second << ifile2.rdbuf();



assert( !first.str().compare( second.str()) );

ifile1.close();
ifile2.close();


std::remove("newIO.txt");

};


DECLARE_OOP_TEST(selection) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	std::string got = t1
						.selectBegin()
						.cursorGoTo(TextEditorCore::position(0, 30))
						.selectEnd()
						.selectSelected();

	
	assert(got ==  "012345678901234567890123456789" );

	
	got = t1
			.selectBegin()
			.cursorGoTo(TextEditorCore::position(0, 30))
			.selectReset()
			.selectEnd()
			.selectSelected();

	assert(got == "");



	got = t1.CtrlHOMEkey()
			.cursorGoOneRight()
			.selectBegin()
			.cursorGoOneDown()
			.cursorGoOneDown()
			.cursorGoOneRight()
			.cursorGoOneDown()
			.selectEnd()
			//.outputSelectPoss()
			.selectSelected();
	assert( !got.compare("12345678901234567890123456789\n012345678901234567890123456789\n012\n01"));


	got = t1.CtrlENDkey()
			.selectBegin()
			.cursorGoOneUp()
			.cursorGoOneUp()
			.cursorGoOneUp()
			.cursorGoOneUp()
			.selectEnd()
			//.outputSelectPoss()
			.selectSelected();

	assert(!got.compare("01234\n\n\n\n012345678901234567890123456789"));


};



DECLARE_OOP_TEST(delete_selected) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	t1.selectBegin()
		.cursorGoTo(TextEditorCore::position(1, 30))
		.selectEnd()
		.deleteSelectedSegment();


	std::string got = t1.selectBegin()
					.cursorGoOneDown()
					.cursorGoOneDown()
					.selectSelected();


	assert( got.compare( "012\n01234\n" ) );



};








DECLARE_OOP_TEST(find) {
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();

	TextEditorCore::position f = t1.find("012345");


	assert(f == TextEditorCore::position{});



	f = t1.findNext();
	assert( f == TextEditorCore::position( 0,10 ) );


	f = t1.findNext();

	assert(f == TextEditorCore::position(0, 20));

	f = t1.findNext();
	assert(f == TextEditorCore::position(1, 0));


	try {
		f = t1.find("");
	} catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::EmptyFindNotAllowed));
	};

	try {
		f = t1.findNext();
	} catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::EmptyFindNotAllowed));
	};
	
	try {
		f = t1.find("Hello");
	} catch (std::logic_error err) {
		assert(!strcmp(err.what(), Messages::NotFound));
	};




};



DECLARE_OOP_TEST(replace) {//replacing non multiline 
	std::ifstream  infile("loremIO.txt");
	TextEditorCore t1(infile);
	infile.close();


	t1.replaceAll("0123456789", "*");
	t1.replaceAll("01234", "");
	t1.replaceAll("012", "");
	t1.replaceAll("01", "");
	t1.replaceAll("*", "");

	assert(t1.replace("0","") == false);
	
	//t1.outputAll();
	
	assert(! t1.CtrlHOMEkey().selectBegin().CtrlENDkey().selectSelected().compare( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" ));

	


};


/*
DECLARE_OOP_TEST(mem_tst_usng_brpnts) {
	TextEditorCore* ptest;

	std::ifstream  infile2("loremIO.txt");
	ptest = new TextEditorCore[1000];



	infile2.close();


	delete[] ptest;
};

*/