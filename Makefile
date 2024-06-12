NAME		=	parsetest

SRC			=	main.cpp ConfigParser.cpp ConfigSection.cpp ConfigReference.cpp ConfigFile.cpp \
				Server.cpp Location.cpp Request.cpp Response.cpp

SRCDIR		=	src
OBJDIR		=	obj
INCDIR		=	include
LIBFLG		=	

OBJ			=	$(foreach o, $(SRC:.cpp=.o),$(OBJDIR)/$(o))
DEP			=	$(foreach d, $(SRC:.cpp=.d),$(OBJDIR)/$(d))
LIBINC		=	$(foreach l, $(LIBS),-I $(l) -L $(l) -l$(l:lib%=%))
LIBARC		=	$(foreach l, $(LIBS),$(l)/$(l).a)
INCLUDE		=	$(foreach i, $(INCDIR),-I $(i)) $(foreach l, $(LIBS),-I $(l))

CFLAGS		=	-Wall -Wextra -Werror -std=c++17
SFLAGS		=	-fsanitize=address -g

#CC 			=	c++

# == Determine OS and CPU core count =
OS := $(shell uname)

ifeq ($(OS),Linux)
  NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
  CC:=g++
endif
ifeq ($(OS),Darwin) # Assume Mac OS X
  NPROCS:=$(shell sysctl hw.ncpu | grep -o '[0-9]\+')
  CC:=c++
endif
# ====================================

all: $(NAME)

san: fclean setdebug $(NAME)

setdebug:
	$(eval CFLAGS := $(CFLAGS) $(SFLAGS))

$(NAME): $(OBJ) $(LIBARC)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBFLG) $(LIBINC) $(MLX42) -o $@ $(OBJ)

-include $(DEP)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if [ $(OBJDIR) != '.' ] && [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c -o $@ $(SRCDIR)/$(notdir $(@:.o=.cpp))

$(LIBARC): %:
	$(MAKE) -j$(NPROCS) -C $(basename $(notdir $@))

clean:
	@$(foreach l, $(LIBS),$(MAKE) -C $(l) clean;)
	@rm -f $(foreach o, $(OBJ),$(o))
	@rm -f $(foreach d, $(DEP),$(d))
	@if [ $(OBJDIR) != '.' ] && [ -d $(OBJDIR) ]; then rmdir $(OBJDIR); fi

fclean: clean
	@$(foreach l, $(LIBS),$(MAKE) -C $(l) fclean;)
	@if [ -f $(NAME) ]; then /bin/rm $(NAME); fi

re: fclean all

.PHONY: bonus
