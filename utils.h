#ifndef ADS_UTILS_H
#define ADS_UTILS_H

#include <QString>
#include <QDebug>

//classes and functions markers
#define in
#define out

#define interface struct

//utility constants
#define NOT_FOUND -1
#define MINUS '-'

namespace Utils {
    //utility functions

    template<typename T>
    inline QString toStr(T value)  {
        return QString::number(value);
    }

    inline bool isCoord(QChar const &c)   {
        return c == '.' || c.isDigit();
    }

    inline QString subStr(QString const& str, int from, int to)   {
        int size = to - from + 2;
        if (size > 0)  {
            QChar* buffer = new QChar[static_cast<unsigned long long>(size)];
            std::copy(str.data() + from, str.data() + to + 1, buffer);
            buffer[size - 1] = '\0';
            return QString(buffer);
        }
        return QString();
    }

    inline int nextNotDigit(QString const& str, int offset)    {
        int i = (str[offset + 1] == MINUS) ? offset + 2 : offset + 1;
        for (; isCoord(str[i]); i++) {
            if (i >= str.size() - 1)
                return NOT_FOUND;
        }
        return i;
    }

    class InvalidFloatException : public std::exception {
        QString str;
    public:
        InvalidFloatException(QString str) : str(str) {}
        QString floatStr() const    {
            return str;
        }
    };

    inline int skipSpaces(QString const& in str, int in offset) {
        while (offset < str.size() - 1 && str[offset + 1] == ' ') {   //skip spaces
            offset++;
        }
        return offset;
    }

    template<int size>
    inline int stringToFloats(QString const& in str, int in offset, float out numbers[size]) {
        int prevDel = skipSpaces(str, offset);
        int nextDel = prevDel;

        for (int i = 0; i < size; i++)  {
            nextDel = Utils::nextNotDigit(str, prevDel);
            if (nextDel == NOT_FOUND) {
                nextDel = str.size();
            }
            QString numStr = subStr(str, prevDel + 1, nextDel - 1);

            bool result;
            numbers[i] = numStr.toFloat(&result);
            if (!result)    {
                //qDebug() << "Invalid float in " << numStr;
                throw InvalidFloatException(numStr);
            }
            if (prevDel == '-')
                numbers[i] = -numbers[i];

            prevDel = skipSpaces(str, nextDel);
        }
        return nextDel;
    }

    inline bool equals(QChar const& a, QChar const& b)    {         //not sensitive to register
        return (a.toLower() == b.toLower());
    }

};

#endif // UTILS_H
