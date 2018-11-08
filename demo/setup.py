#! /usr/bin/python

from distutils.core import setup

setup(name='demo',
	version='1.0',
	url='www.harmonicinc.com',
	maintainer='Omer Dagan',
	maintainer_email='omer.dagan@harmonicinc.com',
	packages=["demo"],
	package_dir={"demo": "python/demo"},
)
