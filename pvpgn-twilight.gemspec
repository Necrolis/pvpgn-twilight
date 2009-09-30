Gem::Specification.new do |s|
  s.name = %q{pvpgn-twilight}
  s.version = "0.0.7"
  s.date = %q{2009-09-30}
  s.authors = ["Lorenzo Boccetti"]
  s.email = %q{necrolis@gamil.com}
  s.summary = %q{A ruby interface for Twilight PVPGN}
  s.homepage = %q{http://www.war3.co.za}
  s.description = %q{PVPGN-Twilight allows ruby scripts to communicate with pvpgn, while providing faster varients of certain functions, like hashing}
  s.files = ["pvpgn-twilight.gemspec","ext/pvpgn.cpp","examples/pvpgn_test.rb","ext/extconf.rb","Changes","docs/pvpgn-twilight.txt","pvpgn-twilight.gemspec"]
  s.extensions << 'ext/extconf.rb'
  s.has_rdoc = true
  s.extra_rdoc_files = ["Changes","ext/pvpgn.cpp","docs/pvpgn-twilight.txt"]
end