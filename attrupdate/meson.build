cxx = meson.get_compiler('cpp')

sdbusplus = dependency(
    'sdbusplus',
    fallback: [
        'sdbusplus',
        'sdbusplus_dep'
    ],  
)

pdbg_deps = [ 
    cxx.find_library('pdbg'),
    cxx.find_library('fdt'),
    cxx.find_library('ekb'),
    cxx.find_library('dt-api'),
    cxx.find_library('phal')
]

systemd = dependency('systemd')

phosphor_logging = dependency(
    'phosphor-logging',
    fallback: ['phosphor-logging', 'phosphor_logging_dep'],
    )

executable(
    'attrread',
    'attr_read.cpp',
    dependencies: [ sdbusplus, pdbg_deps, systemd, phosphor_logging ],
)
executable(
    'attrwrite',
    'attr_write.cpp',
    dependencies: [ sdbusplus, pdbg_deps, systemd, phosphor_logging ],
)
