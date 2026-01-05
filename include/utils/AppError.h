#pragma once

#include <string>

namespace utils {

/**
 * Application error class
 * Represents errors that occur in the application
 */
class AppError {
public:
    enum class ErrorCode {
        NOT_FOUND = 404,
        BAD_REQUEST = 400,
        INTERNAL_ERROR = 500,
        CONFLICT = 409
    };

    AppError(ErrorCode code, const std::string& message)
        : code_(code), message_(message) {}

    ErrorCode getCode() const { return code_; }
    int getHttpCode() const { return static_cast<int>(code_); }
    std::string getMessage() const { return message_; }

    // Factory methods
    static AppError notFound(const std::string& message) {
        return AppError(ErrorCode::NOT_FOUND, message);
    }

    static AppError badRequest(const std::string& message) {
        return AppError(ErrorCode::BAD_REQUEST, message);
    }

    static AppError internalError(const std::string& message) {
        return AppError(ErrorCode::INTERNAL_ERROR, message);
    }

    static AppError conflict(const std::string& message) {
        return AppError(ErrorCode::CONFLICT, message);
    }

private:
    ErrorCode code_;
    std::string message_;
};

} // namespace utils
