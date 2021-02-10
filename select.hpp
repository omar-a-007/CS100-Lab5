#ifndef __SELECT_HPP__
#define __SELECT_HPP__
#include <iostream>
#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.

class Select_Contains: public Select
{
protected:
	int column;
	bool* chosenRows;

public:

	~Select_Contains(){
		delete chosenRows;
	}
	
	Select_Contains(const Spreadsheet* sheet, const std::string& col, const std::string& content){				
		
		chosenRows = new bool[sheet->get_row_size()];
		for(int i = 0; i < sheet->get_row_size(); i++)
			chosenRows[i] = false;
		

		column = sheet->get_column_by_name(col);
		if(column != -1){
			for(int i = 0; i < sheet->get_row_size(); i++){
				if((sheet->cell_data(i, column)) == content){
					setSelection(i);
				}
			}
							
		}	
	}

	void setSelection(int row) {
        	chosenRows[row] = true;
	}

	virtual bool select(int row) const {
        	return chosenRows[row];
	}


};


class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }
/*
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
*/
};

#endif //__SELECT_HPP__
