
#pragma once
#include <memory>
#include <vector>
#include <cstddef>

class Slide;


class ISlideRepository {
public:
    virtual ~ISlideRepository() = default;
    virtual void addSlide(size_t index, std::shared_ptr<Slide> slide) = 0;
    virtual void removeSlide(size_t index) = 0;
    virtual std::shared_ptr<Slide> getSlide(size_t index) const = 0;
    virtual size_t getSlideCount() const = 0;
};

