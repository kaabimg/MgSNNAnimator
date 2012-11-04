MGLIBRARY_DIR = /home/ghaith/git/MgLibraryRepository/MgLibraryRepository/Mg
TEMPLATE = app
TARGET = MgSNNAnimator
QT += core \
    gui \
    opengl \
    webkit     
MG += mg_core \
    mg_gui \
    mg_3d \
    mg_script
include ($$MGLIBRARY_DIR/mglibrary.pri)
DESTDIR = bin
OBJECTS_DIR = .tmp/obj
MOC_DIR = .tmp/moc
RCC_DIR = .tmp/rcc
SOURCES = src/gui/mgvideoexamplewidget.cpp \
    src/shaders/blend/mgspikesglowblendshader.cpp \
    src/shaders/neuronspikeeffect/mgspikecircleshader.cpp \
    src/shaders/filters/mgspikepassfilter.cpp \
    src/main.cpp \
    src/shaders/neuronspikeeffect/mgspikeeffectshader.cpp \
    src/shaders/curve/mgcurveborderdrawshader.cpp \
    src/shaders/curve/mgcurvedrawshader.cpp \
    src/3d/mg3dcurveplotter.cpp \
    src/shaders/neuronspikeeffect/mgneuronspikeeffectshader.cpp \
    src/gui/mganimatortimelinewidget.cpp \
    src/gui/mgsnnanimatorhelpwidget.cpp \
    src/gui/mgsnnanimatormainwindow.cpp \
    src/3d/mgneuralnetworkview.cpp \
    src/shaders/synapse/mgsynapseshader.cpp \
    src/shaders/neuron/mgneuronshader.cpp \
    src/core/mgneuralnetworkio.cpp \
    src/core/mgneuralnetworkanimator.cpp \
    src/core/mgneuron.cpp \
    src/core/mgneuralnetworkgraph.cpp \
    src/core/mgsynapse.cpp
HEADERS = src/gui/mgvideoexamplewidget.h \
    src/shaders/blend/mgspikesglowblendshader.h \
    src/shaders/neuronspikeeffect/mgspikecircleshader.h \
    src/shaders/filters/mgspikepassfilter.h \
    src/shaders/neuronspikeeffect/mgspikeeffectshader.h \
    src/shaders/curve/mgcurveborderdrawshader.h \
    src/shaders/curve/mgcurvedrawshader.h \
    src/3d/mg3dcurveplotter.h \
    src/shaders/neuronspikeeffect/mgneuronspikeeffectshader.h \
    src/gui/mganimatortimelinewidget.h \
    src/gui/mgsnnanimatorhelpwidget.h \
    src/gui/mgsnnanimatormainwindow.h \
    src/3d/mgneuralnetworkview.h \
    src/shaders/synapse/mgsynapseshader.h \
    src/shaders/neuron/mgneuronshader.h \
    src/core/mgneuralnetworkio.h \
    src/core/mgneuralnetworkanimator.h \
    src/core/mgneuralnetworkgraph.h \
    src/core/mgsynapse.h \
    src/core/mgneuron.h
RESOURCES = src/examplesscripts/examplesscripts.qrc \
    src/textures/snnatextures.qrc \
    src/icons/icons.qrc \
    src/shaders/snnashaders.qrc
