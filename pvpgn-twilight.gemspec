Gem::Specification.new do |s|
  s.name = %q{pvpgn-twilight}
  s.version = "0.2.5"
  s.date = %q{2010-02-04}
  s.authors = ["Lorenzo 'Necrolis' Boccetti"]
  s.email = %q{necrolis@gamil.com}
  s.summary = %q{A ruby interface for Twilight PVPGN}
  s.homepage = %q{http://www.twilightgaming.co.za}
  s.description = %q{PVPGN-Twilight allows ruby scripts to communicate with pvpgn, while providing faster varients of certain functions, like hashing}
  s.files = ["pvpgn-twilight.gemspec","ext/pvpgn.c","examples/pvpgn_test.rb","ext/extconf.rb","Changes","docs/pvpgn-twilight.txt","pvpgn-twilight.gemspec"]
  s.extensions << 'ext/extconf.rb'
  s.has_rdoc = true
  s.rubyforge_project = 'see gemcutter.org'
  s.extra_rdoc_files = ["Changes","ext/pvpgn.c","docs/pvpgn-twilight.txt"]
end