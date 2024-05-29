# ****************************************************************************
#    Ledger Ethereum Plugin Boilerplate
#    (c) 2023 Ledger SAS.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
# ****************************************************************************

ifeq ($(BOLOS_SDK),)
$(error Environment variable BOLOS_SDK is not set)
endif

include $(BOLOS_SDK)/Makefile.defines

ifeq ($(TARGET_NAME),$(filter $(TARGET_NAME),TARGET_NANOX TARGET_STAX TARGET_FLEX))
CUSTOM_APP_FLAGS = 0x200
else
CUSTOM_APP_FLAGS = 0x000
endif

# Application name
APPNAME = "Lisk"

# Application version
APPVERSION_M = 1
APPVERSION_N = 0
APPVERSION_P = 0

APPVERSION ?= "$(APPVERSION_M).$(APPVERSION_N).$(APPVERSION_P)"

# Application source files
APP_SOURCE_PATH += src ethereum-plugin-sdk
INCLUDES_PATH += ${BOLOS_SDK}/lib_standard_app

NORMAL_NAME ?= $(shell echo "$(APPNAME)" | tr " ." "_" | tr "[:upper:]" "[:lower:]")
ICON_NANOS = icons/nanos_app_$(NORMAL_NAME).gif
ICON_NANOX = icons/nanox_app_$(NORMAL_NAME).gif
ICON_NANOSP = $(ICON_NANOX)
ICON_STAX = icons/stax_app_$(NORMAL_NAME).gif
ICON_FLEX = icons/flex_app_$(NORMAL_NAME).gif

ifeq ($(TARGET_NAME),$(filter $(TARGET_NAME),TARGET_STAX TARGET_FLEX))
    DEFINES += ICONGLYPH=C_stax_$(NORMAL_NAME)_64px
    DEFINES += ICONBITMAP=C_stax_$(NORMAL_NAME)_64px_bitmap
endif

CURVE_APP_LOAD_PARAMS = secp256k1
PATH_APP_LOAD_PARAMS = "44'/134'"

VARIANT_PARAM = COIN
VARIANT_VALUES = lisk

DISABLE_STANDARD_APP_FILES = 1
DISABLE_STANDARD_SNPRINTF = 1
DISABLE_STANDARD_USB = 1
DISABLE_STANDARD_WEBUSB = 1
DISABLE_STANDARD_BAGL_UX_FLOW = 1
DISABLE_DEBUG_LEDGER_ASSERT = 1
DISABLE_DEBUG_THROW = 1

include $(BOLOS_SDK)/Makefile.standard_app
