#test

import random
from openframeworks import *

class myApp(object):
	def __init__(self):
		self.x = 100.0
		self.y = 100.0
	
	def update(self):
		self.x += random.random()-.5
		self.y += random.random()-.5

	def draw(self):
		ofCircle(self.x,self.y,10)
