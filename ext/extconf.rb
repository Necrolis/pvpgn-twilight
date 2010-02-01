# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# C++ removed...
#$CC = 'g++'
#$CXX = $CC
#cpp_command('g++') if RUBY_VERSION < '1.9'
create_makefile('pvpgn')
