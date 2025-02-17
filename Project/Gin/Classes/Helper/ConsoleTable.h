﻿#ifndef __CONSOLE_TABLE_H__
#define __CONSOLE_TABLE_H__

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class ConsoleTable
{
public:

    typedef std::vector<std::string>              Headers;
    typedef std::vector<std::string>              Footers;
    typedef std::vector<std::vector<std::string>> Rows;
    typedef std::vector<std::size_t>              Widths;

    /// Initialize a new ConsoleTable
    /// \param headers String list of the tables headers
    ConsoleTable(std::initializer_list<std::string> headers);
    ConsoleTable(const std::vector<std::string>& headers);

    /// Sets the distance from the text to the cell border
    /// \param n Spaces between the text and the cell border
    void setPadding(unsigned int n);


    /// Sets the style of the table, default is 0
    /// n = 0 : Basic table style
    /// n = 1 : Single lined table style
    /// n = 2 : Double lined table style
    /// n = 3 : Invisible table lines style
    /// \param n The table style number
    void setStyle(unsigned int n);


    /// Sorts the table rows based on the first column
    /// \param ascending Should table be sorted ascending or descending
    /// \return True if sorting was successful, otherwise false
    bool sort(bool ascending);


    /// Adds a new row to the table
    /// \param row A list of strings to add as row
    /// \return True if the value was added successfully, otherwise false
    bool addRow(std::initializer_list<std::string> row);
    bool addRow(std::vector<std::string> row);


    /// Removes a row from the table by the row index
    /// \param index The index of the row that should be removed
    /// \return True if the row was removed successfully, otherwise false
    bool removeRow(unsigned int index);


    /// Update an existing table cell with new data
    /// \param row The index of the row that needs to be updated
    /// \param header The index of the column that needs to be updated
    /// \param data The new data that should be assigned to teh cell
    void updateRow(unsigned int row, unsigned int header, std::string data);


    /// Update a header with new text
    /// \param header Index of the header that should be updated
    /// \param text The new text of the new header
    void updateHeader(unsigned int header, std::string text);

    /// Update a footer with new text
    /// \param footer Index of the footer that should be updated
    /// \param text The new text of the new footer
    void updateFooter(unsigned int footer, std::string text);

    /// Operator of the addRow() function
    /// \param row A list of strings to add as row
    /// \return this
    ConsoleTable& operator +=(std::initializer_list<std::string> row);


    /// Operator of the removeRow() function
    /// \param rowIndex The index of the row that should be removed
    /// \return this
    ConsoleTable& operator -=(uint32_t rowIndex);

    static std::string toString(const ConsoleTable& consoleTable);
    static std::vector<std::string> toList(const ConsoleTable& consoleTable, bool linebreak = true);
private:

    /// Holds all header strings of the table
    Headers _headers;

    /// Holds all footer strings of the table
    Footers _footers;

    /// Holds all rows of the table
    Rows _rows;

    /// Holds the size of widest string of each column of the table
    Widths _widths;

    /// Defines row type
    struct RowType
    {
        std::string left;
        std::string intersect;
        std::string right;
    };

    /// Defines table style rows (top, middle, bottom etc)
    struct TableStyle
    {
        std::string horizontal;
        std::string vertical;
        RowType     top;
        RowType     middle;
        RowType     bottom;
    };


    /// Basic style - works on all systems, used as default style
    TableStyle BasicStyle = { "-", "|", { "+", "+", "+" }, { "+", "+", "+" }, { "+", "+", "+" }};


    /// Single lined style - requires speecial character support
    TableStyle LineStyle = { "━", "┃", { "┏", "┳", "┓" }, { "┣", "╋", "┫" }, { "┗", "┻", "┛" }};


    /// Single double style - requires speecial character support
    TableStyle DoubleLineStyle = { "═", "║", { "╔", "╦", "╗" }, { "╠", "╬", "╣" }, { "╚", "╩", "╝" }};


    /// No visible table outlines - works on all systems
    TableStyle InvisibleStyle = { " ", " ", { " ", " ", " " }, { " ", " ", " " }, { " ", " ", " " }};


    /// Current table style
    TableStyle style = BasicStyle;


    /// Space character constant
    const std::string SPACE_CHARACTER = " ";


    /// The distance between the cell text and the cell border
    unsigned int padding = 1;

    /// Returns a formatted horizontal separation line for the table
    /// \param rowType The type of the row (top, middle, bottom)
    /// \return The formatted row string
    std::string getLine(const RowType& rowType, bool linebreak = true) const;


    /// Returns a formatted header string
    /// \param headers The Headers-object that holds the header strings
    /// \return The formatted header string
    std::string getHeaders(Headers headers, bool linebreak = true) const;


    /// Returns a formatted footer string
    /// \param footers The Footers-object that holds the footer strings
    /// \return The formatted footer string
    std::string getFooters(Footers footers, bool linebreak = true) const;


    /// Returns a formatted row string
    /// \param rows The Rows-object that holds all rows of the table
    /// \return A formatted string of all rows in the table
    std::string getRows(const Rows& rows) const;
    std::string getRow(const std::vector<std::string>& row, bool linebreak = true) const;


    /// Writes the entire table with all its contents in the output stream
    /// This can be used to display the table using the std::count function
    /// \param out The output stream the table should be written to
    /// \param consoleTable The ConsoleTable-object
    /// \return Output stream with the formatted table string
    friend std::ostream& operator <<(std::ostream& out, const ConsoleTable& consoleTable);
};


/// Repeats a given string n times
/// \param other The string to repeat
/// \param repeats The amount the string should be repeated
/// \return The repeated string
std::string operator *(const std::string& other, int repeats);

#endif // __CONSOLE_TABLE_H__