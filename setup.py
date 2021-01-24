from setuptools import Extension, setup

ext = Extension(
    name='mymodule',
    sources=['./mymodule.cpp'],
    libraries=['User32'],
    extra_compile_args=['/Z7'],
    extra_link_args=['/DEBUG:FULL'],
)

setup(
    name='mymodule',
    version='0.0.0',
    ext_modules=[ext],
)
