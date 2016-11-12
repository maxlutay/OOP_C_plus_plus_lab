#include "texteditorcore.hpp"

//#include <regex>
#include <sstream>
//#include<functional>

bool TEC::validatePosition(TEC::position _pos, const char* _validatemode = "all") const {
	/*
	if (_validatemode == "down") {
		return _pos.row < this->container->size();
	}else if (_validatemode == "up") {
		return _pos.row > 0;
	}else if (_validatemode == "left") {
		return _pos.column > 0;
	}else if(_validatemode == "right"){
		return _pos.column < this->container->at(_pos.row)->first;
	}else {
	*/	return
			_pos.row >= 0 &&
			_pos.column >= 0 &&
			_pos.row <= (this->container->size()) &&
			_pos.column <= (this->container->at(_pos.row)->first);
	//};
	

};


void TEC::multilineInsert(UL _row, UL _col, std::string& _text) {


	if (_text == "") { return; };

	std::string *line = new std::string, *memotail = nullptr;
	std::stringstream ss{_text};

	bool start_appended = false;
	UL count = 0;
	

	while (std::getline( ss, *line, '\n')) {


		if (!start_appended) {
			start_appended = true;
			memotail = new std::string( container->at(_row)->second->substr(_col, container->at(_row)->first - _col) );

			container->at(_row)->second->replace(container->at(_row)->second->begin() + _col, container->at(_row)->second->end(),"");

			container->at(_row)->second->append(*line);
			container->at(_row)->first = container->at(_row)->second->size();
		}else {

			container->insert(container->begin() + _row + ( ++count ), new std::pair< UL, std::string* >{ line->length(), line });

		};
		//no delete 
		line = new std::string;
	};


	if (_text.at(_text.size() - 1 ) == '\n') {
		insertNewLine(_row + (count++) );
	};
	
	container->at(_row + count  )->second->append(*memotail);
	container->at(_row + count  )->first  += memotail->length();

	
	cursorGoTo( _row + count , container->at(_row + count)->first);

	//outputAll();
	refreshTotalsymbolsMaxcolsMaxrows();


};


void TEC::insertNewLine(UL _row) {	

		container->insert(container->begin() + _row + 1, new std::pair<UL, std::string* >{ 0ul, new std::string  });

};


void TEC::multilineDelete(UL _row1, UL _col1, UL _row2, UL _col2) {



	if (_row1 == _row2) {
		if (_col1 == _col2) { return; };
		container->at(_row1)->second->replace(_col1 ,_col2 -  _col1 , "");
	} else {

		auto pstart = container->at(_row1);
		pstart->second->replace(_col1, pstart->first - _col1 , "");

		auto pend = container->at(_row2);
		pend->second->replace(0, _col2, "");


		if ( (_row1 - _row2) > 1) {
			container->erase(container->begin() + _row1 + 1, container->begin() + _row2 - 1);
		};


	};
	
	refreshTotalsymbolsMaxcolsMaxrows();
	
	//outputAll();
};


void TEC::refreshTotalsymbolsMaxcolsMaxrows() {

	totalsymbols = 0;

	for (auto sth : *container) {

		totalsymbols = totalsymbols + sth->first + 1 ;
		

		if (maxcols < sth->first) {
			maxcols = sth->first;
		};
	};
	maxrows = container->size();
};

void TEC::normalizeselection() {
	if (selection.from.row > selection.to.row) {
		position temp(selection.to.row, selection.to.column);
		selection.to = selection.from;
		selection.from = temp;
	} else if ( selection.from.column > selection.to.column ) {
		UL temp = selection.to.column;
		selection.to.column = selection.from.column;
		selection.from.column = temp;
	};
};

TEC& TEC::outputAll()  {
	for (auto sth : *container) {
		std::cout << "\n~" << sth->first << "\t>" << *sth->second;
	};

	return *this;
};


TEC& TEC::outputCursorPos()  {
	std::cout << "\n row: " << getCursorRow() << " col: " << getCursorColumn() << "\n";

	return *this;
};

TEC& TEC::outputSelectPoss() {
	std::cout << "\n from \t row: " << selection.from.row << " col: " << selection.from.column << "\n to \t row: " << selection.to.row << " col: " << selection.to.column << "\n";
	return *this;
};




TEC::TEC(){	

	container = new std::vector<std::pair<UL, std::string* >* >;
	
	std::string* s = new std::string{};//could be setted default phrase

	std::pair<UL, std::string*>* p = new std::pair<UL, std::string*>{ s->length(),s };

	container->push_back(p);

	

	//outputAll();
};

TEC::TEC(std::istream& _inpf):TEC() {//delegated constructor
	
	container->erase(container->begin());//erase 0s row created by default constructor

	std::string* line = new std::string{};


	while (std::getline(_inpf, *line,'\n')) {
		
		
		container->push_back(new  std::pair< UL, std::string* >{ line->length(), line });
		
		//no delete 

		line = new std::string;
	};

	

	refreshTotalsymbolsMaxcolsMaxrows();

	//outputAll();


};

TEC::~TEC() {
	for (auto sth : *container) {
		delete (*sth).second;
		delete sth;
	};

	delete container;
};

 UL TEC::getCursorColumn() const{
	return cursor.column;
};

UL TEC::getCursorRow() const {
	return cursor.row;
};

TEC& TEC::cursorGoOneUp() {

		position* what_is_changing;
		if (currentMode == mode::Edit) {
			what_is_changing = &cursor;
		}else {
			what_is_changing = &selection.to;
		};
		
		
		if ( (*what_is_changing).row > 0) {
			--(*what_is_changing).row;

			if ((*what_is_changing).column > container->at((*what_is_changing).row)->first && (*what_is_changing).row != 0ul) {

				(*what_is_changing).column = container->at((*what_is_changing).row)->first;

			};		
		  };

		return *this;
};

TEC& TEC::cursorGoOneDown() {


		position* what_is_changing;
		if (currentMode == mode::Edit) {
			what_is_changing = &cursor;
		} else {
			what_is_changing = &selection.to;
		};


		if ((*what_is_changing).row + 1  < container->size()) {

			++(*what_is_changing).row;

			if ((*what_is_changing).column > container->at((*what_is_changing).row)->first) {
				(*what_is_changing).column = container->at((*what_is_changing).row)->first;
			};
		};



		return *this;
};

TEC& TEC::cursorGoOneLeft() {

		position* what_is_changing;
		if (currentMode == mode::Edit) {
			what_is_changing = &cursor;
		}
		else {
			what_is_changing = &selection.to;
		};


			if ((*what_is_changing).column > 0) {
				--(*what_is_changing).column;
			} else {
				if ((*what_is_changing).row > 0ul) {
					cursorGoOneUp();
					(*what_is_changing).column = container->at((*what_is_changing).row)->first;
				};
			};
			
			
		return *this;
};

TEC& TEC::cursorGoOneRight() {

	position* what_is_changing;
	if (currentMode == mode::Edit) {
		what_is_changing = &cursor;
	}
	else {
		what_is_changing = &selection.to;
	};



			if ((*what_is_changing).column < container->at((*what_is_changing).row)->first) {

				++(*what_is_changing).column;
			} else {

				if ((*what_is_changing).row + 1 < container->size()) {
					cursorGoOneDown();
					(*what_is_changing).column = 0ul;
				};
			
			};
		


		return *this;
};

TEC& TEC::cursorGoTo(UL _row,UL _col) {


	if ( validatePosition( position(_row,_col) ) ) {
		if (currentMode == mode::Edit) {
			cursor.row = _row;
			cursor.column = _col;
		}else {
			selection.to.row = _row;
			selection.to.column = _col;
		};


	}else {
		throw std::logic_error(Messages::InvalidPosition);
	};

	return *this;
};

TEC& TEC::insert(const char _symbol) {

	char t[2]{ _symbol };

	multilineInsert(cursor.row, cursor.column, std::string(t));

	return *this;
};

TEC& TEC::insert(const char* _cstr) {
	multilineInsert(cursor.row, cursor.column, std::string(_cstr) );



	return *this;
};

TEC& TEC::insert(std::string& _cstr) {
	multilineInsert(cursor.row, cursor.column, _cstr);

	return *this;
};

TEC& TEC::deleteSelectedSegment() {
	multilineDelete( selection.from.row, selection.from.column,selection.to.row,selection.to.column );

	selection.to = selection.from;

	return *this;
};


TEC& TEC::HOMEkey() {
	cursorGoTo( cursor.row,0 );
	return *this;
};

TEC& TEC::ENDkey() {
	cursorGoTo(cursor.row, container->at(cursor.row)->first);
	return *this;
};

TEC& TEC::CtrlHOMEkey() {
	cursorGoTo(0, 0);
	return *this;
};

TEC& TEC::CtrlENDkey() {
	UL tmp = container->size() - 1;
	cursorGoTo( tmp , container->at(tmp )->first );
	return *this;
};



TEC& TEC::saveTo(std::ostream& _stream) {
	std::stringstream ss;

	for (auto p : *container) {

		ss << *p->second << "\n";
	
	};

	//ss.seekp(-1, std::ios_base::end);

	auto ret = ss.str();//got -> std::string
	ret.pop_back();//needed because of last \n
	
	_stream << ret;


	return *this;
};




TEC& TEC::selectBegin() {
	currentMode = mode::Select;
	selection.from.row = selection.to.row = cursor.row;
	selection.from.column = selection.to.column = cursor.column;
	
	return *this;
};

TEC& TEC::selectEnd() {
	currentMode = mode::Edit;
	cursor = selection.to;

	return *this;
};

TEC& TEC::selectReset() {
	selectBegin();

	return *this;
};




std::string& TEC::selectSelected() {
	normalizeselection();
	
	std::stringstream ss;

	if (selection.from.row == selection.to.row) {
		ss << container->at(selection.from.row)->second->substr(selection.from.column, selection.to.column - selection.from.column) ;
	
	} else {
		
		ss << container->at(selection.from.row)->second->substr(selection.from.column, container->at(selection.from.row)->first - selection.from.column) << "\n";


		if (selection.from.row - selection.to.row > 1) {
			for (UL i = (selection.from.row + 1);i < selection.to.row; i++) {

				ss << *container->at(i)->second << "\n";
			};
		};




		ss << container->at(selection.to.row)->second->substr(0, selection.to.column) << "\n";

	};

	std::string* ret = new std::string{ ss.str() };//got -> std::string
	

	return *ret;
};



TEC::position TEC::find(std::string& _str)  {//find first
	delete findparams.searchtext;
	//searchtext = new std::string{ _str };
	//return findNext(*searchtext);
	return finder(_str);
};

TEC::position TEC::find(const char* _str)  {
	delete findparams.searchtext;
	findparams.searchtext = new std::string{ _str };
	return finder(*findparams.searchtext);
};

TEC::position TEC::find(char* _str)  {
	delete findparams.searchtext;
	findparams.searchtext = new std::string{ _str };

	return finder(*findparams.searchtext);
};


TEC::position TEC::findNext() {
	if (!findparams.reachedend) {
		position tmp = findparams.last_found_at;
		tmp.column += findparams.searchtext->length();

		

		position temp = finder(*findparams.searchtext, tmp);

		if (!findparams.reachedend) { return temp; }
		else {  throw std::logic_error(Messages::NotFound);	};
	}else{
		throw std::logic_error(Messages::NotFound);
	};
};

TEC::position TEC::finder(std::string& _str, TEC::position& _p) {

	findparamsPrev = findparams;

	for (UL i = _p.row; i < maxrows; i++) {
		UL j;
		if ((j = container->at(i)->second->find(_str,i == _p.row ? _p.column : 0)) != std::string::npos) { findparams.found = true;  return (findparams.last_found_at = position{ i, j }); };
	};
	findparams.reachedend = true;

	return position{ container->size(),container->at( container->size() - 1 )->first };
};





bool TEC::replace(std::string& _f, std::string& _r) {

	UL len_f = _f.length();

	position temp1 = finder(_f);

	if ((len_f == 0) || (findparams.reachedend == true)) { return false; };

	//std::cout << "\n r1: " << temp1.row << " c1: " << temp1.column << " r2: " << temp1.row << " c2: " << temp1.column + len_f <<"\n";

	multilineDelete(temp1.row, temp1.column, temp1.row, temp1.column + len_f );
	multilineInsert(temp1.row, temp1.column, _r);

	
	//outputAll();
	//std::cout << "\n";

	return true;

};


bool TEC::replace(const char* _f, const char* _r) {
	return replace(std::string{ _f }, std::string{ _r });
};


bool TEC::replace( char* _f,  char* _r) {
	return replace(std::string{ _f }, std::string{ _r });
};



bool TEC::replaceAll(std::string& _f, std::string& _r) {

	while (replace(_f, _r));

	//outputAll();

	return true;
};




bool TEC::replaceAll(const char* _f, const char* _r) {
	return replaceAll(std::string{ _f }, std::string{ _r });
};


bool TEC::replaceAll(char* _f, char* _r) {
	return replaceAll(std::string{ _f }, std::string{ _r });
};



