#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include <stdlib.h>

using namespace std;

class Config
{
public:
    Config(const char* fileName)
    {
        m_fileName.append(fileName);
        loadFile(fileName);
    };
    ~Config(){};

    string getConfig(const char* key)
    {
        auto it = m_map.find(key);
        if (it != m_map.end())
        {
            return it->second;
        }
        return string("");
    }

    int getInteger(const char* key)
    {
        string value = getConfig(key);
        if (value.size() == 0)
        {
            return -1;
        }
        return atoi(value.c_str());
    }

    void print()
    {
        for (auto &kv: m_map)
        {
            cout << kv.first << " = " << kv.second << endl;
        }
    }

private:
    void loadFile(const char* fileName)
    {
        ifstream fp(fileName);
        string s;
        while (!fp.eof())
        {
            getline(fp, s);
            bool readChar = false;
            int start = 0, end = 0;
            string key;
            for (int i = 0; i < s.size(); ++i)
            {
                if (s[i] == ' ')
                {
                    readChar = false;
                    continue;
                }
                if (s[i] == '#')
                {
                    break;
                }
                if (s[i] == '=')
                {
                    if (end > start) 
                    {
                        key = s.substr(start, end - (start - 1));    
                    }
                    readChar = false;
                    start = end = 0;
                }
                if (!readChar) start = i;
                end = i;
                readChar = true;
            }
            if (end > start)
            {
                m_map.insert(make_pair(key, s.substr(start, end - (start - 1))));
            }
        }
    };
    void reloadFile()
    {
        loadFile(m_fileName.c_str());
    };

    string m_fileName;
    map<string, string> m_map;
};

#endif
