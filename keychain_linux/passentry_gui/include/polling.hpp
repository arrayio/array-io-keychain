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
using buf_it = buf_type::iterator;
using it_range = std::pair<buf_it, buf_it>;

class Polling : public QObject
{
    Q_OBJECT
public:
    Polling()  {
        buf.resize(1024);
        it = buf.begin();
    }
    buf_it it;
    buf_type buf;

public slots:
    void Select();
signals:
    void rx(const std::string);
    void poll();
    void err();
private:
    it_range cut_json_obj(buf_it, buf_it);
};

#endif // POLLING_H
