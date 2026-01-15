
#pragma once
#include "ISlideRepository.h"
#include <string>


class ISerializableRepository : public ISlideRepository {
public:
    virtual ~ISerializableRepository() = default;

    virtual std::string serialize() const = 0;

    virtual void deserialize(const std::string& json) = 0;
};

