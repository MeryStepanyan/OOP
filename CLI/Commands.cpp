//Commands.cpp
#include "Commands.h"
#include "ISlideRepository.h"
#include "ISerializableRepository.h"
#include "Presentation.h"
#include "Slide.h"
#include "Serializer.h"
#include "ShapeFactory.h"
#include "IShape.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>

// ============ AddSlideCommand ============

size_t AddSlideCommand::extractPosition(size_t defaultPos, const ISlideRepository& repository) const {
    if (!hasArgument("pos")) {
        return defaultPos;
    }
    
    size_t position = parseSizeTArgument("pos");
    size_t maxPosition = repository.getSlideCount(); // Can insert at end (size), so max is size
    
    if (position > maxPosition) {
        throw std::out_of_range("Position " + std::to_string(position) + 
                                " is out of range. Valid positions: 0 to " + 
                                std::to_string(maxPosition) + " (current slides: " + 
                                std::to_string(repository.getSlideCount()) + ")");
    }
    
    return position;
}

void AddSlideCommand::execute(ISlideRepository& repository) {
    validateAllowedArguments({"pos"});
    
    size_t position = extractPosition(repository.getSlideCount(), repository);
    
    auto slide = std::make_shared<Slide>();
    repository.addSlide(position, slide);
    
    std::cout << "Slide added at position " << position << std::endl;
    std::cout << "Total slides: " << repository.getSlideCount() << std::endl;
}

void AddSlideCommand::undo(ISlideRepository& repository) {
    // This command doesn't support undo
    // Implementation empty by design
}

// ============ RemoveSlideCommand ============

void RemoveSlideCommand::ensureSlidesExist(const ISlideRepository& repository) const {
    if (repository.getSlideCount() == 0) {
        throw std::runtime_error("Cannot remove slide: repository is empty");
    }
}

size_t RemoveSlideCommand::extractPosition(size_t defaultPos, const ISlideRepository& repository) const {
    if (!hasArgument("at")) {
        return defaultPos;
    }
    
    size_t position = parseSizeTArgument("at");
    
    if (position >= repository.getSlideCount()) {
        throw std::out_of_range("Slide index " + std::to_string(position) + 
                               " out of range (max: " + 
                               std::to_string(repository.getSlideCount() - 1) + ")");
    }
    
    return position;
}

void RemoveSlideCommand::execute(ISlideRepository& repository) {
    ensureSlidesExist(repository);
    validateAllowedArguments({"at"});
    
    size_t position = extractPosition(repository.getSlideCount() - 1, repository);
    
    removedSlide = repository.getSlide(position);
    removedPosition = position;
    repository.removeSlide(position);
    executed = true;
    
    std::cout << "Slide removed from position " << position << std::endl;
    std::cout << "Remaining slides: " << repository.getSlideCount() << std::endl;
}

void RemoveSlideCommand::undo(ISlideRepository& repository) {
    if (!executed || !removedSlide) {
        return;
    }
    
    repository.addSlide(removedPosition, removedSlide);
    executed = false;
    removedSlide.reset();
    
    std::cout << "Undo: Slide restored at position " << removedPosition << std::endl;
}

// ============ SaveCommand ============

std::string SaveCommand::extractFilename() const {
    if (!hasArgument("file")) {
        return "presentation.json"; // Default filename
    }
    return getArgumentValue("file");
}

void SaveCommand::execute(ISlideRepository& repository) {
    validateAllowedArguments({"file"});
    
    std::string filename = extractFilename();
    
    // Use interface instead of dynamic_cast - follows DIP
    ISerializableRepository* serializableRepo = dynamic_cast<ISerializableRepository*>(&repository);
    if (!serializableRepo) {
        throw std::runtime_error("Save command requires ISerializableRepository type");
    }
    
    if (Serializer::saveToFile(*serializableRepo, filename)) {
        std::cout << "Presentation saved successfully to: " << filename << std::endl;
    } else {
        throw std::runtime_error("Failed to save presentation");
    }
}

void SaveCommand::undo(ISlideRepository& repository) {
    // Save operation cannot be undone
}

// ============ LoadCommand ============

std::string LoadCommand::extractFilename() const {
    validateRequiredArgument("file");
    return getArgumentValue("file");
}

void LoadCommand::execute(ISlideRepository& repository) {
    validateAllowedArguments({"file"});
    
    std::string filename = extractFilename();
    
    // Use interface instead of dynamic_cast - follows DIP
    ISerializableRepository* serializableRepo = dynamic_cast<ISerializableRepository*>(&repository);
    if (!serializableRepo) {
        throw std::runtime_error("Load command requires ISerializableRepository type");
    }
    
    if (Serializer::loadFromFile(*serializableRepo, filename)) {
        std::cout << "Presentation loaded successfully from: " << filename << std::endl;
        std::cout << "Total slides: " << serializableRepo->getSlideCount() << std::endl;
        
        // Display shape information for each slide
        for (size_t i = 0; i < serializableRepo->getSlideCount(); ++i) {
            auto slide = serializableRepo->getSlide(i);
            std::cout << "  Slide " << i << ": " << slide->getShapeCount() << " shapes" << std::endl;
        }
    } else {
        throw std::runtime_error("Failed to load presentation");
    }
}

void LoadCommand::undo(ISlideRepository& repository) {
    // Load operation cannot be undone
}

// ============ AddShapeCommand ============

size_t AddShapeCommand::extractSlideIndex(const ISlideRepository& repository) const {
    validateRequiredArgument("slide");
    
    size_t slideCount = repository.getSlideCount();
    if (slideCount == 0) {
        throw std::out_of_range("Cannot add shape: presentation has no slides. Add a slide first.");
    }
    
    size_t slideIndex = parseSizeTArgument("slide");
    
    if (slideIndex >= slideCount) {
        throw std::out_of_range("Slide index " + std::to_string(slideIndex) + 
                               " out of range (max: " + 
                               std::to_string(slideCount - 1) + ")");
    }
    
    return slideIndex;
}

std::string AddShapeCommand::extractShapeType() const {
    validateRequiredArgument("type");
    std::string type = getArgumentValue("type");
    
    // Convert to lowercase for case-insensitive matching
    std::string lowerType = type;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
    
    if (lowerType != "circle" && lowerType != "rectangle" && lowerType != "text") {
        throw std::invalid_argument("Invalid shape type: " + type + 
                                   ". Valid types: Circle, Rectangle, Text");
    }
    
    // Return with first letter capitalized
    type[0] = std::toupper(type[0]);
    for (size_t i = 1; i < type.length(); ++i) {
        type[i] = std::tolower(type[i]);
    }
    
    return type;
}

float AddShapeCommand::parseFloatArg(const std::string& key, float defaultValue) const {
    if (!hasArgument(key)) {
        return defaultValue;
    }
    
    try {
        return std::stof(getArgumentValue(key));
    } catch (const std::exception&) {
        throw std::invalid_argument("Argument '" + key + "' must be a valid number");
    }
}

void AddShapeCommand::execute(ISlideRepository& repository) {
    // Validate arguments based on shape type
    std::vector<std::string> allowedArgs = {"slide", "type", "x", "y"};
    
    std::string shapeType = extractShapeType();
    
    // Add type-specific arguments
    if (shapeType == "Circle") {
        allowedArgs.push_back("radius");
    } else if (shapeType == "Rectangle") {
        allowedArgs.push_back("width");
        allowedArgs.push_back("height");
    } else if (shapeType == "Text") {
        allowedArgs.push_back("content");
        allowedArgs.push_back("fontSize");
    }
    
    validateAllowedArguments(allowedArgs);
    
    size_t slideIndex = extractSlideIndex(repository);
    auto slide = repository.getSlide(slideIndex);
    
    float x = parseFloatArg("x", 0.0f);
    float y = parseFloatArg("y", 0.0f);
    
    std::shared_ptr<IShape> shape;
    
    if (shapeType == "Circle") {
        float radius = parseFloatArg("radius", 10.0f);
        shape = ShapeFactory::createCircle(radius, x, y);
    } else if (shapeType == "Rectangle") {
        float width = parseFloatArg("width", 10.0f);
        float height = parseFloatArg("height", 10.0f);
        shape = ShapeFactory::createRectangle(width, height, x, y);
    } else if (shapeType == "Text") {
        std::string content = hasArgument("content") ? getArgumentValue("content") : "";
        float fontSize = parseFloatArg("fontSize", 12.0f);
        shape = ShapeFactory::createText(content, fontSize, x, y);
    }
    
    if (shape) {
        slide->addShape(shape);
        std::cout << shapeType << " added to slide " << slideIndex << std::endl;
        std::cout << "Slide " << slideIndex << " now has " << slide->getShapeCount() << " shapes" << std::endl;
    }
}

void AddShapeCommand::undo(ISlideRepository& repository) {
    // AddShape doesn't support undo yet
}

// ============ RemoveShapeCommand ============

size_t RemoveShapeCommand::extractSlideIndex(const ISlideRepository& repository) const {
    validateRequiredArgument("slide");
    
    size_t slideCount = repository.getSlideCount();
    if (slideCount == 0) {
        throw std::out_of_range("Cannot remove shape: presentation has no slides.");
    }
    
    size_t slideIndex = parseSizeTArgument("slide");
    
    if (slideIndex >= slideCount) {
        throw std::out_of_range("Slide index " + std::to_string(slideIndex) + 
                               " out of range (max: " + 
                               std::to_string(slideCount - 1) + ")");
    }
    
    return slideIndex;
}

size_t RemoveShapeCommand::extractShapeIndex(const ISlideRepository& repository, size_t slideIndex) const {
    validateRequiredArgument("index");
    size_t shapeIndex = parseSizeTArgument("index");
    
    auto slide = repository.getSlide(slideIndex);
    if (shapeIndex >= slide->getShapeCount()) {
        throw std::out_of_range("Shape index " + std::to_string(shapeIndex) + 
                               " out of range (max: " + 
                               std::to_string(slide->getShapeCount() - 1) + ")");
    }
    
    return shapeIndex;
}

void RemoveShapeCommand::execute(ISlideRepository& repository) {
    validateAllowedArguments({"slide", "index"});
    
    size_t slideIndex = extractSlideIndex(repository);
    
    auto slide = repository.getSlide(slideIndex);
    if (slide->getShapeCount() == 0) {
        throw std::runtime_error("Cannot remove shape: slide " + 
                                std::to_string(slideIndex) + " has no shapes");
    }
    
    size_t shapeIndex = extractShapeIndex(repository, slideIndex);
    
    // Save for undo - need to get the shape before removing
    targetSlide = slide;
    removedShapeIndex = shapeIndex;
    
    // Get the shape from slide's internal storage before removing
    const auto& shapes = slide->getShapes();
    if (shapeIndex < shapes.size()) {
        removedShape = shapes[shapeIndex]; // Get shared_ptr from vector
    }
    
    slide->removeShape(shapeIndex);
    executed = true;
    
    std::cout << "Shape removed from slide " << slideIndex << " at index " << shapeIndex << std::endl;
    std::cout << "Slide " << slideIndex << " now has " << slide->getShapeCount() << " shapes" << std::endl;
}

void RemoveShapeCommand::undo(ISlideRepository& repository) {
    if (!executed || targetSlide == nullptr || removedShape == nullptr) {
        return;
    }
    
    // Restore the shape at the same index
    targetSlide->addShape(removedShape);
    executed = false;
    targetSlide.reset();
    removedShape.reset();
    
    std::cout << "Undo: Shape restored" << std::endl;
}
