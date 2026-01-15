#pragma once
#pragma once
#include "IAction.h"
#include "AddSlideAction.h"
#include "RemoveSlideAction.h"
#include "AddShapeAction.h"
#include "RemoveShapeAction.h"
#include <memory>
#include <functional>
#include <map>
#include <stdexcept>

/**
 * ActionFactory - Creates Action objects using Factory Pattern
 *
 * SOLID Principles Applied:
 * - SRP: Single responsibility - creating actions
 * - OCP: Open for extension (register new actions) without modification
 * - DIP: Depends on IAction abstraction
 */
class ActionFactory {
public:
    // Action creator function type
    using ActionCreator = std::function<std::unique_ptr<IAction>()>;

    // Get singleton instance
    static ActionFactory& instance() {
        static ActionFactory factory;
        return factory;
    }

    // Register a new action type
    void registerAction(const std::string& actionType, ActionCreator creator) {
        creators[actionType] = creator;
    }

    // Factory methods for specific actions
    static std::unique_ptr<IAction> createAddSlideAction(
        std::shared_ptr<Slide> slide,
        size_t position)
    {
        if (!slide) {
            throw std::invalid_argument("Cannot create AddSlideAction with null slide");
        }
        return std::make_unique<AddSlideAction>(slide, position);
    }

    static std::unique_ptr<IAction> createRemoveSlideAction(size_t position) {
        return std::make_unique<RemoveSlideAction>(position);
    }

    static std::unique_ptr<IAction> createAddShapeAction(
        size_t slideIndex,
        const StyledShape& styledShape)  
    {
        if (!styledShape.isValid()) {
            throw std::invalid_argument("Cannot create AddShapeAction with invalid styled shape");
        }
        return std::make_unique<AddShapeAction>(slideIndex, styledShape);
    }

    static std::unique_ptr<IAction> createRemoveShapeAction(
        size_t slideIndex,
        size_t shapeIndex)
    {
        return std::make_unique<RemoveShapeAction>(slideIndex, shapeIndex);
    }

    // Generic creation by name (if needed for extensibility)
    std::unique_ptr<IAction> create(const std::string& actionType) {
        auto it = creators.find(actionType);
        if (it == creators.end()) {
            throw std::invalid_argument("Unknown action type: " + actionType);
        }
        return it->second();
    }

private:
    ActionFactory() = default;
    ActionFactory(const ActionFactory&) = delete;
    ActionFactory& operator=(const ActionFactory&) = delete;

    std::map<std::string, ActionCreator> creators;
};