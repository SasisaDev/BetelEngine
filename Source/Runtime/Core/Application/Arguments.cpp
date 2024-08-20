#include "Arguments.h"


ArgumentCollection ArgumentParser::ParseArgs(int argc, char* argv[])
{
    ArgumentCollection collection;

    std::string key, value = "";
    bool processingValue = false;
    bool processingStringValue = false;

    for(int argNum = 0; argNum < argc; ++argNum)
    {
        size_t argLength = strlen(argv[argNum]);
        char curChar;
        for(int chari = 0; chari < argLength; ++chari)
        {
            curChar = argv[argNum][chari];

            if(curChar == '=')
            {
                // Handle as Value Start
                // --Key=Value
                processingValue = true;

                if((argv[argNum][chari+1]) == '"')
                {
                    // Value String Start
                    // --Key="String Value"
                    processingStringValue = true;
                    ++chari;
                }
            } 
            else if(curChar == '"')
            {
                // Value's String End
                processingStringValue = false;
            }
            else
            {
                // Key/Value Entry
                if(chari < 2 && curChar == '-')
                {
                    continue;
                }

                if(processingValue)
                {
                    value += curChar;
                }
                else
                {
                    key += curChar;
                }
            }
        }

        if(processingStringValue)
        {
            value += ' ';
        }
        else
        {
            collection.Push({key, value});

            key = value = "";
            processingValue = false;
        }
    }

    return collection;
}