#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QObject>
#include <QEventLoop>
#include <QFile>
enum JSONValueType {
    JSON_Null,
    JSON_Object,
    JSON_Array,
    JSON_String,
    JSON_Number,
    JSON_Boolean
};

class JSONValue {
public:
    JSONValue() : type(JSON_Null) {}

    JSONValueType getType() const {
        return type;
    }

    bool isNull() const {
        return type == JSON_Null;
    }

    bool isObject() const {
        return type == JSON_Object;
    }

    bool isArray() const {
        return type == JSON_Array;
    }

    bool isString() const {
        return type == JSON_String;
    }

    bool isNumber() const {
        return type == JSON_Number;
    }

    bool isBoolean() const {
        return type == JSON_Boolean;
    }

    const std::vector<std::pair<std::string, JSONValue> >& getObject() const {
        return objectValue;
    }

    const std::vector<JSONValue>& getArray() const {
        return arrayValue;
    }

    const std::string& getString() const {
        return stringValue;
    }

    double getNumber() const {
        return numberValue;
    }

    bool getBoolean() const {
        return booleanValue;
    }

private:
    JSONValueType type;
    std::vector<std::pair<std::string, JSONValue> > objectValue;
    std::vector<JSONValue> arrayValue;
    std::string stringValue;
    double numberValue;
    bool booleanValue;

    friend class JSONParser;
};

class JSONParser : public QObject  {
    Q_OBJECT
public:
    JSONValue parse(const std::string& jsonString);
    Q_INVOKABLE void loadList();

private:
    JSONValue parseValue(std::istringstream& iss);
    JSONValue parseObject(std::istringstream& iss);
    JSONValue parseArray(std::istringstream& iss);
    JSONValue parseString(std::istringstream& iss);
    JSONValue parseNumber(std::istringstream& iss, char ch);
    JSONValue parseBoolean(std::istringstream& iss, char ch);
    JSONValue parseNull(std::istringstream& iss);
    void filterVidData(const JSONValue& json, const std::string& indent = "");

    void skipWhitespace(std::istringstream& iss);
};

JSONValue loadList(const std::string& jsonString);

void printJSON(const JSONValue& json, const std::string& indent = "");

#endif // JSONPARSER_H


