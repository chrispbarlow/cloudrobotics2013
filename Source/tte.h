/*
 * Main include file for all drivers from TTE Systems Limited.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef TTE_H_INCLUDED
#define TTE_H_INCLUDED




#include <stddef.h>
#include <stdint.h>

/**
 * Includes testing frame framework if in unit testing mode.
 */
#ifdef TESTING_MODE
#	ifdef STUB_HEADER
#		define xstr(s) str(s)
#		define str(s) #s
#		include xstr(STUB_HEADER)
#	endif
#include "../Testing/testing.h"
#endif


/**
 * Specifies that the current drivers are for the TTE32 target.
 */
#define TTE_TARGET_TTE32

/**
 * The boolean type. The value should be TRUE or FALSE only.
 */
typedef _Bool boolean_t;

/**
 * The character type. Users are free to use '#define TTE_UNICODE' to force
 * all TTE libraries to accept and use wide character strings. Please use
 * the TTE_STR macro when defining string literals.
 */
#ifndef TTE_UNICODE
typedef char char_t;
#else
typedef wchar_t char_t;
#endif

/**
 * Allows the use of wide string literals, automatically if TTE_UNICODE has
 * been #defined. All string literals should be wrapped with this macro,
 * like so: 'TTE_STR("string literal")'.
 */
#ifndef TTE_UNICODE
#define TTE_STR(string) string
#else
#define TTE_STR(string) L##string
#warning "Unicode support is incomplete and untested!"
#endif

// Miscellaneous #defines

#ifndef TRUE
#	ifdef true
#		define FALSE false
#		define TRUE true
#	else
#		define FALSE 0
#		define TRUE (!FALSE)
#	endif
#endif

/**
 * Forces a static inline function to be inlined, even if all optimisations
 * are turned off in the compiler settings. This is useful when we know it
 * is safe to inline a given function, but other parts of the code may require
 * optimisations to be turned off in order to function correctly.
 *
 * By default, this is only employed in release builds.
 */
#ifndef TTE_ALWAYS_INLINE
#	ifndef DEBUG
#		ifdef __GNUC__
#			define TTE_ALWAYS_INLINE
#		else
#			define TTE_ALWAYS_INLINE
#		endif
#	else
#		define TTE_ALWAYS_INLINE
#	endif
#endif

/**
 * Places a function or variable in a given output section, so that it can
 * be positioned by the linker to a location specified in the linker script.
 */
#ifndef TTE_SECTION_PLACEMENT
#	ifdef __GNUC__
#		define TTE_SECTION_PLACEMENT(pos) __attribute__((section(pos)))
#	else
#		define TTE_SECTION_PLACEMENT(pos)
#	endif
#endif


/**
 * Prevents a warning if a given variable or parameter is not used.
 */
#ifndef TTE_UNUSED
#	ifdef __GNUC__
#		define TTE_UNUSED __attribute__((unused))
#	else
#		define TTE_UNUSED
#	endif
#endif

/**
 * Compile-time assertion that will trigger a compiler error if the expression
 * evaluates to FALSE.  The expression must be constant, as the compiler must
 * be able to evaluate it completely.
 *
 * \param[in]	expr	The expression to assert.
 */
#ifndef TTE_STATIC_ASSERT
#define TTE_STATIC_ASSERT(expr) \
	extern uint8_t assertion_failed[(expr) ? 1 : -1]
#endif

/**
 * Runtime assertion that is only kept while debugging and removed from
 * any non-debug or release builds (whenever DEBUG is not defined). By
 * default, this calls DEBUG_Assert_Failed when the assertion fails.
 *
 * \param[in]	expr	The expression to assert.
 */
#ifndef TTE_DEBUG_ASSERT
#ifdef DEBUG
extern void DEBUG_Assert_Failed(char*, int);
#define TTE_DEBUG_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
			DEBUG_Assert_Failed(__FILE__, __LINE__); \
		} \
	} while (FALSE)
#else // !DEBUG
#define TTE_DEBUG_ASSERT(expr)
#endif // DEBUG
#endif // !TTE_DEBUG_ASSERT

/**
 * Runtime assertion that remains in place even in release code.
 *
 * \param[in]	expr	 	The expression to assert.
 * \param[in]	fail_value	The value to return on failure.
 *
 * \return	The value passed as the fail_value parameter.
 */
#ifndef TTE_RELEASE_ASSERT
#define TTE_RELEASE_ASSERT(expr, fail_value) \
	do \
	{ \
		if (!(expr)) \
		{ \
			return (fail_value); \
		} \
	} while (FALSE)
#endif // !TTE_RELEASE_ASSERT

/**
 * Specifies that intro and exit code should not be generated for a function.
 */
#ifndef TTE_NAKED_FUNC
#    ifdef __GNUC__
#        define TTE_NAKED_FUNC
#    else
#        define TTE_NAKED_FUNC
#    endif
#endif

/**
 * Specifies that a function will be replaced if another exists with the
 * same name.
 */
#ifndef TTE_WEAK_FUNC
#	ifdef __GNUC__
#		define TTE_WEAK_FUNC __attribute__((weak))
#	else
#		define TTE_WEAK_FUNC
#	endif
#endif

static const uint8_t RETURN_NORMAL = 0;
static const uint8_t RETURN_ERROR = 1;

#endif // !TTE_H_INCLUDED
