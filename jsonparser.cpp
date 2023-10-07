#include "jsonparser.h"
#include <iostream>
#include <sstream>
#include <cctype>

JSONValue JSONParser::parse(const std::string& jsonString) {
    std::istringstream iss(jsonString);
    return parseValue(iss);
}

JSONValue JSONParser::parseValue(std::istringstream& iss) {
    skipWhitespace(iss);

    char ch;
    if (!(iss >> ch)) {
        // End of input
        return JSONValue();
    }

    if (ch == '{') {
        return parseObject(iss);
    } else if (ch == '[') {
        return parseArray(iss);
    } else if (ch == '"') {
        return parseString(iss);
    } else if (ch == '-' || (ch >= '0' && ch <= '9')) {
        return parseNumber(iss, ch);
    } else if (ch == 't' || ch == 'f') {
        return parseBoolean(iss, ch);
    } else if (ch == 'n') {
        return parseNull(iss);
    }

    // Invalid value
    return JSONValue();
}

JSONValue JSONParser::parseObject(std::istringstream& iss) {
    JSONValue objectValue;
    objectValue.type = JSON_Object;

    while (true) {
        skipWhitespace(iss);
        char ch;
        if (!(iss >> ch)) {
            // Unexpected end of input
            break;
        }

        if (ch == '}') {
            // End of object
            break;
        }

        if (ch != '"') {
            // Invalid key
            break;
        }

        std::string key = parseString(iss).getString();
        if (key.empty()) {
            // Invalid key
            break;
        }

        skipWhitespace(iss);
        if (!(iss >> ch)) {
            // Unexpected end of input
            break;
        }

        if (ch != ':') {
            // Invalid key-value separator
            break;
        }

        JSONValue value = parseValue(iss);
        objectValue.objectValue.push_back(std::make_pair(key, value));

        skipWhitespace(iss);
        if (!(iss >> ch)) {
            // Unexpected end of input
            break;
        }

        if (ch == '}') {
            // End of object
            break;
        }

        if (ch != ',') {
            // Invalid object delimiter
            break;
        }
    }

    return objectValue;
}

JSONValue JSONParser::parseArray(std::istringstream& iss) {
    JSONValue arrayValue;
    arrayValue.type = JSON_Array;

    while (true) {
        skipWhitespace(iss);
        char ch;
        if (!(iss >> ch)) {
            // Unexpected end of input
            break;
        }

        if (ch == ']') {
            // End of array
            break;
        }

        iss.unget();
        JSONValue value = parseValue(iss);
        arrayValue.arrayValue.push_back(value);

        skipWhitespace(iss);
        if (!(iss >> ch)) {
            // Unexpected end of input
            break;
        }

        if (ch == ']') {
            // End of array
            break;
        }

        if (ch != ',') {
            // Invalid array delimiter
            break;
        }
    }

    return arrayValue;
}

JSONValue JSONParser::parseString(std::istringstream& iss) {
    JSONValue stringValue;
    stringValue.type = JSON_String;

    std::getline(iss, stringValue.stringValue, '"');
    return stringValue;
}

JSONValue JSONParser::parseNumber(std::istringstream& iss, char ch) {
    JSONValue numberValue;
    numberValue.type = JSON_Number;

    std::string numberString;
    numberString += ch;

    while (iss.get(ch)) {
        if ((ch >= '0' && ch <= '9') || ch == '.' || ch == 'e' || ch == 'E' || ch == '+' || ch == '-') {
            numberString += ch;
        } else {
            iss.unget();
            break;
        }
    }

    std::istringstream numberStream(numberString);
    numberStream >> numberValue.numberValue;
    return numberValue;
}

JSONValue JSONParser::parseBoolean(std::istringstream& iss, char ch) {
    JSONValue booleanValue;
    booleanValue.type = JSON_Boolean;

    std::string boolString;
    boolString += ch;

    while (iss.get(ch)) {
        if (ch >= 'a' && ch <= 'z') {
            boolString += ch;
        } else {
            iss.unget();
            break;
        }
    }

    booleanValue.booleanValue = (boolString == "true");
    return booleanValue;
}

JSONValue JSONParser::parseNull(std::istringstream& iss) {
    JSONValue nullValue;
    nullValue.type = JSON_Null;
    std::string nullString;
    nullString.resize(4);
    if (iss.read(&nullString[0], 4) && nullString == "null") {
        return nullValue;
    }

    // Invalid value
    return JSONValue();
}

void JSONParser::skipWhitespace(std::istringstream& iss) {
    while (isspace(iss.peek())) {
        iss.ignore();
    }
}


void JSONParser::loadList(){
    JSONParser parser;
    QByteArray linesFromFile;
    QFile file("test.json");
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            linesFromFile = file.readAll();
        }
    } else {
        std::cout << "File not found!";
    }
    QString response(linesFromFile);


    std::string jsonString = response.toStdString();
    JSONValue json = parser.parse(jsonString);

    if (!json.isNull()) {
        printJSON(json);
    } else {
        std::cout << "Invalid JSON structure." << std::endl;
    }
}

void printJSON(const JSONValue& json, const std::string& indent) {
    if (json.isObject()) {
        const std::vector<std::pair<std::string, JSONValue> >& object = json.getObject();
        for (std::vector<std::pair<std::string, JSONValue> >::const_iterator it = object.begin(); it != object.end(); ++it) {
            const std::string& key = it->first;
            const JSONValue& value = it->second;


            std::cout << indent << "Key: " << key << std::endl;
            std::cout << indent << "Value: ";

            if (value.isString()) {
                std::cout << value.getString() << std::endl;
            } else if (value.isNumber()) {
                std::cout << value.getNumber() << std::endl;
            } else if (value.isBoolean()) {
                std::cout << (value.getBoolean() ? "true" : "false") << std::endl;
            } else if(value.isNull()) {
                std::cout << "null" << std::endl;
            }else if (value.isObject() || value.isArray()) {
                std::cout << std::endl;
                printJSON(value, indent + "  ");
            }
        }
    } else if (json.isArray()) {
        const std::vector<JSONValue>& array = json.getArray();
        for (std::vector<JSONValue>::size_type i = 0; i < array.size(); ++i) {
            const JSONValue& value = array[i];

            std::cout << indent << "Index: " << i << std::endl;
            std::cout << indent << "Value: ";

            if (value.isString()) {
                std::cout << value.getString() << std::endl;
            } else if (value.isNumber()) {
                std::cout << value.getNumber() << std::endl;
            } else if (value.isBoolean()) {
                std::cout << (value.getBoolean() ? "true" : "false") << std::endl;
            } else if(value.isNull()) {
                std::cout << "null" << std::endl;
            } else if (value.isObject() || value.isArray()) {
                std::cout << std::endl;
                printJSON(value, indent + "  ");
            }
        }
    }
}
