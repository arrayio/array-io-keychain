#ifndef POLLING_H
#define POLLING_H

#include <QObject>
#include <QString>

namespace json_parser
{
    static const char LBRACE = '{';
    static const char RBRACE = '}';
    static const char SPACE = ' ';
    static const char LF = '\n';
    static const char CR = '\r';
    static const char TAB = '\t';
    static const char VTAB = '\v';
    static const char NP = '\f';
}

using buf_type = std::vector<char>;
using buf_iterator = buf_type::iterator;
using iter_range = std::pair<buf_iterator, buf_iterator>;

class Polling : public QObject
{
    Q_OBJECT
public:
    Polling() : {
        buf.reserve(1024);
        end = buf.begin();
        pbuf = buf.data();
        remain = buf.size();
    }

    buf_type buf;
    buf_iterator end ;
    buf_type::value_type * pbuf;
    size_t remain;

public slots:
    void Select();
signals:
    void rx(const std::string);
    void poll();
    void err();
private:
    iter_range cut_json_obj(buf_iterator, buf_iterator);
};

#endif // POLLING_H
