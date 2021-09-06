SRCDIR   = src
DEPSDIR  = deps
BUILDDIR = build
GAMEDIR  = game
RESDIR   = resources

PACKURL = http://aos.party/bsresources.zip
RESPACK = $(GAMEDIR)/bresources.zip

ifeq ($(OS),Windows_NT)
    BINARY = betterspades.exe
else
    BINARY = betterspades
endif

CXX = c++

BETTERSPADES_MAJOR = 0
BETTERSPADES_MINOR = 1
BETTERSPADES_PATCH = 8

OPTS  = -DBETTERSPADES_MAJOR=$(BETTERSPADES_MAJOR)
OPTS += -DBETTERSPADES_MINOR=$(BETTERSPADES_MINOR)
OPTS += -DBETTERSPADES_PATCH=$(BETTERSPADES_PATCH)
OPTS += -DBETTERSPADES_VERSION=\"v$(BETTERSPADES_MAJOR).$(BETTERSPADES_MINOR).$(BETTERSPADES_PATCH)\"
OPTS += -DGIT_COMMIT_HASH=\"$(shell git rev-parse HEAD)\"
OPTS += -DUSE_SOUND

CFLAGS = -Wno-narrowing -std=c++2a $(OPTS) -I$(DEPSDIR) -I$(SRCDIR)

ifeq ($(OS),Windows_NT)
    LDFLAGS = -static-libgcc -static-libstdc++ -lopenal -Wl,-Bstatic -lcglm -lenet -ldeflate -lglfw3 -lglew32 -lpthread -Wl,-Bdynamic -lopengl32 -lglu32 -lgdi32 -lwinmm -lws2_32 -Wl,-Bstatic -lgcc_s_seh -lwinpthread -Wl,-Bdynamic
else
    UNAME := $(shell uname -s)

    ifeq ($(UNAME),Linux)
        LDFLAGS = -lcglm -lenet -ldeflate -lopenal -lglfw -lGLEW -lGL -lGLU -lpthread
    endif

    ifeq ($(UNAME),Darwin)
        LDFLAGS = -framework OpenAL -lcglm -lenet -ldeflate -lglfw -lGLEW -lGL -lGLU -lpthread
    endif
endif

HEADERS  = src/common.hpp src/model_normals.hpp

DEPS     = hashtable ini libvxl log microui parson lodepng http stb_truetype dr_wav
MODULES  = aabb camera cameracontroller chunk config file font glx grenade hud main map
MODULES += matrix model network particle player sound texture tracer weapon window utils ping
MODULES += minheap rpc tesselator channel entitysystem

objs = $(addprefix $(1)/,$(addsuffix .o,$(2)))
OBJS = $(call objs,$(BUILDDIR),$(MODULES) $(DEPS))

all: $(BUILDDIR) $(GAMEDIR) $(RESPACK)
binary: $(BUILDDIR) $(BUILDDIR)/$(BINARY)

$(RESPACK):
	wget $(PACKURL) -O $(RESPACK)
	unzip -o $(RESPACK) -d $(GAMEDIR) || true

$(BUILDDIR)/$(BINARY): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(BUILDDIR)/$(BINARY)

$(call objs,$(BUILDDIR),$(MODULES)): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $< -o $@

$(call objs,$(BUILDDIR),$(DEPS)): $(BUILDDIR)/%.o: $(DEPSDIR)/%.cpp $(DEPSDIR)/%.hpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $< -o $@

$(GAMEDIR): $(BUILDDIR)/$(BINARY)
	mkdir -p $(GAMEDIR)
	cp $(BUILDDIR)/$(BINARY) $(GAMEDIR)
	cp -r $(RESDIR)/* $(GAMEDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)/$(BINARY) $(OBJS)
