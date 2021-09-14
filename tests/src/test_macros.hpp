#pragma once
#include <core/gf3d_logger.h>

#define should_be_equal(expected, actual)											\
	if (expected != actual) {														\
		LOGGER_ERROR("Expected: {0}, but is actually: {1}", expected, actual);		\
		return false;																\
	}

#define should_not_be_equal(expected, actual)										\
	if (expected == actual) {														\
		LOGGER_ERROR("Expected: {0} != {1}", expected, actual);						\
		return false;																\
	}

#define expected_float_value(expected, actual)										\
	if (std::fabsf(expected - actual) > .001f) {										\
		LOGGER_ERROR("Expected: {0}, but is actually {1}", expected, actual);		\
		return false;																\
	}

#define expect_to_be_true(actual)													\
	if (actual != true) {															\
		LOGGER_ERROR("Expected true, but is actually false");						\
		return false;																\
	}

#define expect_to_be_false(actual)													\
	if (actual != false) {															\
		LOGGER_ERROR("Expected true, but is actually false");						\
		return false;																\
	}