#include "ConsoleTable.h"

#include <utility>

ConsoleTable::ConsoleTable(std::initializer_list<std::string> headers) : _headers{ headers }
{
    for (const auto& column : headers)
        _widths.push_back(column.length());
}

ConsoleTable::ConsoleTable(const std::vector<std::string>& headers) : _headers{ headers }
{
    for (const auto& column : headers)
        _widths.push_back(column.length());
}

void ConsoleTable::setPadding(unsigned int n)
{
    padding = n;
}

void ConsoleTable::setStyle(unsigned int n)
{
    switch (n)
    {
        case 0:style = BasicStyle;
            break;
        case 1:style = LineStyle;
            break;
        case 2:style = DoubleLineStyle;
            break;
        case 3:style = InvisibleStyle;
            break;
        default:style = BasicStyle;
            break;
    }
}

bool ConsoleTable::addRow(std::initializer_list<std::string> row)
{
    return addRow(std::vector<std::string>{ row });
}

bool ConsoleTable::addRow(std::vector<std::string> row)
{
    if (row.size() > _widths.size())
        throw std::invalid_argument{ "Appended row size must be same as header size" };

    _rows.push_back(row);
    for (unsigned int i = 0; i < row.size(); ++i)
        _widths[i] = std::max(row[i].size(), _widths[i]);

    return true;
}

bool ConsoleTable::removeRow(unsigned int index)
{
    if (index > _rows.size())
        return false;

    _rows.erase(_rows.begin() + index);
    return true;
}

ConsoleTable& ConsoleTable::operator +=(std::initializer_list<std::string> row)
{
    if (row.size() > _widths.size())
        throw std::invalid_argument{ "Appended row size must be same as header size" };

    addRow(row);
    return *this;
}

ConsoleTable& ConsoleTable::operator -=(const uint32_t rowIndex)
{
    if (_rows.size() < rowIndex)
        throw std::out_of_range{ "Row index out of range." };

    removeRow(rowIndex);
    return *this;
}

std::string ConsoleTable::getLine(const RowType& rowType, bool linebreak) const
{
    std::stringstream line;
    line << rowType.left;
    for (unsigned int i = 0; i < _widths.size(); ++i)
    {
        for (unsigned int j = 0; j < (_widths[i] + padding + padding); ++j)
            line << style.horizontal;
        line << (i == _widths.size() - 1 ? rowType.right : rowType.intersect);
    }
    if (linebreak)
        line << "\n";
    return line.str();
}

std::string ConsoleTable::getHeaders(Headers headers, bool linebreak) const
{
    std::stringstream line;
    line << style.vertical;
    for (unsigned int i = 0; i < headers.size(); ++i)
    {
        std::string text = headers[i];
        line << SPACE_CHARACTER * padding + text + SPACE_CHARACTER * (_widths[i] - text.length()) + SPACE_CHARACTER * padding;
        line << style.vertical;
    }
    if (linebreak)
        line << "\n";
    return line.str();
}

std::string ConsoleTable::getFooters(ConsoleTable::Footers footers, bool linebreak) const
{
    std::stringstream line;
    line << style.vertical;
    for (unsigned int i = 0; i < footers.size(); ++i)
    {
        std::string text = footers[i];
        line << SPACE_CHARACTER * padding + text + SPACE_CHARACTER * (_widths[i] - text.length()) + SPACE_CHARACTER * padding;
        line << style.vertical;
    }
    if (linebreak)
        line << "\n";
    return line.str();
}

std::string ConsoleTable::getRows(const Rows& rows) const
{
    std::stringstream line;
    for (const auto& row : rows)
        line << getRow(row);
    return line.str();
}

std::string ConsoleTable::getRow(const std::vector<std::string>& row, bool linebreak) const
{
    std::stringstream line;
    line << style.vertical;
    for (unsigned int j = 0; j < row.size(); ++j)
    {
        const std::string& text = row[j];
        line << SPACE_CHARACTER * padding + text + SPACE_CHARACTER * (_widths[j] - text.length()) + SPACE_CHARACTER * padding;
        line << style.vertical;
    }
    if (linebreak)
        line << "\n";
    return line.str();
}

std::ostream& operator <<(std::ostream& out, const ConsoleTable& consoleTable)
{
    out << consoleTable.getLine(consoleTable.style.top);
    out << consoleTable.getHeaders(consoleTable._headers);
    out << consoleTable.getLine(consoleTable.style.middle);
    out << consoleTable.getRows(consoleTable._rows);
    if (!consoleTable._footers.empty())
    {
        out << consoleTable.getLine(consoleTable.style.middle);
        out << consoleTable.getFooters(consoleTable._footers);
    }
    out << consoleTable.getLine(consoleTable.style.bottom);
    return out;
}

bool ConsoleTable::sort(bool ascending)
{
    if (ascending)
        std::sort(_rows.begin(), _rows.end(), std::less<std::vector<std::string>>());
    else
        std::sort(_rows.begin(), _rows.end(), std::greater<std::vector<std::string>>());
    return true;
}

void ConsoleTable::updateRow(unsigned int row, unsigned int header, std::string data)
{
    if (row > _rows.size() - 1)
        throw std::out_of_range{ "Row index out of range." };
    if (header > _headers.size() - 1)
        throw std::out_of_range{ "Header index out of range." };
    _rows[row][header] = data;
}

void ConsoleTable::updateHeader(unsigned int header, std::string text)
{
    if (header > _headers.size())
        throw std::out_of_range{ "Header index out of range." };
    _headers[header] = text;
}

void ConsoleTable::updateFooter(unsigned int footer, std::string text)
{
    if (footer > _footers.size())
        throw std::out_of_range{ "Footer index out of range." };
    _footers[footer] = text;
}

std::string ConsoleTable::toString(const ConsoleTable& consoleTable)
{
    std::stringstream ss;
    ss << consoleTable;
    return ss.str();
}

std::string operator *(const std::string& other, int repeats)
{
    std::string ret;
    ret.reserve(other.size() * repeats);
    for (; repeats; --repeats)
        ret.append(other);
    return ret;
}

std::vector<std::string> ConsoleTable::toList(const ConsoleTable& consoleTable, bool linebreak)
{
    std::vector<std::string> list;
    list.push_back(consoleTable.getLine(consoleTable.style.top, linebreak));
    list.push_back(consoleTable.getHeaders(consoleTable._headers, linebreak));
    list.push_back(consoleTable.getLine(consoleTable.style.middle, linebreak));
    for (auto& row : consoleTable._rows)
        list.push_back(consoleTable.getRow(row, linebreak));
    if (!consoleTable._footers.empty())
    {
        list.push_back(consoleTable.getLine(consoleTable.style.middle, linebreak));
        list.push_back(consoleTable.getFooters(consoleTable._footers, linebreak));
    }
    list.push_back(consoleTable.getLine(consoleTable.style.bottom, linebreak));
    return list;
}
