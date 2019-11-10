
cmake_minimum_required(VERSION 3.10)

project(kscreensystem)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")
set(TRANSLATION_DOMAIN "kscreensystem")

FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

find_package(ECM 1.2.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS I18n                     Config                   KIO                                             WidgetsAddons)

find_package(EdupalsBase REQUIRED)
find_package(EdupalsN4D REQUIRED)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fexceptions")

add_definitions(-DTRANSLATION_DOMAIN=\"${TRANSLATION_DOMAIN}\")

IF(NOT GETTEXT_MSGFMT_EXECUTABLE)
	MESSAGE(
"------
                 NOTE: msgfmt not found. Translations will *not* be installed
------")
ELSE(NOT GETTEXT_MSGFMT_EXECUTABLE)
        FILE(GLOB PO_FILES translations/*.po)
        SET(GMO_FILES)
        FOREACH(_poFile ${PO_FILES})
                GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
		STRING(REGEX REPLACE "^${TRANSLATION_DOMAIN}_?" "" _langCode ${_poFileName} )
                STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )
		message(${_langCode})
                IF( _langCode )
                        GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
                        SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)
			message("comando ${GETTEXT_MSGFMT_EXECUTABLE} -  ${_poFile} -  ${_poFile}")
			ADD_CUSTOM_COMMAND(OUTPUT ${_gmoFile}
                                COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_poFile}
				DEPENDS ${_poFile})
			INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${TRANSLATION_DOMAIN}.mo)
			message("Aqui no llega")
			LIST(APPEND GMO_FILES ${_gmoFile})
                ENDIF( _langCode )

        ENDFOREACH(_poFile ${PO_FILES})

        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${GMO_FILES})

ENDIF(NOT GETTEXT_MSGFMT_EXECUTABLE)

set( kcm_kscreen_system_SRCS
     src/GeneralPage.cpp
     src/ValidationForm.cpp
)

ki18n_wrap_ui(kcm_kscreen_system_SRCS
    kscreensystem.ui
    validation.ui)

add_library(kcm_kscreen_system MODULE ${kcm_kscreen_system_SRCS})

target_link_libraries(
     kcm_kscreen_system
     KF5::ConfigWidgets
     KF5::KIOWidgets
     KF5::I18n
     KF5::WidgetsAddons
     Edupals::Base Edupals::N4D
)

install(TARGETS kcm_kscreen_system  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES kscreensystem.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

