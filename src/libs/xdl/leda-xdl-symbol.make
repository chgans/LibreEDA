XSD = leda-xdl-symbol.xsd
MAP = leda-xdl-symbol.map

XSDCXX = xsdcxx cxx-parser --std c++11 \
		--xml-parser expat  \
                --namespace-map http://www.leda.org/xdl=xdl::symbol \
		--type-map $(MAP)

GEN = leda-xdl-symbol-pskel.cxx \
	leda-xdl-symbol-pskel.hxx \
	leda-xdl-symbol-pimpl.cxx \
	leda-xdl-symbol-pimpl.hxx \
	leda-xdl-symbol-driver.cxx \
	leda-xdl-symbol-driver.hxx

$(GEN): $(XSD) $(MAP) leda-xdl-symbol.make
	rm -f $(GEN)
	$(XSDCXX) $(XSD)
	$(XSDCXX) --generate-print-impl $(XSD)
	$(XSDCXX) --generate-test-driver $(XSD)

clean:
	rm -f $(GEN)
