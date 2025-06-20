SHELL := /bin/bash
TARGET = lognsh
TEST_TARGET = tester
MAIN_TEST_TARGET = shell_test # match what's in test.h
OUTPUT = executable

# Colors
YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

# Dirs
OBJDIR = obj
OBJEXT = o
SRCEXT = c
DEPEXT = d
INCDIR = include
TARGETDIR = bin
SRCDIR = src
RESDIR = res
EXTDIR = external
TESTDIR = test
TEST_OBJDIR = $(OBJDIR)/test_mode

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -O2 #-g -DLOGMODE -DDEBUGMODE -DDEBUGVMODE # -fsanitize=address
TEST_CFLAGS = -Wall -Wextra -Werror -g -DTESTMODE  # -Werror -Wextra
EXT_CFLAGS = -DEXTENDEDFUNC
LDFLAGS = -L$(LIB_DIR) -lft -lreadline -lncurses # -fsanitize=address
LDFLAGS_SO = -L$(LIB_DIR) -lft -Wl,-rpath,$(LIB_DIR) -lreadline -lncurses

ifeq ($(shell uname), Darwin)  # macOS
    INC += -I$(INCDIR) -I/usr/local/opt/readline/include/
	CFLAGS += -DMACOS
	TEST_CFLAGS += -DMACOS
else  # Linux
    INC += -I$(INCDIR) -I/usr/include/readline
endif

INCDEP = -I$(INCDIR)

RL_DIR = /usr/local/opt/readline/lib

LIB_CPDIR = .
LIB_DIR = lib
LIB_NAME = libft.a
LIB_NAME_SO = libft.so
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)
LIB_PATH_SO = $(LIB_DIR)/$(LIB_NAME_SO)

# ----------------------------------------------------------------
#  DO NOT EDIT BELOW
# ----------------------------------------------------------------

# BONUS_SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
# BONUS_OBJECTS = $(addprefix $(OBJDIR), $(BONUS_SOURCES:.c=.o))
TEST_SOURCES = $(shell find $(TESTDIR) -maxdepth 1 -type f -name '*.$(SRCEXT)')
TEST_OBJECTS = $(patsubst $(TESTDIR)/%,$(OBJDIR)/test/%,$(TEST_SOURCES:.c=.o))
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)" ! -name "test_*.$(SRCEXT)")
OBJECTS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
UNITY_SRC = $(shell find $(EXTDIR)/unity/ -type f -name '*.$(SRCEXT)')
UNITY_OBJ = $(patsubst $(EXTDIR)/unity/%,$(OBJDIR)/test/%,$(UNITY_SRC:.c=.o))
MAIN_TEST_OBJECTS = $(patsubst $(OBJDIR)/%,$(TEST_OBJDIR)/%,$(OBJECTS)) # separate compiled src w/ test flags

# Default Make
all: directories $(TARGET)

# Copy resources from dir to target dir
resources: directories
	@if [ -d "$(RESDIR)" ] && [ "$(shell ls -A $(RESDIR))" ]; then \
    	cp $(RESDIR)/* $(TARGETDIR)/; \
	else \
    	echo "Warning: $(RESDIR) directory is empty or doesn't exist."; \
	fi

#Make the dirs
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)


# Link the executable
$(TARGET): $(OBJECTS) $(LIB_PATH)
	@echo "Linking with object files: $(OBJECTS)"
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LDFLAGS)
	chmod +x $(TARGETDIR)/$(TARGET)
	@cp $(TARGETDIR)/$(TARGET) .

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)  # % ensures obj file associates with src file
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(OBJDIR)/$*.$(DEPEXT)
	@cp -f $(OBJDIR)/$*.$(DEPEXT) $(OBJDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.$(OBJEXT):|' < $(OBJDIR)/$*.$(DEPEXT).tmp > $(OBJDIR)/$*.$(DEPEXT) || exit 1
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.$(DEPEXT)
	@rm -f $(OBJDIR)/$*.$(DEPEXT).tmp


tst: directories $(TEST_TARGET) $(MAIN_TEST_TARGET)

# Link test binaries
$(TEST_TARGET) : $(UNITY_OBJ) $(LIB_PATH) $(TEST_OBJECTS) 
	$(CC) -o $(TARGETDIR)/$(TEST_TARGET) $^ $(LDFLAGS)
	chmod +x $(TARGETDIR)/$(TEST_TARGET)
	@cp $(TARGETDIR)/$(TEST_TARGET) .

$(MAIN_TEST_TARGET): $(MAIN_TEST_OBJECTS) $(LIB_PATH)
	$(CC) -o $(TARGETDIR)/$(MAIN_TEST_TARGET) $^ $(LDFLAGS)
	chmod +x $(TARGETDIR)/$(MAIN_TEST_TARGET)
	@cp $(TARGETDIR)/$(MAIN_TEST_TARGET) .

# Compile Unity
$(OBJDIR)/$(TESTDIR)/%.$(OBJEXT): $(EXTDIR)/unity/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -I$(EXTDIR)/unity -c $< -o $@

# Compile tst
$(OBJDIR)/$(TESTDIR)/%.$(OBJEXT): $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -I$(EXTDIR)/unity -I$(INCDIR) -c $< -o $@

# Compile tst versions of src
$(TEST_OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) $(INC) -c $< -o $@

bonus: .bonus_made

.bonus_made:
	@echo "Creating $(NAME) $(OUTPUT) with extended functionality..."
	$(MAKE) CFLAGS="$(CFLAGS) $(EXT_CFLAGS)"
	-@touch .bonus_made

# make lib
$(LIB_PATH):
	@$(MAKE) -C $(LIB_DIR)
	@echo "Copying $(LIB_NAME) to ../"
	@cp $(LIB_PATH) .

# clean only objects
clean:
	@$(RM) -rf $(OBJDIR)
	@$(RM) -f $(OBJDIR)/*.d
	@$(MAKE) -C $(LIB_DIR) clean

# Clean+, objects and binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -f $(TARGET) # copied to root for lame evaluators
	rm -f $(LIB_NAME) # don't delete so!
	-@rm -f .bonus_made

# Full clean
fclean: cleaner
	rm -f $(LIB_PATH)
	rm -f $(LIB_NAME)
	rm -f $(LIB_NAME_SO)
	@$(MAKE) -C $(LIB_DIR) fclean
	@rm -f $(LIB_DIR)/error.txt

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) -c $(SOURCES) $(BONUS_SOURCES)
	$(CC) -nostartfiles -shared -o pipex.so $(OBJECTS) $(BONUS_OBJECTS)

.PHONY: all tst bonus clean cleaner fclean re resources so
