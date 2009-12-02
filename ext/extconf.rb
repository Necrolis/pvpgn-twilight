# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Do the work
cpp_command('g++')
create_makefile('pvpgn')
