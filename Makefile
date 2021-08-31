SRCDIR   = src
DEPSDIR  = deps
BUILDDIR = build
GAMEDIR  = game
RESDIR   = resources

PACKURL = http://aos.party/bsresources.zip
RESPACK = $(GAMEDIR)/bresources.zip

BINARY  = betterspades
CXX     = c++
CFLAGS  = -std=c++2a -DUSE_SOUND -DUSE_GLFW -I$(DEPSDIR) -I$(SRCDIR)
LDFLAGS = -lopenal -lcglm -lglfw -lGLEW -lGLU -lGL -lenet -ldeflate -lpthread

HEADERS  = src/common.hpp src/model_normals.hpp

DEPS     = hashtable ini libvxl log microui parson lodepng http stb_truetype dr_wav

MODULES  = aabb camera cameracontroller chunk config file font glx grenade hud list main map
MODULES += matrix model network particle player sound texture tracer weapon window utils ping
MODULES += minheap rpc tesselator channel entitysystem

objs = $(addprefix $(1)/,$(addsuffix .o,$(2)))
OBJS = $(call objs,$(BUILDDIR),$(MODULES) $(DEPS))

all: $(BUILDDIR) $(BUILDDIR)/$(BINARY) $(RESPACK)
	cp $(BUILDDIR)/$(BINARY) $(GAMEDIR)
	cp -r $(RESDIR)/* $(GAMEDIR)
	unzip $(RESPACK) -d $(GAMEDIR)

$(RESPACK): $(GAMEDIR)
	wget $(PACKURL) -O $(RESPACK)

$(BUILDDIR)/$(BINARY): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(BUILDDIR)/$(BINARY)

$(call objs,$(BUILDDIR),$(MODULES)): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $< -o $@

$(call objs,$(BUILDDIR),$(DEPS)): $(BUILDDIR)/%.o: $(DEPSDIR)/%.cpp $(DEPSDIR)/%.hpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $< -o $@

$(GAMEDIR):
	mkdir -p $(GAMEDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)/$(BINARY) $(OBJS)
