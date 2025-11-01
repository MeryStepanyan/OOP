#pragma once
#include "Slide.h"
#include "ISerializableRepository.h"
#include <vector>
#include <memory>

class Presentation : public ISerializableRepository
{
public:
	Presentation() = default;
	~Presentation() override = default;


	void addSlide(size_t index, std::shared_ptr<Slide> slide) override;
	void removeSlide(size_t index) override;
	std::shared_ptr<Slide> getSlide(size_t index) const override;
	size_t getSlideCount() const override;
	

	std::string serialize() const override;
	void deserialize(const std::string& json) override;
	
	const std::vector<std::shared_ptr<Slide>>& getSlides() const;

private:
	std::vector<std::shared_ptr<Slide>> slides;
};