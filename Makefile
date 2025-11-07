CXX = g++
CXXFLAGS = -Iimgui -Iimgui/backends -Iglfw/include
LDFLAGS = -Lglfw/lib-mingw-w64
LIBS = -lglfw3 -lopengl32 -lgdi32 -lcomdlg32

SOURCES = main.cpp \
          imgui/imgui.cpp \
          imgui/imgui_draw.cpp \
          imgui/imgui_tables.cpp \
          imgui/imgui_widgets.cpp \
          imgui/backends/imgui_impl_glfw.cpp \
          imgui/backends/imgui_impl_opengl3.cpp

OUTPUT = imgui_app.exe

all:
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(OUTPUT) $(LIBS)
