# Global variables ...
BUILD_SPEC = $$(QMAKESPEC)
BUILD_CONFIG = $$(BUILD_CONFIG)
BUILD_VERSION = $$(BUILD_VERSION)

# Global variable defaults (if not set in environment)
isEmpty(BUILD_CONFIG):BUILD_CONFIG = release

# the default version is 1.0.0
isEmpty(BUILD_VERSION):BUILD_VERSION = 100

