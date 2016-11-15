#include "texteditorcore.hpp"

#include <sstream>


bool TextEditorCore::validatePosition( const TextEditorCore::position _pos) const {
	return
			_pos.row >= 0 &&
			_pos.col >= 0 &&
			_pos.row < (this->container.size() ) &&
			_pos.col <= (this->container.at(_pos.row).length()  );
};


void TextEditorCore::multilineInsert(position _p, const std::string& _text) {	
	if (_text == "") { return; };//do nothing


	std::string line, tail;
	std::stringstream ss{ _text };

	bool is_untill_new_line_appended = false;
	long n_symbols_added = 0, count = 0;//count counts rows for further tail insertion
	


	tail = std::string{ container.at(_p.row).substr(_p.col, container.at(_p.row).length() - _p.col) };//remember tail
	container.at(_p.row).erase(container.at(_p.row).begin() + _p.col, container.at(_p.row).end());//remove tail


	while ( std::getline( ss, line, '\n') ) {


		if (!is_untill_new_line_appended) {

			is_untill_new_line_appended = true;
			container.at(_p.row).append(line);
		
		}else {

			++count;
			container.insert(container.begin() + _p.row + count , line );

		};

		n_symbols_added += line.length() + 1;

		line.erase();
	};


	if (_text.at(_text.size() - 1 ) == '\n') {
		insertNewLine(position{ _p.row + (count++),0 });
	};
	
	
	container.at(_p.row + count  ).append(tail);
	
	cursorGoTo(position{ _p.row + count , container.at(_p.row).size() });

	//outputAll();
	refreshTotalSymbols();


};


void TextEditorCore::insertNewLine(position _p) {	

	container.insert(container.begin() + _p.row + 1, std::string{} );

};


void TextEditorCore::multilineDelete(position _from, position _to) {
	//delete all between from and to

	if (_from.row == _to.row) {

		if (_from.col == _to.col) { return; };     //do nothing or throw exception ?

		container.at(_from.row).erase(_from.col ,_to.col - _from.col );
	
	} else {

		long from_row_len = container.at(_from.row).length();
		container.at(_from.row).erase(_from.col, from_row_len - _from.col);
		
		container.at(_to.col).erase(0, _to.col);
		
		if ( (_to.row - _from.row) > 1 ) { // 100 - 0 = 100 > 1 ; 2 - 0 = 2 > 1 ; 1 - 0 = 1 ;   
			container.erase(container.begin() + _from.row + 1, container.begin() + _to.row - 1); // 0 + 1 = 1, 100 - 1 = 99 : 1 to 99;___ 0 + 1 = 1, 2 - 1 = 1 : 1 to 1;___ 0 + 1 = 1, 1 - 1 = 0 invalid 
		};
		
	};
	
	refreshTotalSymbols();
	
	//outputAll();
};


void TextEditorCore::refreshTotalSymbols() {

	totalsymbols = 0;

	for (auto sth : container) {

		totalsymbols = totalsymbols + sth.length();
	};
};

void TextEditorCore::normalizeSelection() {
	if (selection.from.row > selection.to.row) {
		
		position temp{ selection.to.row, selection.to.col };
		
		selection.to = selection.from;
		
		selection.from = temp;
	
	} else if ( selection.from.col > selection.to.col ) {
		
		long temp = selection.to.col;
		
		selection.to.col = selection.from.col;
		
		selection.from.col = temp;
	};

	//else do nothing
};


TextEditorCore::position* TextEditorCore::whatCursorMethodsChanging()  {

	if (currentMode == mode::Edit) {
		return  &cursor;
	} else {
		return  &selection.to;
	};
};


TextEditorCore& TextEditorCore::outputCursorPos() {
	std::cout << "\n row: " << cursor.row << " col: " << cursor.col << "\n";

	return *this;
};



TextEditorCore& TextEditorCore::outputAll()  {
	for (auto sth : container) {
		std::cout << "\n~" << sth.length() << "\t>" << sth;
	};

	return *this;
};


TextEditorCore& TextEditorCore::outputSelectPoss() {
	std::cout << "\n from \t row: " << selection.from.row << " col: " << selection.from.col << "\n to \t row: " << selection.to.row << " col: " << selection.to.col << "\n";
	
	return *this;
};




TextEditorCore::TextEditorCore(){	

	container.push_back(std::string{});

	//else is default
	
};

TextEditorCore::TextEditorCore(std::istream& _inpf):TextEditorCore() {//delegated constructor
	
	container.erase( container.begin() );//erase 0s row created by default constructor

	std::string line{};


	while (std::getline(_inpf, line,'\n')) {		
		
		container.push_back( line );//push line's  copybyvalue 
		
		line.erase();
	};
		

	refreshTotalSymbols();

	//outputAll();


};


/*
TextEditorCore::~TextEditorCore() {
ok, no pointers - no manual deletion 
};
*/

inline TextEditorCore::position TextEditorCore::getCursorPosition() const{
	return cursor;
};


TextEditorCore& TextEditorCore::cursorGoOneUp() {

	position* what_is_changing = whatCursorMethodsChanging();
		
		
		if ( (*what_is_changing).row > 0l) {//1,2,3,...//no poss above zer0
			--(*what_is_changing).row;//0,1,2,...

			if ((*what_is_changing).col > container.at((*what_is_changing).row).length() /*&& (*what_is_changing).row != 0l*/) {  //old > allowed 

				(*what_is_changing).col = container.at((*what_is_changing).row).length();

			};		
		  };

		//	e.g. old pos: 10,20
		//  wanted pos: 9,20
		//	but 9's row length : 15
		//	so result pos : 9,15 i.e. after last character at 9's string


		return *this;
};

TextEditorCore& TextEditorCore::cursorGoOneDown() {


	position* what_is_changing = whatCursorMethodsChanging();



		if ((*what_is_changing).row + 1  < container.size()) { // if wanted is less or equal then allowed
																// e.g. size is 50, so allowed are from 0 to 49 
																//old: 49,wanted:  50, 49 + 1 < 50 => got false
																//old: 48, wanted: 49, 48 + 1 < 50 => true  

			++(*what_is_changing).row;

			if ((*what_is_changing).col > container.at((*what_is_changing).row).length()) {
				(*what_is_changing).col = container.at((*what_is_changing).row).length();
			};
		};

		//see cursorGoOneUp

		return *this;
};

TextEditorCore& TextEditorCore::cursorGoOneLeft() {

	position* what_is_changing = whatCursorMethodsChanging();



			if ((*what_is_changing).col > 0l) {
				--(*what_is_changing).col;
			} else {//old : x,0
				if ((*what_is_changing).row > 0l) {// old not :0,0
					cursorGoOneUp();
					(*what_is_changing).col = container.at((*what_is_changing).row).length();
				};
				//else do nothing
			};
			//	e.g. old pos: 10,0
			//  wanted pos: 9,(9's length)
			//	9's row length : 15
			//	so result pos : 9,15 i.e. after last character at 9's string

			
		return *this;
};

TextEditorCore& TextEditorCore::cursorGoOneRight() {

	position* what_is_changing = whatCursorMethodsChanging();



			if ((*what_is_changing).col < container.at((*what_is_changing).row).length() ) {

				++(*what_is_changing).col;
			} else {

				if ((*what_is_changing).row + 1 < container.size()) {
					cursorGoOneDown();
					(*what_is_changing).col = 0l;
				};
			
			};
		


		return *this;
};

TextEditorCore& TextEditorCore::cursorGoTo(position _p) {


	if ( validatePosition( _p ) ) {
		
		position* changeMe = whatCursorMethodsChanging();

		changeMe->row = _p.row;
		changeMe->col = _p.col;
		

	}else {
		throw std::logic_error(Messages::InvalidPosition);
	};

	return *this;
};

TextEditorCore& TextEditorCore::insert(const char _symbol) {

	char t[2]{ _symbol };

	multilineInsert(cursor, std::string{ t });

	return *this;
};

TextEditorCore& TextEditorCore::insert(const char* _cstr) {
	multilineInsert(cursor, std::string{ _cstr });
	
	return *this;
};

TextEditorCore& TextEditorCore::insert(const std::string& _cstr) {
	multilineInsert(cursor, _cstr);

	return *this;
};

TextEditorCore& TextEditorCore::deleteSelectedSegment() {
	multilineDelete( selection.from,selection.to );

	selection.to = selection.from;

	return *this;
};


TextEditorCore& TextEditorCore::HOMEkey() {
	cursorGoTo(position{ cursor.row,0l });//temp pos obj
	return *this;
};

TextEditorCore& TextEditorCore::ENDkey() {
	cursorGoTo(position{ cursor.row, container.at(cursor.row).length() });
	return *this;
};

TextEditorCore& TextEditorCore::CtrlHOMEkey() {
	cursorGoTo(position{});
	return *this;
};

TextEditorCore& TextEditorCore::CtrlENDkey() {
	long tmp = container.size() - 1;

	cursorGoTo(position{ tmp , container.at(tmp).length() });

	return *this;
};



TextEditorCore& TextEditorCore::saveTo(std::ostream& _stream)  { 
	
	for (auto str : container) {

		_stream << str.c_str();
	
	};
	
	return *this;
};




TextEditorCore& TextEditorCore::selectBegin() {
	currentMode = mode::Select;
	selection.from.row = selection.to.row = cursor.row;
	selection.from.col = selection.to.col = cursor.col;
	
	return *this;
};

TextEditorCore& TextEditorCore::selectEnd() {
	currentMode = mode::Edit;
	cursor = selection.to;

	return *this;
};

TextEditorCore& TextEditorCore::selectReset() {
	selectBegin();

	return *this;
};




std::string& TextEditorCore::selectSelected()  {
	normalizeSelection();
	
	std::stringstream ss{};

	if (selection.from.row == selection.to.row) {//select on one line

		ss << container.at(selection.from.row).substr(selection.from.col, selection.to.col - selection.from.col) ;
	
	} else {//2 lines and more
		
		ss << container.at(selection.from.row).substr(selection.from.col, container.at(selection.from.row).length() - selection.from.col) << "\n";


		if (selection.from.row - selection.to.row > 1) {
			for (long i = (selection.from.row + 1);i < selection.to.row; i++) {//between head and tail if n of selected lines more than 3

				ss << container.at(i) << "\n";
			};
		};
		
		ss << container.at(selection.to.row).substr(0, selection.to.col) << "\n";

	};

	return ss.str(); //got -> std::string

};



TextEditorCore::position TextEditorCore::find(const std::string& _str)  {//find first instance
	return finder(_str, position{});
};

TextEditorCore::position TextEditorCore::find(const char* _str)  {//find first instance
	return find(std::string{ _str });
};


TextEditorCore::position TextEditorCore::findNext() {
	if (findParams.lastFoundAt != position{ container.size() - 1, container.at(container.size() - 1 ).length() }) {
		
		return finder(findParams.searchString, findParams.lastFoundAt);

	}else{
		throw std::logic_error(Messages::NotFound);
	};
};

TextEditorCore::position TextEditorCore::finder(const std::string& _str, TextEditorCore::position& _p ) {



	findParams.searchString = _str;

	for (long i = _p.row; i  < container.size() ; i++) {
		long j = container.at(i).find(_str, (i == _p.row) ? _p.col : 0);//start searching from _p.col on startsearch _p.row 
		
		if (j != std::string::npos) {//if found// if j != -1
			findParams.lastFoundAt = position{ i,j };
			return findParams.lastFoundAt;
		};
	};

	//else

	return position{ container.size() - 1 ,container.at( container.size() - 1 ).length() };//return last allowed position

};





bool TextEditorCore::replace(std::string& _f, std::string& _r) {

	long len_f = _f.length();

	position temp;
	try {
		temp = find(_f);
	} catch (std::logic_error err) {
		return false;
	};

	if ( len_f == 0 ) { return false; };

	
	multilineDelete(temp , position{ temp.row, temp.col + len_f });
	multilineInsert(temp , _r);

	
	//outputAll();
	
	return true;

};


bool TextEditorCore::replace(const char* _f, const char* _r) {
	return replace(std::string{ _f }, std::string{ _r });
};



bool TextEditorCore::replaceAll(std::string& _f, std::string& _r) {

	while (replace(_f, _r));

	//outputAll();

	return true;
};


bool TextEditorCore::replaceAll(const char* _f, const char* _r) {
	return replaceAll(std::string{ _f }, std::string{ _r });
};


