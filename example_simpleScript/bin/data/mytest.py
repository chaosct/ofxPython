#test
import random
from openframeworks import *

print "LOL"

class myApp(object):
	color = (255,255,255)
	def __init__(self):
		self.x = 100.0
		self.y = 100.0
		self.d = 10
	
	def update(self):
		self.x += random.random()-.5
		self.y += random.random()-.5

	def draw(self):
		color = ofColor(*self.color)
		ofSetColor(color)
		ofDrawCircle(self.x,self.y,self.d)
