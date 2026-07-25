#!make

.DEFAULT_GOAL := help
BUILD_DIR=builds

ifeq ($(MAKE_J),)
MAKE_J := -j$(shell nproc)
endif

.PHONY: build
build:  ## Create the build environment
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake -DCMAKE_BUILD_TYPE=Release -B ${BUILD_DIR} -S .

.PHONY: build-release
build-release: build  ## Build and compile the library release
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake --build ${BUILD_DIR} --target log4lib_test log4lib

.PHONY: tests
tests:	## Run unit tests
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/ctest --test-dir ${BUILD_DIR} -VV

.PHONY: package
package:  ## Create a package
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake --build ${BUILD_DIR} --target package

.PHONY: docs
docs:	## Create documentation
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake --build ${BUILD_DIR} --target docs

.PHONY: test-app
test-app:  ## Build the test application
	@/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake --build ${BUILD_DIR} --target log4libApp

.PHONY: clean
clean:	## Delete the build directory
	@rm -rf ${BUILD_DIR}

.PHONY: test-fetch-content
test-fetch-content:  ## Build the test application
	@cd test-cmake-fetch-content && rm -rf ./${BUILD_DIR} && \
/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake -DCMAKE_BUILD_TYPE=Release -B ${BUILD_DIR} -S . && \
/opt/clion-2026.1.3/bin/cmake/linux/x64/bin/cmake --build ${BUILD_DIR} --target log4libtestfetchcontent && \
./${BUILD_DIR}/log4libtestfetchcontent && \
rm -rf ./${BUILD_DIR}

.PHONY: help
help:
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sed 's/Makefile://' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'
