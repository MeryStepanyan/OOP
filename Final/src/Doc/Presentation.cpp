#include "Presentation.h"
#include "Serializer.h"
#include <stdexcept>

void Presentation::addSlide(size_t index, std::shared_ptr<Slide> slide)
{
	if (!slide) {
		throw std::invalid_argument("Cannot add null slide");
	}

	if (index > slides.size()) {
		throw std::out_of_range("Index out of range");
	}

	slides.insert(slides.begin() + index, slide);
}

void Presentation::removeSlide(size_t index)
{
	if (index >= slides.size()) {
		throw std::out_of_range("Index out of range");
	}

	slides.erase(slides.begin() + index);
}

std::shared_ptr<Slide> Presentation::getSlide(size_t index) const
{
	if (index >= slides.size()) {
		throw std::out_of_range("Index out of range");
	}

	return slides[index];
}

const std::vector<std::shared_ptr<Slide>>& Presentation::getSlides() const
{
	return slides;
}

size_t Presentation::getSlideCount() const
{
	return slides.size();
}

// Implement ISerializableRepository interface
std::string Presentation::serialize() const
{
	return Serializer::serialize(*this);
}

void Presentation::deserialize(const std::string& json)
{
	Serializer::deserialize(*this, json);
}