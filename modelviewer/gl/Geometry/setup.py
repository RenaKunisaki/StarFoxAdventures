'''setup for Geometry
'''

from setuptools import setup, find_packages
from codecs import open
from os import path


long_description = '''
Geometry is python3 package which implements a variety of geometric objects:

- point
- ellipse
- circle
- line
- segment
- ray
- triangle
- rectangle

Install with pip::

 $ sudo pip3 install Geometry

Install from git repository::

 $ git clone https://github.com/JnyJny/Geometry.git
 $ cd Geometry
 $ sudo python3 setup.py install

'''

try:
    here = path.abspath(path.dirname(__file__))
    with open(path.join(here, 'VERSION'), encoding='utf-8') as f:
        version = f.read()[:-1]
except FileNotFoundError:
    version = '0.0.0'

download_url = 'https://github.com/JnyJny/Geometry/archive/{}.tar.gz'

setup(name='Geometry',
      version=version,
      description='Geometry the python way.',
      long_description=long_description,
      url='https://github.com/JnyJny/Geometry',
      download_url=download_url.format(version),
      author="Erik O'Shaughnessy",
      author_email="erik.oshaughnessy@gmail.com",
      license='MIT',
      classifiers=['Development Status :: 3 - Alpha',
                   'Intended Audience :: Developers',
                   'Topic :: Software Development :: Libraries'
                   ':: Python Modules',
                   'Topic :: Scientific/Engineering :: Mathematics',
                   'License :: OSI Approved :: MIT License',
                   'Programming Language :: Python :: 3',
                   'Programming Language :: Python :: 3.4'],
      keywords='geometry point circle line segment triangle rectangle graph',
      packages=find_packages(exclude=['contrib']),
      test_suite='Geometry.tests',
      install_requires=[],
      extras_require={},
      package_data={},
      data_files=[],
      )
