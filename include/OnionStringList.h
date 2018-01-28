#ifndef ONIONSTRINGLIST_H
#define ONIONSTRINGLIST_H

#include <string>
#include <vector>

class OnionStringList
{
    public:
        static int numInstances;

        OnionStringList();
        OnionStringList(std::string filename);
        OnionStringList(std::vector<std::string> stringsIn);
        ~OnionStringList();

        void load(std::string filename);

        std::string at(int index);
        #pragma warning( suppress : 4267 )
        int size() { return strings.size(); };

        void printContents();

    private:
        std::vector<std::string> strings;
};

#endif // ONIONSTRINGLIST_H
