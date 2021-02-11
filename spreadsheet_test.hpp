#ifndef __SPREADSHEET_TEST__
#define __SPREADSHEET_TEST__
#include "spreadsheet.hpp"
#include "select.hpp"

#include <string>
#include <sstream>

#include "gtest/gtest.h"

TEST(SpreadsheetTest, blankSheet)
{
	Spreadsheet sheet;

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SpreadsheetTest, selectAll_noSelection)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "Pet"});
	sheet.add_row({"Jane","Cat"});
	sheet.add_row({"John","Dog"});


	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "Jane Cat\nJohn Dog\n");
}

TEST(SelectTest, selectAll_EmptyString)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "Pet"});
	sheet.add_row({"Jane","Cat"});
	sheet.add_row({"John","Dog"});

	sheet.set_selection(new Select_Contains(&sheet,"Pet", ""));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "Jane Cat\nJohn Dog\n");
}

TEST(SelectTest, select_Substring)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"app"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\napples\nSnapple\n");
}

TEST(SelectTest, select_caseSensitive)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"Apple"});
	sheet.add_row({"APPLE"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\n");
}

TEST(SelectTest, select_noData)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectTest, select_columnDoesntExist)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});

	sheet.set_selection(new Select_Contains(&sheet,"People", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectTest, select_dataDoesntExist)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"orange"});
	sheet.add_row({"watermelon"});
	sheet.set_column_names({"Food"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "soda"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

// Only Checks First Matched Column Name
TEST(SelectTest, select_TwoColumnsSameName1)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "hamburger"});
	sheet.add_row({"orange", "fries"});
	sheet.add_row({"watermelon", "salad"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple hamburger\n");
}

TEST(SelectTest, select_TwoColumnsSameName2NoMatch)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "hamburger"});
	sheet.add_row({"orange", "fries"});
	sheet.add_row({"watermelon", "salad"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "fries"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectTest, select_TwoColumnsSameName3)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "Apple"});
	sheet.add_row({"orange", "Snapple"});
	sheet.add_row({"watermelon", "Mango"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple Apple\n");
}

TEST(SelectTest, selectTwoSameColumns)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "Apple"});
	sheet.add_row({"apples", "Apples"});
	sheet.add_row({"SNAPPLE", "Snapple"});
	sheet.add_row({"Apple", "APPLE"});

	sheet.set_selection(new Select_Contains(&sheet,"Food", "apple"));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple Apple\napples Apples\n");
}

TEST(SelectANDTest, select_OneColumn_TwoSelects1)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"orange"});
	sheet.add_row({"watermelon"});
	sheet.add_row({"hamburger"});
	sheet.add_row({"fries"});
	sheet.add_row({"salad"});

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"Food","e"),
			new Select_Contains(&sheet,"Food","s")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apples\nfries\n");
}

TEST(SelectANDTest, select_TwoColumns_TwoSelects2)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});           //Name doesnt contain "a"
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});           //FavFood doesnt contain "a"
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
    sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"FavFood","a"),
			new Select_Contains(&sheet,"Name","a")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "Adam Smith apple\nJane Smith apples\nTrish Washington orange\nMike Washington watermelon\nRoger Washington hamburger\nZelda Hyrule fruitcake\nMarcus Hyrule fruit salad\n");
}

TEST(SelectANDTest, select_TwoColumns_TwoSelects3)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});           
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});           
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
    sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"FavFood","cake"),
			new Select_Contains(&sheet,"Name","Hyrule")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "Zelda Hyrule fruitcake\n");
}

TEST(SelectANDTest, select_TwoColumns_TwoSelectsNoMatch)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});           
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});           
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
    sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"FavFood","apple"),
			new Select_Contains(&sheet,"Name","Hyrule")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectANDTest, select_TwoColumns_TwoSelectsNoMatchNoColumns)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});           
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});           
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
    sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"YummyFood","fruitcake"),
			new Select_Contains(&sheet,"FirstName","Zelda")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}


TEST(SelectANDTest, select_TwoColumns_TwoSelectsNoMatchNoData)
{
	Spreadsheet sheet;

	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet,"FavFood","fruitcake"),
			new Select_Contains(&sheet,"Name","Zelda")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectORTest, select_OneColumn_TwoSelects1)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"orange"});
	sheet.add_row({"watermelon"});
	sheet.add_row({"hamburger"});
	sheet.add_row({"fries"});
	sheet.add_row({"salad"});

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"Food","apple"),
			new Select_Contains(&sheet,"Food","ad")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\napples\nSnapple\nsalad\n");
}

TEST(SelectORTest, select_TwoColumns_TwoSelects1)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
	sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"FavFood","apples"),
			new Select_Contains(&sheet,"Name","Hyrule")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "Jane Smith apples\nSarah Hyrule fries\nZelda Hyrule fruitcake\nMarcus Hyrule fruit salad\n");
}


TEST(SelectORTest, select_TwoColumns_noMatch1)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
	sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"FavFood","cheese"),
			new Select_Contains(&sheet,"Name","Olivia")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectORTest, select_TwoColumns_noMatch2noColumns)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Name", "FavFood"});
	sheet.add_row({"Adam Smith", "apple"});
	sheet.add_row({"Jane Smith", "apples"});
	sheet.add_row({"John Smith", "Snapple"});
	sheet.add_row({"Trish Washington", "orange"});
	sheet.add_row({"Mike Washington", "watermelon"});
	sheet.add_row({"Roger Washington", "hamburger"});
	sheet.add_row({"Sarah Hyrule", "fries"});
	sheet.add_row({"Zelda Hyrule", "fruitcake"});
	sheet.add_row({"Marcus Hyrule", "fruit salad"});

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"YummyFood","apple"),
			new Select_Contains(&sheet,"FirstName","Zelda")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectORTest, select_TwoColumns_noMatch3noData)
{
	Spreadsheet sheet;

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"FavFood","apple"),
			new Select_Contains(&sheet,"Name","Zelda")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectORTest, selectTwoSameColumns)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "Apple"});
	sheet.add_row({"apples", "Apples"});
	sheet.add_row({"SNAPPLE", "Snapple"});
    sheet.add_row({"Apple", "APPLE"});

	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet,"Food","apple"),
			new Select_Contains(&sheet,"Food","Zelda")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple Apple\napples Apples\n");
}

TEST(SelectNOTTest, select_OneColumnNoMatch)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});

	sheet.set_selection(
		new Select_Not( 
			new Select_Contains(&sheet,"Food","apple")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectNOTTest, select_OneColumnMatchAllNoColumn)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});

	sheet.set_selection(
		new Select_Not( 
			new Select_Contains(&sheet,"FavFood","apples")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\napples\nSnapple\n");
}


TEST(SelectNOTTest, select_OneColumnNoMatchNoData)
{
	Spreadsheet sheet;

	sheet.set_selection(
		new Select_Not( 
			new Select_Contains(&sheet,"Food","Apple")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}

TEST(SelectNOTTest, select_NoMatchTwoSameColumns)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food", "Food"});
	sheet.add_row({"apple", "Apple"});
	sheet.add_row({"apples", "Apples"});
	sheet.add_row({"Snapple", "Snapple"});

	sheet.set_selection(
		new Select_Not( 
			new Select_Contains(&sheet,"Food","apple")));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "");
}


TEST(CompositeSelectTest, select_And_Not)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"Apple"});

	sheet.set_selection(
			new Select_And(
				new Select_Contains(&sheet,"Food","apple"),
				new Select_Not( 
					new Select_Contains(&sheet,"Food","s"))
                ));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\nSnapple\n");
}

TEST(CompositeSelectTest, select_And_Or_Not)
{
	Spreadsheet sheet;
	sheet.set_column_names({"Food"});
	sheet.add_row({"apple"});
	sheet.add_row({"apples"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"Apple"});

	sheet.set_selection(
			new Select_And(
				new Select_Contains(&sheet,"Food","apple"),
				new Select_Or(
					new Select_Contains(&sheet,"Food","s"),
                    new Select_Not(
						new Select_Contains(&sheet,"Food","s"))
                	)));

	std::stringstream ss;
	sheet.print_selection(ss);
	std::string test = ss.str();
	EXPECT_EQ(test, "apple\napples\nSnapple\n");
}






#endif //__SPREADHSHEET_TEST__
