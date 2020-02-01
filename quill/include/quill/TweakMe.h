#pragma once

/**
 * Edit this file to squeeze more performance, or to customise supported
 * features that are enabled/disabled in compile time
 *
 * All the values below can either be passed as compiler flags or get uncommented here
 *
 * HINT:
 * It is better passing those flags using CMake or during compilation instead of uncommenting them
 * here as this gives more flexibility invoking different builds by passing different flags rather
 * than editing this file
 *
 * a) When invoking cmake : cmake . -DCMAKE_CXX_FLAGS="-DQUILL_ACTIVE_LOG_LEVEL=QUILL_LOG_LEVEL_INFO"
 */

/**
 * If QUILL_RDTSC_CLOCK is not defined Quill will use chrono system_clock for timestamps
 *
 * QUILL_TSC_CLOCK mode :
 *
 * TSC clock gives better performance on the caller thread. However, the initialisation time of the application is higher as
 * we have to take multiple samples in the beginning to convert TSC to nanoseconds
 *
 * Consider reading https://stackoverflow.com/questions/42189976/calculate-system-time-using-rdtsc
 *
 * The backend thread is constantly keeping track of the difference between TSC and the system wall clock resulting
 * in accurate timestamps.
 *
 * When using the TSC counter the backend thread will also periodically call chrono::system_clock:now() and will
 * resync the TSC based on the system clock.
 */
#define QUILL_RDTSC_CLOCK

/**
 * This option is only applicable if the RDTSC clock is enabled. When QUILL_RDTSC_CLOCK is not
 * defined this option can be ignored, but it shouldn't be commented out.
 *
 * Quill by default will re-calculate and sync TSC based on the system wall clock
 */
#define QUILL_RDTSC_RESYNC_INTERVAL 10

/**
 * Completely compiles out log level with zero cost.
 *
 * Macros like LOG_TRACE_L3(..), LOG_TRACE_L2(..) will expand to empty statements
 * This helps reducing the number of branches in your compiled code and the number of
 * StaticLogRecordInfo constexpr instances created in compile time
 *
 * The default value is QUILL_LOG_LEVEL_TRACE_L3
 *
 * Options:
 *
 * QUILL_LOG_LEVEL_TRACE_L3
 * QUILL_LOG_LEVEL_TRACE_L2
 * QUILL_LOG_LEVEL_TRACE_L1
 * QUILL_LOG_LEVEL_DEBUG
 * QUILL_LOG_LEVEL_INFO
 * QUILL_LOG_LEVEL_WARNING
 * QUILL_LOG_LEVEL_ERROR
 * QUILL_LOG_LEVEL_CRITICAL
 */
#define QUILL_ACTIVE_LOG_LEVEL QUILL_LOG_LEVEL_TRACE_L3

/**
 * Quill uses a bounded SPSC queue per spawned thread to forward the LogRecords to the backend
 *
 * Since the queue is bounded, during very high logging activity the backend thread won't be
 * able to consume fast enough and the queue will become full. In this scenario the caller thread
 * will block until there is some free space in the queue to push the LogRecord
 *
 * By default Quill is using a 16 Mebibyte queue.
 *
 * The queue size can be increased or decreased based on the user needs.
 *
 * @warning The configured queue size needs to be in bytes, it MUST be a power of two and a multiple
 * of the page size.
 * Look for an online Mebibyte to Byte converted to easily find a correct value
 */
#define QUILL_BOUNDED_SPSC_QUEUE_SIZE 8388608u