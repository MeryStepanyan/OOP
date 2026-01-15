#pragma once
#include <stdexcept>
#include <string>
#include <sstream>

/**
 * Unified Error Handling System
 *
 * SOLID Principles Applied:
 * - SRP: Each exception has a single, clear purpose
 * - LSP: All custom exceptions can substitute std::exception
 * - OCP: Easy to add new exception types
 */

 // ============================================================================
 // BASE EXCEPTIONS
 // ============================================================================

 /**
  * Base exception for all application errors
  */
class ApplicationException : public std::runtime_error {
protected:
    std::string m_context;

public:
    explicit ApplicationException(const std::string& message,
        const std::string& context = "")
        : std::runtime_error(message), m_context(context) {}

    std::string getContext() const { return m_context; }

    virtual std::string getFullMessage() const {
        if (m_context.empty()) {
            return what();
        }
        return std::string(what()) + " [Context: " + m_context + "]";
    }
};

// ============================================================================
// COMMAND EXCEPTIONS
// ============================================================================

/**
 * Exception for command-related errors
 */
class CommandException : public ApplicationException {
public:
    explicit CommandException(const std::string& message,
        const std::string& commandName = "")
        : ApplicationException("Command Error: " + message, commandName) {
    }
};

/**
 * Exception when command is not found
 */
class CommandNotFoundException : public CommandException {
public:
    explicit CommandNotFoundException(const std::string& commandName)
        : CommandException("Command not found: " + commandName, commandName) {
    }
};

/**
 * Exception for invalid command arguments
 */
class InvalidArgumentException : public CommandException {
private:
    std::string m_argumentName;
    std::string m_providedValue;

public:
    InvalidArgumentException(const std::string& argName,
        const std::string& value,
        const std::string& reason)
        : CommandException("Invalid argument '" + argName + "': " + reason),
        m_argumentName(argName),
        m_providedValue(value) {
    }

    std::string getArgumentName() const { return m_argumentName; }
    std::string getProvidedValue() const { return m_providedValue; }
};

/**
 * Exception for missing required arguments
 */
class MissingArgumentException : public CommandException {
private:
    std::string m_argumentName;

public:
    explicit MissingArgumentException(const std::string& argName)
        : CommandException("Missing required argument: " + argName),
        m_argumentName(argName) {
    }

    std::string getArgumentName() const { return m_argumentName; }
};

// ============================================================================
// PRESENTATION EXCEPTIONS
// ============================================================================

/**
 * Exception for presentation-related errors
 */
class PresentationException : public ApplicationException {
public:
    explicit PresentationException(const std::string& message)
        : ApplicationException("Presentation Error: " + message) {
    }
};

/**
 * Exception for slide operations
 */
class SlideException : public PresentationException {
private:
    size_t m_slideIndex;
    bool m_hasIndex;

public:
    explicit SlideException(const std::string& message)
        : PresentationException(message), m_slideIndex(0), m_hasIndex(false) {
    }

    SlideException(const std::string& message, size_t slideIndex)
        : PresentationException(message + " (Slide index: " + std::to_string(slideIndex) + ")"),
        m_slideIndex(slideIndex),
        m_hasIndex(true) {
    }

    bool hasSlideIndex() const { return m_hasIndex; }
    size_t getSlideIndex() const { return m_slideIndex; }
};

/**
 * Exception for shape operations
 */
class ShapeException : public PresentationException {
private:
    size_t m_slideIndex;
    size_t m_shapeIndex;
    bool m_hasIndices;

public:
    explicit ShapeException(const std::string& message)
        : PresentationException(message),
        m_slideIndex(0), m_shapeIndex(0), m_hasIndices(false) {
    }

    ShapeException(const std::string& message, size_t slideIdx, size_t shapeIdx)
        : PresentationException(message + " (Slide: " + std::to_string(slideIdx) +
            ", Shape: " + std::to_string(shapeIdx) + ")"),
        m_slideIndex(slideIdx),
        m_shapeIndex(shapeIdx),
        m_hasIndices(true) {
    }

    bool hasIndices() const { return m_hasIndices; }
    size_t getSlideIndex() const { return m_slideIndex; }
    size_t getShapeIndex() const { return m_shapeIndex; }
};

// ============================================================================
// FILE I/O EXCEPTIONS
// ============================================================================

/**
 * Exception for file operations
 */
class FileException : public ApplicationException {
private:
    std::string m_filename;

public:
    FileException(const std::string& message, const std::string& filename)
        : ApplicationException("File Error: " + message, filename),
        m_filename(filename) {
    }

    std::string getFilename() const { return m_filename; }
};

/**
 * Exception when file cannot be opened
 */
class FileOpenException : public FileException {
public:
    FileOpenException(const std::string& filename, bool forReading)
        : FileException(
            "Cannot open file for " + std::string(forReading ? "reading" : "writing"),
            filename) {
    }
};

/**
 * Exception for serialization errors
 */
class SerializationException : public ApplicationException {
public:
    explicit SerializationException(const std::string& message)
        : ApplicationException("Serialization Error: " + message) {
    }
};

// ============================================================================
// VALIDATION EXCEPTIONS
// ============================================================================

/**
 * Exception for validation errors
 */
class ValidationException : public ApplicationException {
private:
    std::string m_fieldName;

public:
    ValidationException(const std::string& fieldName, const std::string& message)
        : ApplicationException("Validation Error for '" + fieldName + "': " + message),
        m_fieldName(fieldName) {
    }

    std::string getFieldName() const { return m_fieldName; }
};

/**
 * Exception for out-of-range values
 */
class RangeException : public ValidationException {
private:
    size_t m_providedValue;
    size_t m_minValue;
    size_t m_maxValue;

public:
    RangeException(const std::string& fieldName,
        size_t provided,
        size_t minVal,
        size_t maxVal)
        : ValidationException(fieldName,
            "Value " + std::to_string(provided) + " is out of range [" +
            std::to_string(minVal) + ", " + std::to_string(maxVal) + "]"),
        m_providedValue(provided),
        m_minValue(minVal),
        m_maxValue(maxVal) {
    }

    size_t getProvidedValue() const { return m_providedValue; }
    size_t getMinValue() const { return m_minValue; }
    size_t getMaxValue() const { return m_maxValue; }
};

// ============================================================================
// ERROR HANDLING UTILITIES
// ============================================================================

/**
 * Result wrapper for operations that may fail
 * Alternative to exceptions for expected failures
 */
template<typename T>
class Result {
private:
    bool m_success;
    T m_value;
    std::string m_error;

public:
    // Success constructor
    static Result<T> Success(T value) {
        return Result(true, std::move(value), "");
    }

    // Failure constructor
    static Result<T> Failure(const std::string& error) {
        return Result(false, T(), error);
    }

    bool isSuccess() const { return m_success; }
    bool isFailure() const { return !m_success; }

    const T& getValue() const {
        if (!m_success) {
            throw std::logic_error("Cannot get value from failed Result");
        }
        return m_value;
    }

    T& getValue() {
        if (!m_success) {
            throw std::logic_error("Cannot get value from failed Result");
        }
        return m_value;
    }

    const std::string& getError() const { return m_error; }

    // Allow implicit conversion to bool for easy checking
    explicit operator bool() const { return m_success; }

private:
    Result(bool success, T value, const std::string& error)
        : m_success(success), m_value(std::move(value)), m_error(error) {
    }
};

/**
 * Specialization for void operations
 */
template<>
class Result<void> {
private:
    bool m_success;
    std::string m_error;

public:
    static Result<void> Success() {
        return Result(true, "");
    }

    static Result<void> Failure(const std::string& error) {
        return Result(false, error);
    }

    bool isSuccess() const { return m_success; }
    bool isFailure() const { return !m_success; }
    const std::string& getError() const { return m_error; }

    explicit operator bool() const { return m_success; }

private:
    Result(bool success, const std::string& error)
        : m_success(success), m_error(error) {
    }
};

/**
 * Error logger for consistent error reporting
 */
class ErrorLogger {
public:
    enum class Level {
        Info,
        Warning,
        Error,
        Fatal
    };

    static void log(Level level, const std::string& message,
        const std::string& context = "") {
        std::string levelStr;
        switch (level) {
        case Level::Info:    levelStr = "INFO"; break;
        case Level::Warning: levelStr = "WARN"; break;
        case Level::Error:   levelStr = "ERROR"; break;
        case Level::Fatal:   levelStr = "FATAL"; break;
        }

        std::cerr << "[" << levelStr << "] " << message;
        if (!context.empty()) {
            std::cerr << " (Context: " << context << ")";
        }
        std::cerr << std::endl;
    }

    static void logException(const ApplicationException& ex) {
        log(Level::Error, ex.what(), ex.getContext());
    }
};

// ============================================================================
// USAGE EXAMPLES
// ============================================================================

/*
// Example 1: Using custom exceptions
void someFunction() {
    throw CommandNotFoundException("addslide");
}

// Example 2: Using Result for expected failures
Result<int> divide(int a, int b) {
    if (b == 0) {
        return Result<int>::Failure("Division by zero");
    }
    return Result<int>::Success(a / b);
}

// Example 3: Using ErrorLogger
try {
    // some operation
} catch (const ApplicationException& e) {
    ErrorLogger::logException(e);
}
*/