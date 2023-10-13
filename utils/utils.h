#ifndef UTILS_H_
#define UTILS_H_

#define _GNU_SOURCE
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>

# include <arpa/inet.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sched.h>
# include <poll.h>
# include <signal.h>

#include <stdbool.h>

/**
 * Build assertion macro for integer expressions, evaluates to 0
 */
#define BUILD_ASSERT(x) (sizeof(char[(x) ? 0 : -1]))

/**
 * Build time check to assert a is an array, evaluates to 0
 *
 * The address of an array element has a pointer type, which is not compatible
 * to the array type.
 */
#define BUILD_ASSERT_ARRAY(a) \
		BUILD_ASSERT(!__builtin_types_compatible_p(typeof(a), typeof(&(a)[0])))

/**
 * This macro allows counting the number of arguments passed to a macro.
 * Combined with the VA_ARGS_DISPATCH() macro this can be used to implement
 * macro overloading based on the number of arguments.
 * 0 to 10 arguments are currently supported.
 */
#define VA_ARGS_NUM(...) _VA_ARGS_NUM(0,##__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0)
#define _VA_ARGS_NUM(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NUM,...) NUM

/**
 * This macro can be used to dispatch a macro call based on the number of given
 * arguments, for instance:
 *
 * @code
 * #define MY_MACRO(...) VA_ARGS_DISPATCH(MY_MACRO, __VA_ARGS__)(__VA_ARGS__)
 * #define MY_MACRO1(arg) one_arg(arg)
 * #define MY_MACRO2(arg1,arg2) two_args(arg1,arg2)
 * @endcode
 *
 * MY_MACRO() can now be called with either one or two arguments, which will
 * resolve to one_arg(arg) or two_args(arg1,arg2), respectively.
 */
#define VA_ARGS_DISPATCH(func, ...) _VA_ARGS_DISPATCH(func, VA_ARGS_NUM(__VA_ARGS__))
#define _VA_ARGS_DISPATCH(func, num) __VA_ARGS_DISPATCH(func, num)
#define __VA_ARGS_DISPATCH(func, num) func ## num

/**
 * Assign variadic arguments to the given variables.
 *
 * @note The order and types of the variables are significant and must match the
 * variadic arguments passed to the function that calls this macro exactly.
 *
 * @param last		the last argument before ... in the function that calls this
 * @param ...		variable names
 */
#define VA_ARGS_GET(last, ...) ({ \
	va_list _va_args_get_ap; \
	va_start(_va_args_get_ap, last); \
	_VA_ARGS_GET_ASGN(__VA_ARGS__) \
	va_end(_va_args_get_ap); \
})

/**
 * Assign variadic arguments from a va_list to the given variables.
 *
 * @note The order and types of the variables are significant and must match the
 * variadic arguments passed to the function that calls this macro exactly.
 *
 * @param list		the va_list variable in the function that calls this
 * @param ...		variable names
 */
#define VA_ARGS_VGET(list, ...) ({ \
	va_list _va_args_get_ap; \
	va_copy(_va_args_get_ap, list); \
	_VA_ARGS_GET_ASGN(__VA_ARGS__) \
	va_end(_va_args_get_ap); \
})

#define _VA_ARGS_GET_ASGN(...) VA_ARGS_DISPATCH(_VA_ARGS_GET_ASGN, __VA_ARGS__)(__VA_ARGS__)
#define _VA_ARGS_GET_ASGN1(v1) __VA_ARGS_GET_ASGN(v1)
#define _VA_ARGS_GET_ASGN2(v1,v2) __VA_ARGS_GET_ASGN(v1) __VA_ARGS_GET_ASGN(v2)
#define _VA_ARGS_GET_ASGN3(v1,v2,v3) __VA_ARGS_GET_ASGN(v1) __VA_ARGS_GET_ASGN(v2) \
	__VA_ARGS_GET_ASGN(v3)
#define _VA_ARGS_GET_ASGN4(v1,v2,v3,v4) __VA_ARGS_GET_ASGN(v1) __VA_ARGS_GET_ASGN(v2) \
	__VA_ARGS_GET_ASGN(v3) __VA_ARGS_GET_ASGN(v4)
#define _VA_ARGS_GET_ASGN5(v1,v2,v3,v4,v5) __VA_ARGS_GET_ASGN(v1) __VA_ARGS_GET_ASGN(v2) \
	__VA_ARGS_GET_ASGN(v3) __VA_ARGS_GET_ASGN(v4) __VA_ARGS_GET_ASGN(v5)
#define __VA_ARGS_GET_ASGN(v) v = va_arg(_va_args_get_ap, typeof(v));

/**
 * Get the number of elements in an array
 */
#define countof(array) (sizeof(array)/sizeof((array)[0]) \
						+ BUILD_ASSERT_ARRAY(array))

typedef enum status_t status_t;

enum status_t {
	/** Call succeeded */
	SUCCESS,
	/** Call failed */
	FAILED,
	/** Out of resources */
	OUT_OF_RES,
	/** The suggested operation is already done */
	ALREADY_DONE,
	/** Not supported */
	NOT_SUPPORTED,
	/** One of the arguments is invalid */
	INVALID_ARG,
	/** Something could not be found */
	NOT_FOUND,
	/** Error while parsing */
	PARSE_ERROR,
	/** Error while verifying */
	VERIFY_ERROR,
	/** Object in invalid state */
	INVALID_STATE,
	/** Destroy object which called method belongs to */
	DESTROY_ME,
	/** Another call to the method is required */
	NEED_MORE,
};

#endif
