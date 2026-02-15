include("/home/luluwalilith/QTPROJECTS/Practica-1-1S--2026-EDD/Practica1-1s-2026-EDD/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Practica1-1s-2026-EDD-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/luluwalilith/QTPROJECTS/Practica-1-1S--2026-EDD/Practica1-1s-2026-EDD/build/Practica1-1s-2026-EDD"
    GENERATE_QT_CONF
)
