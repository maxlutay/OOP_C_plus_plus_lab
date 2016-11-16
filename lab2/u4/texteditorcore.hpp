#ifndef  __TEXTEDITORCORE_HPP__
#define __TEXTEDITORCORE_HPP__

#include <iostream>
#include <string>
#include<vector>
#include<fstream>

#include"messages.hpp"


class TextEditorCore {//text editor core

public:
	struct position {
			
			position() :row(0l), col(0l) {};
			position(long _r, long _c) :row(_r > 0l ? _r : 0l ), col(_c > 0l ? _c : 0l) {};

			position (const position&)            = default;
			position& operator =(const position&) = default;
			position (position&&)                 = default;
			position& operator =(position&&)      = default;

			bool operator ==(position& _cp) const { return row == _cp.row && col == _cp.col; };
			bool operator !=(position& _cp) const { return row != _cp.row || col != _cp.col; };


			long row, col; //not private because wanted direct manipulation ?or provide method

	};

private:

	enum class mode {
		Edit,
		Select
	};

	struct { position from, to; } selection;	//default 0,0 : 0,0 because of position's default constructor
	mode currentMode = mode::Edit;
	position cursor;							//default 0,0 because of position's default constructor
	std::vector<  std::string > container;
	long totalsymbols = 0;

	struct { 
		position lastFoundAt ;
		std::string searchString = "";
	} findParams;
	

	bool validatePosition( position) const;
	void multilineInsert( position,const std::string&);
	void insertNewLine( position);
	void multilineDelete( position, position);
	void refreshTotalSymbols();
	void normalizeSelection();
		
	position* whatCursorMethodsChanging() ;
	position maxPosition() const;

	
public:
//dev start

	TextEditorCore& outputCursorPos();
	TextEditorCore& outputAll();
	TextEditorCore& outputSelectPoss();


//dev end
//base start

	TextEditorCore();
	TextEditorCore(std::istream&);


	//copy
	TextEditorCore(const TextEditorCore&)             = delete;
	TextEditorCore& operator= (const TextEditorCore&) = delete;


	//move
	TextEditorCore(TextEditorCore&&)                  = delete;
	TextEditorCore& operator= (TextEditorCore&&)      = delete;


	~TextEditorCore() = default;

//base end
//cursor start
	
	inline position getCursorPosition() const {
		return cursor;
	};

	TextEditorCore& cursorGoOneUp();
	TextEditorCore& cursorGoOneRight();
	TextEditorCore& cursorGoOneDown();
	TextEditorCore& cursorGoOneLeft();

	TextEditorCore& cursorGoTo(position);

//cursor end
//edit start
	
	TextEditorCore& insert(char);
	TextEditorCore& insert(const char*);
	TextEditorCore& TextEditorCore::insert(const std::string&);
	TextEditorCore& deleteSelectedSegment();
	
//edit end
//HOME,END start
	
	TextEditorCore& HOMEkey();
	TextEditorCore& ENDkey();
	TextEditorCore& CtrlHOMEkey();
	TextEditorCore& CtrlENDkey();
	
//HOME,END end
//save to output stream start

	TextEditorCore& saveTo(std::ostream& ) ;//!!not const;//can't be const method because of returning
													//const this on const TextEditorCore
													//because of cascade.
											// const ptr(i.e. &) on non-const std::ostream

//save to os end
//select start

	TextEditorCore& selectBegin();
	TextEditorCore& selectEnd();
	TextEditorCore& selectReset();
	std::string selectSelected();//!!not const;//can't be const because of 
												//non-const private normalizeSelection() method

//select end
//find start
	
	position find(const char*)  ;
	position find(const std::string&) ;
	position findNext()  ;
	position finder(const std::string& _str, position& _p )  ;
	
//find end
//replace start

	bool replace( const char*,const char* );
		//bool replace( char*,char*);
	bool replace(std::string&,std::string&);
	bool replaceAll(const char*, const char*);
		//bool replaceAll(char*, char*);
	bool replaceAll(std::string&, std::string&);

//replace end	


};

#endif // ! __TEXTEDITORCORE_HPP__