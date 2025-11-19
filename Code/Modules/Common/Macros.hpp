// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#define NE_ND [[nodiscard]]

#define CAST static_cast
#define CCAST const_cast
#define DCAST dynamic_cast
#define RCAST reinterpret_cast

#define NE_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define NE_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define NE_CLAMP(value, min, max) (X_MIN(X_MAX(value, min), max))

/// @brief Deletes both the move/copy assignment operator and constructor
#define NE_CLASS_PREVENT_MOVES_COPIES(CLASS_NAME)                                                                      \
    CLASS_NAME(const CLASS_NAME&)            = delete;                                                                 \
    CLASS_NAME(CLASS_NAME&&)                 = delete;                                                                 \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;                                                                 \
    CLASS_NAME& operator=(CLASS_NAME&&)      = delete;

/// @brief Deletes the copy constructor and assignment operator
#define NE_CLASS_PREVENT_COPIES(CLASS_NAME)                                                                            \
    CLASS_NAME(const CLASS_NAME&)            = delete;                                                                 \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;

/// @brief Deletes the move constructor and assignment operator
#define NE_CLASS_PREVENT_MOVES(CLASS_NAME)                                                                             \
    CLASS_NAME(CLASS_NAME&&)            = delete;                                                                      \
    CLASS_NAME& operator=(CLASS_NAME&&) = delete;
