#include "texteditorcore.hpp"

#include <sstream>


bool TextEditorCore::validatePosition( TextEditorCore::position _pos) const {
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
	long  count = 0;//count counts rows for further tail insertion
	


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

		

		line.erase();
	};


	if (_text.at(_text.size() - 1 ) == '\n') {
		insertNewLine(position{ _p.row + (count++),0 });
	};
	
	
	container.at(_p.row + count  ).append(tail);
	

	cursorGoTo(position{ _p.row + count , (long)container.at(_p.row + count).size() });

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
		
		if (_from.col != 0) {
			container.at(_from.row).erase(_from.col, from_row_len - _from.col);
		} else {
			container.erase(container.begin() + _from.row);
		};
		
		if (_to.col != container.at(_to.row).length() ) {
			container.at(_to.row).erase(0, _to.col);
		} else {
			container.erase(container.begin() + _to.row);
		};




		if ( (_to.row - _from.row) > 1 ) { // 100 - 0 = 100 > 1 ; 2 - 0 = 2 > 1 ; 1 - 0 = 1 ;   
			container.erase(container.begin() + _from.row + 1, container.begin() + _to.row - 1); // 0 + 1 = 1, 100 - 1 = 99 : 1 to 99;___ 0 + 1 = 1, 2 - 1 = 1 : 1 to 1;___ 0 + 1 = 1, 1 - 1 = 0 invalid 
		};
		
	};
	

	cursorGoTo(_from);

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
	if (selection.from.row > selection.to.row ||
		(selection.from.row ==  selection.to.row  && selection.from.col > selection.to.col)) {
		
		position temp = selection.to;
		
		selection.to = selection.from;
		
		selection.from = temp;
	
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



TextEditorCore::position TextEditorCore::maxPosition() const {
	return position{ (long)(container.size() - 1), (long)(container.at( container.size() - 1 ).length()) };
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

	if (container.size() == 1 && container.at(0).length() == 0) { throw std::logic_error(Messages::EmptyEntryFile); };
		
	
	//needed check if last char in stream is \n 
	//if ( _inpf.rdbuf()->sputbackc(char{}) == '\n')	container.push_back(line); //just a try 
	//char h{};
	//_inpf.rdbuf()->sputbackc(h);
	//std::cout << "~" << h << "~";


	refreshTotalSymbols();

	//outputAll();


};


/*
TextEditorCore::~TextEditorCore() {
ok, no pointers - no manual deletion 
};
*/


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
	cursorGoTo(position{ cursor.row, (long)container.at(cursor.row).length() });
	return *this;
};

TextEditorCore& TextEditorCore::CtrlHOMEkey() {
	cursorGoTo(position{});
	return *this;
};

TextEditorCore& TextEditorCore::CtrlENDkey() {
	long tmp = container.size() - 1;

	cursorGoTo(position{ tmp , (long)container.at(tmp).length() });

	return *this;
};



TextEditorCore& TextEditorCore::saveTo(std::ostream& _stream)  { 
	


	//std::stringstream ss;

	for (auto str : container) {


		//ss << str << "\n";
		_stream << str.c_str() << "\n";
	
	};
	
	/*
	std::string temp = ss.str();
	temp.pop_back(); //pop back last \n

	_stream << temp;
	*/
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




std::string TextEditorCore::selectSelected()  {
	normalizeSelection();
	
	std::stringstream ss{};

	if (selection.from.row == selection.to.row) {//select on one line

		if (selection.from != selection.to) {
			ss << container.at(selection.from.row).substr(selection.from.col, selection.to.col - selection.from.col);
		}else{
			return std::string{ "" };//is it's legal ?)
		};

	} else {//2 lines and more
		
		ss << container.at(selection.from.row).substr(selection.from.col, container.at(selection.from.row).length() - selection.from.col);
		ss << "\n";


		if (selection.to.row - selection.from.row > 1) {
			for (long i = (selection.from.row + 1); i < selection.to.row; i++) {//between head and tail if n of selected lines more than 3

				ss << container.at(i) << "\n";
			};
		};
		
		ss << container.at(selection.to.row).substr(0, selection.to.col) ;

	};

	return ss.str(); //got -> std::string

};






TextEditorCore::position TextEditorCore::finder(const std::string& _str,const TextEditorCore::position& _p) {

	if (_str.length() == 0) { throw std::logic_error(Messages::EmptyFindNotAllowed); };
	
	findParams.searchString = _str;


	for (long i = _p.row; i < container.size(); i += 1) {

		long j = container.at(i).find(_str, (i == _p.row) ? _p.col : 0);

		if (j != std::string::npos) { //i.e. j != -1 // i.e. found
			findParams.lastFoundAt = position{ i,j };
			return findParams.lastFoundAt;
		};
	};

	return maxPosition();
};






TextEditorCore::position TextEditorCore::find(const std::string& _str)  {//find first instance
	position temp = finder(_str, position{});
	if (temp == maxPosition()) { throw std::logic_error(Messages::NotFound); 
	}else{
		return temp;
	};
};

TextEditorCore::position TextEditorCore::find(const char* _str)  {//find first instance
	return find(std::string{ _str });
};


TextEditorCore::position TextEditorCore::findNext() {
	position temp = finder(findParams.searchString,
		position{
				  findParams.lastFoundAt.row ,
				  (long)(findParams.lastFoundAt.col + findParams.searchString.length())
				});
	
	if (temp == maxPosition()) {
		throw std::logic_error(Messages::NotFound);
	} else {
		return temp;
	};
};






bool TextEditorCore::replace(const std::string& _f,const std::string& _r) {//not multiline replace
	if (!_f.length()) { throw std::logic_error(Messages::EmptyReplacingNotAllowed); };
	//empty _r allowed

	position temp = finder(_f, position{});
	if (temp == maxPosition()) { return false; };


	container.at(temp.row).replace(temp.col, findParams.searchString.length(), _r);

	refreshTotalSymbols();

	return true;

};


bool TextEditorCore::replace(const char* _f, const char* _r) {
	return replace(std::string{ _f }, std::string{ _r });
};



bool TextEditorCore::replaceAll(const std::string& _f,const std::string& _r) {
	bool res, onereplaced = false;
	while (res = replace(_f, _r)) { onereplaced = onereplaced || res; };// could be optimized by decreasing number of findloops



	refreshTotalSymbols();

	return onereplaced;
};


bool TextEditorCore::replaceAll(const char* _f, const char* _r) {
	return replaceAll(std::string{ _f }, std::string{ _r });
};


