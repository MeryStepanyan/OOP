
#pragma once
#include "BaseCommand.h"
#include <memory>
#include <vector>
#include <string>


class ISlideRepository;
class Slide;
class IShape;

class AddSlideCommand : public BaseCommand {
private:
    size_t extractPosition(size_t defaultPos, const ISlideRepository& repository) const;
    
public:
    AddSlideCommand() = default;
    explicit AddSlideCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return false; }
    std::string getName() const override { return "AddSlide"; }
};

class RemoveSlideCommand : public BaseCommand {
private:
    std::shared_ptr<Slide> removedSlide;
    size_t removedPosition = 0;
    bool executed = false;
    
    size_t extractPosition(size_t defaultPos, const ISlideRepository& repository) const;
    void ensureSlidesExist(const ISlideRepository& repository) const;
    
public:
    RemoveSlideCommand() = default;
    explicit RemoveSlideCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return true; }
    std::string getName() const override { return "RemoveSlide"; }
};

class SaveCommand : public BaseCommand {
private:
    std::string extractFilename() const;
    
public:
    SaveCommand() = default;
    explicit SaveCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return false; }
    std::string getName() const override { return "Save"; }
};

class LoadCommand : public BaseCommand {
private:
    std::string extractFilename() const;
    
public:
    LoadCommand() = default;
    explicit LoadCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return false; }
    std::string getName() const override { return "Load"; }
};

class AddShapeCommand : public BaseCommand {
private:
    size_t extractSlideIndex(const ISlideRepository& repository) const;
    std::string extractShapeType() const;
    float parseFloatArg(const std::string& key, float defaultValue) const;
    
public:
    AddShapeCommand() = default;
    explicit AddShapeCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return false; }
    std::string getName() const override { return "AddShape"; }
};

class RemoveShapeCommand : public BaseCommand {
private:
    size_t extractSlideIndex(const ISlideRepository& repository) const;
    size_t extractShapeIndex(const ISlideRepository& repository, size_t slideIndex) const;
    std::shared_ptr<Slide> targetSlide;
    size_t removedShapeIndex = 0;
    std::shared_ptr<IShape> removedShape;
    bool executed = false;
    
public:
    RemoveShapeCommand() = default;
    explicit RemoveShapeCommand(const std::unordered_map<std::string, std::string>& args)
        : BaseCommand(args) {
    }

    void execute(ISlideRepository& repository) override;
    void undo(ISlideRepository& repository) override;
    bool canUndo() const override { return true; }
    std::string getName() const override { return "RemoveShape"; }
};
