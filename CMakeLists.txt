
cmake_minimum_required(VERSION 3.10)

project(kscreensystem)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")


find_package(ECM 1.2.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS I18n Activities Auth IdleTime Config DBusAddons Solid  GlobalAccel KIO NotifyConfig Screen KDELibs4Support Wayland WidgetsAddons

find_package(EdupalsBase REQUIRED)
find_package(EdupalsN4D REQUIRED)

add_definitions(-DTRANSLATION_DOMAIN=\"kscreensystem\")

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
     KF5::Solid
     KF5::NotifyConfig
     KF5::KDELibs4Support
     KF5::WidgetsAddons
     Edupals::Base Edupals::N4D
)

install(TARGETS kcm_kscreen_system  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES kscreensystem.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

