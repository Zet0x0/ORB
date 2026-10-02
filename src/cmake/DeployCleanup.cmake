# ORB's own QML modules are compiled into ORB.exe, but the deploy tool still
# copies their folders from the build dir

if(NOT QT_DEPLOY_PREFIX OR NOT QT_DEPLOY_QML_DIR)
    message(
        FATAL_ERROR "DeployCleanup.cmake has to run AFTER Qt's deploy script")
endif()

file(REMOVE_RECURSE "${QT_DEPLOY_PREFIX}/${QT_DEPLOY_QML_DIR}/ORB")
