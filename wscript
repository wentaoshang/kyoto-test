# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

VERSION='0.0.1'
APPNAME='kc_test'

def options(opt):
	opt.load(['compiler_cxx', 'cxx', 'gnu_dirs'])

def configure(conf):
	conf.load(['compiler_cxx', 'cxx', 'gnu_dirs'])
        conf.check_cxx(cxxflags=['-O2', '-std=c++11'], mandatory=True)
	conf.check_cfg(package='kyotocabinet', uselib_store='KYOTO', args=['--cflags', '--libs'], mandatory=True)
        conf.check_cfg(package='libndn-cxx', uselib_store='NDNCXX', args=['--cflags', '--libs'], mandatory=True)

def build(bld):
	bld.program(features='cxx cxxprogram',
                    source='main.cpp',
                    target='test',
                    use='KYOTO NDNCXX',
                    cxxflags='-O2 -std=c++11')

