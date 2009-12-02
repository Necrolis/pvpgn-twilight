# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Do the work
$CC = 'g++'
$CXX = $CC
cpp_command('g++')
create_makefile('pvpgn')