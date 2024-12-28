#pragma once

#include <vector>

class Resource
{
    std::vector<char> buffer;
public:
    Resource(){}
    Resource(std::vector<char> initBuffer) : buffer(initBuffer) {}
    Resource(std::vector<char>& initBuffer) : buffer(initBuffer) {}
    
    std::vector<char> GetBuffer() const {return buffer;}
    void SetBuffer(std::vector<char> &newBuffer) {buffer = newBuffer;}

};