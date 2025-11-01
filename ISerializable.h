
#pragma once
#include <string>
#include <memory>


class ISerializable {
public:
    virtual ~ISerializable() = default;

    virtual std::string serialize() const = 0;
    
    virtual void deserialize(const std::string& json) = 0;
};

