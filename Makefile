# Compiler
CXX = g++

# Include directories
CXXFLAGS = -Iimgui \
           -Iimgui/backends \
           -Iglfw/include \
           -Icpr/include \
           -I. \
           -IC:/msys64/mingw64/include

# Library directories
LDFLAGS = -Lglfw/lib-mingw-w64 \
          -LC:/msys64/mingw64/lib

# Linked libraries
LIBS = -lglfw3 \
       -lopengl32 \
       -lgdi32 \
       -lcomdlg32 \
       -lcurl \
       -lws2_32 \
       -lwldap32 \
       -lcrypt32 \
       -lssl \
       -lcrypto

# Source files
SOURCES = main.cpp \
          imgui/imgui.cpp \
          imgui/imgui_draw.cpp \
          imgui/imgui_tables.cpp \
          imgui/imgui_widgets.cpp \
          imgui/backends/imgui_impl_glfw.cpp \
          imgui/backends/imgui_impl_opengl3.cpp

# Output file
OUTPUT = imgui_app.exe

# Default rule
all:
	$(CXX) $(SOURCES) -o $(OUTPUT) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

# Clean rule
clean:
	del $(OUTPUT)
