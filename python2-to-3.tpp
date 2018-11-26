--title Migrating a Python DEB package from Py2 to Py3
--author Omer Dagan <omer.dagan@harmonicinc.com>
--date November 2018
--ulon
--center https://confluence360.harmonicinc.com/display/EDGE/TODO
--uloff

--newpage Agenda
--boldon
--huge Agenda
--horline
--boldoff
---


1) Motivation
---

2) Key differences between Python2 and Python3 (syntax & semantics)
---

3) Migration strategy (zero downtime)
---

4) Migration tools
---

5) Live demo (if we have time)
---

6) Questions

--newpage Motivation
--heading Should I migrate?
---
* Yes
---
--newpage Motivation
--heading Why ??
---

* Python2 EOL is said to be in 2020
  (see PEP373 https://www.python.org/dev/peps/pep-0373/)
---

* Major python projects are stopping their support in python2 soon:

  - Pandas (Data analisys)
---

  - Numpy
---

  - Django Web development framework
---
  
  - Many others (see partial list at: https://python3statement.org/)
---

* Security holes in Python2.7 (in tempfile, xml, and others)

--newpage Motivation
--heading Why ??

* Python3 is faster (a lot of work has been put on optimization, garbage

  collection improvements, memory consumption)
---

* Async & Await one threaded asyncronous progarmming

  (seperate presetation about this topic is comming soon)

  This is a killer feature that will speed up app using IO by order of magnitude
---

* Type annotations (aka statically typed python)

--beginoutput
def hello(name: str) -> None:
	print('hello {}'.format(name))
--endoutput

	- They work as documenation (aka self documenting code)

	- Using external checkers (see: http://mypy-lang.org/) they can catch 
	
	  problems prior to runtime

	- Some IDEs (pychram) provides better code completion

	- They're optional
---

* Python3 only libraries such as Quarts

--newpage Motivation
--heading Why ??

* Data classes- a decorator and functions for automatically adding generated 

  special methods such as __init__(), __repr__() and comparison operators.
---
This
--beginoutput  
class InventoryItem:
	'''Class for keeping track of an item in inventory.'''
	def __init__(self, name, unit_price, quantity_on_hand):
		self.name = name
		self.unit_price = unit_price
		self.quantity_on_hand = quantity_on_hand
	
	def __repr__(self):
		return (f'{self.__class__.__name__}'
			f'(name={self.name!r}, unit_price={self.unit_price!r}, quantity_on_hand={self.quantity_on_hand!r})')
	
	def __eq__(self, other):
		if other.__class__ is not self.__class__:
			return NotImplemented
		return (self.name, self.unit_price, self.quantity_on_hand) == (other.name, other.unit_price, other.quantity_on_hand)
	
	def total_cost(self) -> float:
	    return self.unit_price * self.quantity_on_hand
--endoutput

becomes:
--newpage Motivation
--heading Why ??
--beginoutput  
@dataclass
class InventoryItem:
    '''Class for keeping track of an item in inventory.'''
    name: str
    unit_price: float
    quantity_on_hand: int = 0

    def total_cost(self) -> float:
        return self.unit_price * self.quantity_on_hand
--endoutput

  Check out this tutorial:

  https://realpython.com/python-data-classes/
---

  However, this is not supported in python3.5 but only in python3.7 :(
---

* Some more: check out the "what's new" pages for each version 3.x:

  https://docs.python.org/3/whatsnew/index.html

--newpage Key Differnces
--heading Key Differnces
---

* Print is now a function
--beginoutput
Old: print "The answer is", 2*2
New: print("The answer is", 2*2)

Old: print x,           # Trailing comma suppresses newline
New: print(x, end=" ")  # Appends a space instead of a newline

Old: print              # Prints a newline
New: print()            # You must call the function!

Old: print >> sys.stderr, "fatal error"
New: print("fatal error", file=sys.stderr)
--endoutput

---
* Exception handling:

--beginoutput
Python2:

	try:
		try_somthing()
	except Exception, e:
		handle()

Python3:

	try:
		try_somthing()
	except Exception as e:
		handle()
--endoutput

--newpage Key Differnces
--heading Key Differnces

* map(), filter() now returns iterators

	- Sometimes it's ok

	- Somethimes it's not ok- for example, in case you use list methods such as append, pop
	
	- If not, you can cast with list(map(...))

	- or better, use list comprehenstions

--beginoutput
Python2:
>>> l = [1,2,3,4,5,6,7,8,9]
>>> filter(lambda x: x % 2 == 0, l)

Python3:
>>> l = [1,2,3,4,5,6,7,8,9]
>>> [x for x in l if x % 2 == 0]

Python2:
>>> l = [1,2,3,4,5,6,7,8,9]
>>> map(lambda x: x * 2, l)

Python3:
>>> l = [1,2,3,4,5,6,7,8,9]
>>> [x * 2 for x in l]
--endoutput

--newpage Key Differnces
--heading Key Differnces

* Division:

	- In python2 int/int --> int, therefore is a floor division.

	- In python3 int/int --> float, therefore is a regular division
---
--beginoutput
>>> 2 / 3		# Python2
0

>>> 2/3			# Python3
0.6666666666666666

>>> 2//3
0
--endoutput

---
* Dictionary methods keys(), values(), items() now return **views** rather than lists

	- Views are 'live' state of the dictionary. Once the dictionary is changed, it is
	
	  reflected in the view.
---
	
	- Sometimes it doesn't matter- for a "one time consume" for instance.
---

	- Sometimes it does- in case you use list methods on returned value: 

	  k = d.keys(); k.sort()

	  ^ This won't work
---
	
	- Instead, use this:

	  k = sorted(d)
---

	- If needed, again you can use casting to list, or list comprehensions

--newpage Key Differnces
--heading Key Differnces

* Text Vs. Data Instead Of Unicode Vs. 8-bit:

	- In Python2 there were <str> and <unicode>. 
	
	- <str> reffered to 8-bit strings, that if used only 7 bits were unambigious,

	  but otherwise needed clarification what mapping to use (e.g ISO-8859-8 for Latin-Hebrew, etc)
---

	- In python3 there are the concepts of "text" and "data" (or "bytes")

	- Text is always Unicode, however, when encoded it becomes data.
---

	- In python2 you could mix unicode and str (8 bit strings), iff the str

	  used only 7 bit (ASCII). Otherwise it would have generated UnicodeDecodeError

	- In python3 any attempt to mix text and data shall result in TypeError

	- Guido Van Rossum said that this huge change was one of the incentives to
	
	  go to Python3
---

	- Transitions between text and bytes are done using decode() and encode()

--newpage Key Differnces
--heading Key Differnces

Example 1:
--beginoutput
	>>> "הרמוניק" # <--- This is valid unicode string in Python3 since hebrew is part of the unicode code point table
	'הרמוניק' 
--endoutput
---

Example 2:
--beginoutput
	>>> "הרמוניק".encode('UTF-8')
	b'\xd7\x94\xd7\xa8\xd7\x9e\xd7\x95\xd7\xa0\xd7\x99\xd7\xa7'

	>>> b'\xd7\x94\xd7\xa8\xd7\x9e\xd7\x95\xd7\xa0\xd7\x99\xd7\xa7'.decode('UTf-8')
	'הרמוניק'

	>>> b'\xd7\x94\xd7\xa8\xd7\x9e\xd7\x95\xd7\xa0\xd7\x99\xd7\xa7'.decode('UTf-16') # <-- The encoding scheme is important
	'铗꣗黗闗ꃗ駗\ua7d7'
--endoutput
---

Example 3:

In python 2 the following snippet would be fine, however in python3 should be altered:
--beginoutput
>>> r = subprocess.check_output("ls") # <------ This returnes bytes now
>>> fh = open("/tmp/b", "w")
>>> fh.write(r) #                       <------ This expects string
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  TypeError: write() argument must be str, not bytes

should become:
>>> r = subprocess.check_output("ls")
>>> fh = open("/tmp/b", "w")
>>> fh.write(r.decode("UTF-8"))
--endoutput
--newpage Key Differnces
--heading Key Differnces

* Reading from: socket, urllib3, subprocess returnes bytes, not text
---

* Some libraries does the decoding for you (e.g requests)
---

* There are a lot more to know about the differences between Python2.X and

  Python3.X. We just covered the ones I believe are more likely to encounter
---

* For detailed documentation about python3 being backward *incompatible*:

	https://docs.python.org/3/whatsnew/3.0.html
---

* Another good refernce (cheat seet style):

	http://python-future.org/compatible_idioms.html

--newpage Migration Strategy
--huge Migration Strategy
--horline

Note: The key thing here is to do it **gradually**. At no point of time would

you want your package to suuport niether python2 nor python3. Keep it in mind.

(Replace a wheel while driving: https://youtu.be/SVGgvevWnls?t=23)
---

1) Make you python libraires dual

1.5) If you have unit tests, make them dual as well

2) Generate python3-foo in addition to python-foo

3) Start migrating your python runnables, one by one

4) Give up python-foo

5) Make your python libraries pure python3 *

* Make sure you don't have python2 clients outside of your package

--newpage Make python library dual
--heading Make python library dual
---

* Use from __future__ import ?
---

* Use helper tools such as 2to3, python2-futurize (from package python-future)
---

* Don't count on them exclusively, go over the diffs carefully
---

* In our example: python2-futurize --stage{12} python

--newpage Generate python3-foo
--heading Generate python3-foo in addition to python-foo
---

* In debian/rules, add "python3" to the --with argument, so relevant

  debhelper scripts will be run
---

* In debian/control:

  a- Add python3, python3-all to source package "Build-Depends" section.
---

  b- Add binary package: "Package: python3-demo" and change dependencies to 
  
     python3 as necessary
---

  Consult this article:

     - https://wiki.debian.org/Python/Pybuild

--newpage Migrate runnables
--heading Migrate runnables
---

* Now that we have two packages, one for legacy python and one for python3, we

  can start migrating the runnales one by one to python3. 
---

* Again we can use 2to3/futurize as an aid.

--newpage Give up python-foo
--heading Give up python-foo
---

* In debian/rules: remove "python2" from the --with argument
---

* In debian/control:

  a- Remove dh-python and python-all from source package "Build-Depends"

     section.
---

  b- Remove "Pakcage: python-foo"

--newpage Make your libs pure python3
--heading Make your libs pure python3
---

* Now we're sure that our package runs only python3 interpreter, there is no

  need to keep python2 compatible code.
---

* This means removing all "from __future__ import *" statements, selective

  imports if there are any ("queue" vs "Queue") etc
---

* Grab a cup of coffie and cheer for yourself :)

--newpage Questions
--boldon
--huge Questions?
--ulon
--horline
--boldoff
--uloff
--boldon

--newpage Thankyou
--boldon
--huge Thank    you    :)
--ulon
--horline
--boldoff
--uloff
--boldon
