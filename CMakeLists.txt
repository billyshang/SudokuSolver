cmake_minimum_required(VERSION 3.16)

project(Sudoku VERSION 0.1 LANGUAGES CXX)

# Qt自动化处理设置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt库，优先使用Qt6，如果不可用则回退到Qt5
find_package(Qt6 COMPONENTS Widgets REQUIRED)
if(NOT TARGET Qt6::Widgets) # 如果Qt6未找到，则回退到Qt5
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5) # 设置Qt版本主号为5
else()
    set(QT_VERSION_MAJOR 6) # 设置Qt版本主号为6（如果Qt6已找到）
endif()

# 设置源文件列表
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    sudokusolver.cpp # 包括sudokusolver.cpp
    mainwindow.h
    mainwindow.ui
)

# 根据Qt版本添加可执行文件
if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_add_executable(Sudoku
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        pic.qrc
    )
else()
    add_executable(Sudoku
        ${PROJECT_SOURCES}
    )
endif()

# 链接Qt库
target_link_libraries(Sudoku PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 设置目标属性
if(WIN32)
    set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/pic.ico")
    configure_file(${ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/pic.ico COPYONLY)
    set(RESOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/pic.rc")
    file(WRITE ${RESOURCE_FILE} "IDI_ICON1 ICON \"pic.ico\"\n")
    target_sources(Sudoku PRIVATE ${RESOURCE_FILE})
endif()


if(APPLE)
    if(${QT_VERSION} VERSION_LESS 6.1.0)
        set_target_properties(Sudoku PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Sudoku
        )
    endif()
endif()

set_target_properties(Sudoku PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
include(GNUInstallDirs)
install(TARGETS Sudoku
    BUNDLE DESTINATION ${CMAKE_INSTALL_FULL_BUNDLEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 对于Qt6，需要手动完成可执行文件的最终化（如果使用了qt_add_executable且设置了MANUAL_FINALIZATION）
if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(Sudoku)
endif()
