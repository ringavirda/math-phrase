#pragma once

/**
 * This headed exposes main entry points into different modes that the interpreter
 * can work in. Currently there is per-operation and per-phrase modes. The former
 * one behaves like a simple calculator while the latter can understand complex
 * mathematical expressions.
 */

/** @brief Contains main execution loop for the per-operation mode. */
void per_operation_mode();
/** @brief Contains main operation loop for the per-phrase mode. */
void per_phrase_mode();