require 'rubygems'
require 'pvpgn'
#puts 'PVPGN Gem Version:' + PVPGN::Version
puts 'the following will hash to 2e0236a3649381fe99cdce4b5b59adffa3167c7b:'
puts 'hello -> ' + PVPGN::BNHash('hello') #will hash to: 2e0236a3649381fe99cdce4b5b59adffa3167c7b
puts 'HELLO -> ' + PVPGN::BNHash('HELLO') #will hash to: 2e0236a3649381fe99cdce4b5b59adffa3167c7b
puts 'HeLlO -> ' + PVPGN::BNHash('HeLlO') #will hash to: 2e0236a3649381fe99cdce4b5b59adffa3167c7b
