#ifndef  __TEXTEDITORCORE_HPP__
#define __TEXTEDITORCORE_HPP__


#include <iostream>
#include<fstream>
#include <string>
//#include <list> // or vector ?
#include<vector>
#include<utility>//for std::pair
#include<fstream>

#include"messages.hpp"

typedef unsigned long UL;



class TEC {//text editor core
private:
	enum class mode{
		Edit,
		Select,
		//Find
	
	}currentMode = mode::Edit;

	
	
	UL 
		totalsymbols = 0,
		maxcols = 0,
		maxrows = 0;



	struct position {
		position() :row(0ul), column(0ul) {};
		position(UL _r, UL _c) :row(_r), column(_c) {};
		position (const position&) = default;
		position& operator =(const position&) = default;
		position (position&&) = default;
		position& operator =(position&&) = default;

		UL 
		row,
		column;
	}
	cursor;



	struct {
		std::string* searchtext = nullptr;
		bool reachedend = false, found = false;
		position last_found_at;
	} findparams , findparamsPrev;



	struct {
		position from, to;
	} selection	;

	
	
	std::vector< std::pair< UL , std::string* >* >* container;


	bool validatePosition(position,const char*) const;

	void multilineInsert(UL, UL, std::string&);

	void insertNewLine(UL);

	void multilineDelete(UL, UL, UL, UL);

	void refreshTotalsymbolsMaxcolsMaxrows();

	void normalizeselection();




	
	
	
public:
	//dev
	TEC& outputCursorPos();
	TEC& outputAll();
	TEC& outputSelectPoss();


	UL getTotal() { return totalsymbols; };
	UL getMaxRows() { return maxrows; };

	//base start
	TEC();
	TEC(std::istream&);

	TEC(const TEC&)             = delete;
	TEC& operator= (const TEC&) = delete;

	TEC(TEC&&)                  = delete;
	TEC& operator= (TEC&&)      = delete;

	~TEC();

	//base end
	//cursor start
	
	UL getCursorRow() const;
	UL getCursorColumn() const;

	TEC& cursorGoOneUp();
	TEC& cursorGoOneRight();
	TEC& cursorGoOneDown();
	TEC& cursorGoOneLeft();

	TEC& cursorGoTo(UL, UL);

	//cursor end
	//edit start
	
	TEC& insert(const char);
	TEC& insert(const char*);
	TEC& TEC::insert(std::string&);
	TEC& deleteSelectedSegment();
	
	//edit end
	//HOME,END start
	
	TEC& HOMEkey();
	TEC& ENDkey();
	TEC& CtrlHOMEkey();
	TEC& CtrlENDkey();

	
	//HOME,END end
	
	
	//save to output stream
	TEC& saveTo( std::ostream& );///param shld bi stream
	
				  
	//select start
	TEC& selectBegin();
	TEC& selectEnd();
	TEC& selectReset();
	std::string& selectSelected()  ;

	//select end
	//find && replace start
	
	position find(const char*) ;
	position find(char*) ;
	position find(std::string&) ;
	position findNext();
	position finder(std::string& _str, position& _p  = position{ 0,0 }) ;
	bool replace( const char*,const char* );
	bool replace( char*,char*);
	bool replace(std::string&,std::string&);
	bool replaceAll(const char*, const char*);
	bool replaceAll(char*, char*);
	bool replaceAll(std::string&, std::string&);

	//find && replace end
	







};










#endif // ! __TEXTEDITORCORE_HPP__